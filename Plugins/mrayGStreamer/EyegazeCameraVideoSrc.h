

#ifndef __EYEGAZECAMERAVIDEOSRC__
#define __EYEGAZECAMERAVIDEOSRC__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "ICustomVideoSrc.h" 
#include <vector>

namespace mray
{
namespace video
{
class EyegazeCameraVideoSrcImpl;

class EyegazeCameraVideoSrc :public ICustomVideoSrc
{
protected:
	EyegazeCameraVideoSrcImpl* m_data;

	virtual std::string _generateString(int i);
	virtual std::string _generateFullString();
public:
	EyegazeCameraVideoSrc();
	virtual ~EyegazeCameraVideoSrc();

	std::string GetCameraStr(int i);
	virtual std::string GetPipelineStr(int i);

	virtual void  SetResolution(int width, int height, int fps, bool free);
	virtual void SetSeparateStreams(bool separate){}
	virtual bool IsSeparateStreams() { return false; }
	virtual void SetBitRate(int biterate);

	virtual std::string BuildStringH264();
	std::string GetEncodingStr();

	void SetCameraSource(ICustomVideoSrc* source);

	void SetEyegazePos(const std::vector<math::vector2df>& poses);
	void SetEyegazeCrop(int w, int h);
	void SetEyegazeLevels(int levels);
	int GetEyegazeLevels();

	void LinkWithPipeline(void* pipeline);
	virtual math::vector2di GetFrameSize(int i);


	virtual void Start();
	virtual void Pause();
	virtual void Close();
	virtual void LoadParameters(xml::XMLElement* e);

};

}
}

#endif

