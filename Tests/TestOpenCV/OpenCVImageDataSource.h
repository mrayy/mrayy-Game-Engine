

/********************************************************************
	created:	2012/05/20
	created:	20:5:2012   21:02
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\OpenCVImageDataSource.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	OpenCVImageDataSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __OpenCVImageDataSource__
#define __OpenCVImageDataSource__

#include "IImageDataSource.h"

namespace mray
{
#if 0

class OpenCVImageDataSource:public IImageDataSource
{
protected:
	std::list<video::ImageInfo*> m_images;
	OS::IMutex* m_imagesMutex;

	cv::VideoCapture *m_cap;
	int m_maxCache;

	video::ITexture* m_tex;

	cv::Mat m_frame;
public:
	OpenCVImageDataSource(cv::VideoCapture *cap);
	virtual ~OpenCVImageDataSource();
	void Clear();
	virtual video::ImageInfo* PopImage();
	virtual bool HasNewImage();
	virtual void AttachTexture(video::ITexture* tex);
	void AddNewImage(cv::Mat& frame );

	virtual void CaptureImage();
};

#endif
}

#endif

