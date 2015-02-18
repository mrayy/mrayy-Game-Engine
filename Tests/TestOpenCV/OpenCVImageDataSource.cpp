

#include "stdafx.h"
#if 0

#include "OpenCVImageDataSource.h"

#include "IThreadManager.h"
#include "MutexLocks.h"



namespace Robomec
{


OpenCVImageDataSource::OpenCVImageDataSource(cv::VideoCapture *cap)
{
	m_cap=cap;
	m_tex=0;
	m_maxCache=2;
	m_imagesMutex=OS::IThreadManager::getInstance().createMutex();
}
OpenCVImageDataSource::~OpenCVImageDataSource()
{
	Clear();
	delete m_imagesMutex;
	m_cap->release();
	delete m_cap;
}
void OpenCVImageDataSource::Clear()
{
	OS::ScopedLock lock(m_imagesMutex);
	std::list<video::ImageInfo*>::iterator it= m_images.begin();
	for(;it!=m_images.end();++it)
	{
		delete *it;
	}
	m_images.clear();
}
video::ImageInfo* OpenCVImageDataSource::PopImage()
{
	OS::ScopedLock lock(m_imagesMutex);
	if(m_images.size()==0)
		return 0;
	video::ImageInfo* img=*m_images.begin();
	m_images.erase(m_images.begin());
	return img;
}
bool OpenCVImageDataSource::HasNewImage()
{
	OS::ScopedLock lock(m_imagesMutex);
	return m_images.size()!=0;
}

void OpenCVImageDataSource::AttachTexture(video::ITexture* tex)
{
	m_tex=tex;
}
void OpenCVImageDataSource::AddNewImage(cv::Mat& frame )
{
	if(m_tex)
	{
		video::LockedPixelBox box(math::box3d(0,0,0,frame.rows,frame.cols,1),video::EPixel_R8G8B8,frame.data);
		m_tex->getSurface(0)->blitFromMemory(box);
	}else
	{
		video::ImageInfo* img=0;

		{
			OS::ScopedLock lock(m_imagesMutex);
			if(m_images.size()>=m_maxCache)
			{
				img=*m_images.begin();
				m_images.erase(m_images.begin());
			}
		}
		if(!img)
		{
			img=new video::ImageInfo();
			img->createData(math::vector3d(frame.cols,frame.rows,1),video::EPixel_R8G8B8);
		}

		//memcpy(img->imageData,frame.data,img->imageDataSize);

		video::ColorConverter::convert24BitTo24Bit(frame.data,img->imageData,
			math::vector2d(frame.cols,frame.rows),0,true,0);

		OS::ScopedLock lock(m_imagesMutex);
		m_images.push_back(img);
	}
}

void OpenCVImageDataSource::CaptureImage()
{
	if(m_cap && m_cap->isOpened() ) 
	{
		(*m_cap) >> m_frame;
		AddNewImage(m_frame);
 	}
}

}

#endif