


/********************************************************************
created:	2012/05/22
created:	22:5:2012   15:21
filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\BrowserWindow.h
file path:	d:\Development\mrayEngine\Tests\TestOpenCV
file base:	BrowserWindow
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __BrowserWindow__
#define __BrowserWindow__

#include <mString.h>
#include <ListenerContainer.h>

namespace mray
{
	namespace video
	{
		class ITexture;
	}

	class KeyboardEvent;
	class MouseEvent;
	class Event;
namespace web
{

	class BrowserWindowImpl;
	class BrowserWindow;

	class IBrowserWindowListener
	{
	public:
		virtual void OnAddressChanged(BrowserWindow* wnd,const core::string& addr){}
		virtual void OnStartLoading(BrowserWindow* wnd,const core::string& addr){}
		virtual void OnLoad(BrowserWindow* wnd){}
		virtual void OnCrashed(BrowserWindow* wnd){}
		virtual void OnCrashWorker(BrowserWindow* wnd){}
		virtual void OnCrashPlugin(BrowserWindow* wnd,const core::stringw& plugin){}
		virtual void OnNavigationRequested(BrowserWindow* wnd,const core::string& url,const core::string& referrer,bool newWnd){}
		virtual void OnLoadingStateChanged(BrowserWindow* wnd,bool isLoading){}
		virtual void OnJavascriptCallback(BrowserWindow* wnd,const core::string& url,const core::stringw& function,const std::vector<core::stringw>& args){}
	};

class BrowserWindow:public ListenerContainer<IBrowserWindowListener*>
{
protected:
	BrowserWindowImpl* m_impl;
	friend class BrowserWindowImpl;

	DECLARE_FIRE_METHOD(OnAddressChanged,(BrowserWindow* wnd,const core::string& addr),(wnd,addr));
	DECLARE_FIRE_METHOD(OnStartLoading,(BrowserWindow* wnd,const core::string& addr),(wnd,addr));
	DECLARE_FIRE_METHOD(OnLoad,(BrowserWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(OnCrashed,(BrowserWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(OnCrashWorker,(BrowserWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(OnCrashPlugin,(BrowserWindow* wnd,const core::stringw& plugin),(wnd,plugin));
	DECLARE_FIRE_METHOD(OnNavigationRequested,(BrowserWindow* wnd,const core::string& url,const core::string& referrer,bool newWnd),(wnd,url,referrer,newWnd));
	DECLARE_FIRE_METHOD(OnLoadingStateChanged,(BrowserWindow* wnd,bool isLoading),(wnd,isLoading));
	DECLARE_FIRE_METHOD(OnJavascriptCallback,(BrowserWindow* wnd,const core::string& url,const core::stringw& function,const std::vector<core::stringw>& args),(wnd,url,function,args));
public:
	BrowserWindow(int w,int h,void* context);
	~BrowserWindow();
	video::ITexture* GetTexture();

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e,const math::rectf& ownerRect);

	void Resize(int w,int h);

	math::vector2di GetSize();

	math::vector2d mapCoord(const math::vector2d& c,const math::vector2d& screen);

	bool NavigateToUrl(const core::string& url);
	void ExecuteScript(const core::stringw& script);
	void BindFunction(const core::stringw& name);

};

}
}

#endif
