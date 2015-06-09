

#ifndef LeapMotionImageRetrival_h__
#define LeapMotionImageRetrival_h__


#include "TextureUnit.h"
#include "ParsedShaderPP.h"
#include "LeapMotionController.h"
#include "IVideoGrabber.h"
#include "IMutex.h"

namespace mray
{
	
class LeapMotionImageRetrival :public video::IVideoGrabber
{
protected:
	int m_index;
	video::ITexturePtr finalTexture;

	video::ITexturePtr mainTexture;
	video::ITexturePtr distortionXTex;
	video::ITexturePtr distortionYTex;
	video::TextureUnit mainTU, disXTU, disYTU;

	video::ParsedShaderPP* undistortShader;

	GCPtr<OS::IMutex> m_mutex;

	void _SetMainTex(int width, int height);
	void _SetDistortionTex(int width, int height);
	void  _LoadMainTexture(const uchar* data);
	void _EncodeDisortion(Leap::Image &image);

	math::vector2di m_frameSize;
	video::ImageInfo m_imageInfo;
	ulong m_bufferID;
	bool m_hasNewFrame;
public:
	LeapMotionImageRetrival(int index);
	virtual ~LeapMotionImageRetrival();

	void SetImageIndex(int index){ m_index = index; }
	int GetImageIndex(){ return m_index; }

	bool Capture(Leap::Frame &frame);


	video::ITexturePtr GetCapturedTexture(){ return mainTexture; }
	video::ITexturePtr GetResult(){ return finalTexture; }

	//////////////////////////////////////////////////////////////////////////

	virtual void SetFrameSize(int w, int h);
	virtual const math::vector2di& GetFrameSize();

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual bool GrabFrame();
	virtual bool HasNewFrame();
	virtual ulong GetBufferID();// incremented once per frame

	virtual float GetCaptureFrameRate(){ return 30; }

	virtual const video::ImageInfo* GetLastFrame();
	virtual void Lock();
	virtual void Unlock();
};

}

#endif // LeapMotionImageRetrival_h__
