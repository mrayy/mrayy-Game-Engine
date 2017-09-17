

#include "stdafx.h"
#include "CustomAudioGrabber.h"

#include "StringConverter.h"
#include "ILogManager.h"
#include "GstPipelineHandler.h"
#include "AudioAppSinkHandler.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

namespace mray
{
namespace video
{
class CustomAudioGrabberImpl :public GstPipelineHandler
{
protected:
	core::string pipeline;
	int channels;
	int samplingrate;

	GstAppSink* m_audioSink;
	AudioAppSinkHandler audiohandler;

	std::vector<float> frame;

public:
	CustomAudioGrabberImpl()
	{
	}
	virtual ~CustomAudioGrabberImpl(){
		Close();
	}


	void Init(const core::string &pipeline, int channels, int samplingrate)
	{

		this->pipeline = pipeline;
		this->channels = channels;
		this->samplingrate = samplingrate;
	}

	core::string BuildPipeline()
	{

		core::string audioStr =pipeline;

		audioStr += " ! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=" + core::StringConverter::toString(samplingrate) + ",channels=" + core::StringConverter::toString(channels) + " "
			//" ! audiochebband mode=band-pass lower-frequency=1000 upper-frequency=6000 poles=4 "
			" ! audioconvert ! appsink name=sink";

		return audioStr;
	}

	bool Start()
	{
		if (GetPipeline()!=0 && IsPaused())
		{
			SetPaused(false);
			return true;
		}
		GError *err = 0;
		core::string pipline = BuildPipeline();
		GstElement* p = gst_parse_launch(pipline.c_str(), &err);
		gLogManager.log("CustomAudioGrabber::Starting with pipeline: " + pipline, ELL_INFO);
		if (err)
		{
			gLogManager.log("CustomAudioGrabber: Pipeline error: " + core::string(err->message), ELL_WARNING);
			gst_object_unref(p);
			p = 0;
		}
		if (!p)
			return false;
		gLogManager.log("CustomAudioGrabber::Finished Linking Pipeline", ELL_INFO);
		SetPipeline(p);

		if (!CreatePipeline())
			return false;

		m_audioSink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(p), "sink"));
		audiohandler.SetSink(m_audioSink);

		//g_signal_connect(m_audioSink, "new-sample", G_CALLBACK(new_buffer), this);
		//attach videosink callbacks
		GstAppSinkCallbacks gstCallbacks;
		gstCallbacks.eos = &AudioAppSinkHandler::on_eos_from_source;
		gstCallbacks.new_preroll = &AudioAppSinkHandler::on_new_preroll_from_source;
#if GST_VERSION_MAJOR==0
		gstCallbacks.new_buffer = &AudioAppSinkHandler::on_new_buffer_from_source;
#else
		gstCallbacks.new_sample = &AudioAppSinkHandler::on_new_buffer_from_source;
#endif
		gst_app_sink_set_callbacks(GST_APP_SINK(m_audioSink), &gstCallbacks, &audiohandler, NULL);
		//gst_app_sink_set_emit_signals(GST_APP_SINK(m_videoSink), true);


		// 		gst_base_sink_set_async_enabled(GST_BASE_SINK(m_videoSink), TRUE);
// 		gst_base_sink_set_sync(GST_BASE_SINK(m_audioSink), false);
// 		gst_app_sink_set_drop(GST_APP_SINK(m_audioSink), TRUE);
// 		gst_app_sink_set_max_buffers(GST_APP_SINK(m_audioSink), 8);
// 		gst_base_sink_set_max_lateness(GST_BASE_SINK(m_audioSink), 0);

		SetPaused(false);


		return true;
	}
	void Pause()
	{
		SetPaused(true);
	}
	void Close()
	{
		GstPipelineHandler::Close();
	}
	bool IsStarted()
	{
		return GstPipelineHandler::IsPlaying();
	}

	uint GetSamplingRate()
	{
		return samplingrate;
	}
	uint GetChannelsCount()
	{
		return channels;
	}

	bool GrabFrame()
	{
		if (audiohandler.GrabFrame() && audiohandler.GetFrameSize() > 0)
		{
			frame.resize(audiohandler.GetFrameSize());
			audiohandler.CopyAudioFrame(&frame[0]);
			return true;
		}
		return false;
	}
	uchar* GetAudioFrame()
	{
		return (uchar*)&frame[0];
	}
	uint GetAudioFrameSize()
	{
		return frame.size()*sizeof(float);
	}

};



CustomAudioGrabber::CustomAudioGrabber()
{
	m_impl = new CustomAudioGrabberImpl();
}
CustomAudioGrabber::~CustomAudioGrabber()
{
	delete m_impl;
}

void CustomAudioGrabber::Init(const core::string &pipeline, int channels, int samplingrate)
{
	m_impl->Init(pipeline, channels, samplingrate);
}

bool CustomAudioGrabber::Start()
{
	return  m_impl->Start();
}
void CustomAudioGrabber::Pause()
{
	m_impl->Pause();
}
void CustomAudioGrabber::Close()
{
	m_impl->Close();
}
bool CustomAudioGrabber::IsStarted()
{
	return m_impl->IsStarted();
}

uint CustomAudioGrabber::GetSamplingRate()
{
	return m_impl->GetSamplingRate();
}
uint CustomAudioGrabber::GetChannelsCount()
{
	return m_impl->GetChannelsCount();
}

bool CustomAudioGrabber::GrabFrame()
{
	return m_impl->GrabFrame();
}
uchar* CustomAudioGrabber::GetAudioFrame()
{
	return m_impl->GetAudioFrame();
}
uint CustomAudioGrabber::GetAudioFrameSize()
{
	return m_impl->GetAudioFrameSize();
}

}
}

