

#include "stdafx.h"
#include "AwesomiumSurfaceFactory.h"
#include "Engine.h"




namespace mray
{
namespace web
{

Awesomium::Surface* AwesomiumSurfaceFactory::CreateSurface(Awesomium::WebView* view,
	int w,
	int h)
{
	AwesomiumSurface* surf = new AwesomiumSurface(view,w,h);
	return surf;
}

void AwesomiumSurfaceFactory::DestroySurface(Awesomium::Surface* surface)
{

}



AwesomiumSurface::AwesomiumSurface(Awesomium::WebView* view,int w, int h)
{
	m_view = view;
	m_texture = gEngine.getDevice()->createEmptyTexture2D(false);
	m_texture->setMipmapsFilter(false);
	m_texture->setAnisotropicFilter(true);
	m_texture->createTexture(math::vector3di(w, h, 1), video::EPixel_B8G8R8A8);
}


AwesomiumSurface::~AwesomiumSurface()
{
}

	
void AwesomiumSurface::Paint(unsigned char* src_buffer,
	int src_row_span,
	const Awesomium::Rect& src_rect,
	const Awesomium::Rect& dest_rect)
{
	video::IHardwarePixelBuffer* surface = m_texture->getSurface(0);

	
	video::LockedPixelBox lockedBox(
		math::box3d(dest_rect.x, dest_rect.y, 0,
		dest_rect.x + src_rect.width, dest_rect.y + src_rect.height, 1), video::EPixel_B8G8R8A8, (void*)src_buffer);

	surface->blitFromMemory(lockedBox);

}

void AwesomiumSurface::Scroll(int dx,
	int dy,
	const Awesomium::Rect& clip_rect)
{

}

}
}
