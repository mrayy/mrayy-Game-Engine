

#ifndef ___WinTIMER___
#define ___WinTIMER___


#include <mTypes.h>
#include "compileConfig.h"
#include <ITimer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace mray{
namespace OS{

class MRAY_Win32_DLL WinTimer:public ITimer
{
	
	LONGLONG m_lastTime;
	DWORD m_startTick;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_frequency;
	DWORD_PTR m_timerMask;

public:
	WinTimer();
	virtual~WinTimer(){};

	void initTimer();

	//! get ms value
	virtual ulong getMilliseconds(void);;

	virtual ulong getMicroseconds();
	virtual double getSeconds()
	{
		return (double)getMicroseconds()*0.001;
	}
	
};


};//OS
};//mray



#endif






















