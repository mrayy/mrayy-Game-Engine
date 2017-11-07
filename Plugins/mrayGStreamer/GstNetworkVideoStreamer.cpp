

#include "stdafx.h"
#include "GstNetworkVideoStreamer.h"

#include "GstPipelineHandler.h"

#include "IVideoGrabber.h"
#include "CMySrc.h"
#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "StringConverter.h"
#include "ILogManager.h"
#include "IThreadManager.h"
#include "CMyListener.h"
#include "AveragePer.h"

#include <gst/app/gstappsrc.h>
namespace mray
{
namespace video
{

class GstNetworkVideoStreamerImpl :public GstPipelineHandler,public IPipelineListener,public IMyListenerCallback
{
protected:

	GstNetworkVideoStreamer* m_owner;

	core::string m_ipAddr;
	std::vector<uint> m_videoPorts;
	bool m_rtcp;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;

	ICustomVideoSrc* m_videoSrc;

	video::ImageInfo m_tmp;

	AveragePer m_bytesSent;

	bool m_addListeners;

	struct VideoSrcData
	{
		VideoSrcData()
		{
			o = 0;
			videoSink = 0;
			preSent = 0;
		}
		GstNetworkVideoStreamerImpl* o;

		GstMyListener* preSent;
		GstElement* videoSink;
	};
	std::vector<VideoSrcData> m_srcData;


public:
	GstNetworkVideoStreamerImpl(GstNetworkVideoStreamer* owner)
	{
		m_owner = owner;
		m_ipAddr = "127.0.0.1";

		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;
		m_addListeners = false;

		AddListener(this);

	}

	virtual ~GstNetworkVideoStreamerImpl()
	{/*
		for (int i = 0; i < m_srcData.size(); ++i)
		{
			if (m_srcData[i].videoSink && m_srcData[i].videoSink->m_client)
			{
				m_srcData[i].videoSink->m_client->Close();
			}
		}*/
		Stop();
	}

	void BuildStringCompressed()
	{
		m_pipeLineString = "";
		if (m_rtcp)
		{
			m_pipeLineString = "rtpbin  name=rtpbin ";
		}

		for (int i = 0; i < m_videoSrc->GetStreamsCount(); ++i)
		{
			m_pipeLineString += m_videoSrc->GetPipelineStr(i);
			m_pipeLineString = FinalizePipeline(m_pipeLineString, i);

		}

	}
	std::string FinalizePipeline(const std::string &pipeline,int i)
	{
		std::string videoStr = pipeline;
		
		if (m_rtcp)
		{
			core::string session = core::StringConverter::toString(i);

			core::string rtpSink = "udpsink name=videoSink" + session + " host=" + m_ipAddr + " port=" + core::StringConverter::toString(m_videoPorts[i] + 1) + " ts-offset=0 force-ipv4=1 ";
			core::string rtcpSink = "udpsink name=videoRtcpSink" + session + " host=" + m_ipAddr + " port=" + core::StringConverter::toString(m_videoPorts[i] + 2) + " sync=false async=false ";
			core::string rtcpSrc = "udpsrc name=videoRtcpSrc" + session + " port=" + core::StringConverter::toString(m_videoPorts[i] + 3);

			videoStr = videoStr +
				"! rtpbin.send_rtp_sink_" + session +
				" rtpbin.send_rtp_src_" + session + " ! " + rtpSink +
				" rtpbin.send_rtcp_src_" + session + " ! " + rtcpSink+
				rtcpSrc + " ! rtpbin.recv_rtcp_sink_" + session + " ";
		}
		else
		{
		//	if(m_addListeners)
		//		videoStr += " ! mylistener name=preSent" + core::StringConverter::toString(i);
			videoStr += " ! udpsink name=videoSink" + core::StringConverter::toString(i) + " port=" + core::StringConverter::toString(m_videoPorts[i]) + " host=" + m_ipAddr+"  sync=true ";
			//videoStr += " ! videoconvert ! fpsdisplaysink sync=false";
		}
		return videoStr;
	}

	void SetVideoSrc(ICustomVideoSrc* src)
	{
		m_videoSrc = src;
		m_srcData.resize(src->GetStreamsCount());
		m_videoPorts.resize(src->GetStreamsCount());
		for (int i = 0; i < m_videoPorts.size(); ++i)
			m_videoPorts[i] = 0;
	}

	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
//#define SET_SINK(elem,name,p) elem=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), name)); if(elem){elem->SetPort(m_ipAddr,p);}

