

#include <windowsx.h>
#include "mozillaWin.h"

#include <LogManager.h>
#include <TraceManager.h>
#include <Event.h>
#include <MouseEvent.h>
#include <KeyboardEvent.h>
#include <ChangeModeEvent.h>
#include <SystemEvent.h>

#include <EventQueue.h>
#include <EventMemoryManager.h>

#include <Win32CallbackProc.h>
#include <WinCursorControl.h>

#include <CMouseController.h>
#include <CKeyboardController.h>

#include "CMozillaPlugin.h"

namespace mray{
	Win32MessageProc* g_messageProcManager=0;

	LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
	{
		if(msg==WM_PAINT || msg==WM_TIMER){
			CMozillaPlugin::getInstance().loopBody(core::CFPS::getInstance().dt());
		}
		if(g_messageProcManager)
			return g_messageProcManager->WndProc(hWnd,msg,wParam,lParam);
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}

mozillaWin::mozillaWin(){

	g_messageProcManager=new Win32MessageProc();
	m_cursorControl=new WinCursorControl(0,1,0);
	if(EventQueue::isExist()){
		EventQueue::getInstance().addEventHandler(this,
			Event::getEventMaskingValue(ET_Mouse) | 
			Event::getEventMaskingValue(ET_ResizeEvent) | 
			Event::getEventMaskingValue(ET_Keyboard) | 
			Event::getEventMaskingValue(ET_SystemEvent) | 
			Event::getEventMaskingValue(ET_Joystic) ); 
	}
}
mozillaWin::~mozillaWin(){
	delete g_messageProcManager;
	g_messageProcManager=0;
}

void mozillaWin::windowResized(){
	m_resized=true;
}
void mozillaWin::resizeDevice(){
	if(!m_resized)return;
	m_resized=false;
}

bool mozillaWin::startup(HWND hWnd,math::Point2di size,int bits){

	traceFunction(eEngine);
	gLogManager.startSection(mT("mozillaWin"));

	WinSize=size;
	WinBits=bits;

	m_resized=true;

	this->hWnd=hWnd;

	RECT r;
	GetWindowRect(hWnd, &r);
	WinSize.x = r.right - r.left;
	WinSize.y = r.bottom - r.top;


	m_mouse=new controllers::CMouseController();
	m_keyboard=new controllers::CKeyboardController();

	((WinCursorControl*)m_cursorControl.pointer())->resetHWND(hWnd);
	m_cursorControl->setWindowSize(WinSize);

	m_oldProc = SubclassWindow(hWnd, (WNDPROC)WndProc);

	// associate window with our CMozillaPlugin object so we can access 
	// it in the window procedure
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);

	gLogManager.endSection(true);


	return true;
}

bool mozillaWin::restart(){

	return true;
}


bool mozillaWin::run(){

	traceFunction(eEngine);
	if(m_cursorControl ){
		m_cursorControl->updatePos();
	}
	resizeDevice();
	return true;
}

void mozillaWin::shutdown(){
	SubclassWindow(hWnd, m_oldProc);

}

void mozillaWin::onEvent(GCPtr<Event> event){
	traceFunction(eEngine);
	switch(event->getType()){
		case ET_Mouse:
			{
				GCPtr<MouseEvent>e=event;
				math::vector3di newPos(e->x,e->y,e->MouseWheel);
				m_mouse->setPos(newPos);
				switch(e->Event){
				case MET_LEFTDOWN:
					m_mouse->setPressed(controllers::EMB_Left,true);
					break;
				case MET_LEFTUP:
					m_mouse->setPressed(controllers::EMB_Left,false);
					break;
				case MET_RIGHTDOWN:
					m_mouse->setPressed(controllers::EMB_Right,true);
					break;
				case MET_RIGHTUP:
					m_mouse->setPressed(controllers::EMB_Right,false);
					break;
				case MET_MIDDLEDOWN:
					m_mouse->setPressed(controllers::EMB_Middle,true);
					break;
				case MET_MIDDLEUP:
					m_mouse->setPressed(controllers::EMB_Middle,false);
					break;
				}
			}
			break;

		case ET_Keyboard:
			{
				GCPtr<KeyboardEvent>e=event;
				m_keyboard->setAltState(e->alt,e->lalt);
				m_keyboard->setCtrlState(e->ctrl,e->lctrl);
				m_keyboard->setShiftState(e->shift,e->lshift);
				m_keyboard->setKeyState(e->key,e->Char,e->press);
			}
			break;
		case ET_SystemEvent:
			{
				GCPtr<SystemEvent>e=event;
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
					windowActive=0;
					break;
				case ESET_GotActive:
					windowActive=1;
					break;
				case ESET_Resized:
					windowResized();
					break;
				case ESET_Destroy:
					shutdown();
					break;
				}
			}break;
	}

	CMozillaPlugin::getInstance().onEvent(event);
}



};//mray
