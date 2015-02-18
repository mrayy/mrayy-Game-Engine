



/********************************************************************
	created:	2012/05/22
	created:	22:5:2012   16:13
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\SingleCameraCaptureThread.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	SingleCameraCaptureThread
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SingleCameraCaptureThread__
#define __SingleCameraCaptureThread__

#include "IThreadFunction.h"

namespace mray
{


class IImageDataSource;
//class OpenCVImageDataSource;

class SingleCameraCaptureThread:public OS::IThreadFunction
{
protected:
	float m_fps;
	OS::IMutex* m_fpsMutex;

	IImageDataSource* m_DS;

	video::ImageInfo*m_image;

	void SetFPS(float fps);
public:
	SingleCameraCaptureThread(IImageDataSource *ds);
	virtual ~SingleCameraCaptureThread();
	IImageDataSource* GetDataSource();

	video::ImageInfo*GetImage(){return m_image;}

	virtual void setup();
	float GetFPS();
	virtual void execute(OS::IThread*caller,void*arg);
};



}

#endif
