
#include "stdafx.h"
#include "WinThreadEvent.h"


namespace mray{
namespace OS{

WinThreadEvent::WinThreadEvent()
{
	m_eventHandler=CreateEvent(NULL, TRUE, FALSE, NULL);
}
WinThreadEvent::~WinThreadEvent()
{
	if(m_eventHandler!=NULL)
	{
		CloseHandle(m_eventHandler);
	}
}

HANDLE WinThreadEvent::GetEventHandler()
{
	return m_eventHandler;
}

void WinThreadEvent::SetEvent()
{

	if(m_eventHandler!=0)
	{
		::SetEvent(m_eventHandler);
	}
}
void WinThreadEvent::ResetEvent()
{
	if(m_eventHandler!=0)
	{
		::ResetEvent(m_eventHandler);
	}
}
void WinThreadEvent::Close()
{
	if(m_eventHandler!=0)
	{
		::CloseHandle(m_eventHandler);
		m_eventHandler = NULL;
	}
}
EThreadEventRetType WinThreadEvent::Wait(ulong ms)
{
	if(m_eventHandler!=0)
	{
		DWORD ret=WaitForSingleObject(m_eventHandler,INFINITE);
		switch(ret)
		{
		case WAIT_TIMEOUT:
			return ETEvent_Timeout;
		case WAIT_FAILED:
		case WAIT_ABANDONED:
			return ETEvent_Failed;
		default:
			return ETEvent_Object0;
		}
	}
	return ETEvent_Failed;
}


}
}
