

#include "stdafx.h"
#include "UDPVideoGrabber.h"

namespace mray
{
namespace TBee
{

UDPVideoGrabber::UDPVideoGrabber(BaseDataReceiver*r,ECameraSource s)
{
	m_src=s;
	m_receiver=r;
	m_receiver->AddListener(this);
	m_hasNewImage=false;
	m_image.autoDel=true;
}

UDPVideoGrabber::~UDPVideoGrabber()
{
	m_receiver->RemoveListener(this);
}


void UDPVideoGrabber::OnVideoImage(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src)
{
	if(src!=m_src)
		return;
	m_hasNewImage=true;
	m_image.setData(image->imageData,image->Size,image->format);
	m_sz.set(image->Size.x,image->Size.y);
}


void UDPVideoGrabber::SetFrameSize(int w,int h)
{
}

const math::vector2di& UDPVideoGrabber::GetFrameSize()
{
	return m_sz;
}


void UDPVideoGrabber::SetImageFormat(video::EPixelFormat fmt)
{
}

video::EPixelFormat UDPVideoGrabber::GetImageFormat()
{
	return m_image.format;
}


bool UDPVideoGrabber::GrabFrame()
{
	return false;
}

bool UDPVideoGrabber::HasNewFrame()
{
	return m_hasNewImage;
}



const video::ImageInfo* UDPVideoGrabber::GetLastFrame()
{
	m_hasNewImage=false;
	return &m_image;
}

}
}



