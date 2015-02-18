

/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   10:17
	filename: 	d:\Development\mrayEngine\Engine\mrayGLDevice\GLWin32RenderWindow.h
	file path:	d:\Development\mrayEngine\Engine\mrayGLDevice
	file base:	GLWin32RenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GLWin32RenderWindow__
#define __GLWin32RenderWindow__

#include "compileConfig.h"
#include "RenderWindow.h"

#include <Windows.h>

namespace mray
{
namespace video
{
	class Win32Context;
	class Win32GLSupport;
	class GLDev;

class MRAY_GL_DLL GLWin32RenderWindow:public RenderWindow
{
private:
protected:
	HWND m_hWnd;
	HDC m_hDc;
	HGLRC m_hRc;
	bool m_closed;
	uint m_colorDepth;
	bool m_depthBuffered;
	uint m_fsaa;
	bool m_hwGamma;
	bool m_isClosed;

	bool m_isExternal;
	bool m_externalGLRC;

	DWORD m_displayFrequency;

	Win32GLSupport* m_support;

	Win32Context* m_context;
	int m_targetMonitor;

	GLDev* m_device;
public:
	GLWin32RenderWindow(const core::string&name,Win32GLSupport* support,GLDev* dev);
	virtual~GLWin32RenderWindow();


	HWND GetHWND(){return m_hWnd;}
	HDC GetHDC(){return m_hDc;}
	HGLRC GetHRC(){return m_hRc;}

	Win32Context* GetContext(){return m_context;}

	virtual void Create(const math::vector2di& size,bool fullScreen,const OptionContainer&params,ulong hwnd);

	virtual void SetFullScreen(const math::vector2di& size,bool fullScreen);

	virtual void Destroy();

	virtual void Resize(const math::vector2di& size);
	virtual void SetPosition(const math::vector2di& pos);
	virtual uint GetColorDepth()const
	{
		return m_colorDepth;
	}
	virtual void SetActive(bool a);

	virtual void Show();
	virtual void Hide();
	virtual bool IsVisible()const;

	virtual bool IsClosed()const
	{
		return m_isClosed;
	}

	virtual void SetActiveWindow();

	virtual void SwapBuffers(bool vSync);

	virtual void Render(bool swap);

	virtual void OnMoveResize();
	virtual void GetCustomParam(const core::string& name,void*value);

	virtual bool TakeScreenShot(const video::LockedPixelBox& dst);

	virtual void SetTargetWindow(int index);

};

}
}

#endif

