

#ifndef __EYEGAZECAMERAVIDEOSRC__
#define __EYEGAZECAMERAVIDEOSRC__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "CameraVideoSrc.h" 
#include <vector>

namespace mray
{
namespace video
{
class EyegazeCameraVideoSrcImpl;

class EyegazeCameraVideoSrc :public CameraVideoSrc
{
protected:
	EyegazeCameraVideoSrcImpl* m_data;

	virtual std::string _generateString(int i);
	virtual std::string _generateFullString();
public:
	EyegazeCameraVideoSrc();
	virtual ~EyegazeCameraVideoSrc();

	virtual std::string BuildStringH264();

	void SetEyegazePos(const std::vector<math::vector2df>& poses);
	void SetEyegazeCrop(int w, int h);

	void LinkWithPipeline(void* pipeline);

};

}
}

#endif

