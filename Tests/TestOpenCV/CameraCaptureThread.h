



/********************************************************************
	created:	2012/05/20
	created:	20:5:2012   21:05
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\CameraCaptureThread.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	CameraCaptureThread
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CameraCaptureThread__
#define __CameraCaptureThread__


#include "IThreadFunction.h"

namespace mray
{

class IImageDataSource;
//class OpenCVImageDataSource;

class CameraCaptureThread:public OS::IThreadFunction
{
protected:
// 	cv::VideoCapture *m_cap0;
// 	cv::VideoCapture *m_cap1;
	float m_fps;
	OS::IMutex* m_fpsMutex;

//	OpenCVImageDataSource* m_leftDS;
//	OpenCVImageDataSource* m_rightDS;

	IImageDataSource* m_leftDS;
	IImageDataSource* m_rightDS;

	void SetFPS(float fps);
public:
	CameraCaptureThread(IImageDataSource *c0,IImageDataSource *c1);
	virtual ~CameraCaptureThread();
	IImageDataSource* GetLeftDataSource();
	IImageDataSource* GetRightDataSource();


	virtual void setup();
	float GetFPS();
	virtual void execute(OS::IThread*caller,void*arg);
};


}

#endif