		for (int i = 0; i < m_videoSrc->GetStreamsCount(); ++i)
		{
#if 1
			core::string videoSinkName="videoSink"+core::StringConverter::toString(i);
			m_srcData[i].videoSink=gst_bin_get_by_name(GST_BIN(GetPipeline()), videoSinkName.c_str());
			g_object_set(m_srcData[i].videoSink, "port", m_videoPorts[i],  0);
			g_object_set(m_srcData[i].videoSink, "host", m_ipAddr.c_str(), 0);
		//	SET_SINK(m_srcData[i].videoSink, videoSinkName.c_str(), m_videoPorts[i]);
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

	virtual void ListenerOnDataChained(_GstMyListener* src, GstBuffer * buffer)
	{
		GstMapInfo map;
		gst_buffer_map(buffer, &map, GST_MAP_READ);
		m_bytesSent.Add(map.size);
		gst_buffer_unmap(buffer, &map);
	}
	void BindPorts(const std::string& addr, uint *videoPorts, uint count, bool rtcp)
	{
		if (count != m_videoSrc->GetStreamsCount())
		{
			gLogManager.log("GstNetworkVideoStreamer:BindPorts(): Failed to bound ports since ports counts doesn't match video source count!", ELL_WARNING);
		}
		bool skip = (addr==m_ipAddr);
		if (skip)
		{
			for (int i = 0; i < m_videoPorts.size();++i)
				if (m_videoPorts[i] != videoPorts[i])
				{
					skip = false;
					break;
				}
		}
		if (skip)
			return;
		m_ipAddr = addr;
		m_videoPorts.clear();
		for (int i = 0; i < count; ++i)
			m_videoPorts.push_back(videoPorts[i]);
		m_rtcp = rtcp;
		/*
		std::string msg = "GstNetworkVideoStreamer:BindPorts(): Ports bound to:";
		for (int i = 0; i < count; ++i)
			msg += core::StringConverter::toString(videoPorts[i])+", ";
		gLogManager.log(msg, ELL_INFO);*/

		_UpdatePorts();
	}
	bool CreateStream()
	{
		GError *err = 0;
		if (!m_videoSrc)
			return false;
		 BuildStringCompressed();


		//printf("\n\n%s\n\n", m_pipeLineString.c_str());
		 GstElement* pipeline = 0;
		 
		 // int trials = 0;
		 // while (trials < 3 && pipeline==0)
		 {
			 pipeline = gst_parse_launch(m_pipeLineString.c_str(), &err);
			 gLogManager.log("Starting with pipeline: " + m_pipeLineString, ELL_INFO);
			 if (err)
			 {
				 gLogManager.log("GstNetworkVideoStreamer: Pipeline error: " + core::string(err->message), ELL_WARNING);
				 gst_object_unref(pipeline);
				 pipeline = 0;
			 }
		//	 ++trials;
		 }
		if (!pipeline)
			return false;
		gLogManager.log("Finished Linking Pipeline",ELL_INFO);


		SetPipeline(pipeline);
		_UpdatePorts();
		m_videoSrc->LinkWithPipeline(static_cast<void*>(pipeline));
		for (int i = 0; i < m_videoSrc->GetStreamsCount(); ++i)
		{
			core::string name="preSent" + core::StringConverter::toString(i);
			m_srcData[i].preSent = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline),name.c_str()));
			if (m_srcData[i].preSent)
				m_srcData[i].preSent->listeners->AddListener(this);
		}
		/*
		printf("Starting video streams\nPort Numbers:\n");
		for (int i=0;i<m_srcData.size();++i)
			printf("\t[%d] %d\n", i,m_srcData[i].videoSink->m_client->Port());*/

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
		GstPipelineHandler::Close();
	}

	int GetAverageBytesSent()
	{
		return m_bytesSent.GetAverage();
	}

	virtual void OnPipelineReady(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerReady(m_owner); }
	virtual void OnPipelinePlaying(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStarted(m_owner); }
	virtual void OnPipelineStopped(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStopped(m_owner); }
};


GstNetworkVideoStreamer::GstNetworkVideoStreamer()
{
	m_impl = new GstNetworkVideoStreamerImpl(this);
}

GstNetworkVideoStreamer::~GstNetworkVideoStreamer()
{
	delete m_impl;
}
void GstNetworkVideoStreamer::Stream()
{
	m_impl->Stream();
}
void GstNetworkVideoStreamer::Stop()
{
	m_impl->Stop();
}


int GstNetworkVideoStreamer::GetAverageBytesSent()
{
	return m_impl->GetAverageBytesSent();
}
void GstNetworkVideoStreamer::BindPorts(const std::string& addr, uint *videoPorts, uint count, bool rtcp)
{
	m_impl->BindPorts(addr, videoPorts,count, rtcp);
}

bool GstNetworkVideoStreamer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstNetworkVideoStreamer::Close()
{
	m_impl->Close();
}
bool GstNetworkVideoStreamer::IsStreaming()
{
	return m_impl->IsStreaming();
}

void GstNetworkVideoStreamer::SetVideoSrc(ICustomVideoSrc* src)
{
	m_impl->SetVideoSrc(src);
}
void GstNetworkVideoStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstNetworkVideoStreamer::IsPaused()
{
	return m_impl->IsPaused();
}
GstPipelineHandler* GstNetworkVideoStreamer::GetPipeline()
{
	return m_impl;
}

}
}

