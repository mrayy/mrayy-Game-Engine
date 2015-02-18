
/********************************************************************
	created:	2012/08/13
	created:	13:8:2012   15:30
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\DirectShowImageDataSource.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	DirectShowImageDataSource
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___DirectShowImageDataSource___
#define ___DirectShowImageDataSource___

#include "IImageDataSource.h"

namespace mray
{
	namespace video
	{
		class DirectShowVideoGrabber;
	}

class DirectShowImageDataSource:public IImageDataSource
{
protected:
	video::DirectShowVideoGrabber* m_grabber;
public:
	DirectShowImageDataSource(int c,int w=320,int h=240,int fps=30);
	virtual~DirectShowImageDataSource();
	

	virtual const video::ImageInfo* PopImage();

	virtual bool HasNewImage();

	virtual void CaptureImage();

};

}

#endif
