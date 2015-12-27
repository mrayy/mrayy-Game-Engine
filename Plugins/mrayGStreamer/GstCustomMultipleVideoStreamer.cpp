

#include "stdafx.h"
#include "GstCustomMultipleVideoStreamer.h"

#include "GstPipelineHandler.h"

#include "IVideoGrabber.h"
#include "CMySrc.h"
#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "StringConverter.h"
#include "ILogManager.h"
#include "IThreadManager.h"

#include <gst/app/gstappsrc.h>
namespace mray
{
namespace video
{

class GstCustomMultipleVideoStreamerImpl :public GstPipelineHandler,public IPipelineListener
{
protected:

	GstCustomMultipleVideoStreamer* m_owner;

	core::string m_ipAddr;
	uint m_videoPorts[2];
	uint m_clockPort;
	bool m_rtcp;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;

	ICustomVideoSrc* m_videoSrc;

	video::ImageInfo m_tmp;

	struct VideoSrcData
	{
		VideoSrcData()
		{
			o = 0;
			videoSink = 0;
		}
		GstCustomMultipleVideoStreamerImpl* o;

		GstMyUDPSink* videoSink;
	};
	std::vector<VideoSrcData> m_srcData;


public:
	GstCustomMultipleVideoStreamerImpl(GstCustomMultipleVideoStreamer* owner)
	{
		m_owner = owner;
		m_ipAddr = "127.0.0.1";
		m_videoPorts[0] = 0;
		m_videoPorts[1] = 0;
		m_clockPort = 0;

		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;


		AddListener(this);

	}

	virtual ~GstCustomMultipleVideoStreamerImpl()
	{
		for (int i = 0; i < m_srcData.size(); ++i)
		{
			if (m_srcData[i].videoSink && m_srcData[i].videoSink->m_client)
			{
				m_srcData[i].videoSink->m_client->Close();
			}
		}
		Stop();
	}

	void BuildStringCompressed()
	{
		m_pipeLineString="";

		for (int i = 0; i < m_videoSrc->GetVideoSrcCount(); ++i)
		{
			m_pipeLineString += m_videoSrc->GetPipelineStr(i);
			m_pipeLineString = FinalizePipeline(m_pipeLineString, i);

		}

	}
	std::string FinalizePipeline(const std::string &pipeline,int i)
	{
		std::string videoStr = pipeline;
		videoStr += " ! myudpsink name=videoSink" + core::StringConverter::toString(i) + " sync=false ";
		return videoStr;
	}

	void SetVideoSrc(ICustomVideoSrc* src)
	{
		m_videoSrc = src;
		m_srcData.resize(src->GetVideoSrcCount());
	}

	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(elem,name,p) elem=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), name)); if(elem){elem->SetPort(m_ipAddr,p);}

		for (int i = 0; i < m_videoSrc->GetVideoSrcCount(); ++i)
		{
#if 1
			core::string videoSinkName="videoSink"+core::StringConverter::toString(i);
			SET_SINK(m_srcData[i].videoSink, videoSinkName.c_str(), m_videoPorts[i]);
		//	gst_base_sink_set_async_enabled(GST_BASE_SINK(m_srcData[i].videoSink), false);
		//	gst_base_sink_set_sync(GST_BASE_SINK(m_srcData[i].videoSink), false);
#else

			m_srcData[i].videoSrc = GST_MySRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), name.c_str()));
			m_srcData[i].o = this;
			m_srcData[i].index = i;
			if (m_srcData[i].videoSrc){
				m_srcData[i].videoSrc->need_buffer = need_buffer;
				m_srcData[i].videoSrc->data = &m_srcData[i];
			}
#endif


		}
		//g_object_set(G_OBJECT(m_videoSink), "sync", false, (void*)NULL);

		if (m_rtcp){
			//SET_SRC(videoRtcpSrc, (m_baseVideoPort + 1));
//			SET_SINK(videoRtcpSink, (m_baseVideoPort + 2));
		}

	}

	void BindPorts(const std::string& addr, uint *videoPorts, uint count, uint clockPort, bool rtcp)
	{
		if (count < 2)
			return;
		if (m_ipAddr == addr && m_videoPorts[0] == videoPorts[0] && m_videoPorts[1] == videoPorts[1] && m_rtcp == rtcp)
			return;
		m_ipAddr = addr;
		m_videoPorts[0] = videoPorts[0];
		m_videoPorts[1] = videoPorts[1];
		m_rtcp = rtcp;
		m_clockPort = clockPort;

		_UpdatePorts();
	}
	bool CreateStream()
	{
		GError *err = 0;
		if (!m_videoSrc)
			return false;
		 BuildStringCompressed();

		//printf("\n\n%s\n\n", m_pipeLineString.c_str());
		GstElement* pipeline = gst_parse_launch(m_pipeLineString.c_str(), &err);
		gLogManager.log("Starting with pipeline: " + m_pipeLineString, ELL_INFO);
		if (err)
		{
			gLogManager.log("GstCustomMultipleVideoStreamer: Pipeline error: " + core::string(err->message),ELL_WARNING);
		}
		if (!pipeline)
			return false;
		gLogManager.log("Finished Linking Pipeline",ELL_INFO);

		SetPipeline(pipeline);
		_UpdatePorts();
		m_videoSrc->LinkWithPipeline(static_cast<void*>(pipeline));
		/*
		printf("Starting video streams\nPort Numbers:\n");
		for (int i=0;i<m_srcData.size();++i)
			printf("\t[%d] %d\n", i,m_srcData[i].videoSink->m_client->Port());*/

		return CreatePipeline(true,"",m_clockPort);

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
		GstPipelineHandler::Close();
	}


	virtual void OnPipelineReady(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerReady(m_owner); }
	virtual void OnPipelinePlaying(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStarted(m_owner); }
	virtual void OnPipelineStopped(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStopped(m_owner); }
};


GstCustomMultipleVideoStreamer::GstCustomMultipleVideoStreamer()
{
	m_impl = new GstCustomMultipleVideoStreamerImpl(this);
}

GstCustomMultipleVideoStreamer::~GstCustomMultipleVideoStreamer()
{
	delete m_impl;
}
void GstCustomMultipleVideoStreamer::Stream()
{
	m_impl->Stream();
}
void GstCustomMultipleVideoStreamer::Stop()
{
	m_impl->Stop();
}


void GstCustomMultipleVideoStreamer::BindPorts(const std::string& addr, uint *videoPorts, uint count, uint clockPort, bool rtcp)
{
	m_impl->BindPorts(addr, videoPorts,count, clockPort, rtcp);
}

bool GstCustomMultipleVideoStreamer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstCustomMultipleVideoStreamer::Close()
{
	m_impl->Close();
}
bool GstCustomMultipleVideoStreamer::IsStreaming()
{
	return m_impl->IsStreaming();
}

void GstCustomMultipleVideoStreamer::SetVideoSrc(ICustomVideoSrc* src)
{
	m_impl->SetVideoSrc(src);
}
void GstCustomMultipleVideoStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstCustomMultipleVideoStreamer::IsPaused()
{
	return m_impl->IsPaused();
}
GstPipelineHandler* GstCustomMultipleVideoStreamer::GetPipeline()
{
	return m_impl;
}

}
}

