#include "StdAfx.h"
#include "DirectShowImageDataSource.h"
#include "DirectShowVideoGrabber.h"

namespace mray
{

DirectShowImageDataSource::DirectShowImageDataSource(int c,int w,int h,int fps)
{
	m_grabber=new video::DirectShowVideoGrabber();
	m_grabber->InitDevice(c,w,h,fps);
}

DirectShowImageDataSource::~DirectShowImageDataSource()
{
	delete m_grabber;
}



const video::ImageInfo* DirectShowImageDataSource::PopImage()
{
	return m_grabber->GetLastFrame();
}


bool DirectShowImageDataSource::HasNewImage()
{
	return m_grabber->HasNewFrame();
}


void DirectShowImageDataSource::CaptureImage()
{
	m_grabber->GrabFrame();
}


}
