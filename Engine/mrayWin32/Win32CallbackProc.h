
/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   18:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayWin32\Win32CallbackProc.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayWin32
	file base:	Win32CallbackProc
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Win32CallbackProc___
#define ___Win32CallbackProc___

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ISingleton.h>
#include "compileConfig.h"

namespace mray{

class MRAY_Win32_DLL Win32MessageProc:public ISingleton<Win32MessageProc>
{
public:
	Win32MessageProc();
	virtual~Win32MessageProc();


	LRESULT WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
};

}


#endif //___Win32CallbackProc___
