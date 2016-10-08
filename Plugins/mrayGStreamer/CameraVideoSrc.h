

#ifndef __CAMERAVIDEOSRC__
#define __CAMERAVIDEOSRC__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "ICustomVideoSrc.h" 
#include <vector>

namespace mray
{
namespace video
{
	class CameraVideoSrcImpl;

class CameraVideoSrc:public ICustomVideoSrc
{
protected:
	CameraVideoSrcImpl* m_impl;
	std::string GetEncodingStr();

	std::string _generateString(int i);
	std::string _generateFullString();
public:
	CameraVideoSrc();
	virtual ~CameraVideoSrc();
	

	void SetSeparateStreams(bool separate);
	bool IsSeparateStreams();

	void SetCameraIndex(std::vector<int> cams);
	void  SetResolution(int width, int height, int fps, bool free);
	void SetCaptureType(const std::string &type);

	std::string GetPipelineStr(int i);

	void LinkWithPipeline(void* pipeline);
	int GetVideoSrcCount();

	int GetStreamsCount();

};

}
}

#endif

