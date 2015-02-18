

#include "stdafx.h"
#include "GstNetworkVideoStreamer.h"

#include "GstPipelineHandler.h"

#include "CMyUDPSrc.h"
#include "CMyUDPSink.h"

#include "StringConverter.h"

namespace mray
{
namespace video
{

class GstNetworkVideoStreamerImpl :public GstPipelineHandler
{
protected:
	core::string m_ipAddr;
	int m_videoPort;

	int m_cam0, m_cam1;
	int m_bitRate;
	int m_fps;
	bool m_rtcp;

	math::vector2di m_frameSize;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoSink;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;


public:
	GstNetworkVideoStreamerImpl()
	{
		m_ipAddr = "127.0.0.1";
		m_videoPort = 5000;

		m_bitRate = 5000;
		m_cam0 = 0;
		m_cam1 = 1;
		m_fps = 30;

		m_videoSink = 0;
		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;
		m_frameSize.set(1280, 720);

	}

	virtual ~GstNetworkVideoStreamerImpl()
	{
	}

	core::string _buildCamString()
	{
		core::string videoStr;

		if (m_cam0 < 0)m_cam0 = 0;
		if (m_cam1 < 0)m_cam1 = 0;
		if (m_cam0 == m_cam1)
		{
			//ksvideosrc
			videoStr = "ksvideosrc name=src device-index=" + core::StringConverter::toString(m_cam0) + // device=" + m_cam0.guidPath + "" +//
				" ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert  ! videoflip method = 4 ";// !videoflip method = 1  ";

		}
		else
		{

			int halfW = m_frameSize.x / 2;
			videoStr = "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" + core::StringConverter::toString(halfW) + "   sink_2::ypos=0  sink_2::zorder=1  ";

			videoStr += "videotestsrc pattern=\"black\" ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) + " !  mix.sink_0 ";

			//first camera
			videoStr += "ksvideosrc name=src1 device-index=" + core::StringConverter::toString(m_cam0) + "  ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert ! videoflip method=4 ! videoscale !"
				"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y) + " ! mix.sink_1 ";

			//second camera
			videoStr += "ksvideosrc name=src2 device-index=" + core::StringConverter::toString(m_cam1) + "  ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert ! videoflip method=4 ! videoscale ! "
				"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y) + "! mix.sink_2 ";

			videoStr += " mix. ";

		}
		return videoStr;
	}


	void BuildString()
	{
		core::string videoStr;

		videoStr=_buildCamString();
		//encoder string
		videoStr +="! x264enc name=videoEnc bitrate=" + core::StringConverter::toString(m_bitRate) + " speed-preset=superfast tune=zerolatency sync-lookahead=0  pass=qual ! rtph264pay ";
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
	void SetCameras(int cam0, int cam1)
	{
		m_cam0 = cam0;
		m_cam1 = cam1;
	}
	bool IsStereo()
	{
		return m_cam0 != m_cam1;
	}

	void SetResolution(int width, int height, int fps)
	{
		m_frameSize.set(width, height);
		m_fps = fps;
	}

	void _UpdatePorts()
	{

		if (!m_gstPipeline)
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(m_gstPipeline), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}

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
			printf("GstNetworkVideoStreamer: Pipeline error: %s", err->message);
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


GstNetworkVideoStreamer::GstNetworkVideoStreamer()
{
	m_impl = new GstNetworkVideoStreamerImpl();
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


void GstNetworkVideoStreamer::BindPorts(const core::string& addr, int videoPort, bool rtcp)
{
	m_impl->BindPorts(addr,videoPort,rtcp);
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

void GstNetworkVideoStreamer::SetResolution(int width, int height,int fps)
{
	m_impl->SetResolution(width, height, fps);
}

void GstNetworkVideoStreamer::SetBitRate(int bitRate)
{
	m_impl->SetBitRate(bitRate);
}


void GstNetworkVideoStreamer::SetCameras(int cam0, int cam1)
{
	m_impl->SetCameras(cam0, cam1);
}
bool GstNetworkVideoStreamer::IsStereo()
{
	return m_impl->IsStereo();
}

void GstNetworkVideoStreamer::SetPaused(bool paused)
{
	m_impl->SetPaused(paused);
}

bool GstNetworkVideoStreamer::IsPaused()
{
	return !m_impl->IsPlaying();
}

}
}

