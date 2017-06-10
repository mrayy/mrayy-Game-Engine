

#include "stdafx.h"
#include "GstCustomDataStreamer.h"

#include "GstPipelineHandler.h"

#include <gst/app/gstappsrc.h>
#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"
#include "ILogManager.h"
#include "IThreadManager.h"
#include "IMutex.h"

namespace mray
{
namespace video
{

class GstCustomDataStreamerImpl :public GstPipelineHandler
{
protected:
	core::string m_ipAddr;
	uint m_dataPort;

	core::string m_pipeLineString;
	GstMyUDPSink* m_dataSink;
	core::string m_dataType;
	int m_payload;
	bool m_autotimestamp;
	
	GstAppSrc* m_dataSrc;
	unsigned long long m_bufferID;
	GstClockTime m_prevTimeStamp;

	OS::IMutex* m_dataMutex;

	struct DataSegment
	{
		DataSegment()
		{
			data = 0;
			length = 0;
			owner = 0;
			index = 0;
			avail = true;
		}
		~DataSegment()
		{
			if (data)
				delete[] data;
		}
		uchar* data;
		int length;
		GstCustomDataStreamerImpl* owner;
		int index;

		bool avail;
	};
	std::vector<DataSegment*> m_data;
public:
	GstCustomDataStreamerImpl()
	{
		m_dataSink = 0;
		m_dataSrc = 0;
		m_ipAddr = "127.0.0.1";
		m_dataPort = 5005;
		m_bufferID = 0;
		m_prevTimeStamp = 0;
		m_dataMutex = OS::IThreadManager::getInstance().createMutex();
	}
	virtual ~GstCustomDataStreamerImpl()
	{
		Close();
		delete m_dataMutex;
		for (int i = 0; i < m_data.size(); ++i)
			delete m_data[i];
		m_data.clear();
	}
	void SetApplicationDataType(const std::string& dataType, bool autotimestamp, int payload = 98)
	{
		m_dataType = dataType;
		m_payload = payload;
		m_autotimestamp = autotimestamp;
	}
	
	static void _DestroyNotify(gpointer       data)
	{
		DataSegment* d = static_cast<DataSegment*>(data);
		if (d)
		{
			d->owner->m_dataMutex->lock();
			d->avail = true;
			d->owner->m_dataMutex->unlock();
		}
	}

	DataSegment* GetDataSegment(int size)
	{
		m_dataMutex->lock();
		DataSegment* d = 0;
		for (int i = 0; i < m_data.size(); ++i)
		{
			if (m_data[i]->avail && m_data[i]->length>=size)
			{
				d = m_data[i];
				break;
			}
		}
		if (!d)
		{
			d = new DataSegment();
			d->owner = this;
			d->data = new uchar[size];
			d->length = size;
			m_data.push_back(d);
		}
		d->avail = false;
		m_dataMutex->unlock();
		return d;
	}

	GstClockTime getTimeStamp(){
		if (!IsStreaming()) return GST_CLOCK_TIME_NONE;
		GstClock * clock = gst_pipeline_get_clock(GST_PIPELINE(GetPipeline()));

		gst_object_ref(clock);
		GstClockTime now = gst_clock_get_time(clock) - gst_element_get_base_time(GetPipeline());
		gst_object_unref(clock);
		return now;
	}
	void AddDataFrame(uchar* data, int length)
	{
		if (!IsStreaming())
			return;

		auto d=GetDataSegment(length);
		memcpy(d->data, data, length);

		GstBuffer * buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, (void*)d->data, length, 0, length, d, (GDestroyNotify)&_DestroyNotify);

		GstClockTime now = GST_CLOCK_TIME_NONE;
		if (now == GST_CLOCK_TIME_NONE){
			now = getTimeStamp();
		}

		if (!m_autotimestamp){
			GST_BUFFER_OFFSET(buffer) = m_bufferID++;
			GST_BUFFER_OFFSET_END(buffer) = m_bufferID;
			GST_BUFFER_DTS(buffer) = now;
			GST_BUFFER_PTS(buffer) = now;
			GST_BUFFER_DURATION(buffer) = now - m_prevTimeStamp;
			m_prevTimeStamp = now;
		}

