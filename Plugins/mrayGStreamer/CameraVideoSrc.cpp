
#include "stdafx.h"
#include "CameraVideoSrc.h"
#include "StringConverter.h"
#include "CMyListener.h"
#include "AveragePer.h"
#include "ILogManager.h"

#include <hash_map>

namespace mray
{
namespace video
{

	class CameraVideoSrcImpl :public IMyListenerCallback
{
public:

	std::string m_captureType;
	std::vector<int> m_cams;
	math::vector2di m_frameSize;
	bool m_separateStreams;

	std::vector<AveragePer> m_currentFps;

	//ImageProcessorListener listener;

	std::vector<GstMyListener*> m_imagecapListener;

	std::hash_map<int, std::vector<IMyListenerCallback*>> listeners;

public:
	CameraVideoSrcImpl()
	{
		m_frameSize.set(1280, 720);
		m_captureType = "RAW";
		m_separateStreams = false;
	}
	virtual ~CameraVideoSrcImpl()
	{

	}

	virtual void ListenerOnDataChained(_GstMyListener* src, GstBuffer * bfr)
	{
		int index = -1;
		for (int i = 0; i < m_imagecapListener.size(); ++i)
		{
			if (src == m_imagecapListener[i])
			{
				index = i;
				break;
			}
		}
		if (index == -1)
			return;
		if (listeners.find(index) != listeners.end())
		{
			for (IMyListenerCallback* i : listeners[index])
				i->ListenerOnDataChained(src, bfr);
		}

		m_currentFps[index].Add(1);
	}

	void SetCameraIndex(std::vector<int> cams)
	{
		m_cams = cams;
		m_currentFps.resize(cams.size());
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
		m_imagecapListener.clear();
		for (int i = 0; i < m_cams.size(); ++i)
		{
			GstMyListener *l = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline), ("imagecap" + core::StringConverter::toString(i)).c_str()));
			if (l)
			{
				l->listeners->AddListener(this);
			}

			m_imagecapListener.push_back(l);
		}
	}

	void AddListener(IMyListenerCallback* l, int i)
	{
		if (listeners.find(i) == listeners.end())
		{
			listeners.insert(make_pair(i, std::vector<IMyListenerCallback*>()));
		}
		listeners[i].push_back(l);
	}
	int GetVideoSrcCount()
	{
		return  m_cams.size();
	}
	int GetStreamsCount()
	{
		return m_separateStreams ? m_cams.size() : 1;
	}

	int GetCurrentFPS(int i)
	{
		if (i >= m_currentFps.size())
			return -1;
		return m_currentFps[i].GetAverage();
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

			//videoStr += "! x264enc speed-preset=superfast ! avdec_h264 ! rawvideoparse format=gray8 width=" + core::StringConverter::toString(m_impl->m_frameSize.x * 2) +
				//	" height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " !videoconvert ";

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
		
		//videoStr = "videotestsrc ";/**/

		//videoStr += "videotestsrc ";
		//" do-timestamp=true is-live=true "//"block=true"
		videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
			",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);
		videoStr += " ! rawvideoparse format=gray8 width=" + core::StringConverter::toString(m_impl->m_frameSize.x * 2) +
			" height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ";
		if (m_fps > 0)
			videoStr += " ! videorate max-rate=" + core::StringConverter::toString(m_fps);
		videoStr += " ! mylistener name=imagecap" + core::StringConverter::toString(i) + " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

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
					",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);

				if (m_fps > 0)
					videoStr += " ! videorate max-rate=" + core::StringConverter::toString(m_fps);

				videoStr += " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
				videoStr += " ! mylistener name=imagecap" + core::StringConverter::toString(i)+" ";
// 				videoStr += " ! rawvideoparse format=gray8 width=" + core::StringConverter::toString(m_impl->m_frameSize.x * 2) +
// 					" height=" + core::StringConverter::toString(m_impl->m_frameSize.y);

// 					" caps=\"video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x * 2) +
// 					",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + ",format=GRAY8\" ";
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



std::string CameraVideoSrc::GetCameraStr(int i)
{
	return _generateString(i);
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

void CameraVideoSrc::AddPostCaptureListener(IMyListenerCallback* listener, int i)
{
	m_impl->AddListener(listener,i);
}
math::vector2di CameraVideoSrc::GetFrameSize(int i)
{
	return m_impl->m_frameSize;
}
void CameraVideoSrc::SetSeparateStreams(bool separate)
{
	m_impl->m_separateStreams = separate;
}
bool CameraVideoSrc::IsSeparateStreams()
{
	return m_impl->m_separateStreams;
}

int CameraVideoSrc::GetCurrentFPS(int i)
{
	return m_impl->GetCurrentFPS(i);
}


}
}

