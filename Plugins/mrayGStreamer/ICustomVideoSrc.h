

#ifndef __ICUSTOMVIDEOSRC__
#define __ICUSTOMVIDEOSRC__

// Created: 2015/12/26
// Author: MHD Yamen Saraiji

namespace mray
{
namespace video
{

class ICustomVideoSrc
{
protected:
public:
	ICustomVideoSrc(){}
	virtual ~ICustomVideoSrc(){}

	virtual std::string GetPipelineStr(int i)=0;

	virtual void LinkWithPipeline(void* pipeline) = 0;

	virtual int GetVideoSrcCount(){ return 1; }
	virtual void  SetResolution(int width, int height, int fps, bool free){}
	virtual void SetBitRate(int biterate){}

	// return type of presented data:
	// RAW: Raw Image Data
	// MJPEG: Decoded JPEG Data
	// H264: Decoded H264 Data
	virtual std::string GetDataType() = 0;

	virtual void Start() {};
	virtual void Pause() {};
	virtual void Close() {};


};

}
}

#endif