		GstFlowReturn flow_return = gst_app_src_push_buffer((GstAppSrc*)m_dataSrc, buffer);

		if (flow_return != GST_FLOW_OK) {
			gLogManager.log("GstCustomDataStreamer::Failed to push data", ELL_WARNING, EVL_Heavy);
		}
	}
	void BuildString()
	{
		//actual-buffer-time=0 actual-latency-time=0
		core::string pipline = "appsrc is-live=1 format=time do-timestamp=" + core::string(m_autotimestamp ? "1" : "0") + " name=dataSrc ! application/x-" + m_dataType + " ";
		
		pipline += " ! rtpgstpay pt=" + core::StringConverter::toString(m_payload) + " ";
		m_pipeLineString = pipline + " ! "
			"udpsink name=dataSink ";

	}
	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
// #define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
// #define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}


	//	SET_SINK(dataSink, m_dataPort);
		g_object_set(m_dataSink, "port", m_dataPort, 0);
		g_object_set(m_dataSink, "host", m_ipAddr.c_str(), 0);
	}


	// addr: target address to stream video to
	// audioport: port for the audio stream , audio rtcp is allocated as audioPort+1 and audioPort+2
	void BindPorts(const std::string& addr, uint dataPort, bool rtcp)
	{
		m_ipAddr = addr;
		m_dataPort = dataPort;
		_UpdatePorts();
	}

	bool CreateStream()
	{
		GError *err = 0;
		BuildString();
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		gLogManager.log("GstCustomDataStreamer::Starting with pipeline: " + m_pipeLineString, ELL_INFO);
		if (err)
		{
			gLogManager.log("GstCustomDataStreamer: Pipeline error: " + core::string(err->message), ELL_WARNING);
		}
		if (!p)
			return false;
		gLogManager.log("GstCustomDataStreamer::Finished Linking Pipeline", ELL_INFO);
		SetPipeline(p);
		_UpdatePorts();
		m_bufferID = 0;

		m_dataSrc=GST_APP_SRC(gst_bin_get_by_name(GST_BIN((GstElement*)p), "dataSrc"));
		gst_app_src_set_stream_type((GstAppSrc*)m_dataSrc, GST_APP_STREAM_TYPE_STREAM);

		return CreatePipeline();

	}
	void Stream()
	{
		SetPaused(false);
	}
	void Stop()
	{
		SetPaused(true);
	}
	bool IsStreaming()
	{
		return IsPlaying();
	}
	virtual void Close()
	{
		if (m_dataSrc)
			gst_element_send_event(GST_ELEMENT(m_dataSrc), gst_event_new_eos());
		GstPipelineHandler::Close();
		m_dataSrc = 0;
	}

};


GstCustomDataStreamer::GstCustomDataStreamer()
{
	m_impl = new GstCustomDataStreamerImpl();
}
GstCustomDataStreamer::~GstCustomDataStreamer()
{
	delete m_impl;
}

GstPipelineHandler* GstCustomDataStreamer::GetPipeline()
{
	return m_impl;
}

void GstCustomDataStreamer::BindPorts(const std::string& addr, uint *port, uint count, bool rtcp)
{
	m_impl->BindPorts(addr, port[0], rtcp);
}

void GstCustomDataStreamer::SetApplicationDataType(const std::string& dataType, bool autotimestamp, int payload )
{
	m_impl->SetApplicationDataType(dataType, autotimestamp, payload);
}

void GstCustomDataStreamer::AddDataFrame(uchar* data, int length)
{
	m_impl->AddDataFrame(data, length);
}

void GstCustomDataStreamer::Stream()
{
	m_impl->Stream();
}


void GstCustomDataStreamer::Stop()
{
	m_impl->Stop();
}
bool GstCustomDataStreamer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstCustomDataStreamer::Close()
{
	m_impl->Close();
}
bool GstCustomDataStreamer::IsStreaming()
{
	return m_impl->IsStreaming();
}

void GstCustomDataStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstCustomDataStreamer::IsPaused()
{
	return !m_impl->IsPlaying();
}


}
}


