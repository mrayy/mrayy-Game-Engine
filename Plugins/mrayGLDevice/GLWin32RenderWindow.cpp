
#include "stdafx.h"
#include "GLWin32RenderWindow.h"
#include "StringConverter.h"
#include "Win32RenderWindowUtil.h"
#include "RenderWindowUtils.h"
#include "VideoLoggerSystem.h"
#include "Win32GLSupport.h"
#include "Win32Context.h"
#include "ViewPort.h"
#include "VideoLoggerSystem.h"
#include "GLTextureUtil.h"
#include "PixelUtil.h"
#include "Engine.h"
#include "IVideoDevice.h"
#include "GLDev.h"

#include <gl/ExtGL.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#include <OptionContainer.h>
#include <IMonitorDeviceManager.h>
#include <IMonitorDevice.h>

#include <conio.h>

namespace mray
{
namespace video
{
GLWin32RenderWindow::GLWin32RenderWindow(const core::string&name,Win32GLSupport* support,GLDev* device)
:RenderWindow(name),m_hWnd(0),m_hDc(0),m_hRc(0),m_context(0),m_closed(false),m_support(support),m_device(device),m_displayFrequency(50)
{
/*
	DISPLAY_DEVICE d;
	d.cb=sizeof(DISPLAY_DEVICE);
	int monitor=0;
	int i=0;
	int result;

	do
	{

		result=EnumDisplayDevices(0,i,&d,0);
		if(d.StateFlags &DISPLAY_DEVICE_ACTIVE)
		{
			++monitor;
			DISPLAY_DEVICE monitor;
			monitor.cb=sizeof(DISPLAY_DEVICE);
			EnumDisplayDevices(d.DeviceName,0,&monitor,0);
			printf("DisplayDevice:%s\n%s\n%s\n",d.DeviceName,d.DeviceString,d.DeviceID);
			printf("Monitor:%s\n%s\n%s\n",monitor.DeviceName,monitor.DeviceString,monitor.DeviceID);
			DEVMODE dm;
			if(EnumDisplaySettings(d.DeviceName,ENUM_CURRENT_SETTINGS,&dm))
			{
				printf("Freq:%d\nBPP:%d\n",dm.dmDisplayFrequency,dm.dmBitsPerPel);
				printf("Width:%d\Height:%d\n",dm.dmPelsWidth,dm.dmPelsHeight);
			}
		}
		++i;
	}while(result);*/
	m_targetMonitor=0;

}
GLWin32RenderWindow::~GLWin32RenderWindow()
{
	Destroy();
	RenderWindowUtils::RemoveRenderWindow(this);

}
void GLWin32RenderWindow::Create(const math::vector2di& size,bool fullScreen,const OptionContainer&params,ulong hwnd)
{
	if(m_hWnd)
		Destroy();
	m_isFullscreen=fullScreen;
	m_closed=false;
	m_position=-1;
	m_size=size;
	m_colorDepth=m_isFullscreen?32:GetDeviceCaps(GetDC(0),BITSPIXEL);
	m_depthBuffered=true;
	m_isExternal=false;
	m_externalGLRC=false;
	m_fsaa=0;
	bool visible=true;
	m_targetMonitor=0;

	HWND parent=0;
	const SOptionElement* opt=0;
	bool hwGamma=0;
	core::string border(mT(""));
	core::string title(mT("MRAY Engine OGLRenderWindow"));

	opt=params.GetOptionByName(mT("title"));
	if(opt)
		title=opt->value;

	opt=params.GetOptionByName(mT("left"));
	if(opt)
		m_position.x=core::StringConverter::toInt(opt->value);

	opt=params.GetOptionByName(mT("top"));
	if(opt)
		m_position.y=core::StringConverter::toInt(opt->value);

	opt=params.GetOptionByName(mT("border"));
	if(opt)
	{
		border=opt->value;
		border.make_lower();
	}

	opt=params.GetOptionByName(mT("depthBuffer"));
	if(opt)
		m_depthBuffered=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("Vsync"));
	if(opt)
		m_vsync=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("Multisample"));
	if(opt)
		m_fsaa=core::StringConverter::toUInt(opt->value);

