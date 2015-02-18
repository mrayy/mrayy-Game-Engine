

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

namespace mray
{
namespace video
{

class GstCustomVideoStreamerImpl :public GstPipelineHandler
{
protected:
	core::string m_ipAddr;
	int m_videoPort;

	IVideoGrabber* m_grabber;
	int m_bitRate;
	bool m_rtcp;

	GstMySrc* m_videoSrc;
	core::string m_pipeLineString;
	GstMyUDPSink* m_videoSink;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;

public:
	GstCustomVideoStreamerImpl()
	{
		m_ipAddr = "127.0.0.1";
		m_videoPort = 5000;

		m_bitRate = 5000;
		m_grabber = 0;

		m_videoSink = 0;
		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;
	}

	virtual ~GstCustomVideoStreamerImpl()
	{
	}
	void BuildString()
	{
		core::string videoStr;
		if (m_grabber)
		{
			core::string format="RGB";
			switch (m_grabber->GetImageFormat())
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
			//ksvideosrc
			videoStr = "mysrc name=src "
				" ! video/x-raw,format=" + format + ",width=" + core::StringConverter::toString(m_grabber->GetFrameSize().x) + 
				",height=" + core::StringConverter::toString(m_grabber->GetFrameSize().y) + ",framerate=30/1 ! videoconvert   ";// !videoflip method = 1  ";

		}
		else{
			videoStr = "mysrc name=src "
				" ! video/x-raw,format=RGB  ";// !videoflip method = 1  ";
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
	void SetVideoGrabber(IVideoGrabber* grabber)
	{
		m_grabber = grabber;
	}


	GstFlowReturn NeedBuffer(GstMySrc * sink, GstBuffer ** buffer)
	{
		if (!m_grabber)
		{
			gLogManager.log("No video grabber is assigned to CustomVideoStreamer", ELL_WARNING);
			return GST_FLOW_ERROR;
		}
		do
		{
			OS::IThreadManager::getInstance().sleep(30);
		} while (!m_grabber->GrabFrame());
// 		if (!m_grabber->GrabFrame())
// 		{
// 			return GST_FLOW_ERROR;
// 		}
		const video::ImageInfo* ifo= m_grabber->GetLastFrame();
		*buffer = gst_buffer_new_wrapped(ifo->imageData,ifo->imageDataSize);

		return GST_FLOW_OK;
	}

	static GstFlowReturn need_buffer(GstMySrc * sink, gpointer data, GstBuffer ** buffer)
	{
		GstCustomVideoStreamerImpl* o = static_cast<GstCustomVideoStreamerImpl*>(data);
		if (o)
		{
			return o->NeedBuffer(sink, buffer);
		}
		return GST_FLOW_ERROR;
	}

	void _UpdatePorts()
	{

		if (!m_gstPipeline)
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}


		m_videoSrc = GST_MySRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), "src"));
		if (m_videoSrc){
			m_videoSrc->data = this;
			m_videoSrc->need_buffer = need_buffer;
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


void GstCustomVideoStreamer::SetVideoGrabber(IVideoGrabber* grabber)
{
	m_impl->SetVideoGrabber(grabber);
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

