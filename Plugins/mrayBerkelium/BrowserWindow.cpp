

#include "stdafx.h"
#include "BrowserWindow.h"
#include "Engine.h"
#include "ColorConverter.h"

#include <berkelium/WindowDelegate.hpp>
#include "MouseEvent.h"
#include "keyboardEvent.h"

namespace mray
{

namespace web
{
	using namespace Berkelium;

class BrowserWindowImpl:public Berkelium::WindowDelegate
{
protected:
	math::vector2di m_size;
	GCPtr<video::ITexture> m_texture;

	Berkelium::Window* m_window;
	mray::uchar* m_flipped;

	bool m_refresh;

	BrowserWindow* m_owner;

public:
	BrowserWindowImpl(BrowserWindow* o,int w,int h,Berkelium::Context* c);
	~BrowserWindowImpl();
	video::ITexture* GetTexture();

	Berkelium::Window* GetWindow();

	void Resize(int w,int h);

	math::vector2di GetSize();

	math::vector2d mapCoord(const math::vector2d& c,const math::vector2d& screen);

	void ExecuteScript(const core::stringw& script);

	void BindFunction(const core::stringw& name);

	//

	virtual void onAddressBarChanged(Window *win, URLString newURL) 
	{
		m_owner->__FIRE_OnAddressChanged(m_owner,newURL.data());
	}

	virtual void onStartLoading(Window *win, URLString newURL)
	{
		m_owner->__FIRE_OnStartLoading(m_owner,newURL.data());
	}
	
    virtual void onLoad(Window *win)
	{
		m_owner->__FIRE_OnLoad(m_owner);
	}

    virtual void onCrashedWorker(Window *win)
	{
		m_owner->__FIRE_OnCrashWorker(m_owner);
	}

    virtual void onCrashedPlugin(Window *win, WideString pluginName)
	{
		m_owner->__FIRE_OnCrashPlugin(m_owner,pluginName.data());
	}

    virtual void onNavigationRequested(Window *win, URLString newUrl,
                                       URLString referrer, bool isNewWindow,
                                       bool &cancelDefaultAction)
	{
		WindowDelegate::onNavigationRequested(win,newUrl,referrer,isNewWindow,cancelDefaultAction);
		m_owner->__FIRE_OnNavigationRequested(m_owner,newUrl.data(),referrer.data(),isNewWindow);
	}

    virtual void onLoadingStateChanged(Window *win, bool isLoading)
	{
		m_owner->__FIRE_OnLoadingStateChanged(m_owner,isLoading);
	}

    virtual void onCrashed(Window *win)
	{
		m_owner->__FIRE_OnCrashed(m_owner);
	}

    virtual void onJavascriptCallback(Window *win, void* replyMsg, URLString origin, WideString funcName, Script::Variant *args, size_t numArgs)
	{
		WindowDelegate::onJavascriptCallback(win,replyMsg,origin,funcName,args,numArgs);
		std::vector<core::stringw> argsVal;
		for (int i=0;i<numArgs;++i)
		{
			switch(args[i].type())
			{
			case Script::Variant::JSSTRING:
				argsVal.push_back(args[i].toString().data());
				break;
			case Script::Variant::JSBOOLEAN:
				argsVal.push_back(core::string_to_wchar(core::StringConverter::toString(args[i].toBoolean())));
				break;
			case Script::Variant::JSDOUBLE:
				argsVal.push_back(core::string_to_wchar(core::StringConverter::toString(args[i].toDouble())));
				break;
			default:
				argsVal.push_back(L"0");
			}
		}
		m_owner->__FIRE_OnJavascriptCallback(m_owner,origin.data(),funcName.data(),argsVal);

	}

	virtual void onPaint(
		Berkelium::Window *win,
		const unsigned char *sourceBuffer,
		const Berkelium::Rect &sourceBufferRect,
		size_t numCopyRects,
		const Berkelium::Rect *copyRects,
		int dx, int dy,
		const Berkelium::Rect &scrollRect);

