#include "stdafx.h"


#include "WinThreadManager.h"
#include "WinThread.h"
#include "WinMutex.h"
#include "WinCondition.h"
#include "WinThreadEvent.h"

namespace mray{
namespace OS{



WinThreadManager::WinThreadManager(){
	m_tlsID=TlsAlloc();
}
WinThreadManager::~WinThreadManager(){
	TlsFree(m_tlsID);
}
IThread* WinThreadManager::createThread(IThreadFunction*f,EThreadPriority priority){
	IThread* t= new WinThread(f);
	t->setThreadPriority(priority);
	m_threads.push_back(t);
	return t;
}
IMutex* WinThreadManager::createMutex(){
	return new WinMutex();
}
ICondition* WinThreadManager::createCondition(){
	return new WinCondition();
}

IThreadEvent* WinThreadManager::createEvent()
{
	return new WinThreadEvent();
}
void WinThreadManager::sleep(uint ms){
	Sleep(ms);
}

IThread*WinThreadManager::getCurrentThread(){
	void*data= TlsGetValue(m_tlsID);
	if(!data)
		return 0;
	return (IThread*)data;
}

int WinThreadManager::WaitForEvents(IThreadEvent**events,int count,bool waitForAll,ulong ms)
{
	std::vector<HANDLE> handles;
	for(int i=0;i<count;++i)
	{
		handles.push_back(((WinThreadEvent*)events[i])->GetEventHandler());
	}
	DWORD ret=WaitForMultipleObjects(count,&handles[0],waitForAll,ms);

	switch(ret)
	{
	case WAIT_TIMEOUT:
		return -1;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		return -1;
	default:
		return ETEvent_Object0+ret-WAIT_OBJECT_0;
	}
}

}
}

