#include "stdafx.h"


#include "WinTimer.h"
#include <stdio.h>
#include "mMath.h"

namespace mray{
	


namespace OS{


WinTimer::WinTimer(){
	m_timerMask = 0;

	initTimer();
}

/**/
void WinTimer::initTimer()
{
	DWORD_PTR procMask;
	DWORD_PTR sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	// If procMask is 0, consider there is only one core available
	// (using 0 as procMask will cause an infinite loop below)
	if (procMask == 0)
		procMask = 1;

	// Find the lowest core that this process uses
	if (m_timerMask == 0)
	{
		m_timerMask = 1;
		while ((m_timerMask & procMask) == 0)
		{
			m_timerMask <<= 1;
		}
	}

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_timerMask);

	// Get the constant frequency
	QueryPerformanceFrequency(&m_frequency);

	// Query the timer
	QueryPerformanceCounter(&m_startTime);
	m_startTick = GetTickCount();

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	m_lastTime = 0;

}

ulong WinTimer::getMilliseconds(void)
{
	
	LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_timerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

	// scale by 1000 for milliseconds
	unsigned long newTicks = (unsigned long)(1000 * newTime / m_frequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - m_startTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
		m_startTime.QuadPart += adjust;
		newTime -= adjust;

		// Re-calculate milliseconds
		newTicks = (unsigned long)(1000 * newTime / m_frequency.QuadPart);
	}

	// Record last time for adjust
	m_lastTime = newTime;

	return newTicks;

	
}

ulong WinTimer::getMicroseconds(void)
{
	LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_timerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

	// get milliseconds to check against GetTickCount
	unsigned long newTicks = (unsigned long)(1000 * newTime / m_frequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - m_startTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
		m_startTime.QuadPart += adjust;
		newTime -= adjust;
	}

	// Record last time for adjust
	m_lastTime = newTime;

	// scale by 1000000 for microseconds
	unsigned long newMicro = (unsigned long)(1000000 * newTime / m_frequency.QuadPart);

	return newMicro;
}





}
}