	opt=params.GetOptionByName(mT("Monitor"));
	if(opt)
		m_targetMonitor=core::StringConverter::toUInt(opt->value);

	opt=params.GetOptionByName(mT("gamma"));
	if(opt)
		hwGamma=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("Visible"));
	if(opt)
		visible=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("GLContext"));
	if(opt)
	{
		m_hRc=(HGLRC)core::StringConverter::toULong(opt->value);
		if(m_hRc)
			m_externalGLRC=true;
	}


	opt=params.GetOptionByName(mT("colorDepth"));
	if(opt)
	{
		uint cdepth=core::StringConverter::toUInt(opt->value);
		if(!m_isFullscreen)
		{
			if(cdepth<m_colorDepth)
				m_colorDepth=cdepth;
		}else
			m_colorDepth=cdepth;
	}
	opt=params.GetOptionByName(mT("parentHWND"));
	if(opt)
	{
		parent=(HWND)core::StringConverter::toULong(opt->value);
	}

	if(hwnd)
	{
		m_hWnd=(HWND)hwnd;
		m_isExternal=true;
		m_isFullscreen=false;
	}

;
	IMonitorDevice* monitor=IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor);
	if(!monitor)
	{
		m_targetMonitor=0;
		monitor=IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor);
	}
	if(!monitor)
	{
		gLogManager.log(mT("Couldn't Retrive Monitor: ")+core::StringConverter::toString(m_targetMonitor),ELL_WARNING);
	}

	if(!m_isExternal)
	{
		DWORD dwstyle=(visible?WS_VISIBLE :0 ) | WS_CLIPCHILDREN;
		DWORD dwStyleEx = 0;
		math::vector2di outerSz;

		if(m_isFullscreen)
		{
			dwstyle|=WS_POPUP;
			dwStyleEx|=WS_EX_TOPMOST;
			outerSz=m_size;
			m_position=0;
			if(monitor)
 				m_position=monitor->GetStartPosition();
		}else
		{
			if(parent)
				dwstyle|=WS_CHILD;
			else
			{
				if(border==mT("none"))
					dwstyle|=WS_POPUP;
				else if(border==mT("fixed"))
					dwstyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION |
					WS_SYSMENU | WS_MINIMIZEBOX;
				else
					dwstyle |= WS_OVERLAPPEDWINDOW;
			}
			int screenW=GetSystemMetrics(SM_CXSCREEN);
			int screenH=GetSystemMetrics(SM_CYSCREEN);
			if(monitor)
			{
				screenW=monitor->GetSize().x;
				screenH=monitor->GetSize().y;
			}
			if(outerSz==0)
			{
				RECT rc={0,0,m_size.x,m_size.y};
				AdjustWindowRect(&rc,dwstyle,false);
				outerSz.x=(rc.right-rc.left)<screenW?rc.right-rc.left : screenW;
				outerSz.y=(rc.bottom-rc.top)<screenH?rc.bottom-rc.top : screenH;
			}
			if(m_position.x<0)
				m_position.x=(screenW-outerSz.x)/2;
			if(m_position.y<0)
				m_position.y=(screenH-outerSz.y)/2;

			if(m_position.x>screenW-outerSz.x)
				m_position.x=(screenW-outerSz.x);
			if(m_position.y>screenH-outerSz.y)
				m_position.y=(screenH-outerSz.y);
			if(monitor)
				m_position+=monitor->GetStartPosition();
		}

#ifdef MRAY_GLDEVICE_LIB
		HINSTANCE hinst = GetModuleHandle( NULL );
#else
	HINSTANCE hinst = GetModuleHandle(mT("mrayGLDevice.dll"));
#endif
		//HINSTANCE hinst=GetModuleHandle(0);

		WNDCLASS wc={ CS_OWNDC, Win32RenderWindowUtil::_WndProc, 0, 0, hinst,
			LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, mT("MRAYGLWindow") };
		RegisterClass(&wc);

		{
			core::string msg;
			msg=core::StringConverter::toString(m_position);
			msg+=mT(" size:");
			msg+=core::StringConverter::toString(outerSz);
			gLogManager.log(mT("Creating Window:")+msg,ELL_INFO,EVL_Heavy);
		}
		m_hWnd=CreateWindowEx(dwStyleEx, mT("MRAYGLWindow"), title.c_str(),
			dwstyle, m_position.x, m_position.y, outerSz.x, outerSz.y, parent, 0, hinst, this);
		gVideoLoggerSystem.log(mT("GL Window Created :")+m_name,ELL_INFO);

		RenderWindowUtils::AddRenderWindow(this);

		if(m_isFullscreen)
		{/*
			
			MONITORINFOEX monIfo;
			memset(&monIfo,0,sizeof(monIfo));
			monIfo.cbSize=sizeof(monIfo);
			GetMonitorInfo(hMonitor,&monIfo);

			DEVMODE dm;
			bool found=false;
			for(int i=0;!found && EnumDisplaySettings(monIfo.szDevice,i,&dm) ;++i)
			{
				found=(dm.dmPelsWidth==m_size.x && dm.dmPelsHeight==m_size.y  && dm.dmBitsPerPel==m_colorDepth);
			}

			if(!found)
			{
				dm.dmSize=sizeof(DEVMODE);
				dm.dmBitsPerPel = m_colorDepth;
				dm.dmPelsWidth = m_size.x;
				dm.dmPelsHeight = m_size.y;
			}
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			bool changed=false;

			if (m_displayFrequency)
			{
				dm.dmDisplayFrequency = m_displayFrequency;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
				if (ChangeDisplaySettingsEx(monIfo.szDevice,&dm,0, CDS_FULLSCREEN | CDS_TEST,0) != DISP_CHANGE_SUCCESSFUL)
				{
					gVideoLoggerSystem.log(mT("ChangeDisplaySettings : Failed to set user display frequency."),ELL_WARNING);
					dm.dmFields ^= DM_DISPLAYFREQUENCY;
				}
			}
			if (ChangeDisplaySettingsEx(monIfo.szDevice,&dm,0, CDS_FULLSCREEN,0) != DISP_CHANGE_SUCCESSFUL)
				gVideoLoggerSystem.log(mT("ChangeDisplaySettings: Failed"),ELL_WARNING);
			monitor->Refresh();*/
			if(monitor && monitor->ChangeSettings(m_size.x,m_size.y,m_colorDepth,m_displayFrequency))
			{
				//reposition the window in the new monitor position
				SetWindowPos(m_hWnd, 0, monitor->GetStartPosition().x,monitor->GetStartPosition().y, 0, 0,
					SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
	}

	HDC old_hdc = wglGetCurrentDC();
	HGLRC old_context = wglGetCurrentContext();

	RECT rc;

	GetWindowRect(m_hWnd, &rc);
	m_position.x = rc.left;
	m_position.y = rc.top;
	
	GetClientRect(m_hWnd, &rc);
	m_size.x = rc.right;
	m_size.y = rc.bottom;

	m_hDc = GetDC(m_hWnd);

	if (!m_externalGLRC)
	{
		int testFsaa = m_fsaa;
		bool testHwGamma = hwGamma;
		bool formatOk = m_support->selectPixelFormat(m_hDc, m_colorDepth, testFsaa, testHwGamma);
		if (!formatOk)
		{
			if (m_fsaa > 0)
			{
				// try without FSAA
				testFsaa = 0;
				formatOk = m_support->selectPixelFormat(m_hDc, m_colorDepth, testFsaa, testHwGamma);
			}

			if (!formatOk && hwGamma)
			{
				// try without sRGB
				testHwGamma = false;
				testFsaa = m_fsaa;
				formatOk = m_support->selectPixelFormat(m_hDc, m_colorDepth, testFsaa, testHwGamma);
			}

			if (!formatOk && hwGamma && (m_fsaa > 0))
			{
				// try without both
				testHwGamma = false;
				testFsaa = 0;
				formatOk = m_support->selectPixelFormat(m_hDc, m_colorDepth, testFsaa, testHwGamma);
			}

			if (!formatOk)
				gVideoLoggerSystem.log(mT("Win32Window::create() :selectPixelFormat failed"),ELL_ERROR);

		}
		// record what gamma option we used in the end
		// this will control enabling of sRGB state flags when used
		m_hwGamma = testHwGamma;
		m_fsaa = testFsaa;
	}
	if (!m_externalGLRC)
	{
		m_hRc = wglCreateContext(m_hDc);
		if (!m_hRc)
			gVideoLoggerSystem.log(mT("wglCreateContext failed: ") + m_support->TranslateGLErrorString(), ELL_ERROR);
	}
	if (!wglMakeCurrent(m_hDc, m_hRc))
		gVideoLoggerSystem.log(mT("wglMakeCurrent"),ELL_ERROR);

	// Do not change vsync if the external window has the OpenGL control
	if (!m_externalGLRC) {
		// Don't use wglew as if this is the first window, we won't have initialised yet
		PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT = 
			(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (_wglSwapIntervalEXT)
			_wglSwapIntervalEXT(m_vsync? 1 : 0);
	}

	if (old_context && old_context != m_hRc)
	{
		// Restore old context
		if (!wglMakeCurrent(old_hdc, old_context))
			gVideoLoggerSystem.log(mT("wglMakeCurrent() failed"),ELL_ERROR);

		// Share lists with old context
		if (!wglShareLists(old_context, m_hRc))
			gVideoLoggerSystem.log(mT("wglShareLists() failed"),ELL_ERROR);
	}

	SetWindowPos(m_hWnd,0,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);


	// Create RenderSystem context
	m_context = new Win32Context(m_hDc, m_hRc,m_device);

	m_Active = true;
}

void GLWin32RenderWindow::SetFullScreen(const math::vector2di& size,bool fullScreen)
{
	if (m_isFullscreen != fullScreen || size!=size)
	{
		m_isFullscreen = fullScreen;
		DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;

		IMonitorDevice* monitor=IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor);
		if (m_isFullscreen)
		{
			dwStyle |= WS_POPUP;

			monitor->ChangeSettings(size.x,size.y,m_colorDepth,m_displayFrequency);

/*
				
			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmBitsPerPel = m_colorDepth;
			dm.dmPelsWidth = size.x;
			dm.dmPelsHeight = size.y;
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if (m_displayFrequency)
			{
				dm.dmDisplayFrequency = m_displayFrequency;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
				if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
				{
					gVideoLoggerSystem.log(mT("ChangeDisplaySettings with user display frequency failed"),ELL_WARNING);
					dm.dmFields ^= DM_DISPLAYFREQUENCY;
				}
			}
			else
			{
				// try a few
				dm.dmDisplayFrequency = 100;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
				if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
				{
					dm.dmDisplayFrequency = 75;
					if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
					{
						dm.dmFields ^= DM_DISPLAYFREQUENCY;
					}
				}

			}
			if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				gVideoLoggerSystem.log(mT( "ChangeDisplaySettings failed"),ELL_WARNING);*/


			SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
			math::vector2di pos=0;
			if(monitor)
				pos=monitor->GetStartPosition();

			SetWindowPos(m_hWnd, HWND_TOPMOST, pos.x, pos.y, size.x,size.y,
				SWP_NOACTIVATE);
			
			m_size=size;


		}
		else
		{
			dwStyle |= WS_OVERLAPPEDWINDOW;

			monitor->RestoreSettings();

			// calculate overall dimensions for requested client area
			RECT rc = { 0, 0, size.x, size.y};
			AdjustWindowRect(&rc, dwStyle, false);
			unsigned int winWidth = rc.right - rc.left;
			unsigned int winHeight = rc.bottom - rc.top;

			int screenw = GetSystemMetrics(SM_CXSCREEN);
			int screenh = GetSystemMetrics(SM_CYSCREEN);
			int left = (screenw - winWidth) / 2;
			int top = (screenh - winHeight) / 2;


			SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
			SetWindowPos(m_hWnd, HWND_NOTOPMOST, left, top, winWidth, winHeight,
				SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOACTIVATE);
			m_size=size;

		}

	}
}

void GLWin32RenderWindow::Destroy()
{
	if (!m_hWnd)
		return;

	delete m_context;
	m_context=0;

	if (!m_externalGLRC && m_hRc)
	{
		wglDeleteContext(m_hRc);
		m_hRc = 0;
	}
	if (!m_isExternal)
	{
		RenderWindowUtils::RemoveRenderWindow(this);

		if (m_isFullscreen)
		{
			IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor)->ChangeSettings(m_size.x,m_size.y,m_colorDepth,m_displayFrequency);
		}
		DestroyWindow(m_hWnd);
	}
	else
	{
		// just release the DC
		ReleaseDC(m_hWnd, m_hDc);
	}

	m_Active = false;
	m_isClosed = true;
	m_hDc = 0;
	m_hWnd = 0;
}

void GLWin32RenderWindow::Resize(const math::vector2di& size)
{
	if (m_hWnd && !m_isFullscreen)
	{
		RECT rc = { 0, 0, size.x, size.y };
		AdjustWindowRect(&rc, GetWindowLong(m_hWnd, GWL_STYLE), false);
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		SetWindowPos(m_hWnd, 0, 0, 0, width, height,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		FIRE_LISTENR_METHOD(WindowResized,(this));
	}
}
void GLWin32RenderWindow::SetPosition(const math::vector2di& pos)
{
	if (m_hWnd && !m_isFullscreen)
	{
		SetWindowPos(m_hWnd, 0, pos.x,pos.y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		FIRE_LISTENR_METHOD(WindowMoved,(this));
	}
}

void GLWin32RenderWindow::OnMoveResize()
{
	if (!IsVisible())
		return;

	RECT rc;
	// top and left represent outer window position
	GetWindowRect(m_hWnd, &rc);
	m_position.y = rc.top;
	m_position.x = rc.left;
	// width and height represent drawable area only
	GetClientRect(m_hWnd, &rc);

	if (m_size.x == rc.right && m_size.y == rc.bottom)
		return;

	m_size.x = rc.right;
	m_size.y = rc.bottom;

	ViewPortMultiList::iterator vpIt=m_viewports.begin();
	for(;vpIt!=m_viewports.end();++vpIt)
	{
		// Notify viewports of resize
		ViewPortList::iterator it, end;
		it = (*vpIt).begin();
		end = (*vpIt).end();
		for( ; it != end; ++it )
			(*it)->updateViewPort();
	}

}

void GLWin32RenderWindow::SetActive(bool a)
{
	m_Active = a;
	SetForegroundWindow(m_hWnd);
	if( m_isFullscreen )
	{
		if( a == false )
		{	//Restore Desktop
			IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor)->RestoreSettings();
			ShowWindow(m_hWnd, SW_SHOWMINNOACTIVE);
		}
		else
		{	//Restore App
			ShowWindow(m_hWnd, SW_SHOWNORMAL);
			IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor)->ChangeSettings(m_size.x,m_size.y,m_colorDepth,m_displayFrequency);
		}
	}
	FIRE_LISTENR_METHOD(WindowFocusChanged,(this));
}

void GLWin32RenderWindow::Show()
{
	if(!IsVisible())
	{
		ShowWindow(m_hWnd,SW_SHOW);
		if(m_isFullscreen)
			IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor)->ChangeSettings(m_size.x,m_size.y,m_colorDepth,m_displayFrequency);
	}
}
void GLWin32RenderWindow::Hide()
{
	if(IsVisible())
	{
		ShowWindow(m_hWnd,SW_HIDE);
		if(m_isFullscreen)
			IMonitorDeviceManager::getInstance().GetMonitor(m_targetMonitor)->RestoreSettings();
	}
}


