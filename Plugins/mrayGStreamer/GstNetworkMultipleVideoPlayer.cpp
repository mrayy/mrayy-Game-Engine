

#include "stdafx.h"
#include "GstNetworkMultipleVideoPlayer.h"

#include "ILogManager.h"

#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "CMySrc.h"
#include "CMySink.h"

#include "VideoAppSinkHandler.h"
#include "GstPipelineHandler.h"
#include "Engine.h"
#include "ITimer.h"
#include "FPSCalc.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

namespace mray
{
namespace video
{


class GstNetworkMultipleVideoPlayerImpl :public GstPipelineHandler,IPipelineListener
{
	GstNetworkMultipleVideoPlayer* m_owner;
	core::string m_ipAddr;
	uint m_baseVideoSrc;
	int m_playersCount;

	core::string m_pipeLineString;


	GstMyUDPSrc* m_videoRtcpSrc;
	GstMyUDPSink* m_videoRtcpSink;
	bool m_rtcp;

	struct VideoHandler
	{
		VideoHandler()
		{
			videoPort = 5000;
			videoSrc = 0;
			videoSink = 0;
		}
		VideoAppSinkHandler handler;
		uint videoPort;

		GstElement* videoSrc;
		GstAppSink* videoSink;
	};

	std::vector<VideoHandler> m_videoHandler;

public:
	GstNetworkMultipleVideoPlayerImpl(GstNetworkMultipleVideoPlayer* o)
	{
		m_owner = o;
		m_ipAddr = "127.0.0.1";
		m_videoRtcpSrc = 0;
		m_videoRtcpSink = 0;
		m_playersCount = 0;
		m_videoHandler.resize(1);
		AddListener(this);
	}
	virtual ~GstNetworkMultipleVideoPlayerImpl()
	{

	}

	core::string _BuildPipelineH264(int i)
	{
		core::string videoStr =
			//video rtp
			"myudpsrc name=videoSrc"+core::StringConverter::toString(i)+" !"
			//"udpsrc port=7000 buffer-size=2097152 do-timestamp=true !"
			"application/x-rtp ";
		if (m_rtcp)
		{
			videoStr =
				"rtpbin "
				"name=rtpbin "
				+ videoStr +
				"! rtpbin.recv_rtp_sink_0 "
				"rtpbin. ! rtph264depay !  avdec_h264 ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink" + core::StringConverter::toString(i) +

				//video rtcp
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 "
				"rtpbin.send_rtcp_src_0 !  myudpsink name=videoRtcpSink sync=false async=false ";
		}
		else
		{
			videoStr = videoStr + "! queue !"
				"rtpjitterbuffer !  "
				"rtph264depay ! h264parse !  avdec_h264 ! "
				// " videorate  ! "//"video/x-raw,framerate=60/1 ! "
				//	"videoconvert ! video/x-raw,format=RGB  !" // Very slow!!
				"videorate ! videoconvert ! video/x-raw,format=I420  !"
			//	" timeoverlay halignment=right text=\"Local Time =\"! "
			" appsink name=videoSink" + core::StringConverter::toString(i) + " sync=false  emit-signals=false ";
				//"fpsdisplaysink sync=false";

		}
		return videoStr;
	}
	core::string _BuildPipelineVP8(int i)
	{
		core::string videoStr =
			//video rtp
			"myudpsrc name=videoSrc" + core::StringConverter::toString(i) + " !"
			//"udpsrc port=7000 buffer-size=2097152 do-timestamp=true !"
			"application/x-rtp ";
		if (m_rtcp)
		{
			videoStr =
				"rtpbin "
				"name=rtpbin "
				+ videoStr +
				"! rtpbin.recv_rtp_sink_0 "
				"rtpbin. ! rtph264depay !  avdec_h264 ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink" + core::StringConverter::toString(i) +

				//video rtcp
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 "
				"rtpbin.send_rtcp_src_0 !  myudpsink name=videoRtcpSink sync=false async=false ";
		}
		else
		{
			videoStr = videoStr + "! queue !"
				"rtpjitterbuffer !  "
				"rtph264depay ! h264parse !  avdec_h264 ! "
				// " videorate  ! "//"video/x-raw,framerate=60/1 ! "
				//	"videoconvert ! video/x-raw,format=RGB  !" // Very slow!!
				"videorate ! videoconvert ! video/x-raw,format=I420  !"
				//	" timeoverlay halignment=right text=\"Local Time =\"! "
				" appsink name=videoSink" + core::StringConverter::toString(i) + " sync=false  emit-signals=false ";
			//"fpsdisplaysink sync=false";

		}
		/* ksvideosrc ! video/x-raw,width=1280,height=720 !
 videorate max-rate=40 ! videoconvert ! x264enc bitrate=1024 speed-preset=superf
 ast pass=cbr tune=zerolatency sync-lookahead=0 rc-lookahead=0 sliced-threads=tru
 e key-int-max=20 ! rtph264pay ! udpsink port=5001 host=127.0.0.1 sync=false -v*/
		return videoStr;
	}


