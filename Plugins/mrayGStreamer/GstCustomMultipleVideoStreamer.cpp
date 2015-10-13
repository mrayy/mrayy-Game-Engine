

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

	std::vector<IVideoGrabber*> m_grabber;
	int m_bitRate;
	bool m_rtcp;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;

	int m_fps;
	math::vector2di m_frameSize;
	bool m_freeSize;

	video::ImageInfo m_tmp;

	struct VideoSrcData
	{
		VideoSrcData()
		{
			videoSrc = 0;
			index = 0;
			sourceID = 0;
			o = 0;
			videoSink = 0;
		}
		GstAppSrcCallbacks srcCB;
		GstAppSrc* videoSrc;
		//GstMySrc * videoSrc;
		int index;
		GstCustomMultipleVideoStreamerImpl* o;
		int sourceID;

		GstMyUDPSink* videoSink;
	};
	std::vector<VideoSrcData> m_videoSrc;


public:
	GstCustomMultipleVideoStreamerImpl(GstCustomMultipleVideoStreamer* owner)
	{
		m_owner = owner;
		m_ipAddr = "127.0.0.1";
		m_videoPorts[0] = 0;
		m_videoPorts[1] = 0;
		m_clockPort = 0;

		m_bitRate = 5000;

		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;

		m_frameSize.set(1280, 720);
		m_fps = 30;
		m_freeSize = true;

		AddListener(this);

	}

	virtual ~GstCustomMultipleVideoStreamerImpl()
	{
		for (int i = 0; i < m_videoSrc.size(); ++i)
		{
			if (m_videoSrc[i].videoSink && m_videoSrc[i].videoSink->m_client)
			{
				m_videoSrc[i].videoSink->m_client->Close();
			}
		}
		Stop();
	}
	void  SetResolution(int width, int height, int fps,bool free)
	{
		m_freeSize = free;
		m_frameSize.set(width, height);
		m_fps = fps;
	}

	core::string GetFormatStr(EPixelFormat fmt)
	{

		core::string format = "RGB";
		switch (fmt)
		{
		case EPixel_R8G8B8:
			format = "RGB";
			break;
		case EPixel_R8G8B8A8:
			format = "RGBA";
			break;
		case EPixel_X8R8G8B8:
			format = "xRGB";
			break;

		case EPixel_B8G8R8:
			format = "BGR";
			break;
		case EPixel_B8G8R8A8:
			format = "BGRA";
			break;
		case EPixel_X8B8G8R8:
			format = "xBGR";
			break;
		case EPixel_YUYV:
			format = "Y41B";
			break;

		case EPixel_LUMINANCE8:
		case EPixel_Alpha8:
			format = "GRAY8";
			break;
		}
		return format;
	}

	core::string BuildGStr(int i)
	{

		core::string videoStr;
		if (m_grabber[i])
		{
				core::string format = GetFormatStr(m_grabber[i]->GetImageFormat());
				//ksvideosrc

				videoStr = "appsrc";
				videoStr += " name=src" + core::StringConverter::toString(i) +
					" do-timestamp=true is-live=true "//"block=true"
					" ! video/x-raw,format=" + format + ",width=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().x) +
					",height=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().y) + ",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

				videoStr += "! videoconvert  ! video/x-raw,format=I420 ";// !videoflip method = 1  ";
				//videoStr += "! videorate ";//  max-rate=" + core::StringConverter::toString(m_fps) + " ";
				//videoStr += " ! queue ";
				//	if (m_grabber[i]->GetImageFormat()!=video::EPixel_YUYV)
		}
		else{
			videoStr = "mysrc name=src" + core::StringConverter::toString(i) +
				" ! video/x-raw,format=RGB  ";// !videoflip method = 1  ";
		}
		//add time stamp

		return videoStr;
	}

	void BuildString()
	{
		core::string videoStr;

		for (int i = 0; i < m_grabber.size(); ++i)
		{
			if (m_grabber[i])
			{
				videoStr += BuildGStr(i);
				if (!m_freeSize)
				{
					videoStr += "! videoscale ! video/x-raw,width=" + core::StringConverter::toString(m_frameSize.x) +
						",height=" + core::StringConverter::toString(m_frameSize.y) + ",framerate=" + core::StringConverter::toString(m_fps) + "/1";

				}

				
				videoStr += "! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / m_grabber.size()) +
						" speed-preset=superfast pass=qual tune=zerolatency sync-lookahead=0 rc-lookahead=0  ip-factor=1.8 interlaced=true sliced-threads=false  "// 
					" ! rtph264pay ";
					/*
				//videoStr += " ! vp8enc ! rtpvp8pay ";
				videoStr += " ! theoraenc ! rtptheorapay ";*/
				videoStr +=" ! myudpsink name=videoSink"+core::StringConverter::toString(i)+" sync=false ";

				//videoStr += " ! autovideosink sync=false ";
			}
		}

		m_pipeLineString = videoStr;
		return;

		if (m_rtcp)
		{
			m_pipeLineString = "rtpbin  name=rtpbin " +
				videoStr +
				"! rtpbin.send_rtp_sink_0 "

				"rtpbin.send_rtp_src_0 ! "
				"myudpsink name=videoSink  "

				"rtpbin.send_rtcp_src_0 ! "
				"myudpsink name=videoRtcpSink sync=false async=false "
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 ";
		}
		else
		{

			//"udpsink host=127.0.0.1 port=7000";
		}

	}

	void SetBitRate(int bitRate)
	{
		m_bitRate = bitRate;
	}
	void SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers)
	{
		m_grabber = grabbers;
		m_videoSrc.resize(grabbers.size());
	}

	GstFlowReturn NeedBuffer(GstMySrc * sink, GstBuffer ** buffer,int index)
	{
		if (!m_grabber[index])
		{
			gLogManager.log("No video grabber is assigned to CustomVideoStreamer", ELL_WARNING);
			return GST_FLOW_ERROR;
		}
// 		do 
// 		{
// 			OS::IThreadManager::getInstance().sleep (1);
// 		} while (!m_grabber[index]->GrabFrame());
 		if (!m_grabber[index]->GrabFrame())
  		{
  			return GST_FLOW_ERROR;
  		}
		m_grabber[index]->Lock();

		const video::ImageInfo* ifo = m_grabber[index]->GetLastFrame();
		int len = ifo->imageDataSize;
		GstMapInfo map;
		GstBuffer* outbuf = gst_buffer_new_and_alloc(len);
		gst_buffer_map(outbuf, &map, GST_MAP_WRITE);
		memcpy(map.data, ifo->imageData, len);


		gst_buffer_unmap(outbuf, &map);
		m_grabber[index]->Unlock();
		*buffer = outbuf;
		OS::IThreadManager::getInstance().sleep(5);
		return GST_FLOW_OK;
	}

	static GstFlowReturn need_buffer(GstMySrc * sink, gpointer data, GstBuffer ** buffer)
	{
		VideoSrcData* d = static_cast<VideoSrcData*>(data);
		if (d)
		{
			return d->o->NeedBuffer(sink, buffer,d->index);
		}
		return GST_FLOW_ERROR;
	}


	static gboolean read_data(VideoSrcData *d)
	{
		GstFlowReturn ret;

		GstBuffer *buffer;
		if (d->o->NeedBuffer(0, &buffer, d->index) == GST_FLOW_OK)
		{
			ret = gst_app_src_push_buffer(d->videoSrc, buffer);
			if (ret != GST_FLOW_OK){
				ret = gst_app_src_end_of_stream(d->videoSrc);
				return FALSE;
			}
		}
		return TRUE;

	}
	static void start_feed(GstAppSrc *source, guint size, gpointer data)
	{
		VideoSrcData* o = static_cast<VideoSrcData*>(data);
		if (o->sourceID != 0)
		{
			g_source_remove(o->sourceID);
			o->sourceID = 0;
		}
		if (o->sourceID == 0) {
			GST_DEBUG("start feeding");
			o->sourceID = g_idle_add((GSourceFunc)read_data, o);
		}
	}


	static void stop_feed(GstAppSrc *source, gpointer data)
	{
		VideoSrcData* o = static_cast<VideoSrcData*>(data);
		if (o->sourceID != 0) {
			GST_DEBUG("stop feeding");
			g_source_remove(o->sourceID);
			o->sourceID = 0;
		}
	}
	static gboolean seek_data(GstAppSrc *src, guint64 offset, gpointer user_data)
	{
		return TRUE;
	}
	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(elem,name,p) elem=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), name)); if(elem){elem->SetPort(m_ipAddr,p);}

		for (int i = 0; i < m_grabber.size(); ++i)
		{
			core::string name = "src" + core::StringConverter::toString(i);
#if 1
			m_videoSrc[i].videoSrc = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(GetPipeline()), name.c_str()));
			m_videoSrc[i].o = this;
			m_videoSrc[i].index = i;
			if (m_videoSrc[i].videoSrc){

				gst_base_src_set_blocksize(GST_BASE_SRC(m_videoSrc[i].videoSrc), 640 * 480 * 3);
				gst_base_src_set_live(GST_BASE_SRC(m_videoSrc[i].videoSrc), true);
				gst_base_src_set_async(GST_BASE_SRC(m_videoSrc[i].videoSrc), false);
				gst_base_src_set_do_timestamp(GST_BASE_SRC(m_videoSrc[i].videoSrc), true);

				gst_app_src_set_max_bytes(m_videoSrc[i].videoSrc, 640 * 480 * 3);
				gst_app_src_set_emit_signals(m_videoSrc[i].videoSrc, false);

				m_videoSrc[i].srcCB.need_data = &start_feed;
				m_videoSrc[i].srcCB.enough_data = &stop_feed;
				m_videoSrc[i].srcCB.seek_data = &seek_data;
				gst_app_src_set_callbacks(m_videoSrc[i].videoSrc, &m_videoSrc[i].srcCB, &m_videoSrc[i], NULL);

				core::string videoSinkName="videoSink"+core::StringConverter::toString(i);
				SET_SINK(m_videoSrc[i].videoSink, videoSinkName.c_str(), m_videoPorts[i]);
			}
