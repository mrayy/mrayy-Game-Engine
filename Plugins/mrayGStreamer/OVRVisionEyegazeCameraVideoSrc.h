

#ifndef __OVRVisionEyegazeCameraVideoSrc__
#define __OVRVisionEyegazeCameraVideoSrc__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "EyegazeCameraVideoSrc.h" 

namespace mray
{
namespace video
{

class OVRVisionEyegazeCameraVideoSrc :public EyegazeCameraVideoSrc
{
protected:
	virtual std::string _generateFullString();
	virtual void _setFoveatedRectsCount(int count);
public:
	OVRVisionEyegazeCameraVideoSrc();
	virtual ~OVRVisionEyegazeCameraVideoSrc();

};

}
}

#endif
