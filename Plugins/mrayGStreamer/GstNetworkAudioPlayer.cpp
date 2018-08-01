

#include "stdafx.h"
#include "GstNetworkAudioPlayer.h"

#include "ILogManager.h"

#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "CMySrc.h"
#include "CMySink.h"
#include "GstPipelineHandler.h"
#include "IFileSystem.h"
#include "StreamReader.h"
#include "AudioAppSinkHandler.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

namespace mray
{
namespace video
{

class GstNetworkAudioPlayerImpl :public GstPipelineHandler
{
	core::string m_ipAddr;
	uint m_audioPort;

	core::string m_pipeLineString;

	GstElement* m_audioSrc;
	GstMyUDPSrc* m_audioRtcpSrc;
	GstMyUDPSink* m_audioRtcpSink;
	int m_sampleRate;
	bool m_rtcp;

	bool m_customAudioInterface;
	GstAppSink* m_audioSink;
	AudioAppSinkHandler m_audioHandler;
public:
	GstNetworkAudioPlayerImpl()
	{
		m_ipAddr = "127.0.0.1";
		m_audioPort = 5005;
		m_audioSrc = 0;
		m_audioRtcpSrc = 0;
		m_audioRtcpSink = 0;
		m_sampleRate = 32000;
		m_customAudioInterface = false;
	}
	virtual ~GstNetworkAudioPlayerImpl()
	{

	}

#define OPUS_ENC
	void _BuildPipeline()
	{
#ifdef FLAC_ENC
		core::string audiocaps = " caps=application/x-rtp,media=application,clock-rate=90000,encoding-name=X-GST,payload=96 ";
		core::string audioStr = " rtpgstdepay ! audio/x-flac,rate=8000,channels=1! flacdec !  audioconvert ! audioresample  ";
#elif defined OPUS_ENC
		std::string audiocaps = "caps=application/x-rtp ";// ",media=(string)audio,clock-rate=48000,encoding-name=OPUS,payload=96,encoding-params=2 ";
		char buffer[128];
		sprintf(buffer, "%d", m_sampleRate);

		std::string audioStr = " queue ! rtpopusdepay ! opusdec plc=true ! audioconvert ! volume volume=1 ! audioresample  ! audio/x-raw,rate=";
		audioStr+=buffer;
		audioStr += " ";
#elif defined VORBIS_ENC
		std::string audiocaps = "caps=application/x-rtp,media=(string)audio,clock-rate=32000,encoding-name=VORBIS,payload=96,encoding-params=2 ";
		char buffer[128];
		sprintf(buffer, "%d", m_sampleRate);

		std::string audioStr = " rtpvorbisdepay ! vorbisdec ! audioconvert ! audioresample  ! audio/x-raw, rate=";
		audioStr+=buffer;
		audioStr += " ";
#elif defined SPEEX_ENC
		core::string audiocaps = "caps=application/x-rtp,media=(string)audio,clock-rate=(int)22000,encoding-name=(string)SPEEX ";
		core::string audioStr = " rtpspeexdepay ! speexdec ! audioconvert ! audioresample  ";
#else
		core::string audiocaps = "caps=application/x-rtp,media=(string)audio,clock-rate=(int)8000,encoding-name=(string)AMR,encoding-params=(string)1,octet-align=(string)1 ";
		core::string audioStr = " rtpamrdepay ! amrnbdec  ";
#endif
		if (m_rtcp)
		{
			m_pipeLineString =
				"rtpbin name=rtpbin "
				"udpsrc name=audioSrc "+ audiocaps +
				//audio rtp
				"! rtpbin.recv_rtp_sink_0 "

				"rtpbin. ! " + audioStr+ " ! directsoundsink "

				"udpsrc name=audioRtcpSrc ! rtpbin.recv_rtcp_sink_0 "

				//audio rtcp
				"rtpbin.send_rtcp_src_0 ! myudpsink name=audioRtcpSink sync=false async=false ";
		}
		else
		{
			m_pipeLineString =
				"udpsrc name=audioSrc " + audiocaps + "!" + audioStr;//
			if (m_customAudioInterface)
				m_pipeLineString += " ! appsink name=audioSink sync=false  emit-signals=false";
			else
				m_pipeLineString += " ! queue ! directsoundsink   ";// "buffer-time=200000 latency-time=100 sync=false" //buffer-time=80000//sync=false;

		}

	}
	void _UpdatePorts()
	{
		if (!GetPipeline())
			return;
//#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
//#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}


		m_audioSrc = gst_bin_get_by_name(GST_BIN(GetPipeline()), "audioSrc");
		g_object_set(m_audioSrc, "port", m_audioPort, 0);
		gLogManager.log("GstNetworkAudioPlayer::Started at port: " + core::StringConverter::toString(m_audioPort), ELL_INFO);
	//	SET_SRC(audioSrc, m_audioPort);
		if (m_rtcp)
		{
		//	SET_SINK(audioRtcpSink, (m_audioPort + 1));
		//	SET_SRC(audioRtcpSrc, (m_audioPort + 2));
		}

	}

