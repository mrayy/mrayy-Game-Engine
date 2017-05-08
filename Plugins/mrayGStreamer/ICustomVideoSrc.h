

#ifndef __ICUSTOMVIDEOSRC__
#define __ICUSTOMVIDEOSRC__

// Created: 2015/12/26
// Author: MHD Yamen Saraiji

#include "XMLElement.h"

namespace mray
{
namespace video
{

class ICustomVideoSrc
{
protected:
	std::map<std::string, std::string> m_encoderParams;

	std::string m_encoder;
	int m_fps;
	int m_bitRate;
	int m_mtuSize;
public:
	ICustomVideoSrc()
	{
		m_mtuSize = 1400;
		m_bitRate = 3000;
		m_fps = 30;
		m_encoder = "H264";
	}
	virtual ~ICustomVideoSrc(){}

	virtual std::string BuildStringH264();
	virtual std::string BuildStringVP8();

	virtual std::string GetCameraStr(int i) = 0;
	virtual std::string GetPipelineStr(int i) = 0;

	virtual void LinkWithPipeline(void* pipeline) = 0;

	virtual int GetVideoSrcCount(){ return 1; }
	virtual int GetStreamsCount(){ return GetVideoSrcCount(); }
	//virtual bool IsAvailable(int index) = 0;
	virtual void  SetResolution(int width, int height, int fps, bool free){ m_fps = fps; }
	virtual void SetBitRate(int biterate){ m_bitRate = biterate; }

	virtual void SetSeparateStreams(bool separate) = 0;
	virtual bool IsSeparateStreams() = 0;

	// return type of presented data:
	// RAW: Raw Image Data
	// MJPEG: Decoded JPEG Data
	// H264: Decoded H264 Data
	virtual void SetEncoderType(const std::string &type);
	virtual std::string GetEncoderType() { return m_encoder; }

	virtual void LoadParameters(xml::XMLElement* e);

	virtual math::vector2di GetFrameSize(int i)=0;


	virtual void Start() {};
	virtual void Pause() {};
	virtual void Close() {};


};

}
}

#endif
