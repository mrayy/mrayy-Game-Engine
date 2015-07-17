

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

class GstNetworkVideoStreamerImpl :public GstPipelineHandler, public IPipelineListener
{
protected:
	GstNetworkVideoStreamer* m_owner;
	core::string m_ipAddr;
	uint m_videoPort;
	uint m_clockPort;

	int m_camIdx[2];
	int m_bitRate;
	int m_fps;
	bool m_rtcp;

	math::vector2di m_cameraSize;
	math::vector2di m_frameSize;

	core::string m_pipeLineString;
	GstMyUDPSink* m_videoSink;
	GstMyUDPSink* m_videoRtcpSink;
	GstMyUDPSrc* m_videoRtcpSrc;


public:
	GstNetworkVideoStreamerImpl(GstNetworkVideoStreamer* o)
	{
		m_owner = o;
		m_ipAddr = "127.0.0.1";
		m_videoPort = 5000;
		m_clockPort = 5010;

		m_bitRate = 5000;
		m_camIdx[0] = 0;
		m_camIdx[1] = 1;
		m_fps = 30;

		m_videoSink = 0;
		m_videoRtcpSink = 0;
		m_videoRtcpSrc = 0;
		m_frameSize.set(1280, 720);
		m_cameraSize.set(1280, 720);

		AddListener(this);
	}

	virtual ~GstNetworkVideoStreamerImpl()
	{
	}

	core::string getVdoString(int i)
	{
		core::string str;
		
		core::string caps = " video/x-raw,width=" + core::StringConverter::toString(m_cameraSize.x) + ",height=" + core::StringConverter::toString(m_cameraSize.y);

		str = "ksvideosrc name=src" + core::StringConverter::toString(i) + " device-index=" + core::StringConverter::toString(m_camIdx[i]) + " ! " + caps + " ! videorate max-rate=" + core::StringConverter::toString(m_fps)+" ";
		str += "! videoconvert ! video/x-raw,format=I420 ";
		return str;
	}

	core::string _buildCamString()
	{
		core::string videoStr;

		if (m_camIdx[0] < 0)m_camIdx[0] = 0;
		if (m_camIdx[1] < 0)m_camIdx[1] = 0;
		if (m_camIdx[0] == m_camIdx[1])
		{
			//ksvideosrc
#if 0
			videoStr = "ksvideosrc name=src device-index=" + core::StringConverter::toString(m_cam0) + // device=" + m_cam0.guidPath + "" +//
				" ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert  ! videoflip method=4 ";// !videoflip method = 1  ";
#else
			videoStr = getVdoString(0);
#endif

		}
		else
		{

			int halfW = m_frameSize.x / 2;
			videoStr = "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" + core::StringConverter::toString(halfW) + "   sink_2::ypos=0  sink_2::zorder=1  ";

			videoStr += "videotestsrc pattern=\"black\" ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) + " !  mix.sink_0 ";

#if 0
			//first camera
			videoStr += "ksvideosrc name=src1 device-index=" + core::StringConverter::toString(m_cam0) + "  ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert ! videoflip method=4 ! videoscale !"
				"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y) + " ! mix.sink_1 ";

			//second camera
			videoStr += "ksvideosrc name=src2 device-index=" + core::StringConverter::toString(m_cam1) + "  ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_frameSize.x) + ",height=" + core::StringConverter::toString(m_frameSize.y) +
				",framerate=" + core::StringConverter::toString(m_fps) + "/1 ! videoconvert ! videoflip method=4 ! videoscale ! "
				"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y) + "! mix.sink_2 ";
#else
			//first camera
			videoStr += getVdoString(0);
			
			if (m_cameraSize.x > halfW || m_cameraSize.y > m_frameSize.y)
			{
				videoStr += " ! videoscale !"
					"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y);
			}
			videoStr+=" ! mix.sink_1 ";

			//second camera
			videoStr += getVdoString(1);

			if (m_cameraSize.x > halfW || m_cameraSize.y > m_frameSize.y)
			{
				videoStr += " ! videoscale !"
					"video/x-raw,format=I420,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_frameSize.y);
			}
			videoStr += " ! mix.sink_2 ";

#endif
			videoStr += " mix. ";

		}
		return videoStr;
	}


	void BuildString()
	{
		core::string videoStr;

		videoStr=_buildCamString();
		//encoder string
		videoStr +="! x264enc name=videoEnc bitrate=" + core::StringConverter::toString(m_bitRate) + " speed-preset=superfast tune=zerolatency sync-lookahead=0 sliced-threads=false pass=qual ! rtph264pay ";//rc-lookahead=0 
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
				"myudpsink name=videoSink sync=false";
		}

	}

	void SetBitRate(int bitRate)
	{
		m_bitRate = bitRate;
	}
	void SetCameras(int cam0, int cam1)
	{
		m_camIdx[0] = cam0;
		m_camIdx[1] = cam1;
	}
	bool IsStereo()
	{
		return m_camIdx[0] != m_camIdx[1];
	}

	void SetCameraResolution(int width, int height, int fps)
	{
		m_cameraSize.set(width, height);
		m_fps = fps;
	}
	void SetFrameResolution(int width, int height)
	{
		m_frameSize.set(width, height);
	}

	void _UpdatePorts()
	{

		if (!GetPipeline())
			return;
#define SET_SRC(name,p) m_##name=GST_MyUDPSrc(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(p);}
#define SET_SINK(name,p) m_##name=GST_MyUDPSink(gst_bin_get_by_name(GST_BIN(GetPipeline()), #name)); if(m_##name){m_##name->SetPort(m_ipAddr,p);}

		SET_SINK(videoSink, m_videoPort);
		if (m_rtcp){
			SET_SRC(videoRtcpSrc, (m_videoPort + 1));
			SET_SINK(videoRtcpSink, (m_videoPort + 2));
		}

	}

	void BindPorts(const core::string& addr, uint videoPort,uint clockPort, bool rtcp)
	{
		m_ipAddr = addr;
		m_videoPort = videoPort;
		m_rtcp = rtcp;
		m_clockPort = clockPort;

		_UpdatePorts();
	}
	bool CreateStream()
	{
		GError *err = 0;
		BuildString();
		GstElement* p = gst_parse_launch(m_pipeLineString.c_str(), &err);
		if (err)
		{
			printf("GstNetworkVideoStreamer: Pipeline error: %s", err->message);
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

	virtual void OnPipelineReady(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerReady(m_owner); }
	virtual void OnPipelinePlaying(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStarted(m_owner); }
	virtual void OnPipelineStopped(GstPipelineHandler* p){ m_owner->__FIRE_OnStreamerStopped(m_owner); }

};


GstNetworkVideoStreamer::GstNetworkVideoStreamer()
{
	m_impl = new GstNetworkVideoStreamerImpl(this);
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


void GstNetworkVideoStreamer::BindPorts(const core::string& addr, uint videoPort, uint clockPort, bool rtcp)
{
	m_impl->BindPorts(addr,videoPort,clockPort ,rtcp);
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

void GstNetworkVideoStreamer::SetCameraResolution(int width, int height,int fps)
{
	m_impl->SetCameraResolution(width, height, fps);
}

void GstNetworkVideoStreamer::SetFrameResolution(int width, int height)
{
	m_impl->SetFrameResolution(width, height);
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

GstPipelineHandler* GstNetworkVideoStreamer::GetPipeline()
{
	return m_impl;
}

}
}

