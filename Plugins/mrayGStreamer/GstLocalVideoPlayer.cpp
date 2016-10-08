

#include "stdafx.h"
#include "GstLocalVideoPlayer.h"

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

class GstLocalVideoPlayerImpl :public GstPipelineHandler
{
	core::string m_pipeLineString;

	int m_cam0, m_cam1;
	int m_fps;
	math::vector2di m_frameSize;

	VideoAppSinkHandler m_videoHandler;
	GstAppSink* m_videoSink;

public:
	GstLocalVideoPlayerImpl()
	{
		m_cam0 = 0;
		m_cam1 = 1;
		m_fps = 30;
		m_frameSize.set(1280, 720);

		m_videoSink = 0;

	}
	virtual ~GstLocalVideoPlayerImpl()
	{

	}

	void _BuildPipelineUncompressed()
	{
		std::stringstream ss;
		if (m_cam0 == m_cam1)
		{
			ss << "ksvideosrc device-index=" << m_cam0 << " ! video/x-raw,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
			ss << "! videorate ! video/x-raw,framerate=" << m_fps << "/1 ";

		}
		else
		{

			int halfW = m_frameSize.x / 2;
			ss << "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos="<<halfW<<"   sink_2::ypos=0  sink_2::zorder=1  ";

			ss << "videotestsrc pattern=\"black\" ! video/x-raw,format=I420,width=" << m_frameSize.x << ",height=" << m_frameSize.y << " !  mix.sink_0 ";

			//first camera
			ss << "ksvideosrc name=src1 device-index=" << m_cam0 << "  ! video/x-raw,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
			ss << "! videorate ! video/x-raw,framerate=" << m_fps << "/1 ";
			ss << "! videoscale !"
				"video/x-raw,format=I420,width=" << halfW << ",height=" << m_frameSize.y << " ! mix.sink_1 ";

			//second camera
			ss << "ksvideosrc name=src2 device-index=" << m_cam1 << "  ! video/x-raw,,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
			ss << "! videorate ! video/x-raw,framerate=" << m_fps << "/1 ";
			ss << "! videoscale !"
				"video/x-raw,format=I420,width=" << halfW << ",height=" << m_frameSize.y << " ! mix.sink_2 ";

			ss<< " mix. ";
		}

		{
			ss << "! videoconvert ! video/x-raw,format=I420 ! appsink name=videosink";

		}
		m_pipeLineString = ss.str();
	}


	void SetCameras(int cam0, int cam1)
	{
		m_cam0 = cam0;
		m_cam1 = cam1;
	}
	void SetResolution(int width, int height, int fps = 30)
	{
		m_frameSize.set(width, height);
		m_fps = fps;
	}

	void _BuildPipelineMJPEG()
	{
		std::stringstream ss;
		if (m_cam0 == m_cam1)
		{
			ss << "ksvideosrc device-index=" << m_cam0 << " ! image/jpeg,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
			//ss << "! videorate ! image/jpeg,framerate=" << m_fps << "/1";
			ss << " ! jpegdec ";

		}
		else
		{

			int halfW = m_frameSize.x / 2;
			ss << "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" << halfW << "   sink_2::ypos=0  sink_2::zorder=1  ";

			ss << "videotestsrc pattern=\"black\" ! video/x-raw,format=I420,width=" << m_frameSize.x << ",height=" << m_frameSize.y << " !  mix.sink_0 ";

			//first camera
			ss << "ksvideosrc name=src1 device-index=" << m_cam0 << "  ! image/jpeg,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
		//	ss << "! videorate ! image/jpeg,framerate=" << m_fps << "/1 ";
			ss << " ! jpegdec ! videoscale !"
				"video/x-raw,width=" << halfW << ",height=" << m_frameSize.y << " ! mix.sink_1 ";

			//second camera
			ss << "ksvideosrc name=src2 device-index=" << m_cam1 << "  ! image/jpeg,width=" << m_frameSize.x << ",height=" << m_frameSize.y;
			//ss << "! videorate ! image/jpeg,framerate=" << m_fps << "/1 ";
			ss << "!  jpegdec ! videoscale !"
				"video/x-raw,width=" << halfW << ",height=" << m_frameSize.y << " ! mix.sink_2 ";

			ss << " mix. ";
		}

		{
			ss << "! videoconvert ! video/x-raw,format=RGB ! appsink name=videosink";

		}
		m_pipeLineString = ss.str();
	}

	bool CreateStream()
	{
		if (IsLoaded())
			return true;
		_BuildPipelineMJPEG();

		GError *err = 0;
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			printf("GstLocalVideoPlayer: Pipeline error: %s", err->message);
		}
		if (!p)
			return false;
		SetPipeline(p);

		m_videoSink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(p), "videosink"));


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
		g_object_set(G_OBJECT(GetPipeline()), "volume", (gdouble)vol, (void*)0);
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


GstLocalVideoPlayer::GstLocalVideoPlayer()
{
	m_impl = new GstLocalVideoPlayerImpl();
}

GstLocalVideoPlayer::~GstLocalVideoPlayer()
{
	delete m_impl;
}
void GstLocalVideoPlayer::SetCameras(int cam0, int cam1)
{
	m_impl->SetCameras(cam0, cam1);
}
void GstLocalVideoPlayer::SetResolution(int width, int height, int fps)
{
	m_impl->SetResolution( width,  height,  fps);
}
bool GstLocalVideoPlayer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstLocalVideoPlayer::SetVolume(float vol)
{
	m_impl->SetVolume(vol);
}
bool GstLocalVideoPlayer::IsStream()
{
	return m_impl->IsStream();
}

void GstLocalVideoPlayer::Play()
{
	m_impl->Play();
}
void GstLocalVideoPlayer::Stop()
{
	m_impl->Stop();
}
void GstLocalVideoPlayer::Pause()
{
	m_impl->Pause();
}
bool GstLocalVideoPlayer::IsLoaded()
{
	return m_impl->IsLoaded();
}
bool GstLocalVideoPlayer::IsPlaying()
{
	return m_impl->IsPlaying();
}
void GstLocalVideoPlayer::Close()
{
	m_impl->Close();

}

GstPipelineHandler*GstLocalVideoPlayer::GetPipeline()
{
	return m_impl;
}


const math::vector2di& GstLocalVideoPlayer::GetFrameSize()
{
	return m_impl->GetFrameSize();
}

video::EPixelFormat GstLocalVideoPlayer::GetImageFormat()
{
	return m_impl->GetImageFormat();
}

bool GstLocalVideoPlayer::GrabFrame()
{
	return m_impl->GrabFrame();
}

bool GstLocalVideoPlayer::HasNewFrame()
{
	return m_impl->HasNewFrame();
}

ulong GstLocalVideoPlayer::GetBufferID()
{
	return m_impl->GetBufferID();
}

float GstLocalVideoPlayer::GetCaptureFrameRate()
{
	return m_impl->GetCaptureFrameRate();
}

const ImageInfo* GstLocalVideoPlayer::GetLastFrame()
{
	return m_impl->GetLastFrame();
}


}
}


