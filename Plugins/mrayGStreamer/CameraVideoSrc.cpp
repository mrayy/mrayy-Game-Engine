
#include "stdafx.h"
#include "CameraVideoSrc.h"
#include "StringConverter.h"


namespace mray
{
namespace video
{

class CameraVideoSrcImpl
{
protected:

	std::string m_captureType;
	std::string m_encodeType;
	std::vector<int> m_cams;
	int m_fps;
	int m_bitRate;
	math::vector2di m_frameSize;

public:
	CameraVideoSrcImpl()
	{
		m_bitRate = 3000;

		m_fps = 30;
		m_frameSize.set(1280, 720);
		m_captureType = "RAW";
		m_encodeType = "H264";
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
		m_fps = fps;
	}
	void SetBitRate(int biterate)
	{
		m_bitRate = biterate;
	}
	void SetCaptureType(const std::string &type)
	{
		m_captureType = type;
	}
	void SetEncodeType(const std::string &type)
	{
		m_encodeType = type;
	}

	std::string GetEncodingStr()
	{
		std::string videoStr;

		if (m_encodeType == "H264")
		{

			if (m_captureType != "H264")
			{
				videoStr += "! videoconvert  ! video/x-raw,format=I420 ";//",framerate=1/" + core::StringConverter::toString(m_fps);// !videoflip method = 1  ";
				videoStr += "! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / m_cams.size()) +
					" speed-preset=superfast pass=cbr tune=zerolatency sync-lookahead=0 rc-lookahead=0 sliced-threads=true key-int-max=5"
					" psy-tune=1 ";
			}

				//interlaced=true sliced-threads=false  "// 
			videoStr+= " ! rtph264pay ";
		}
		if (m_encodeType=="JPEG")
		{
			if (m_captureType != "JPEG")
			{
				videoStr = "! jpegenc  ";
			}
			videoStr += " ! rtpjpegpay ";
		}

		return videoStr;
	}
	std::string GetPipelineStr(int i)
	{
		std::string videoStr;
		if (m_cams[i] != -1)
		{
			videoStr = "ksvideosrc";
			videoStr += " name=src" + core::StringConverter::toString(i);
			videoStr += " device-index=" + core::StringConverter::toString(m_cams[i]);

			if (m_captureType == "RAW")
			{
				//" do-timestamp=true is-live=true "//"block=true"
				videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_frameSize.x) +
					",height=" + core::StringConverter::toString(m_frameSize.y);// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
			}
			else if (m_captureType == "JPEG")
			{
				videoStr += " ! image/jpeg,width=" + core::StringConverter::toString(m_frameSize.x) +
					",height=" + core::StringConverter::toString(m_frameSize.y);// + ",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

				if (m_captureType != m_encodeType)
				{
					videoStr += "! jpegdec ";
				}
			}
			videoStr += GetEncodingStr();
		}
		return videoStr;
	}

	void LinkWithPipeline(void* pipeline)
	{

	}
	int GetVideoSrcCount()
	{
		return m_cams.size();
	}
	std::string GetDataType()
	{
		return m_encodeType;
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
	m_impl->SetResolution(width, height, fps, free);
}
void CameraVideoSrc::SetBitRate(int biterate)
{
	m_impl->SetBitRate(biterate);
}
void CameraVideoSrc::SetCaptureType(const std::string &type)
{
	m_impl->SetCaptureType(type);
}
void CameraVideoSrc::SetEncodeType(const std::string &type)
{
	m_impl->SetEncodeType(type);
}

std::string CameraVideoSrc::GetPipelineStr(int i)
{
	return m_impl->GetPipelineStr(i);
}

void CameraVideoSrc::LinkWithPipeline(void* pipeline)
{
	m_impl->LinkWithPipeline(pipeline);
}
int CameraVideoSrc::GetVideoSrcCount()
{
	return m_impl->GetVideoSrcCount();
}
std::string CameraVideoSrc::GetDataType()
{
	return m_impl->GetDataType();
}


}
}

