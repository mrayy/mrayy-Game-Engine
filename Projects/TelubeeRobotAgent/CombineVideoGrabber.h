

/********************************************************************
created:	2013/12/04
created:	4:12:2013   20:05
filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\CombineVideoGrabber.h
file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
file base:	CombineVideoGrabber
file ext:	h
author:		MHD Yamen Saraiji
	
purpose:	
*********************************************************************/

#ifndef __CombineVideoGrabber__
#define __CombineVideoGrabber__


#include "IVideoGrabber.h"

namespace mray
{
#define USE_GPU 0
class CombineVideoGrabber :public video::IVideoGrabber
{
protected:
	video::IVideoGrabber* m_g1;
	video::IVideoGrabber* m_g2;

	math::vector2di m_targetSize;
	video::ImageInfo m_lastImage;
	bool m_newFrame;
	ulong m_bufferID;
	//void _RotateImage(const video::ImageInfo* src, video::ImageInfo* dst, const math::recti &srcRect, bool cw);

public:

	CombineVideoGrabber(video::IVideoGrabber* g1, video::IVideoGrabber* g2);

	void SetGrabbers(video::IVideoGrabber* g1, video::IVideoGrabber* g2);

	virtual void SetFrameSize(int w, int h);
	virtual const math::vector2di& GetFrameSize();

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual bool GrabFrame();
	virtual bool HasNewFrame();

	virtual float GetCaptureFrameRate() { return m_g1->GetCaptureFrameRate(); }

	virtual ulong GetBufferID(){ return m_g1->GetBufferID(); }
	virtual const video::ImageInfo* GetLastFrame();
};

}


#endif
