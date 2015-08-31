


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
	m_index = index;
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
	if (!m_grabber->GrabFrame(m_index))
		return false;
	const video::ImageInfo* ifo = m_grabber->GetLastFrame(m_index);
	if (ifo->Size.x == 0 || ifo->Size.y == 0)
		return false;
	if (m_texture->getSize().x != ifo->Size.x ||
		m_texture->getSize().y != ifo->Size.y)
	{
		m_texture->setMipmapsFilter(false);
		m_texture->createTexture(math::vector3d(ifo->Size.x, ifo->Size.y, 1), ifo->format);
	}

	video::LockedPixelBox box(math::box3d(0,ifo->Size),ifo->format,ifo->imageData);
	m_texture->getSurface(0)->blitFromMemory(box);
	return true;
}

}
}