	core::string _BuildPipelineMJPEG(int i)
	{
		core::string videoStr =
			//video rtp
			"udpsrc "
			"name=videoSrc" + core::StringConverter::toString(i) + 
			"caps=application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)JPEG ";
		if (m_rtcp)
		{
			videoStr =
				"rtpbin "
				"name=rtpbin "
				+ videoStr +
				"! rtpbin.recv_rtp_sink_0 "
				"rtpbin. ! rtpjpegdepay !  jpegdec ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink" + core::StringConverter::toString(i) + 

				//video rtcp
				"myudpsrc name=videoRtcpSrc ! rtpbin.recv_rtcp_sink_0 "
				"rtpbin.send_rtcp_src_0 !  myudpsink name=videoRtcpSink sync=false async=false ";
		}
		else
		{
			videoStr = videoStr + "!"
				"rtpjpegdepay !  jpegdec ! "
				"videorate ! "
				"videoconvert ! video/x-raw,format=RGB  !"
				" appsink name=videoSink" + core::StringConverter::toString(i) + " sync=false";

		}
		return videoStr;
	}

	void _BuildPipeline()
	{
		m_pipeLineString = "";
		for (int i = 0; i < m_playersCount; ++i)
			m_pipeLineString+= _BuildPipelineH264(i)+" ";
	}

	void _UpdatePorts()
	{
		if (!GetPipeline())
			return;
#define SET_SRC(idx,name,p) m_videoHandler[idx].videoSrc=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), name)); if(m_videoHandler[idx].videoSrc){m_videoHandler[idx].videoSrc->SetPort(p);}
#define SET_SINK(idx,name,p) m_videoHandler[idx].videoSink=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), name)); if(m_videoHandler[idx].videoSink){m_videoHandler[idx].videoSink->SetPort(m_ipAddr,p);}

		for (int i = 0; i < m_playersCount; ++i)
		{
			core::string name = ("videoSrc" + core::StringConverter::toString(i));
			m_videoHandler[i].videoSrc= gst_bin_get_by_name(GST_BIN(GetPipeline()), name.c_str());
			if (m_videoHandler[i].videoSrc)
				g_object_set(m_videoHandler[i].videoSrc, "port", m_videoHandler[i].videoPort, "host", m_ipAddr.c_str(), 0);

//			SET_SRC(i, name.c_str(), m_videoHandler[i].videoPort);
// 			SET_SINK(i, videoRtcpSink, (m_videoHandler[i].videoPort + 1));
// 			SET_SRC(i, videoRtcpSrc, (m_videoHandler[i].videoPort + 2));
		}
	}

	void SetIPAddress(const std::string& ip, uint videoPort, int count, bool rtcp)
	{
		m_playersCount = count;
		m_ipAddr = ip;
		m_baseVideoSrc= videoPort;
		m_rtcp = rtcp;

		m_videoHandler.resize(m_playersCount);
		for (int i = 0; i < m_playersCount; ++i)
		{
			m_videoHandler[i].videoPort = m_baseVideoSrc + i;
		}

		//set src and sinks elements
		_UpdatePorts();
	}

	
	static GstFlowReturn new_buffer(GstElement *sink, void *data) {
		GstBuffer *buffer;
		GstMapInfo map;
		GstSample *sample;
		//gsize size;

		static core::FPSCalc m_fpsCounter;


		g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
		if (sample) {

			m_fpsCounter.regFrame(gEngine.getTimer()->getSeconds());
			buffer = gst_sample_get_buffer(sample);

			gst_buffer_map(buffer, &map, GST_MAP_READ);

			g_print("FPS=%d , Data Size=%d\n", (int)m_fpsCounter.getFPS(),(int)map.size);

		//	fwrite(map.data, 1, map.size, data->audio_file);

			gst_buffer_unmap(buffer, &map);
			gst_sample_unref(sample);

		}

		return GST_FLOW_OK;
	}

