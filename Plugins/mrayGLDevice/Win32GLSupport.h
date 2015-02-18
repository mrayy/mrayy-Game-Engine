


/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   11:00
	filename: 	d:\Development\mrayEngine\Engine\mrayGLDevice\Win32GLSupport.h
	file path:	d:\Development\mrayEngine\Engine\mrayGLDevice
	file base:	Win32GLSupport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Win32GLSupport__
#define __Win32GLSupport__

#include "GLSupport.h"

#include <Windows.h>
#include <mstring.h>
#include <set>

namespace mray
{
namespace video
{
	class GLDev;
	class GLWin32RenderWindow;

class MRAY_GL_DLL Win32GLSupport:public GLSupport
{
private:
protected:
	bool m_HasPixelFormatARB;
	bool m_HasMultisample;
	bool m_HasHardwareGamma;
	std::vector<int> m_fsaaSupport;

	GLWin32RenderWindow* m_InitialWindow;

	std::vector<DEVMODE> m_deviceModes;
	GLDev* m_device;

public:
	Win32GLSupport();
	virtual~Win32GLSupport();

	virtual void getConfigParams(OptionContainer& params);
	GLDev* GetDevice(){return m_device;}
	void SetDevice(GLDev*dev){m_device=dev;}

	void InitaliseWGL();

	bool selectPixelFormat(HDC hdc, int colourDepth, int multisample, bool hwGamma);

	static core::string TranslateGLErrorString();
	static core::string TranslateGLErrorString(int code);

	virtual void InitExtensions();


	virtual RenderWindow* CreateRenderWindow(const core::string& name,const math::vector2di& size,
		bool fullScreen,const OptionContainer&params,ulong hWND);
protected:
	static LRESULT CALLBACK dummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp);
};


}
}

#endif
