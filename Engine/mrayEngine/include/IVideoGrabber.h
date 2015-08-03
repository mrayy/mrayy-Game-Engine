

/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   13:54
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IVideoGrabber.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IVideoGrabber
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IVideoGrabber___
#define ___IVideoGrabber___

#include "ImageInfo.h"

namespace mray
{
namespace video
{

class IVideoGrabber
{
protected:

public:
	IVideoGrabber(){}
	virtual~IVideoGrabber(){}

	virtual void SetFrameSize(int w,int h)=0;
	virtual const math::vector2di& GetFrameSize()=0;

	virtual void SetImageFormat(video::EPixelFormat fmt)=0;
	virtual video::EPixelFormat GetImageFormat()=0;

	virtual int GetFramesCount(){ return 1; }
	virtual bool GrabFrame(int i=0) = 0;
	virtual bool HasNewFrame(int i=0) = 0;
	virtual ulong GetBufferID(int i=0) = 0;// incremented once per frame
	virtual float GetCaptureFrameRate(int i=0) = 0;


	virtual const ImageInfo* GetLastFrame(int index=0) = 0;

	virtual void Lock(){}
	virtual void Unlock(){}
	
};

}
}

#endif
