

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
	

	void  SetResolution(int width, int height, int fps, bool free);
	
	std::string GetPipelineStr(int i);

	void LinkWithPipeline(void* pipeline) ;

	void SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers);
	int GetVideoSrcCount();

	void Start() ;
	void Pause() ;
	void Close() ;
};

}
}

#endif
