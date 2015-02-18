

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   20:12
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\UDPVideoGrabber.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	UDPVideoGrabber
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _UDPVideoGrabber_
#define _UDPVideoGrabber_

#include "IVideoGrabber.h"
#include "BaseDataReceiver.h"

namespace mray
{
namespace TBee
{

class UDPVideoGrabber:public video::IVideoGrabber,public IBaseDataReceiverListener
{
protected:
	BaseDataReceiver* m_receiver;
	bool m_hasNewImage;
	video::ImageInfo m_image;
	math::vector2d m_sz;
	ECameraSource m_src ;
public:
	UDPVideoGrabber(BaseDataReceiver*r,ECameraSource s);
	virtual~UDPVideoGrabber();

	void OnVideoImage(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src);

	virtual void SetFrameSize(int w,int h);
	virtual const math::vector2di& GetFrameSize();

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual bool GrabFrame();
	virtual bool HasNewFrame();


	virtual const video::ImageInfo* GetLastFrame();
};

}
}

#endif