	void SetIPAddress(const std::string& ip, uint audioPort, bool rtcp)
	{
		m_ipAddr = ip;
		m_audioPort = audioPort;
		m_rtcp = rtcp;

		//set src and sinks elements
		_UpdatePorts();
	}
	bool CreateStream()
	{
		if (IsLoaded())
			return true;

		_BuildPipeline();
		gLogManager.log("GstNetworkAudioPlayer::Starting with pipeline: " + m_pipeLineString, ELL_INFO);

		GError *err = 0;
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		gLogManager.log("Finished pipeline parsing", ELL_INFO);
		if (err)
		{
			gLogManager.log("GstNetworkAudioPlayer: Pipeline error: " + core::string(err->message), ELL_WARNING);
		}
		if (!p)
			return false;
		gLogManager.log("GstNetworkAudioPlayer::Finished Linking Pipeline", ELL_INFO);
		SetPipeline(p);
		_UpdatePorts();

		if (m_customAudioInterface)
		{

			m_audioSink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(p), "audioSink"));
			m_audioHandler.SetSink(m_audioSink);

			g_signal_connect(m_audioSink, "new-sample", G_CALLBACK(new_buffer), this);
			//attach videosink callbacks
			GstAppSinkCallbacks gstCallbacks;
			gstCallbacks.eos = &AudioAppSinkHandler::on_eos_from_source;
			gstCallbacks.new_preroll = &AudioAppSinkHandler::on_new_preroll_from_source;
#if GST_VERSION_MAJOR==0
			gstCallbacks.new_buffer = &AudioAppSinkHandler::on_new_buffer_from_source;
#else
			gstCallbacks.new_sample = &AudioAppSinkHandler::on_new_buffer_from_source;
#endif
			gst_app_sink_set_callbacks(GST_APP_SINK(m_audioSink), &gstCallbacks, &m_audioHandler, NULL);
			//gst_app_sink_set_emit_signals(GST_APP_SINK(m_videoSink), true);


			// 		gst_base_sink_set_async_enabled(GST_BASE_SINK(m_videoSink), TRUE);
			gst_base_sink_set_sync(GST_BASE_SINK(m_audioSink), false);
			gst_app_sink_set_drop(GST_APP_SINK(m_audioSink), TRUE);
			gst_app_sink_set_max_buffers(GST_APP_SINK(m_audioSink), 8);
			gst_base_sink_set_max_lateness(GST_BASE_SINK(m_audioSink), 0);
		}

		gLogManager.log("NetworkAudioPlayer:CreateStream() - Pipeline created", ELL_INFO);
		return CreatePipeline();

	}

	bool IsStream()
	{
		return true;
	}

	virtual void Close()
	{
//		if (m_audioSrc && m_audioSrc->m_client)
//			m_audioSrc->m_client->Close();
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
	int GetPort()
	{
	//	if (m_audioSrc && m_audioSrc->m_client)
	//		return m_audioSrc->m_client->Port();//return the generated port
	//	else 
		return m_audioPort;
	}

	virtual void UseCustomAudioInterface(bool use)
	{
		m_customAudioInterface = use;
	}
	virtual bool IsUsingCustomAudioInterface()
	{
		return m_customAudioInterface;
	}


	static GstFlowReturn new_buffer(GstElement *sink, void *data) {
		GstBuffer *buffer;
		GstMapInfo map;
		GstSample *sample;
		//gsize size;


		g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
		if (sample) {

			buffer = gst_sample_get_buffer(sample);

			gst_buffer_map(buffer, &map, GST_MAP_READ);

			//	fwrite(map.data, 1, map.size, data->audio_file);

			gst_buffer_unmap(buffer, &map);
			gst_sample_unref(sample);

		}

		return GST_FLOW_OK;
	}

};


GstNetworkAudioPlayer::GstNetworkAudioPlayer()
{
	m_impl = new GstNetworkAudioPlayerImpl();
}

GstNetworkAudioPlayer::~GstNetworkAudioPlayer()
{
	delete m_impl;
}
void GstNetworkAudioPlayer::SetIPAddress(const std::string& ip, uint audioPort, bool rtcp)
{
	m_impl->SetIPAddress(ip, audioPort, rtcp);
}
bool GstNetworkAudioPlayer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstNetworkAudioPlayer::SetVolume(float vol)
{
	m_impl->SetVolume(vol);
}
bool GstNetworkAudioPlayer::IsStream()
{
	return m_impl->IsStream();
}
GstPipelineHandler*GstNetworkAudioPlayer::GetPipeline()
{
	return m_impl;
}


void GstNetworkAudioPlayer::Play()
{
	m_impl->Play();
}
void GstNetworkAudioPlayer::Stop()
{
	m_impl->Stop();
}
void GstNetworkAudioPlayer::Pause()
{
	m_impl->Pause();
}
bool GstNetworkAudioPlayer::IsLoaded()
{
	return m_impl->IsLoaded();
}
bool GstNetworkAudioPlayer::IsPlaying()
{
	return m_impl->IsPlaying();
}
void GstNetworkAudioPlayer::Close()
{
	m_impl->Close();

}

int GstNetworkAudioPlayer::GetPort(int i){
	return m_impl->GetPort();
}

}
}