	virtual void onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) ;


};


BrowserWindowImpl::BrowserWindowImpl(BrowserWindow*o,int w,int h,Berkelium::Context* c)
	:m_size(w,h),m_owner(o)
{
	m_texture=Engine::getInstance().getDevice()->createEmptyTexture2D(true);
	m_texture->setMipmapsFilter(false);
	m_window=Berkelium::Window::create(c);
	m_window->setDelegate(this);

	m_refresh=true;
	m_flipped=0;
	Resize(w,h);
}
BrowserWindowImpl::~BrowserWindowImpl()
{
	delete m_window;
	delete [] m_flipped;

}
video::ITexture* BrowserWindowImpl::GetTexture(){return m_texture;}

Berkelium::Window* BrowserWindowImpl::GetWindow(){return m_window;}

void BrowserWindowImpl::Resize(int w,int h)
{
	m_size.x=w;
	m_size.y=h;
	delete [] m_flipped;
	m_window->resize(w,h);
	m_texture->setAnisotropicFilter(true);

	m_texture->createTexture(math::vector3di(m_size.x,m_size.y,1),video::EPixel_X8R8G8B8);
	m_flipped=new mray::uchar[w*h*4];


}

math::vector2di BrowserWindowImpl::GetSize(){return m_size;}

math::vector2d BrowserWindowImpl::mapCoord(const math::vector2d& c,const math::vector2d& screen)
{
	return (c*m_size)/screen;
}

void BrowserWindowImpl::onPaint(
	Berkelium::Window *win,
	const unsigned char *sourceBuffer,
	const Berkelium::Rect &sourceBufferRect,
	size_t numCopyRects,
	const Berkelium::Rect *copyRects,
	int dx, int dy,
	const Berkelium::Rect &scrollRect)
{
//	std::cout<<"Paint:"<<std::endl;
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
	//	std::cout << "\tRect: " << copyRects[i].width()<<" "<<copyRects[i].height()<< std::endl;

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

void BrowserWindowImpl::ExecuteScript(const core::stringw& script)
{
	m_window->executeJavascript(Berkelium::WideString::point_to(script.c_str(),script.length()));
}

void BrowserWindowImpl::onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) {
//	std::cout << "Error! " << std::string(error, errorLength) << std::endl;
}


void BrowserWindowImpl::BindFunction(const core::stringw& name)
{
	Berkelium::WideString lval=Berkelium::WideString::point_to(name.c_str(),name.length());

	//m_window->bind(lval,Berkelium::Script::Variant());
	m_window->addBindOnStartLoading(lval,Berkelium::Script::Variant::bindFunction(lval,false));
}


BrowserWindow::BrowserWindow(int w,int h,void* context)
{
	m_impl=new BrowserWindowImpl(this,w,h,(Berkelium::Context*)context);
}
BrowserWindow::~BrowserWindow()
{
	delete m_impl;
}
video::ITexture* BrowserWindow::GetTexture()
{
	return m_impl->GetTexture();
}

void BrowserWindow::Resize(int w,int h)
{
	m_impl->Resize(w,h);
}

math::vector2di BrowserWindow::GetSize()
{
	return m_impl->GetSize();
}

math::vector2d BrowserWindow::mapCoord(const math::vector2d& c,const math::vector2d& screen)
{
	return m_impl->mapCoord(c,screen);
}
bool BrowserWindow::NavigateToUrl(const core::string& url)
{
	return m_impl->GetWindow()->navigateTo(url.c_str(),url.length());
}

void BrowserWindow::ExecuteScript(const core::stringw& script)
{
	m_impl->ExecuteScript(script);
}


bool BrowserWindow::_OnKeyboardEvent(KeyboardEvent* e)
{
	wchar_t c[2];
	c[0]=e->Char;
	c[1]=0;
	m_impl->GetWindow()->textEvent(c,1);


	return true;
}
bool BrowserWindow::_OnMouseEvent(MouseEvent* e,const math::rectf& ownerRect)
{
	if(!ownerRect.IsPointInside(e->pos))
		return false;
	if(e->event==MET_MOVED)
	{

		math::vector2d mapped=e->pos-ownerRect.ULPoint;
		mapped=m_impl->mapCoord(mapped,ownerRect.getSize());
		//mapped=mapped*m_impl->window->GetSize()/GetDefaultRegion()->GetRect().getSize();
		m_impl->GetWindow()->mouseMoved(mapped.x,mapped.y);
		m_impl->GetWindow()->mouseWheel(0,e->MouseWheelRel);
	}
	if(e->event==MET_LEFTDOWN)
	{
		m_impl->GetWindow()->mouseButton(0,true);
	}else
		if(e->event==MET_LEFTUP)
			m_impl->GetWindow()->mouseButton(0,false);
		else
			if(e->event==MET_MIDDLEDOWN)
				m_impl->GetWindow()->mouseButton(1,true);
			else
				if(e->event==MET_MIDDLEUP)
					m_impl->GetWindow()->mouseButton(1,false);
				else
					if(e->event==MET_RIGHTDOWN)
						m_impl->GetWindow()->mouseButton(2,true);
					else
						if(e->event==MET_RIGHTUP)
							m_impl->GetWindow()->mouseButton(2,false);
	return true;
}


void BrowserWindow::BindFunction(const core::stringw& name)
{
	m_impl->BindFunction(name);
}

}
}
