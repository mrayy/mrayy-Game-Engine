

#ifndef __APPSRCVIDEOSRC__
#define __APPSRCVIDEOSRC__

// Created: 2015/12/26
// Author: MHD Yamen Saraiji

#include "ICustomVideoSrc.h"
#include <vector>
#include "IVideoGrabber.h"

namespace mray
{
namespace video
{

	class AppSrcVideoSrcImpl;
class AppSrcVideoSrc:public ICustomVideoSrc
{
protected:
	AppSrcVideoSrcImpl *m_impl;
public:
	AppSrcVideoSrc();
	virtual ~AppSrcVideoSrc();
	

	void SetSeparateStreams(bool separate){}
	bool IsSeparateStreams(){
		return true;
	}
	void  SetResolution(int width, int height, int fps, bool free);

	std::string GetCameraStr(int i);
	std::string GetPipelineStr(int i);

	void LinkWithPipeline(void* pipeline) ;

	void SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers);
	int GetVideoSrcCount();

	virtual math::vector2di GetFrameSize(int i);

	virtual int GetCurrentFPS();

	void Start() ;
	void Pause() ;
	void Close() ;
};

}
}

#endif
