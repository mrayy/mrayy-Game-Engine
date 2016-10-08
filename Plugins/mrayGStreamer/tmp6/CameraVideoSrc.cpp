
#include "stdafx.h"
#include "CameraVideoSrc.h"
#include "StringConverter.h"


namespace mray
{
namespace video
{

class CameraVideoSrcImpl
{
public:

	std::string m_captureType;
	std::vector<int> m_cams;
	math::vector2di m_frameSize;

public:
	CameraVideoSrcImpl()
	{
		m_frameSize.set(1280, 720);
		m_captureType = "RAW";
	}
	virtual ~CameraVideoSrcImpl()
	{

	}


	void SetCameraIndex(std::vector<int> cams)
	{
		m_cams = cams;
	}
	void  SetResolution(int width, int height, int fps, bool free)
	{
		//m_freeSize = free;
		m_frameSize.set(width, height);
	}
	void SetCaptureType(const std::string &type)
	{
		m_captureType = type;
	}

	void LinkWithPipeline(void* pipeline)
	{

	}
	int GetVideoSrcCount()
	{
		return m_cams.size();
	}
};

CameraVideoSrc::CameraVideoSrc()
{
	m_impl = new CameraVideoSrcImpl();
}

CameraVideoSrc::~CameraVideoSrc()
{
	delete m_impl;
}

void CameraVideoSrc::SetCameraIndex(std::vector<int> cams)
{
	m_impl->SetCameraIndex(cams);
}
void CameraVideoSrc::SetResolution(int width, int height, int fps, bool free)
{
	ICustomVideoSrc::SetResolution(width, height, fps, free);
	m_impl->SetResolution(width, height, fps, free);
}
void CameraVideoSrc::SetCaptureType(const std::string &type)
{
	m_impl->SetCaptureType(type);
}
std::string CameraVideoSrc::GetEncodingStr()
{
	std::string videoStr;

	if (m_encoder == "H264")
	{

		if (m_impl->m_captureType != "H264")
		{/*
			videoStr += "! videoconvert  ! video/x-raw,format=I420 ";//",framerate=1/" + core::StringConverter::toString(m_fps);// !videoflip method = 1  ";
			videoStr += "! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / m_cams.size()) +
				" speed-preset=superfast pass=cbr tune=zerolatency sync-lookahead=0 rc-lookahead=0 sliced-threads=true key-int-max=5"
				" psy-tune=1 ";*/

			videoStr += BuildStringH264();
		}

		//interlaced=true sliced-threads=false  "// 
		//videoStr += " ! rtph264pay ";
	}
	if (m_encoder == "JPEG")
	{
		if (m_impl->m_captureType != "JPEG")
		{
			videoStr = "! jpegenc  ";
		}
		videoStr += " ! rtpjpegpay ";
	}

	return videoStr;
}
std::string CameraVideoSrc::GetPipelineStr(int i)
{

	std::string videoStr;
	if (m_impl->m_cams[i] != -1)
	{
		videoStr = "ksvideosrc";
		videoStr += " name=src" + core::StringConverter::toString(i);
		videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

		if (m_impl->m_captureType == "RAW")
		{
			//" do-timestamp=true is-live=true "//"block=true"
			videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
				",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ! videorate max-rate="+core::StringConverter::toString(m_fps)+" ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
		}
		else if (m_impl->m_captureType == "JPEG")
		{
			videoStr += " ! image/jpeg,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
				",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);// + ",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

			if (m_impl->m_captureType != m_encoder)
			{
				videoStr += "! jpegdec ";
			}
		}
		videoStr += GetEncodingStr();
	} 
	return videoStr;
}

void CameraVideoSrc::LinkWithPipeline(void* pipeline)
{
	m_impl->LinkWithPipeline(pipeline);
}
int CameraVideoSrc::GetVideoSrcCount()
{
	return m_impl->GetVideoSrcCount();
}

}
}

