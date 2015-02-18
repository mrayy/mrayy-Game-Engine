

#include "stdafx.h"
#include "GstCustomVideoStreamer.h"

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

class GstCustomVideoStreamerImpl :public GstPipelineHandler
{
protected:
	core::string m_ipAddr;
	int m_videoPort;

	IVideoGrabber* m_grabber[2];
	int m_bitRate;
	bool m_rtcp;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoSink;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;

	int m_fps;
	math::vector2di m_frameSize;

	video::ImageInfo m_tmp;

	struct VideoSrcData
	{
		GstAppSrc* videoSrc;
		int index;
		GstCustomVideoStreamerImpl* o;
		int sourceID;
	};
	VideoSrcData m_videoSrc[2];
public:
	GstCustomVideoStreamerImpl()
	{
		m_ipAddr = "127.0.0.1";
		m_videoPort = 5000;

		m_bitRate = 5000;
		m_grabber[0] = m_grabber[1] = 0;

		m_videoSink = 0;
		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;

		m_videoSrc[0].sourceID = 0;
		m_videoSrc[1].sourceID = 0;

		m_frameSize.set(1280, 720);
		m_fps = 30;
	}

	virtual ~GstCustomVideoStreamerImpl()
	{
	}
	void  SetResolution(int width, int height, int fps)
	{
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
			core::string format =GetFormatStr(m_grabber[i]->GetImageFormat());
			//ksvideosrc
			videoStr = "appsrc name=src"+ core::StringConverter::toString(i)+
				" ! video/x-raw,format=" + format + ",width=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().x) +
				",height=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().y) + ",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert   ";// !videoflip method = 1  ";

		}
		else{
			videoStr = "mysrc name=src" + core::StringConverter::toString(i) +
				" ! video/x-raw,format=RGB  ";// !videoflip method = 1  ";
		}
		return videoStr;
	}

	void BuildString()
	{
		core::string videoStr;

		bool mixer = false;
		int halfW = m_frameSize.x / 2;

		if (m_grabber[0] != 0 && m_grabber[1] != 0)
		{
			mixer = true;
			videoStr = "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" + core::StringConverter::toString(halfW) + "   sink_2::ypos=0  sink_2::zorder=1  ";

			videoStr += "videotestsrc pattern=\"black\" ! video/x-raw,format="+GetFormatStr(m_grabber[0]->GetImageFormat())+",width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) + " !  mix.sink_0 ";

		}
		for (int i = 0; i < 2; ++i)
		{
			if (m_grabber[i])
			{
				videoStr += BuildGStr(i);

				if (mixer)
				{
					videoStr += "videoscale ! "
						"video/x-raw,format=" + GetFormatStr(m_grabber[0]->GetImageFormat()) + ",width=" + core::StringConverter::toString(halfW) +
						",height=" + core::StringConverter::toString(m_frameSize.y) + "! mix.sink_"+core::StringConverter::toString(i+1)+" ";
				}
			}
		}

		//encoder string
		videoStr += "! x264enc name=videoEnc bitrate=" + core::StringConverter::toString(m_bitRate) + 
			" speed-preset=superfast tune=zerolatency sync-lookahead=0  pass=qual ! rtph264pay ";
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
			m_pipeLineString = videoStr + " ! "
				"myudpsink name=videoSink ";
		}

	}

	void SetBitRate(int bitRate)
	{
		m_bitRate = bitRate;
	}
	void SetVideoGrabber(IVideoGrabber* grabber0, IVideoGrabber* grabber1)
	{
		m_grabber[0] = grabber0;
		m_grabber[1] = grabber1;
	}


	GstFlowReturn NeedBuffer(GstMySrc * sink, GstBuffer ** buffer,int index)
	{
		if (!m_grabber[index])
		{
			gLogManager.log("No video grabber is assigned to CustomVideoStreamer", ELL_WARNING);
			return GST_FLOW_ERROR;
		}
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
			//gst_buffer_unref(buffer);
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

	void _UpdatePorts()
	{

		if (!m_gstPipeline)
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}

		for (int i = 0; i < 2; ++i)
		{
			core::string name = "src" + core::StringConverter::toString(i);
			m_videoSrc[i].videoSrc = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), name.c_str()));
			m_videoSrc[i].o = this;
			m_videoSrc[i].index = i;
			if (m_videoSrc[i].videoSrc){
				GstAppSrcCallbacks srcCB;
				srcCB.need_data = &start_feed;
				srcCB.enough_data = &stop_feed;
				srcCB.seek_data = 0;
				gst_app_src_set_callbacks(m_videoSrc[i].videoSrc, &srcCB, &m_videoSrc, NULL);
			}
		}

		SET_SINK(videoSink, m_videoPort);
		if (m_rtcp){
			SET_SRC(videoRtcpSrc, (m_videoPort + 1));
			SET_SINK(videoRtcpSink, (m_videoPort + 2));
		}

	}

	void BindPorts(const core::string& addr, int videoPort, bool rtcp)
	{
		m_ipAddr = addr;
		m_videoPort = videoPort;
		m_rtcp = rtcp;

		_UpdatePorts();
	}
	bool CreateStream()
	{
		GError *err = 0;
		BuildString();
		m_gstPipeline = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			printf("GstCustomVideoStreamer: Pipeline error: %s", err->message);
		}
		if (!m_gstPipeline)
			return false;
		_UpdatePorts();

		return CreatePipeline();

	}
	void Stream()
	{
		SetPaused(false);
	}
	bool IsStreaming()
	{
		return !m_paused;
	}
	virtual void Close()
	{
		GstPipelineHandler::Close();
	}


};


GstCustomVideoStreamer::GstCustomVideoStreamer()
{
	m_impl = new GstCustomVideoStreamerImpl();
}

GstCustomVideoStreamer::~GstCustomVideoStreamer()
{
	delete m_impl;
}
void GstCustomVideoStreamer::Stream()
{
	m_impl->Stream();
}
void GstCustomVideoStreamer::Stop()
{
	m_impl->Stop();
}


void GstCustomVideoStreamer::BindPorts(const core::string& addr, int videoPort, bool rtcp)
{
	m_impl->BindPorts(addr, videoPort, rtcp);
}

bool GstCustomVideoStreamer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstCustomVideoStreamer::Close()
{
	m_impl->Close();
}
bool GstCustomVideoStreamer::IsStreaming()
{
	return m_impl->IsStreaming();
}

void GstCustomVideoStreamer::SetBitRate(int bitRate)
{
	m_impl->SetBitRate(bitRate);
}

void GstCustomVideoStreamer::SetResolution(int width, int height, int fps)
{
	m_impl->SetResolution(width, height, fps);
}

void GstCustomVideoStreamer::SetVideoGrabber(IVideoGrabber* grabber0, IVideoGrabber* grabber1)
{
	m_impl->SetVideoGrabber(grabber0,grabber1);
}
void GstCustomVideoStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstCustomVideoStreamer::IsPaused()
{
	return !m_impl->IsPlaying();
}

}
}

