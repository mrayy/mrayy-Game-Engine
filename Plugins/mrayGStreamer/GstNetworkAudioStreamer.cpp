

#include "stdafx.h"
#include "GstNetworkAudioStreamer.h"

#include "GstPipelineHandler.h"

#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"





namespace mray
{
namespace video
{



class GstNetworkAudioStreamerImpl :public GstPipelineHandler
{
protected:
	core::string m_ipAddr;
	uint m_audioPort;
	uint m_clockPort;

	core::string m_pipeLineString;
	GstMyUDPSink* m_audioSink;
	GstMyUDPSink* m_audioRtcpSink;
	GstMyUDPSrc* m_audioRtcpSrc;
	bool m_rtcp;
public:
	GstNetworkAudioStreamerImpl()
	{
		m_audioSink = 0;
		m_audioRtcpSink = 0;
		m_audioRtcpSrc = 0;
		m_ipAddr = "127.0.0.1";
		m_audioPort = 5005;
		m_clockPort = 5010;
	}
	virtual ~GstNetworkAudioStreamerImpl()
	{
	}
#define VORBIS_ENC
	void BuildString()
	{
#ifdef FLAC_ENC

		core::string audioStr = "directsoundsrc! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1 ! audioconvert ! flacenc quality=2 ! rtpgstpay ";
#else 
#ifdef VORBIS_ENC
		core::string audioStr = "directsoundsrc! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=22000,channels=2   ! audioconvert ! "
			"audiochebband mode=band-pass lower-frequency=1000 upper-frequency=4000 type=2 ! "
			"vorbisenc quality=0.7 ! rtpvorbispay config-interval=3 ";
#else
#ifdef SPEEX_ENC
		core::string audioStr = "directsoundsrc! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=22000,channels=2   ! speexenc ! rtpspeexpay";
#else
		core::string audioStr = "directsoundsrc! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1   ! amrnbenc ! rtpamrpay";
#endif
#endif
#endif
		if (m_rtcp)
		{
			m_pipeLineString = "rtpbin  name=rtpbin " + audioStr + " ! "
				" rtpbin.send_rtp_sink_0 "

				"rtpbin.send_rtp_src_0 ! "
				"myudpsink name=audioSink  "

				"rtpbin.send_rtcp_src_0 ! "
				"myudpsink name=audioRtcpSink sync=false async=false "
				"myudpsrc name=audioRtcpSrc ! rtpbin.recv_rtcp_sink_0 ";
		}
		else
		{
			m_pipeLineString = audioStr + " ! "
				"myudpsink name=audioSink  ";

		}


	}
	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}


		SET_SINK(audioSink, m_audioPort);
		SET_SRC(audioRtcpSrc, (m_audioPort + 1));
		SET_SINK(audioRtcpSink, (m_audioPort + 2));

	}

	// addr: target address to stream video to
	// audioport: port for the audio stream , audio rtcp is allocated as audioPort+1 and audioPort+2
	void BindPorts(const core::string& addr, uint audioPort,uint clockPort, bool rtcp)
	{
		m_ipAddr = addr;
		m_audioPort = audioPort;
		m_clockPort = clockPort;
		m_rtcp = rtcp;
		_UpdatePorts();
	}

	bool CreateStream()
	{
		GError *err = 0;
		BuildString();
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			printf("GstAudioNetworkStreamer: Pipeline error: %s", err->message);
		}
		if (!p)
			return false;
		SetPipeline(p);
		_UpdatePorts();

		return CreatePipeline(true, "", m_clockPort);

	}
	void Stream()
	{
		SetPaused(false);
	}
	bool IsStreaming()
	{
		return IsPlaying();
	}
	virtual void Close()
	{
		GstPipelineHandler::Close();
	}

};


GstNetworkAudioStreamer::GstNetworkAudioStreamer()
{
	m_impl = new GstNetworkAudioStreamerImpl();
}

GstNetworkAudioStreamer::~GstNetworkAudioStreamer()
{
	delete m_impl;
}
void GstNetworkAudioStreamer::Stream()
{
	m_impl->Stream();
}
void GstNetworkAudioStreamer::Stop()
{
	m_impl->Stop();
}
GstPipelineHandler* GstNetworkAudioStreamer::GetPipeline()
{
	return m_impl;
}

void GstNetworkAudioStreamer::BindPorts(const core::string& addr, uint* ports, uint count, uint clockPort, bool rtcp)
{
	m_impl->BindPorts(addr, ports[0], clockPort, rtcp);
}

bool GstNetworkAudioStreamer::CreateStream()
{
	return m_impl->CreateStream();
}

void GstNetworkAudioStreamer::Close()
{
	m_impl->Close();
}
bool GstNetworkAudioStreamer::IsStreaming()
{
	return m_impl->IsStreaming();
}

void GstNetworkAudioStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstNetworkAudioStreamer::IsPaused()
{
	return !m_impl->IsPlaying();
}

}
}
