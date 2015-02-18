


/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   10:07
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\RenderWindow.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	RenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __RenderWindow__
#define __RenderWindow__

#include "mString.h"
//#include "OptionContainer.h"
#include "ListenerContainer.h"
#include "LockedPixelBox.h"



namespace mray
{
	class OptionContainer;
	class InputManager;
namespace scene
{
	class ViewPort;
	class CameraNode;
}

namespace video
{
	class IRenderTarget;

class RenderWindow;
class IRenderWindowListener
{
public:
	virtual ~IRenderWindowListener(){}
	virtual void WindowMoved(RenderWindow* window){}
	virtual void WindowResized(RenderWindow* window){}
	virtual bool WindowClosing(RenderWindow* window){return true;}
	virtual void WindowClosed(RenderWindow* window){}
	virtual void WindowFocusChanged(RenderWindow* window){}

	virtual void WindowPreRender(RenderWindow* wnd){}
	virtual void WindowPostRender(RenderWindow* wnd){}
	virtual void WindowSwapBuffer(RenderWindow* wnd){}

	virtual void WindowPreViweportUpdate(RenderWindow* wnd,scene::ViewPort* vp){}
	virtual void WindowPostViweportUpdate(RenderWindow* wnd,scene::ViewPort* vp){}
	virtual void WindowViweportAdd(RenderWindow* wnd,scene::ViewPort* vp){}
	virtual void WindowViweportRemoved(RenderWindow* wnd,scene::ViewPort* vp){}
};
class MRAY_DLL RenderWindow:public ListenerContainer<IRenderWindowListener*>
{
public:
	typedef std::list<scene::ViewPort* > ViewPortList;
	typedef std::list<ViewPortList> ViewPortMultiList;

protected:
	ViewPortMultiList m_viewports;
	
	core::string m_name;
	bool m_Active;
	bool m_isPrimary;
	bool m_isFullscreen;
	bool m_autoDeactivate;
	bool m_vsync;

	math::vector2di m_size;
	math::vector2di m_position;

	InputManager* m_inputManager;

	bool _AddViewport(scene::ViewPort*vp);

	DECLARE_FIRE_METHOD(WindowMoved,(RenderWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(WindowResized,(RenderWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(WindowFocusChanged,(RenderWindow* wnd),(wnd));

	DECLARE_FIRE_METHOD(WindowPreRender,(RenderWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(WindowPostRender,(RenderWindow* wnd),(wnd));
	DECLARE_FIRE_METHOD(WindowSwapBuffer,(RenderWindow* wnd),(wnd));

	DECLARE_FIRE_METHOD(WindowPreViweportUpdate,(RenderWindow* wnd,scene::ViewPort* vp),(wnd,vp));
	DECLARE_FIRE_METHOD(WindowPostViweportUpdate,(RenderWindow* wnd,scene::ViewPort* vp),(wnd,vp));
	DECLARE_FIRE_METHOD(WindowViweportAdd,(RenderWindow* wnd,scene::ViewPort* vp),(wnd,vp));
	DECLARE_FIRE_METHOD(WindowViweportRemoved,(RenderWindow* wnd,scene::ViewPort* vp),(wnd,vp));

	DECLARE_FIRE_METHOD(WindowClosed,(RenderWindow* wnd),(wnd));
public:

	bool __FIRE_WindowClosing(RenderWindow* wnd)
	{
		bool closing=true;
		std::list<ListenerType>::iterator it= m_listeners.begin();
		for(;it!=m_listeners.end();++it)
		{
			closing&=(*it)->WindowClosing(wnd);
		}
		return closing;
	}
public:
	RenderWindow(const core::string&name);
	virtual~RenderWindow();

	const core::string& GetName()const{return m_name;}

	virtual void Create(const math::vector2di& size,bool fullScreen,const OptionContainer&params,ulong hwnd)=0;

	virtual scene::ViewPort* CreateViewport(const core::string&name,scene::CameraNode* camera,video::IRenderTarget* rt,
		const math::rectf&vprect,int zorder);
	virtual scene::ViewPort* GetViewport(const core::string&name);
	const ViewPortMultiList& GetViewports(){return m_viewports;}

	scene::ViewPort* GetViewportFromPoint(int x,int y,bool includeInactive);

	virtual void ClearViewports();

	virtual void SetActiveWindow(){}

	virtual void Render(bool swap);
	
	virtual void SetFullScreen(const math::vector2di& size,bool fullScreen)=0;
	virtual bool IsFullScreen()const
	{
		return m_isFullscreen;
	}

	virtual void Destroy()=0;

	virtual void Resize(const math::vector2di& size)=0;
	virtual const math::vector2di& GetSize()const
	{
		return m_size;
	}

	virtual void SetPosition(const math::vector2di& pos)=0;
	virtual const math::vector2di& GetPosition()const
	{
		return m_position;
	}
	virtual uint GetColorDepth()const=0;

	virtual void Show()=0;
	virtual void Hide()=0;
	virtual bool IsVisible()const=0;

	virtual void SetActive(bool a)=0;
	virtual bool IsActive()const
	{
		return m_Active && IsVisible();
	}

	virtual void SetDeactivateOnFocusChange(bool s)
	{
		m_autoDeactivate=s;
	}
	virtual bool IsDeactivateOnFocusChange()const
	{
		return m_autoDeactivate;
	}

	virtual bool IsClosed()const=0;

	virtual bool IsPrimary()const
	{
		return m_isPrimary;
	}

	virtual void OnMoveResize()=0;

	virtual void SwapBuffers(bool vSync)=0;

	virtual bool  TakeScreenShot(const LockedPixelBox& dst)=0;
	virtual void GetCustomParam(const core::string& name,void*value){};

	virtual void SetInputManager(InputManager* m){m_inputManager=m;}
	virtual InputManager* GetInputManager(){return m_inputManager;}
};


}
}

#endif
