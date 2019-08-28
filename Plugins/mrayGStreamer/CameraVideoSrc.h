

#ifndef __CAMERAVIDEOSRC__
#define __CAMERAVIDEOSRC__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "ICustomVideoSrc.h" 
#include "CMyListener.h" 
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
	virtual std::string GetEncodingStr(int i);

	virtual std::string _generateString(int i);
	virtual std::string _generateFullString();
public:
	CameraVideoSrc();
	virtual ~CameraVideoSrc();
	

	void SetSeparateStreams(bool separate);
	bool IsSeparateStreams();

	void SetCameraIndex(std::vector<int> cams);
	void  SetResolution(int width, int height, int fps, bool free);
	void SetCaptureType(const std::string &type);
	virtual std::string GetCameraStr(int i);
	virtual std::string GetPipelineStr(int i);

	void ConvertToGray8(bool convert);

	virtual void SetScalingFactor(float factor) {
		m_scalingFactor = factor;
	}

	virtual math::vector2di GetFrameSize(int i);

	void AddPostCaptureListener(IMyListenerCallback* listener,int i);

	virtual void LinkWithPipeline(void* pipeline);
	int GetVideoSrcCount();

	virtual int GetCurrentFPS(int i);

	int GetStreamsCount();

};

}
}

#endif

