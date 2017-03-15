

#ifndef __CAMERAVIDEOSRCIMPL__
#define __CAMERAVIDEOSRCIMPL__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include <vector>

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
	bool m_separateStreams;

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
		return  m_cams.size();
	}
	int GetStreamsCount()
	{
		return m_separateStreams ? m_cams.size() : 1;
	}
};
}
}

#endif

