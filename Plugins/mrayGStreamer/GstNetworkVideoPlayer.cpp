

#include "stdafx.h"
#include "GstNetworkVideoPlayer.h"

#include "ILogManager.h"

#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "CMySrc.h"
#include "CMySink.h"

#include "VideoAppSinkHandler.h"
#include "GstPipelineHandler.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

namespace mray
{
namespace video
{

class GstNetworkVideoPlayerImpl :public GstPipelineHandler
{
	core::string m_ipAddr;
	int m_videoPort;

	core::string m_pipeLineString;

	GstMyUDPSrc* m_videoSrc;
	GstMyUDPSrc* m_videoRtcpSrc;
	GstMyUDPSink* m_videoRtcpSink;

	GstAppSink* m_videoSink;
	bool m_rtcp;

	VideoAppSinkHandler m_videoHandler;

public:
	GstNetworkVideoPlayerImpl()
	{
		m_ipAddr = "127.0.0.1";
		m_videoPort = 5000;
		m_gstPipeline = 0;
		m_videoSrc = 0;
		m_videoRtcpSrc = 0;
		m_videoRtcpSink = 0;
		m_videoSink = 0;

	}
	virtual ~GstNetworkVideoPlayerImpl()
	{

	}

	void _BuildPipelineH264()
	{
		core::string videoStr =
			//video rtp
			//"myudpsrc "
			//"name=videoSrc "
			"udpsrc port=7000 !"
			"application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264 ";
		if (m_rtcp)
		{
			m_pipeLineString =
				"rtpbin "
				"name=rtpbin "
				+ videoStr +
				"! rtpbin.recv_rtp_sink_0 "
				"rtpbin. ! rtph264depay !  avdec_h264 ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink "

				//video rtcp
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 "
				"rtpbin.send_rtcp_src_0 !  myudpsink name=videoRtcpSink sync=false async=false ";
		}
		else
		{
			m_pipeLineString = videoStr + "!"
				"rtpjitterbuffer ! "
				"rtph264depay !  avdec_h264 ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink ";

		}
	}


	void _BuildPipelineMJPEG()
	{
		core::string videoStr =
			//video rtp
			"udpsrc "
			"name=videoSrc "
			"caps=application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)JPEG ";
		if (m_rtcp)
		{
			m_pipeLineString =
				"rtpbin "
				"name=rtpbin "
				+ videoStr +
				"! rtpbin.recv_rtp_sink_0 "
				"rtpbin. ! rtpjpegdepay !  jpegdec ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink "

				//video rtcp
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 "
				"rtpbin.send_rtcp_src_0 !  myudpsink name=videoRtcpSink sync=false async=false ";
		}
		else
		{
			m_pipeLineString = videoStr + "!"
				"rtpjpegdepay !  jpegdec ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink ";

		}
	}

	void _UpdatePorts()
	{
		if (!m_gstPipeline)
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}

		SET_SRC(videoSrc, m_videoPort);
		SET_SINK(videoRtcpSink, (m_videoPort + 1));
		SET_SRC(videoRtcpSrc, (m_videoPort + 2));

	}

	void SetIPAddress(const core::string& ip, int videoPort,bool rtcp)
	{
		m_ipAddr = ip;
		m_videoPort = videoPort;
		m_rtcp = rtcp;

		//set src and sinks elements
		_UpdatePorts();
	}
	bool CreateStream()
	{
		if (m_Loaded)
			return true;
		_BuildPipelineH264();

		GError *err = 0;
		m_gstPipeline = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			printf("GstNetworkVideoPlayer: Pipeline error: %s", err->message);
		}
		if (!m_gstPipeline)
			return false;

		printf("Connecting Video stream with IP:%s\n", m_ipAddr.c_str());

		_UpdatePorts();

		m_videoSink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(m_gstPipeline), "videoSink"));

		m_videoHandler.SetSink(m_videoSink);

		//setup video sink
		gst_base_sink_set_sync(GST_BASE_SINK(m_videoSink), true);
		g_object_set(G_OBJECT(m_videoSink), "emit-signals", FALSE, "sync", false, (void*)NULL);

		//attach videosink callbacks
		GstAppSinkCallbacks gstCallbacks;
		gstCallbacks.eos = &VideoAppSinkHandler::on_eos_from_source;
		gstCallbacks.new_preroll = &VideoAppSinkHandler::on_new_preroll_from_source;
