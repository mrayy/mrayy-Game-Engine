
#include "stdafx.h"
#include "CameraCaptureThread.h"

#include "OpenCVImageDataSource.h"

#include "IThreadManager.h"
#include "MutexLocks.h"


namespace mray
{


CameraCaptureThread::CameraCaptureThread(IImageDataSource *c0,IImageDataSource *c1)
{
	m_fps=0;
	m_leftDS=c0;
	m_rightDS=c1;

// 	m_leftDS=new OpenCVImageDataSource();
// 	m_rightDS=new OpenCVImageDataSource();

	

	m_fpsMutex=OS::IThreadManager::getInstance().createMutex();
}
CameraCaptureThread::~CameraCaptureThread()
{
	delete m_fpsMutex;

	delete m_leftDS;
	delete m_rightDS;
}

IImageDataSource* CameraCaptureThread::GetLeftDataSource()
{
	return m_leftDS;
}

IImageDataSource* CameraCaptureThread::GetRightDataSource()
{
	return m_rightDS;
}


void CameraCaptureThread::SetFPS(float fps)
{
	OS::ScopedLock lock(m_fpsMutex);
	m_fps=fps;
}
float CameraCaptureThread::GetFPS()
{
	OS::ScopedLock lock(m_fpsMutex);
	return m_fps;
}
void CameraCaptureThread::setup()
{
}
void CameraCaptureThread::execute(OS::IThread*caller,void*arg)
{
	int frame=0;
	float t0=gTimer.getSeconds();
	float accTime=0;
	int frameCtr=0;
	while(caller->isActive())
	{
		//webカメラを使うための初期設定
// 		if(m_cap0->isOpened() ) 
// 		{
// 			(*m_cap0) >> frame0b;
// 			m_leftDS->AddNewImage(frame0b);
// 		}
// 		if(m_cap1->isOpened() ) 
// 		{
// 			(*m_cap1) >> frame1b;
// 			m_rightDS->AddNewImage(frame1b);
// 		}

		m_leftDS->CaptureImage();
		m_rightDS->CaptureImage();

		//画像の定義

		//cv::Mat hsv; 
		//	while(1) {
		//cv::resize(frame0b, frame0a, cv::Size(), 2, 1);
		//cv::resize(frame1b, frame1a, cv::Size(), 2, 1);

		//	IplImage output = frame0a;//画像データは共有状態
		//	cvSaveImage("testout.png",&output);
		/*

		char path[256];
		sprintf(path,"pics\\out%d_L.png",frame);
		cv::imwrite(path, frame0b);
		sprintf(path,"pics\\out%d_R.png",frame);		
		cv::imwrite(path, frame1b);
		*/

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

		//OS::IThreadManager::getInstance().sleep(1);
	}
}


}


