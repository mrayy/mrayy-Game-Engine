
#include "stdafx.h"
#include "D3DRenderWindow.h"
#include "D3D9Driver.h"

#include "StringConverter.h"
#include "RenderWindowUtils.h"
#include "VideoLoggerSystem.h"
#include "ViewPort.h"
#include "VideoLoggerSystem.h"
#include "PixelUtil.h"
#include "Engine.h"
#include "IVideoDevice.h"
#include "D3D9Device.h"
#include "..\mrayWin32\Win32RenderWindowUtil.h"
#include <OptionContainer.h>

namespace mray
{
namespace video
{

D3DRenderWindow::D3DRenderWindow(const core::string&name,HINSTANCE hInst,D3D9Driver* driver,LPDIRECT3DDEVICE9 devSwapChain):
	RenderWindow(name),
	m_hInst(hInst),m_driver(driver),m_renderSurface(0),m_renderZBuffer(0),
	m_isSwapChain(devSwapChain!=0),m_hWnd(0),m_isExternal(0),m_Sizing(0),
	m_closed(0),m_switchingFullScreen(0),m_displayFrequency(0),m_colorDepth(0)
{
}

D3DRenderWindow::~D3DRenderWindow()
{
	Destroy();
}


void D3DRenderWindow::Create(const math::vector2di& size,bool fullScreen,const OptionContainer&params,ulong hwnd)
{
	if(m_hWnd)
		Destroy();
	m_isFullscreen=fullScreen;
	m_closed=false;
	m_position=-1;
	m_size=size;
	m_colorDepth=32;
	m_isExternal=false;
	m_fsaaQuality=0;
	m_FSAAType=D3DMULTISAMPLE_NONE;
	m_vsync=false;
	m_position=-1;
	m_depthBuffered=true;
	m_fsaa=0;
	m_hwGamma=false;

	HWND parent=0;
	const SOptionElement* opt=0;
	core::string border(mT(""));
	core::string title(mT("MRAY Engine D3DRenderWindow"));

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

	opt=params.GetOptionByName(mT("vsync"));
	if(opt)
		m_vsync=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("displayFrequncy"));
	if(opt)
		m_displayFrequency=core::StringConverter::toUInt(opt->value);

	opt=params.GetOptionByName(mT("FSAA"));
	if(opt)
	{
		m_fsaa=core::StringConverter::toUInt(opt->value);
		m_FSAAType=(D3DMULTISAMPLE_TYPE)m_fsaa;
	}
	opt=params.GetOptionByName(mT("FSAAQuality"));
	if(opt)
		m_fsaaQuality=core::StringConverter::toUInt(opt->value);

	opt=params.GetOptionByName(mT("gamma"));
	if(opt)
		m_hwGamma=core::StringConverter::toBool(opt->value);

	opt=params.GetOptionByName(mT("colorDepth"));
	if(opt)
		m_colorDepth=core::StringConverter::toUInt(opt->value);

	opt=params.GetOptionByName(mT("depthBuffer"));
	if(opt)
		m_depthBuffered=core::StringConverter::toBool(opt->value);
	opt=params.GetOptionByName(mT("parentHWND"));
	if(opt)
	{
		parent=(HWND)core::StringConverter::toULong(opt->value);
	}
	m_isExternal=(hwnd!=0);


	if(!m_isExternal)
	{

		DWORD dwstyle=WS_VISIBLE | WS_CLIPCHILDREN;
		DWORD dwStyleEx = 0;
		math::vector2di outerSz;

		if(m_isFullscreen)
		{
			dwstyle|=WS_POPUP;
			dwStyleEx|=WS_EX_TOPMOST;
			outerSz=m_size;
			m_position=0;
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
		}

		WNDCLASS wc={ CS_OWNDC, Win32RenderWindowUtil::_WndProc, 0, 0, m_hInst,
			LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, mT("MRAYGLWindow") };
		RegisterClass(&wc);

		m_hWnd=CreateWindowEx(dwStyleEx, mT("MRAYGLWindow"), title.c_str(),
			dwstyle, m_position.x, m_position.y, outerSz.x, outerSz.y, parent, 0, m_hInst, this);

		RenderWindowUtils::AddRenderWindow(this);

		gVideoLoggerSystem.log(mT("GL Window Created :")+m_name,ELL_INFO);
	}else
	{
		m_hWnd=(HWND)hwnd;
	}

	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	m_position.x=rc.left;
	m_position.y=rc.top;
	GetClientRect(m_hWnd,&rc);
	m_size.x=rc.right;
	m_size.y=rc.bottom;