bool GLWin32RenderWindow::IsVisible()const
{
	return (m_hWnd && !IsIconic(m_hWnd));
}

void GLWin32RenderWindow::SetActiveWindow()
{
	m_device->switchContext(m_context);
}
void GLWin32RenderWindow::SwapBuffers(bool vSync)
{
	if (!m_externalGLRC) {
		::SwapBuffers(m_hDc);
	}
	FIRE_LISTENR_METHOD(WindowSwapBuffer,(this))
}

void GLWin32RenderWindow::GetCustomParam(const core::string& name,void*value)
{
	if(name==mT("WINDOW"))
		(*(HWND*)value)=m_hWnd;
}
bool  GLWin32RenderWindow::TakeScreenShot(const LockedPixelBox& dst)
{

	const math::box3d& box=dst.box;
	if(box.MinP.x<0 || box.getWidth()>m_size.x ||
		box.MinP.y<0 || box.getHeight()>m_size.y ||
		box.MinP.z!=0 || box.getDepth()!=1)
	{
		gVideoLoggerSystem.log(mT("GLWin32RenderWindow::TakeScreenShot() : Invalid Box"),ELL_WARNING);
		return false;
	}

	bool backBuffer=m_isFullscreen? false : true;

	GLenum format=GLTextureUtil::getGLOrginalFormat(dst.format);
	GLenum type=GLTextureUtil::getGLDataType(dst.format);


	if ((format == GL_NONE) || (type == 0))
	{
		gVideoLoggerSystem.log(mT("GLWin32RenderWindow::TakeScreenShot() : Unsupported format"),ELL_WARNING);
		return false;
	}
	IVideoDevice* device= Engine::getInstance().getDevice();
	scene::ViewPort* vp= *(*m_viewports.begin()).begin();

	device->setViewport(vp);


	// Must change the packing to ensure no overruns!
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//read in the screen data

	glReadBuffer(backBuffer ? GL_BACK:GL_FRONT);

	glReadPixels((GLint)box.MinP.x, (GLint)box.MinP.y,(GLsizei)box.getWidth(),(GLsizei)box.getHeight(),format,type, dst.data);

	// restore default alignment
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	//vertical flip
	if(0)
	{
		size_t rowSpan = box.getWidth() * PixelUtil::getPixelDescription(dst.format).elemSizeB;
		size_t height = box.getHeight();
		uchar *tmpData = new uchar[rowSpan * height];
		uchar *srcRow = (uchar *)dst.data, *tmpRow = tmpData + (height - 1) * rowSpan;

		while (tmpRow >= tmpData)
		{
			memcpy(tmpRow, srcRow, rowSpan);
			srcRow += rowSpan;
			tmpRow -= rowSpan;
		}
		memcpy(dst.data, tmpData, rowSpan * height);

		delete [] tmpData;
	}

	return true;
}
void GLWin32RenderWindow::Render(bool swap)
{
	RenderWindow::Render(swap);
}

void GLWin32RenderWindow::SetTargetWindow(int index)
{
	IMonitorDevice* monitor=IMonitorDeviceManager::getInstance().GetMonitor(index);
	if(!monitor)
		return;
	m_targetMonitor=index;
	SetPosition(monitor->GetStartPosition());

}

}
}

