
#include "stdafx.h"
#include "CameraVideoSrc.h"
#include "StringConverter.h"
#include "CameraVideoSrcImpl.h"


namespace mray
{
namespace video
{


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
std::string CameraVideoSrc::_generateString(int i)
{
	std::string videoStr;
	if (m_impl->m_cams[i] != -1)
	{
		videoStr = "ksvideosrc";
		videoStr += " name=src" + core::StringConverter::toString(i);
		videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

		//videoStr += "videotestsrc ";
		//" do-timestamp=true is-live=true "//"block=true"
		videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
			",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
		
	}
	else
		videoStr="videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
		",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);

	return videoStr;

}
std::string CameraVideoSrc::_generateFullString()
{

	std::string videoStr;
	bool mixer = false;

	int camsCount = 0;
	float totalWidth = 0;
	float totalHeight = 0;
	for (int i = 0; i < m_impl->m_cams.size(); ++i)
	{
		if (m_impl->m_cams[i] != -1)
		{
			camsCount++;
			totalWidth += m_impl->m_frameSize.x;
			totalHeight += m_impl->m_frameSize.y;
		}
	}
	if (camsCount == 0)
	{
		videoStr = "videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
			",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);
	}
	else
	{
		if (!m_impl->m_separateStreams  && camsCount > 1)
		{
			mixer = true;
			videoStr = "videomixer name=mix "
				"  sink_0::xpos=0 sink_0::ypos=0  sink_0::zorder=0 sink_0::alpha=1  ";
			int xpos = 0;
			int ypos = 0;
			for (int i = 0; i < camsCount; ++i)
			{
				std::string name = "sink_" + core::StringConverter::toString(i + 1);
				//videoStr += "  " + name + "::xpos=" + core::StringConverter::toString(xpos) + " " + name + "::ypos=0  " + name + "::zorder=0 " + name + "::zorder=1  ";
				videoStr += "  " + name + "::xpos=0 " + name + "::ypos=" + core::StringConverter::toString(ypos) + " " + name + "::zorder=0 " + name + "::zorder=1  ";
				xpos += m_impl->m_frameSize.x;
				ypos += m_impl->m_frameSize.y;
			}
		}

		int counter = 1;
		for (int i = 0; i < m_impl->m_cams.size(); ++i)
		{
			if (m_impl->m_cams[i] != -1)
			{
				videoStr += "ksvideosrc";
				videoStr += " name=src" + core::StringConverter::toString(i);
				videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

				//videoStr += "videotestsrc ";
				//" do-timestamp=true is-live=true "//"block=true"
				videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
					",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
				if (mixer)
				{
					videoStr += +" ! mix.sink_" + core::StringConverter::toString(counter) + " ";
				}
				++counter;
			}

		}
		if (mixer)
			videoStr += " mix. ! videoflip method=5 ";// "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
	}
	return videoStr;
}
std::string CameraVideoSrc::GetPipelineStr(int index)
{
	std::string videoStr;
	if (m_impl->m_separateStreams)
		videoStr = _generateString(index);
	else videoStr = _generateFullString();
	videoStr += GetEncodingStr();
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
int CameraVideoSrc::GetStreamsCount()
{
	return m_impl->GetStreamsCount();

}

void CameraVideoSrc::SetSeparateStreams(bool separate)
{
	m_impl->m_separateStreams = separate;
}
bool CameraVideoSrc::IsSeparateStreams()
{
	return m_impl->m_separateStreams;
}

}
}

