

#include "stdafx.h"
#include "SingleCameraCaptureThread.h"


#include "OpenCVImageDataSource.h"

#include "IThreadManager.h"
#include "MutexLocks.h"


namespace mray
{


SingleCameraCaptureThread::SingleCameraCaptureThread(IImageDataSource *c)
{
	m_fps=0;
	m_DS=c;

	m_image=new video::ImageInfo();
	// 	m_leftDS=new OpenCVImageDataSource();
	// 	m_rightDS=new OpenCVImageDataSource();
	m_fpsMutex=OS::IThreadManager::getInstance().createMutex();
}
SingleCameraCaptureThread::~SingleCameraCaptureThread()
{
	delete m_fpsMutex;

	delete m_DS;
	delete m_image;
}

IImageDataSource* SingleCameraCaptureThread::GetDataSource()
{
	return m_DS;
}


void SingleCameraCaptureThread::SetFPS(float fps)
{
	OS::ScopedLock lock(m_fpsMutex);
	m_fps=fps;
}
float SingleCameraCaptureThread::GetFPS()
{
	OS::ScopedLock lock(m_fpsMutex);
	return m_fps;
}
void SingleCameraCaptureThread::setup()
{
}
void SingleCameraCaptureThread::execute(OS::IThread*caller,void*arg)
{
	int frame=0;
	float t0=gTimer.getSeconds();
	float accTime=0;
	int frameCtr=0;
	while(caller->isActive())
	{

		m_DS->CaptureImage();
		if(m_DS->HasNewImage())
		{
			const video::ImageInfo*tmp=m_DS->PopImage();

			m_image->setData(tmp->imageData,tmp->Size,tmp->format);
		}
		++frame;
		float t1=gTimer.getSeconds();
		float dt=t1-t0;
		accTime+=dt;
		if(accTime>1000)
		{
			accTime-=1000;
			SetFPS(frameCtr);
			frameCtr=0;
		}
		++frameCtr;
		t0=t1;

		OS::IThreadManager::getInstance().sleep(30);
	}
}


}



