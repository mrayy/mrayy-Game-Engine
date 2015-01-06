

#ifndef LeapMotionImageRetrival_h__
#define LeapMotionImageRetrival_h__


#include "TextureUnit.h"
#include "ParsedShaderPP.h"
#include "LeapMotionController.h"

namespace mray
{
	
class LeapMotionImageRetrival
{
protected:
	int m_index;
	video::ITexturePtr finalTexture;

	video::ITexturePtr mainTexture;
	video::ITexturePtr distortionXTex;
	video::ITexturePtr distortionYTex;
	video::TextureUnit mainTU, disXTU, disYTU;

	video::ParsedShaderPP* undistortShader;

	void _SetMainTex(int width, int height);
	void _SetDistortionTex(int width, int height);
	void  _LoadMainTexture(const uchar* data);
	void _EncodeDisortion(Leap::Image &image);


public:
	LeapMotionImageRetrival(int index);
	virtual ~LeapMotionImageRetrival();

	void SetImageIndex(int index){ m_index = index; }
	int GetImageIndex(){ return m_index; }

	bool Capture(Leap::Frame &frame);

	video::ITexturePtr GetCapturedTexture(){ return mainTexture; }
	video::ITexturePtr GetResult(){ return finalTexture; }
};

}

#endif // LeapMotionImageRetrival_h__
