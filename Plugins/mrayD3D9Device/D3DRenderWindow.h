

/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   12:01
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DRenderWindow.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DRenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DRenderWindow__
#define __D3DRenderWindow__

#include "CompileConfig.h"
#include "RenderWindow.h"

namespace mray
{
namespace video
{

	class D3D9Driver;

class D3DRenderWindow:public RenderWindow
{
private:
protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	D3D9Driver* m_driver;
	bool m_closed;
	bool m_Sizing;
	bool m_isExternal;
	bool m_isSwapChain;
	bool m_switchingFullScreen;
	bool m_hwGamma;
	bool m_depthBuffered;


	LPDIRECT3DSWAPCHAIN9 m_swapChain;
	D3DPRESENT_PARAMETERS m_d3dpp;
	LPDIRECT3DSURFACE9 m_renderSurface;
	LPDIRECT3DSURFACE9 m_renderZBuffer;
	D3DMULTISAMPLE_TYPE m_FSAAType;
	DWORD m_fsaaQuality;
	DWORD m_displayFrequency;
	uint m_fsaa;

	uint m_colorDepth;

	void _CreateD3DStaff(const OptionContainer&params);
public:
	D3DRenderWindow(const core::string&name,HINSTANCE hInst,D3D9Driver* driver,LPDIRECT3DDEVICE9 devSwapChain=0);
	virtual~D3DRenderWindow();
	
	virtual void Create(const math::vector2di& size,bool fullScreen,const OptionContainer&params,ulong hwnd);

	virtual void SetFullScreen(const math::vector2di& size,bool fullScreen);

	virtual void Destroy();

	virtual void Resize(const math::vector2di& size);
	virtual void SetPosition(const math::vector2di& pos);
	virtual uint GetColorDepth()const;
	virtual void SetActive(bool a);

	virtual bool IsVisible()const;

	virtual bool IsClosed()const;

	virtual void SwapBuffers(bool vSync);

	virtual void Render(bool swap);

	virtual void OnMoveResize();
	virtual void GetCustomParam(const core::string& name,void*value);

	virtual bool TakeScreenShot(const video::LockedPixelBox& dst);
};


}
}

#endif