	m_Active=true;
	m_closed=false;

	_CreateD3DStaff(params);
}

void D3DRenderWindow::_CreateD3DStaff(const OptionContainer&params)
{
	LPDIRECT3DDEVICE9 lpDev=m_driver->GetD3DDevice();
	if(m_isSwapChain && !lpDev)
	{
		gVideoLoggerSystem.log(mT("D3DRenderWindow::_CreateD3DStaff() - secondary window hasn't been given the device from the primary"),ELL_ERROR);
	}
	SAFE_RELEASE(m_renderSurface);

	HRESULT hr;
	LPDIRECT3D9 pD3D=m_driver->GetD3D();
	D3DDEVTYPE devT=D3DDEVTYPE_HAL;

	ZeroMemory(&m_d3dpp,sizeof(m_d3dpp));
	m_d3dpp.Windowed	=!m_isFullscreen;
	m_d3dpp.BackBufferCount=m_vsync ? 2: 1;
	m_d3dpp.EnableAutoDepthStencil=m_depthBuffered;
	m_d3dpp.hDeviceWindow=m_hWnd;
	m_d3dpp.BackBufferWidth=m_size.x;
	m_d3dpp.BackBufferHeight=m_size.y;
	m_d3dpp.FullScreen_RefreshRateInHz=m_isFullscreen?m_displayFrequency:0;
	m_d3dpp.SwapEffect	=D3DSWAPEFFECT_DISCARD;

	if(m_vsync)
	{
		m_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	}else
	{
		m_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	m_d3dpp.BackBufferFormat=D3DFMT_R5G6B5;
	if(m_colorDepth>16)
	{
		m_d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;

		//try to create a 32 bit depth,8 bit stencil
		if(FAILED(pD3D->CheckDeviceFormat(m_driver->GetAdapterNumber(),devT,m_d3dpp.BackBufferFormat,
			D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		{
			//no 8 bit hardware stencil, try 32 bit zbuffer
			if(FAILED(pD3D->CheckDeviceFormat(m_driver->GetAdapterNumber(),devT,  m_d3dpp.BackBufferFormat,  D3DUSAGE_DEPTHSTENCIL, 
				D3DRTYPE_SURFACE, D3DFMT_D32)))
				//fallback to 16 bit depth
				m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16;
			else
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		}else
		{
			if(SUCCEEDED(pD3D->CheckDepthStencilMatch( m_driver->GetAdapterNumber(), devT,
				m_d3dpp.BackBufferFormat, m_d3dpp.BackBufferFormat, D3DFMT_D24S8 ) ) )
			{
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; 
			} 
			else 
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8; 
		}
	}else
		m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16;

	m_d3dpp.MultiSampleType = m_FSAAType;
	m_d3dpp.MultiSampleQuality =  m_fsaaQuality;

	if(m_isSwapChain)
	{
	}else
	{
		if (!lpDev)
		{
			// We haven't created the device yet, this must be the first time

			// Do we want to preserve the FPU mode? Might be useful for scientific apps
			DWORD extraFlags = 0;

			const SOptionElement* opt=0;

			opt=params.GetOptionByName(mT("FloatingPoint"));
			if(opt && opt->value=="Consistent")
				extraFlags |= D3DCREATE_FPU_PRESERVE;
#if MRAY_THREAD_SUPPORT == 1
			extraFlags |= D3DCREATE_MULTITHREADED;
#endif
			// Set default settings (use the one Ogre discovered as a default)
			UINT adapterToUse = m_driver->GetAdapterNumber();
/*
			if (mUseNVPerfHUD)
			{
				// Look for 'NVIDIA NVPerfHUD' adapter (<= v4)
				// or 'NVIDIA PerfHUD' (v5)
				// If it is present, override default settings
				for (UINT adapter=0; adapter < m_driver->GetD3D()->GetAdapterCount(); ++adapter)
				{
					D3DADAPTER_IDENTIFIER9 identifier;
					HRESULT res;
					res = mDriver->getD3D()->GetAdapterIdentifier(adapter,0,&identifier);
					if (strstr(identifier.Description,"PerfHUD") != 0)
					{
						adapterToUse = adapter;
						devT = D3DDEVTYPE_REF;
						break;
					}
				}
			}*/

			int tryCount=0;
			do
			{
				hr = pD3D->CreateDevice(adapterToUse, devT, m_hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_d3dpp, &lpDev );
				++tryCount;
			}while(FAILED(hr) && tryCount<4);

			if( FAILED( hr ) )
			{
				// software device
				devT = D3DDEVTYPE_REF;
				hr = pD3D->CreateDevice( adapterToUse, devT, m_hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_d3dpp, &lpDev );
			}
			if( FAILED( hr ) )
			{
				Destroy();
				gVideoLoggerSystem.log(mT("D3DRenderWindow::_CreateD3DStaff() - Failed to create D3D Device"), ELL_ERROR);
			}
		}
		// update device in driver
		m_driver->SetD3DDevice( lpDev );
		// Store references to buffers for convenience
		SAFE_RELEASE(m_renderSurface);
		lpDev->GetRenderTarget( 0, &m_renderSurface );
		SAFE_RELEASE(m_renderZBuffer);
		lpDev->GetDepthStencilSurface( &m_renderZBuffer );
		// release immediately so we don't hog them
		m_renderZBuffer->Release();
	}
}

void D3DRenderWindow::SetFullScreen(const math::vector2di& size,bool fullScreen)
{
}


void D3DRenderWindow::Destroy()
{
	if(m_isSwapChain)
	{
		SAFE_RELEASE(m_renderSurface);
		SAFE_RELEASE(m_renderZBuffer);
	}else
	{
		m_renderZBuffer=0;
	}
	SAFE_RELEASE(m_renderSurface);

	if(m_hWnd && !m_isExternal)
	{
		RenderWindowUtils::RemoveRenderWindow(this);
		DestroyWindow(m_hWnd);
	}
	m_hWnd=0;
	m_Active=false;
	m_closed=true;
}


void D3DRenderWindow::Resize(const math::vector2di& size)
{
	if(m_hWnd && !m_isFullscreen)
	{
		RECT rc = { 0, 0, size.x, size.y };
		AdjustWindowRect(&rc, GetWindowLong(m_hWnd, GWL_STYLE), false);
		LONG width = rc.right - rc.left;
		LONG height = rc.bottom - rc.top;
		SetWindowPos(m_hWnd, 0, 0, 0, width, height,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void D3DRenderWindow::SetPosition(const math::vector2di& pos)
{
	if(m_hWnd && !m_isFullscreen)
	{
		SetWindowPos(m_hWnd,0,pos.x,pos.y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

uint D3DRenderWindow::GetColorDepth()const
{
	return m_colorDepth;
}
void D3DRenderWindow::SetActive(bool a)
{
}


bool D3DRenderWindow::IsVisible()const
{
	return (m_hWnd && !IsIconic(m_hWnd));
}


bool D3DRenderWindow::IsClosed()const
{
	return m_closed;
}

void D3DRenderWindow::SwapBuffers(bool vSync)
{

	// access device through driver
	LPDIRECT3DDEVICE9 mpD3DDevice = m_driver->GetD3DDevice();
	if( mpD3DDevice )
	{
		HRESULT hr;
		if (m_isSwapChain)
		{
			hr = m_swapChain->Present(NULL, NULL, NULL, NULL, 0);
		}
		else
		{
			hr = mpD3DDevice->Present( NULL, NULL, 0, NULL );
		}
		if( hr == D3DERR_DEVICELOST  )
		{
			SAFE_RELEASE(m_renderSurface);
			static_cast<D3D9Device*>(Engine::getInstance().getDevice().pointer())->_notifyDeviceLost();
		}
		else if( FAILED(hr) )
			gVideoLoggerSystem.log(mT("D3DRenderWindow::SwapBuffers() - Error presenting surfaces"),ELL_WARNING);
	}
}


void D3DRenderWindow::Render(bool swap)
{
	D3D9Device* rs = static_cast<D3D9Device*>(Engine::getInstance().getDevice().pointer());


	// access device through driver
	LPDIRECT3DDEVICE9 mpD3DDevice = m_driver->GetD3DDevice();

	if (rs->IsDeviceLost())
	{
		// Test the cooperative mode first
		HRESULT hr = mpD3DDevice->TestCooperativeLevel();
		if (hr == D3DERR_DEVICELOST)
		{
			// device lost, and we can't reset
			// can't do anything about it here, wait until we get 
			// D3DERR_DEVICENOTRESET; rendering calls will silently fail until 
			// then (except Present, but we ignore device lost there too)
			SAFE_RELEASE(m_renderSurface);
			// need to release if swap chain
			if (!m_swapChain)
				m_renderZBuffer = 0;
			else
				SAFE_RELEASE (m_renderZBuffer);
			Sleep(50);
			return;
		}
		else
		{
			// device lost, and we can reset
			rs->RestoreLostDevice();

			// Still lost?
			if (rs->IsDeviceLost())
			{
				// Wait a while
				Sleep(50);
				return;
			}

			if (!m_swapChain) 
			{
				// re-qeuery buffers
				SAFE_RELEASE(m_renderSurface);
				mpD3DDevice->GetRenderTarget( 0, &m_renderSurface );
				SAFE_RELEASE(m_renderZBuffer);
				mpD3DDevice->GetDepthStencilSurface( &m_renderZBuffer );
				// release immediately so we don't hog them
				m_renderZBuffer->Release();
			}
			else 
			{
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
		}

	}
	RenderWindow::Render(swap);
}

void D3DRenderWindow::OnMoveResize()
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


	SAFE_RELEASE(m_renderSurface);
	if(m_isSwapChain)
	{

		D3DPRESENT_PARAMETERS pp = m_d3dpp;

		pp.BackBufferWidth  = rc.right;
		pp.BackBufferHeight = rc.bottom;

		SAFE_RELEASE( m_renderZBuffer );
		SAFE_RELEASE( m_swapChain );

		HRESULT hr = m_driver->GetD3DDevice()->CreateAdditionalSwapChain(
			&pp,
			&m_swapChain);

		if (FAILED(hr)) 
		{
			gVideoLoggerSystem.log(mT("D3DRenderWindow::OnMoveResize() - Faield to reset device to new dimensions"),ELL_WARNING);
			
			// try to recover
			hr = m_driver->GetD3DDevice()->CreateAdditionalSwapChain(
				&m_d3dpp,
				&m_swapChain);

			if (FAILED(hr))
				gVideoLoggerSystem.log(mT("D3DRenderWindow::OnMoveResize() - Faield to recover device's old dimensions"),ELL_ERROR);
		}	
		else 
		{
			m_d3dpp = pp;

			m_size.x = rc.right;
			m_size.y = rc.bottom;

			SAFE_RELEASE(m_renderSurface);
			hr = m_swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_renderSurface);
			SAFE_RELEASE(m_renderZBuffer);
			hr = m_driver->GetD3DDevice()->CreateDepthStencilSurface(
				m_size.x , m_size.y,
				m_d3dpp.AutoDepthStencilFormat,
				m_d3dpp.MultiSampleType,
				m_d3dpp.MultiSampleQuality, 
				(m_d3dpp.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL),
				&m_renderZBuffer, NULL
				);

			if (FAILED(hr)) 
				gVideoLoggerSystem.log(mT("D3DRenderWindow::OnMoveResize() - Faield to create depth stencil surface for swap chain"),ELL_ERROR);
		}
	}
	// primary windows must reset the device
	else 
	{
		m_d3dpp.BackBufferWidth = m_size.x = rc.right;
		m_d3dpp.BackBufferHeight = m_size.y = rc.bottom;
		static_cast<D3D9Device*>(Engine::getInstance().getDevice().pointer())->_notifyDeviceLost();
	}

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

void D3DRenderWindow::GetCustomParam(const core::string& name,void*value)
{
	if(name==mT("WINDOW"))
		(*(HWND*)value)=m_hWnd;
}


bool D3DRenderWindow::TakeScreenShot(const video::LockedPixelBox& dst)
{
	return false;
}



}
}

