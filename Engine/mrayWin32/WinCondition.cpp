#include "stdafx.h"

#include "WinCondition.h"
#include "IThreadManager.h"
#include "WinThread.h"
#include "IMutex.h"
#include "MutexLocks.h"


namespace mray{
namespace OS{


WinCondition::WinCondition(){
	m_waiters=0;
	m_sema=CreateSemaphore(NULL,0,0x7fffffff,NULL);
	m_waitersDone=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_wasBroadcast=0;
}
WinCondition::~WinCondition(){
	CloseHandle(m_sema);
	CloseHandle(m_waitersDone);
}

int WinCondition::cooperativeWait(HANDLE h,uint ms){
	int res=0;
	WinThread*wt=dynamic_cast<WinThread*>(IThreadManager::getInstance().getCurrentThread());
	if(wt){
		HANDLE cancelHandle = wt->getCancelEvent();
		HANDLE handleSet[2] = {h, cancelHandle};

		res = WaitForMultipleObjects(2,handleSet,FALSE,ms);
		if(res == WAIT_OBJECT_0 + 1 ) {
			return -1;
		}
	}else{
		res=WaitForSingleObject(h,ms);
	}
	return res;
}

int WinCondition::wait(IMutex*m){
	return wait(m,INFINITE);
}
int WinCondition::wait(IMutex*m,uint ms){
	// Prevent race conditions on the <waiters_> count.
	InterlockedIncrement(&m_waiters);

	int result = 0;

	ReverseScopedLock lock(m);

	// wait in timeslices, giving testCancel() a change to
	// exit the thread if requested.
	result = 	cooperativeWait(m_sema, ms);
	if(result ==-1){
		// thread is canceled in cooperative wait , do cleanup
		InterlockedDecrement(&m_waiters);
		long w = InterlockedExchangeAdd(&m_waiters,0);
		int last_waiter = m_wasBroadcast && w == 0;

		if (last_waiter)  SetEvent(m_waitersDone);
		// rethrow
		return -1;
	}


	// We're ready to return, so there's one less waiter.
	InterlockedDecrement(&m_waiters);
	long w = InterlockedExchangeAdd(&m_waiters,0);
	int last_waiter = m_wasBroadcast && w == 0;

	if (result != -1 && last_waiter)
		SetEvent(m_waitersDone);

	return result;
}

int WinCondition::signal(){
	long w = InterlockedExchangeAdd(&m_waiters,0);
	int have_waiters = w > 0;

	int result = 0;

	if (have_waiters)
	{
		if( !ReleaseSemaphore(m_sema,1,NULL) )
			result = -1;
	}
	return result;
}

int WinCondition::broadcast(){
	int have_waiters = 0;
	long w = InterlockedExchangeAdd(&m_waiters,0);

	if (w > 0)
	{
		// we are broadcasting.  
		m_wasBroadcast = 1;
		have_waiters = 1;
	}

	int result = 0;
	if (have_waiters)
	{
		// Wake up all the waiters.
		ReleaseSemaphore(m_sema,m_waiters,NULL);

		cooperativeWait(m_waitersDone, INFINITE);

		//end of broadcasting
		m_wasBroadcast = 0;
	}
	return result;
}


}
}

