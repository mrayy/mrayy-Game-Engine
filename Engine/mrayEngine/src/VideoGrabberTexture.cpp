


#include "stdafx.h"
#include "VideoGrabberTexture.h"
#include "Engine.h"


namespace mray
{
namespace video
{


VideoGrabberTexture::VideoGrabberTexture()
{
	m_index = 0;
}

VideoGrabberTexture::~VideoGrabberTexture()
{
}


void VideoGrabberTexture::Set(const GCPtr<IVideoGrabber>& grabber, ITextureCRef tex, int index)
{
	m_grabber=grabber;
	m_texture=tex;
	m_index = 0;
	if (!m_texture)
	{
		m_texture = gEngine.getDevice()->createEmptyTexture2D(false);
		m_texture->setMipmapsFilter(false);
	}
}


bool VideoGrabberTexture::Blit()
{
	if(!m_texture || !m_grabber )
		return false;
	if(!m_grabber->GrabFrame())
		return false;
	if (m_grabber->GetFrameSize().x == 0 || m_grabber->GetFrameSize().y == 0)
		return false;
	if(m_texture->getSize().x!=m_grabber->GetFrameSize().x ||
		m_texture->getSize().y!=m_grabber->GetFrameSize().y )
	{
		m_texture->setMipmapsFilter(false);
		m_texture->createTexture(math::vector3d(m_grabber->GetFrameSize().x,m_grabber->GetFrameSize().y,1),m_grabber->GetImageFormat());
	}
	const video::ImageInfo* ifo= m_grabber->GetLastFrame(m_index);

	video::LockedPixelBox box(math::box3d(0,ifo->Size),ifo->format,ifo->imageData);
	m_texture->getSurface(0)->blitFromMemory(box);
	return true;
}

}
}
