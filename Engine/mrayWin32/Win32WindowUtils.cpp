#include "stdafx.h"


#include "Win32WindowUtils.h"
#include <Event.h>
#include <ChangeModeEvent.h>
#include <SystemEvent.h>
#include <EventQueue.h>
#include <EventMemoryManager.h>
#include <CMemory.h>
#include <LogManager.h>
#include <IEventReciver.h>
#include <ITimer.h>
#include <TraceManager.h>
#include <stringConverter.h>
#include <sstream>
#include "Win32CallbackProc.h"
#include "WinCursorControl.h"
//#include <DefaultInputManager.h>
 

namespace mray{



Win32WindowUtils::Win32WindowUtils(){

	traceFunction(eEngine);

	m_cursorControl=new WinCursorControl(0,1,0);


	if(EventQueue::isExist()){
		std::set<uint> mask;
		mask.insert(ET_SystemEvent.ID());
		EventQueue::getInstance().addEventHandler(this,mask); 
	}
}

Win32WindowUtils::~Win32WindowUtils()
{
	traceFunction(eEngine);
	if(EventQueue::isExist()){
		EventQueue::getInstance().removeEventHandler(this);
	}

	m_cursorControl=0;
	gLogManager.log(mT("Win32WindowUtils shutdown"),ELL_SUCCESS);
	
}

void Win32WindowUtils::shutdown()
{
	traceFunction(eEngine);

	
	PostQuitMessage(0);

	
}


bool Win32WindowUtils::doMessagePump()
{
	traceFunction(eEngine);
	MSG msg;
	bool done=0;


//	OS::gTimer.tick();
	while(PeekMessage(&msg,0,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message==WM_QUIT)
			done=1;
	}
//	if(InputManager::isExist())
//		InputManager::getInstance().capture();
	if(ICursorControl::isExist() ){
		ICursorControl::getInstance().updatePos();
 	}
	return !done;
}

void Win32WindowUtils::onEvent(Event* event)
{
	traceFunction(eEngine);
	if(event->getType()==ET_SystemEvent)
	{
		SystemEvent*e=dynamic_cast<SystemEvent*>(event);
		switch(e->m_eventType){
			case ESET_Show:
				break;
			case ESET_Hide:
				break;
			case ESET_LostFocus:
				break;
			case ESET_GotFocus:
				break;
			case ESET_LostActive:
				break;
			case ESET_GotActive:
				break;
			case ESET_Resized:
				break;
			case ESET_Destroy:
				shutdown();
				break;
		}
	}
	
}






}