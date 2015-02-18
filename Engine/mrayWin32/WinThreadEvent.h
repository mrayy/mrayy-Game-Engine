

/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   15:25
	filename: 	d:\Development\mrayEngine\Engine\mrayWin32\WinThreadEvent.h
	file path:	d:\Development\mrayEngine\Engine\mrayWin32
	file base:	WinThreadEvent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __WinThreadEvent__
#define __WinThreadEvent__

#include "CompileConfig.h"
#include <IThreadEvent.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace mray{
namespace OS{

class MRAY_Win32_DLL  WinThreadEvent:public IThreadEvent
{
private:
protected:

	HANDLE m_eventHandler;
public:
	WinThreadEvent();
	~WinThreadEvent();

	HANDLE GetEventHandler();

	void SetEvent();
	void ResetEvent();
	void Close();
	EThreadEventRetType Wait(ulong ms);
};


}
}

#endif