#if GST_VERSION_MAJOR==0
		gstCallbacks.new_buffer = &VideoAppSinkHandler::on_new_buffer_from_source;
#else
		gstCallbacks.new_sample = &VideoAppSinkHandler::on_new_buffer_from_source;
#endif
		gst_app_sink_set_callbacks(GST_APP_SINK(m_videoSink), &gstCallbacks, &m_videoHandler, NULL);


		return CreatePipeline();

	}

	bool IsStream()
	{
		return true;
	}

	virtual void Close()
	{
		if (m_videoSrc && m_videoSrc->m_client)
			m_videoSrc->m_client->Close();
		GstPipelineHandler::Close();
		m_videoHandler.Close();
	}

	void Play()
	{
		SetPaused(false);
	}

	void Pause()
	{
		SetPaused(true);
	}
	void SetVolume(float vol)
	{
		g_object_set(G_OBJECT(m_gstPipeline), "volume", (gdouble)vol, (void*)0);
	}


	virtual const math::vector2di& GetFrameSize()
	{
		return m_videoHandler.GetFrameSize();
	}

	virtual video::EPixelFormat GetImageFormat()
	{
		return m_videoHandler.getPixelsRef()->format;
	}

	virtual bool GrabFrame(){ return m_videoHandler.GrabFrame(); }
	virtual bool HasNewFrame(){ return m_videoHandler.isFrameNew(); }
	virtual ulong GetBufferID(){ return m_videoHandler.GetFrameID(); }

	virtual float GetCaptureFrameRate(){ return m_videoHandler.GetCaptureFrameRate(); }

	virtual const ImageInfo* GetLastFrame(){ return m_videoHandler.getPixelsRef(); }
};


GstNetworkVideoPlayer::GstNetworkVideoPlayer()
{
	m_impl = new GstNetworkVideoPlayerImpl();
}

GstNetworkVideoPlayer::~GstNetworkVideoPlayer()
{
	delete m_impl;
}
void GstNetworkVideoPlayer::SetIPAddress(const core::string& ip, int videoPort, bool rtcp)
{
	m_impl->SetIPAddress(ip, videoPort,rtcp);
}
bool GstNetworkVideoPlayer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstNetworkVideoPlayer::SetVolume(float vol)
{
	m_impl->SetVolume(vol);
}
bool GstNetworkVideoPlayer::IsStream()
{
	return m_impl->IsStream();
}

void GstNetworkVideoPlayer::Play()
{
	m_impl->Play();
}
void GstNetworkVideoPlayer::Stop()
{
	m_impl->Stop();
}
void GstNetworkVideoPlayer::Pause()
{
	m_impl->Pause();
}
bool GstNetworkVideoPlayer::IsLoaded()
{
	return m_impl->IsLoaded();
}
bool GstNetworkVideoPlayer::IsPlaying()
{
	return m_impl->IsPlaying();
}
void GstNetworkVideoPlayer::Close()
{
	m_impl->Close();

}

const math::vector2di& GstNetworkVideoPlayer::GetFrameSize()
{
	return m_impl->GetFrameSize();
}

video::EPixelFormat GstNetworkVideoPlayer::GetImageFormat()
{
	return m_impl->GetImageFormat();
}

bool GstNetworkVideoPlayer::GrabFrame()
{
	return m_impl->GrabFrame();
}

bool GstNetworkVideoPlayer::HasNewFrame()
{
	return m_impl->HasNewFrame();
}

ulong GstNetworkVideoPlayer::GetBufferID()
{
	return m_impl->GetBufferID();
}

float GstNetworkVideoPlayer::GetCaptureFrameRate()
{
	return m_impl->GetCaptureFrameRate();
}

const ImageInfo* GstNetworkVideoPlayer::GetLastFrame()
{
	return m_impl->GetLastFrame();
}


}
}


