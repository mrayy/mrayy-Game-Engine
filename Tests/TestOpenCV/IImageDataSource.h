


/********************************************************************
	created:	2012/05/20
	created:	20:5:2012   21:00
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\IImageDataSource.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	IImageDataSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IImageDataSource__
#define __IImageDataSource__

namespace mray
{
namespace video
{
	class ImageInfo;
}

// image data source interface 
// responsible to provide the application with images from the target camera
class IImageDataSource
{
public:
	IImageDataSource(){}
	virtual ~IImageDataSource(){}
	// retrive an image from the source
	// null if no new image exists
	virtual const video::ImageInfo* PopImage()=0;

	// asks whether there is a new image or not
	virtual bool HasNewImage()=0;

	virtual void CaptureImage()=0;

	// attach a texture to data source, so the data source will manage the uploading the image to the texture
	// on the data source's side
	//virtual void AttachTexture(video::ITexture* tex)=0;
};



}

#endif