	bool CreateStream()
	{
		if (IsLoaded())
			return true;
		_BuildPipeline();

		GError *err = 0;
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			gLogManager.log("GstNetworkMultipleVideoPlayer: Pipeline error: "+ core::string(err->message),ELL_WARNING);
		}
		if (!p)
			return false;
		SetPipeline(p);
		printf("Connecting Video stream with IP:%s\n", m_ipAddr.c_str());

		_UpdatePorts();

		for (int i = 0; i < m_playersCount; ++i)
		{
			core::string name = "videoSink" + core::StringConverter::toString(i);
			m_videoHandler[i].videoSink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(p), name.c_str()));

			m_videoHandler[i].handler.SetSink(m_videoHandler[i].videoSink);
			g_signal_connect(m_videoHandler[i].videoSink, "new-sample", G_CALLBACK(new_buffer), this);
			//attach videosink callbacks
			GstAppSinkCallbacks gstCallbacks;
			gstCallbacks.eos = &VideoAppSinkHandler::on_eos_from_source;
			gstCallbacks.new_preroll = &VideoAppSinkHandler::on_new_preroll_from_source;
	#if GST_VERSION_MAJOR==0
			gstCallbacks.new_buffer = &VideoAppSinkHandler::on_new_buffer_from_source;
	#else
			gstCallbacks.new_sample = &VideoAppSinkHandler::on_new_buffer_from_source;
	#endif
			gst_app_sink_set_callbacks(GST_APP_SINK(m_videoHandler[i].videoSink), &gstCallbacks, &m_videoHandler[i].handler, NULL);
			//gst_app_sink_set_emit_signals(GST_APP_SINK(m_videoSink), true);

		
			// 		gst_base_sink_set_async_enabled(GST_BASE_SINK(m_videoSink), TRUE);
			gst_base_sink_set_sync(GST_BASE_SINK(m_videoHandler[i].videoSink), false);
			gst_app_sink_set_drop(GST_APP_SINK(m_videoHandler[i].videoSink), TRUE);
			gst_app_sink_set_max_buffers(GST_APP_SINK(m_videoHandler[i].videoSink), 8);
			gst_base_sink_set_max_lateness(GST_BASE_SINK(m_videoHandler[i].videoSink), 0);
			/*
			GstCaps* caps;
			caps = gst_caps_new_simple("video/x-raw",
				"format", G_TYPE_STRING, "RGB",
				"pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
				"width", G_TYPE_INT, 1280,
				"height", G_TYPE_INT, 720,
				NULL);

			gst_app_sink_set_caps(GST_APP_SINK(m_videoSink), caps);
			gst_caps_unref(caps);
			// Set the configured video appsink to the main pipeline
			g_object_set(m_gstPipeline, "video-sink", m_videoSink, (void*)NULL);
			// Tell the video appsink that it should not emit signals as the buffer retrieving is handled via callback methods
			g_object_set(m_videoSink, "emit-signals", false, "sync", false, "async", false, (void*)NULL);
			*/

		}

