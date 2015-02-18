

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
	bool m_newFrame;
	video::ImageInfo m_lastImage;
#if USE_GPU
	video::ITexturePtr m_tex1;
	video::ITexturePtr m_tex2;

	video::ITexturePtr m_rtTex;
	video::IRenderTargetPtr m_rt;
#else
	video::ImageInfo m_frame1;
	video::ImageInfo m_frame2;

#endif

	bool m_rotate90;
	ulong m_bufferID;
	void _RotateImage(const video::ImageInfo* src, video::ImageInfo* dst, const math::recti &srcRect, bool cw);

public:

	CombineVideoGrabber();

	void SetGrabbers(video::IVideoGrabber* g1, video::IVideoGrabber* g2);
	void SetRotate90(bool r){ m_rotate90 = r; }

	virtual void SetFrameSize(int w, int h);
	virtual const math::vector2di& GetFrameSize();

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual bool GrabFrame();
	virtual bool HasNewFrame();

	virtual ulong GetBufferID(){ return m_g1->GetBufferID(); }
	virtual const video::ImageInfo* GetLastFrame();
};

}


#endif
