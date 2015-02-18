

#include "stdafx.h"
#include "BrowserWindow.h"


namespace mray
{

BrowserWindow::BrowserWindow(int w,int h,Berkelium::Context* c)
	:m_size(w,h)
{
	m_texture=Engine::getInstance().getDevice()->createEmptyTexture2D(true);
	m_texture->setMipmapsFilter(false);
	m_window=Berkelium::Window::create(c);
	m_window->setDelegate(this);

	m_refresh=true;
	m_flipped=0;
	Resize(w,h);
}
BrowserWindow::~BrowserWindow()
{
	delete m_window;
	delete [] m_flipped;

}
video::ITexture* BrowserWindow::GetTexture(){return m_texture;}

Berkelium::Window* BrowserWindow::GetWindow(){return m_window;}

void BrowserWindow::Resize(int w,int h)
{
	m_size.x=w;
	m_size.y=h;
	delete [] m_flipped;
	m_window->resize(w,h);
	m_texture->setAnisotropicFilter(true);

	m_texture->createTexture(math::vector3di(m_size.x,m_size.y,1),video::EPixel_X8R8G8B8);
	m_flipped=new mray::uchar[w*h*4];


}

math::vector2di BrowserWindow::GetSize(){return m_size;}

void BrowserWindow::onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL) 
{
	std::cout << "onStartLoading: " << newURL.data() << std::endl;
}

math::vector2d BrowserWindow::mapCoord(const math::vector2d& c,const math::vector2d& screen)
{
	return (c*m_size)/screen;
}

void BrowserWindow::onPaint(
	Berkelium::Window *win,
	const unsigned char *sourceBuffer,
	const Berkelium::Rect &sourceBufferRect,
	size_t numCopyRects,
	const Berkelium::Rect *copyRects,
	int dx, int dy,
	const Berkelium::Rect &scrollRect)
{
	std::cout<<"Paint:"<<std::endl;
	video::IHardwarePixelBuffer* surface= m_texture->getSurface(0);


	video::ColorConverter::convert32BitTo32Bit(sourceBuffer,m_flipped,math::vector2d(sourceBufferRect.width(),sourceBufferRect.height()),0,1,1);

	video::LockedPixelBox lockedBox(
		math::box3d(sourceBufferRect.left(),sourceBufferRect.top(),0,
		sourceBufferRect.right(),sourceBufferRect.bottom(),1),video::EPixel_X8R8G8B8,(void*)m_flipped);

	lockedBox.box.MinP.y=m_size.y-lockedBox.box.MinP.y;
	lockedBox.box.MaxP.y=m_size.y-lockedBox.box.MaxP.y;

	math::Swap(lockedBox.box.MinP.y,lockedBox.box.MaxP.y);

	if(m_refresh)
	{
		surface->blitFromMemory(lockedBox);
		m_refresh=false;
		return;
	}

	for(int i=0;i<numCopyRects;++i)
	{
		std::cout << "\tRect: " << copyRects[i].width()<<" "<<copyRects[i].height()<< std::endl;

		math::box3d lockBox(copyRects[i].left(),copyRects[i].top(),0,
			copyRects[i].right(),copyRects[i].bottom(),1);
		lockBox.MinP.y=m_size.y-lockBox.MinP.y;
		lockBox.MaxP.y=m_size.y-lockBox.MaxP.y;
		math::Swap(lockBox.MinP.y,lockBox.MaxP.y);

		video::LockedPixelBox lb=lockedBox.getSubBox(lockBox);
		surface->blitFromMemory(lb);
		// 			const video::LockedPixelBox& lb= surface->lock(lockBox,video::IHardwareBuffer::ELO_Discard);
		// 			memcpy(lb.data,sourceBuffer,sourceBufferRect.width()*sourceBufferRect.height()*4
	}


}

void BrowserWindow::ExecuteScript(const core::stringw& script)
{
	m_window->executeJavascript(Berkelium::WideString::point_to(script.c_str(),script.length()));
}

void BrowserWindow::onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) {
	std::cout << "Error! " << std::string(error, errorLength) << std::endl;
}

}