		return CreatePipeline();

	}

	bool IsStream()
	{
		return true;
	}

	virtual void Close()
	{
		for (int i = 0; i < m_playersCount; ++i)
		{
/*			if (m_videoHandler[i].videoSrc && m_videoHandler[i].videoSrc->m_client)
				m_videoHandler[i].videoSrc->m_client->Close();*/
			m_videoHandler[i].handler.Close();
		}
		GstPipelineHandler::Close();
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
		g_object_set(G_OBJECT(GetPipeline()), "volume", (gdouble)vol, (void*)0);
	}
	virtual int GetPort(int i)
	{
		if (i >= m_videoHandler.size())return 0;
		if (m_videoHandler[i].videoSrc)
		{
			gint port;
			g_object_get(m_videoHandler[i].videoSrc, "port", &port,0);
			return port;
		}else
		return m_videoHandler[i].videoPort;
	}




	virtual const math::vector2di& GetFrameSize(int i)
	{
		return m_videoHandler[i].handler.GetFrameSize();
	}

	virtual video::EPixelFormat GetImageFormat(int i)
	{
		return m_videoHandler[i].handler.getPixelsRef()->format;
	}
	int GetFramesCount()
	{
		return m_playersCount;
	}
	virtual bool GrabFrame(int i)
	{ 
		if (i >= m_playersCount)
			return false;
		return m_videoHandler[i].handler.GrabFrame();
	}
	virtual bool HasNewFrame(int i)
	{
		if (i >= m_playersCount)
			return false;
		return m_videoHandler[i].handler.isFrameNew();
	}
	virtual ulong GetBufferID(int i)
	{
		if (i >= m_playersCount)
			return 0;
		return m_videoHandler[i].handler.GetFrameID();
	}

	virtual float GetCaptureFrameRate(int i){ 
		if (i>m_videoHandler.size())
			return 0;
		return m_videoHandler[i].handler.GetCaptureFrameRate(); 
	}

	virtual const ImageInfo* GetLastFrame(int i)
	{ 
		if (i > m_playersCount)
			return 0;
		else
			return m_videoHandler[i].handler.getPixelsRef(); 
	}

	const GstImageFrame* GetLastDataFrame(int i)
	{
		if (i > m_playersCount)
			return 0;
		else
			return m_videoHandler[i].handler.getPixelFrame();
	}

	virtual void OnPipelineReady(GstPipelineHandler* p){ m_owner->__FIRE_OnPlayerReady(m_owner); }
	virtual void OnPipelinePlaying(GstPipelineHandler* p){ m_owner->__FIRE_OnPlayerStarted(m_owner); }
	virtual void OnPipelineStopped(GstPipelineHandler* p){ m_owner->__FIRE_OnPlayerStopped(m_owner); }

};


GstNetworkMultipleVideoPlayer::GstNetworkMultipleVideoPlayer()
{
	m_impl = new GstNetworkMultipleVideoPlayerImpl(this);
}

GstNetworkMultipleVideoPlayer::~GstNetworkMultipleVideoPlayer()
{
	delete m_impl;
}
void GstNetworkMultipleVideoPlayer::SetIPAddress(const std::string& ip, uint videoPort, uint count, bool rtcp)
{
	m_impl->SetIPAddress(ip, videoPort,count,rtcp);
}
bool GstNetworkMultipleVideoPlayer::CreateStream()
{
	return m_impl->CreateStream();
}
GstPipelineHandler*GstNetworkMultipleVideoPlayer::GetPipeline()
{
	return m_impl;
}


void GstNetworkMultipleVideoPlayer::SetVolume(float vol)
{
	m_impl->SetVolume(vol);
}
bool GstNetworkMultipleVideoPlayer::IsStream()
{
	return m_impl->IsStream();
}

void GstNetworkMultipleVideoPlayer::Play()
{
	m_impl->Play();
}
void GstNetworkMultipleVideoPlayer::Stop()
{
	m_impl->Stop();
}
void GstNetworkMultipleVideoPlayer::Pause()
{
	m_impl->Pause();
}
bool GstNetworkMultipleVideoPlayer::IsLoaded()
{
	return m_impl->IsLoaded();
}
bool GstNetworkMultipleVideoPlayer::IsPlaying()
{
	return m_impl->IsPlaying();
}
void GstNetworkMultipleVideoPlayer::Close()
{
	m_impl->Close();

}

const math::vector2di& GstNetworkMultipleVideoPlayer::GetFrameSize()
{
	return m_impl->GetFrameSize(0);
}

video::EPixelFormat GstNetworkMultipleVideoPlayer::GetImageFormat()
{
	return m_impl->GetImageFormat(0);
}

int GstNetworkMultipleVideoPlayer::GetFramesCount()
{
	return m_impl->GetFramesCount();
}

bool GstNetworkMultipleVideoPlayer::GrabFrame(int i)
{
	return m_impl->GrabFrame(i);
}

bool GstNetworkMultipleVideoPlayer::HasNewFrame(int i)
{
	return m_impl->HasNewFrame(i);
}

ulong GstNetworkMultipleVideoPlayer::GetBufferID(int i)
{
	return m_impl->GetBufferID(i);
}

float GstNetworkMultipleVideoPlayer::GetCaptureFrameRate(int i)
{
	return m_impl->GetCaptureFrameRate(i);
}

const ImageInfo* GstNetworkMultipleVideoPlayer::GetLastFrame(int i)
{
	return m_impl->GetLastFrame(i);
}

const GstImageFrame* GstNetworkMultipleVideoPlayer::GetLastDataFrame(int i)
{
	return m_impl->GetLastDataFrame(i);
}

int GstNetworkMultipleVideoPlayer::GetPort(int i)
{
	return m_impl->GetPort(i);
}

}
}


