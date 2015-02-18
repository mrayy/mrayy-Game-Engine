


/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   10:42
	filename: 	d:\Development\mrayEngine\Engine\mrayGLDevice\Win32RenderWindowUtil.h
	file path:	d:\Development\mrayEngine\Engine\mrayGLDevice
	file base:	Win32RenderWindowUtil
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Win32RenderWindowUtil__
#define __Win32RenderWindowUtil__

#include "compileConfig.h"
#include <windows.h>

namespace mray
{
namespace video
{

class Win32RenderWindowUtil
{
private:
protected:
public:
	Win32RenderWindowUtil(){}
	virtual~Win32RenderWindowUtil(){}


	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	

};

}
}

#endif