#else

			m_videoSrc[i].videoSrc = GST_MySRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), name.c_str()));
			m_videoSrc[i].o = this;
			m_videoSrc[i].index = i;
			if (m_videoSrc[i].videoSrc){
				m_videoSrc[i].videoSrc->need_buffer = need_buffer;
				m_videoSrc[i].videoSrc->data = &m_videoSrc[i];
			}
#endif


		}
		//g_object_set(G_OBJECT(m_videoSink), "sync", false, (void*)NULL);

		if (m_rtcp){
			//SET_SRC(videoRtcpSrc, (m_baseVideoPort + 1));
//			SET_SINK(videoRtcpSink, (m_baseVideoPort + 2));
		}

	}

	void BindPorts(const core::string& addr, uint *videoPorts, uint count, uint clockPort, bool rtcp)
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
		BuildString();
		//printf("\n\n%s\n\n", m_pipeLineString.c_str());
		GstElement* pipeline = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			gLogManager.log("GstCustomMultipleVideoStreamer: Pipeline error: " + core::string(err->message),ELL_WARNING);
		}
		if (!pipeline)
			return false;

		SetPipeline(pipeline);
		_UpdatePorts();

		printf("Starting video streams\n"
			"\tPorts number:%d,%d , Ports Count:%d\n", m_videoPorts[0], m_videoPorts[1], m_grabber.size());

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


void GstCustomMultipleVideoStreamer::BindPorts(const core::string& addr, uint *videoPorts, uint count, uint clockPort, bool rtcp)
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

void GstCustomMultipleVideoStreamer::SetBitRate(int bitRate)
{
	m_impl->SetBitRate(bitRate);
}

void GstCustomMultipleVideoStreamer::SetResolution(int width, int height, int fps,bool free)
{
	m_impl->SetResolution(width, height, fps, free);
}

void GstCustomMultipleVideoStreamer::SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers)
{
	m_impl->SetVideoGrabber(grabbers);
}
void GstCustomMultipleVideoStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstCustomMultipleVideoStreamer::IsPaused()
{
	return !m_impl->IsPlaying();
}
GstPipelineHandler* GstCustomMultipleVideoStreamer::GetPipeline()
{
	return m_impl;
}

}
}

