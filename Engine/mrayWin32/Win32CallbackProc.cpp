#include "stdafx.h"

#include "Win32CallbackProc.h"
#include <Event.h>
#include <MouseEvent.h>
#include <KeyboardEvent.h>
#include <JoystickEvent.h>
#include <ChangeModeEvent.h>
#include <SystemEvent.h>
#include <ResizeEvent.h>

#include <EventQueue.h>
#include <EventMemoryManager.h>
#include <InputManager.h>

#include <RenderWindowUtils.h>
#include <RenderWindow.h>
#include <InputManager.h>

namespace mray{

 

Win32MessageProc::Win32MessageProc(){
}
Win32MessageProc::~Win32MessageProc(){
}

LRESULT Win32MessageProc::WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif



	if(msg==WM_CREATE)
	{
		SetWindowLongPtr(hWnd,GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
		return 0;
	}
	video::RenderWindow* wnd= (video::RenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!wnd)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	bool g_disableWindowsMouseCallback=false;
	bool g_disableWindowsKBCallback=false;
	InputManager* inMngr= wnd->GetInputManager();
	if(inMngr )
	{
		if(inMngr->getMouse())
			g_disableWindowsMouseCallback=true;
		if(inMngr->getKeyboard())
			g_disableWindowsKBCallback=true;
	}

	video::RenderWindowUtils::RenderWindowListenerMap &listeners=video::RenderWindowUtils::GetListners();

	video::RenderWindowUtils::RenderWindowListenerMap::iterator index,
		start = listeners.lower_bound(wnd),
		end = listeners.upper_bound(wnd);


	/*if(InputManager::isExist()){
		g_disableWindowsKBCallback=InputManager::getInstance().usingInputManagment();
		g_disableWindowsMouseCallback=InputManager::getInstance().usingInputManagment();
	}*/
	POINT p;
	BYTE keys[256];

	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);

		}break;
	case WM_ACTIVATE:
		{
			bool active = (LOWORD(wParam) != WA_INACTIVE);
			if( active )
			{
				wnd->SetActive( true );
			}
			else
			{
				if( wnd->IsDeactivateOnFocusChange() )
				{
					wnd->SetActive( false );
				}
			}

			for( ; start != end; ++start )
				(start->second)->WindowFocusChanged(wnd);


			if(EventMemoryManager::isExist()){
				GCPtr<SystemEvent>e=EventMemoryManager::getInstance().createEvent(ET_SystemEvent);
				if(!HIWORD(wParam))
					e->m_eventType=ESET_GotActive;
				else
					e->m_eventType=ESET_LostActive;

				e->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(e);
			}
		}
		break;
	case WM_SETFOCUS:
		{
			if(EventMemoryManager::isExist()){
				GCPtr<SystemEvent>e=EventMemoryManager::getInstance().createEvent(ET_SystemEvent);
				e->m_eventType=ESET_GotFocus;
				e->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(e);
			}
		}
		break;
	case WM_KILLFOCUS:
		{
			if(EventMemoryManager::isExist()){
				GCPtr<SystemEvent>e=EventMemoryManager::getInstance().createEvent(ET_SystemEvent);
				e->m_eventType=ESET_LostFocus;
				e->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(e);
			}
		}
		break;
	case WM_MOVE:
		{
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowMoved(wnd);
		}
		break;
	case WM_DISPLAYCHANGE:
		{
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowResized(wnd);
		}break;
	case WM_SIZE:
		{
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowResized(wnd);

			if(EventMemoryManager::isExist()){
				{
					GCPtr<ResizeEvent>e=EventMemoryManager::getInstance().createEvent(ET_ResizeEvent);
					RECT rc;
					GetClientRect(hWnd,&rc);
					//GetWindowRect(hWnd,&rc);
					e->width=rc.right-rc.left;//-Border.x;
					e->height=rc.bottom-rc.top;//-Border.y;
					e->SetOwnerRenderWindow(wnd);
					if(EventQueue::isExist())
						EventQueue::getInstance().pushEvent(e);
				}


				{
					GCPtr<SystemEvent>e=EventMemoryManager::getInstance().createEvent(ET_SystemEvent);
					e->m_eventType=ESET_Resized;
					e->SetOwnerRenderWindow(wnd);
					if(EventQueue::isExist())
						EventQueue::getInstance().pushEvent(e);
				}
			}
		}
		break;
	case WM_GETMINMAXINFO:
		// Prevent the window from going smaller than some minimu size
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;
	case WM_CLOSE:
		{
			//log->logMessage("WM_CLOSE");
			bool close = true;
			for(index = start; index != end; ++index)
			{
				if (!(index->second)->WindowClosing(wnd))
					close = false;
			}
			if (!close) return 0;

			for(index = listeners.lower_bound(wnd); index != end; ++index)
				(index->second)->WindowClosed(wnd);
			wnd->Destroy();
			return 0;
		}
	case WM_DESTROY:
		{
			if(EventMemoryManager::isExist()){
				GCPtr<SystemEvent>e=EventMemoryManager::getInstance().createEvent(ET_SystemEvent);
				e->m_eventType=ESET_Destroy;
				e->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(e);
			}
		}
		return 0;
	case WM_SYSCOMMAND:
		if(wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER)
			return 0;
		break;
		/*case WM_SETCURSOR:
		if(!dev->getCursorControl()->isVisible())
		SetCursor(0);
		else 
		dev->getCursorControl()->restoreCursor();

		return 0;*/
	case WM_MOUSEMOVE:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>e=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			e->event=MET_MOVED;
			e->SetOwnerRenderWindow(wnd);

			e->pos.set(LOWORD(lParam),HIWORD(lParam));
			e->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(e);

		}
		return 0;

	case WM_MOUSEWHEEL:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_WHEEL;
			event->MouseWheel=(float)((short)HIWORD(wParam))/(float)WHEEL_DELTA;

			event->SetOwnerRenderWindow(wnd);

			p.x=LOWORD(lParam);
			p.y=HIWORD(lParam);
			ScreenToClient(hWnd,&p);
			event->pos.set(p.x,p.y);
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_LBUTTONDOWN:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_LEFTDOWN;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_LBUTTONUP:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_LEFTUP;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_LBUTTONDBLCLK:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_LEFTDblClick;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;


	case WM_RBUTTONDOWN:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_RIGHTDOWN;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_RBUTTONUP:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_RIGHTUP;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_RBUTTONDBLCLK:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_RIGHTDblClick;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_MBUTTONDOWN:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_MIDDLEDOWN;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_MBUTTONUP:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_MIDDLEUP;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_MBUTTONDBLCLK:
		{
			if(g_disableWindowsMouseCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<MouseEvent>event=EventMemoryManager::getInstance().createEvent(ET_Mouse);
			event->event=MET_MIDDLEDblClick;
			event->SetOwnerRenderWindow(wnd);

			event->pos.set(LOWORD(lParam),HIWORD(lParam));
			event->MouseWheel=0;
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
		}
		return 0;

	case WM_SYSKEYDOWN:
		switch( wParam )
		{
		case VK_CONTROL:
			{

				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif

				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LCONTROL;
				event->shift=((keys[VK_SHIFT] & 0x80)!=0);
				event->ctrl=1;
				event->alt=((keys[VK_MENU] & 0x80)!=0);
				event->press=1;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		case VK_SHIFT:
			{

				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				mchar keyUni=0;
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LSHIFT;
				event->shift=1;
				event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
				event->alt=((keys[VK_MENU] & 0x80)!=0);
				event->press=1;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		case VK_MENU: 
			{

				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				mchar keyUni=0;
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LMENU;
				event->shift=((keys[VK_SHIFT] & 0x80)!=0);
				event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
				event->alt=1;
				event->press=1;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		}
		break;
	case WM_SYSKEYUP:
		switch( wParam )
		{
		case VK_CONTROL:
			{
				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				mchar keyUni=0;
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LCONTROL;
				event->shift=((keys[VK_SHIFT] & 0x80)!=0);
				event->ctrl=0;
				event->alt=((keys[VK_CONTROL] & 0x80)!=0);
				event->press=0;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		case VK_SHIFT:
			{

				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				mchar keyUni=0;
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LSHIFT;
				event->shift=0;
				event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
				event->alt=((keys[VK_CONTROL] & 0x80)!=0);
				event->press=0;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		case VK_MENU: 
			{

				if(g_disableWindowsKBCallback)return 0;
				if(!EventMemoryManager::isExist())break;
				GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
				mchar keyUni=0;
				GetKeyboardState(keys);
#ifdef UNICODE
				mchar keyCode=0;
				ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
				WORD keyCode=0;
				ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
				event->Char=keyCode;
				event->key=(EKEY_CODE)KEY_LMENU;
				event->shift=((keys[VK_SHIFT] & 0x80)!=0);
				event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
				event->alt=0;
				event->press=0;
				event->SetOwnerRenderWindow(wnd);
				if(EventQueue::isExist())
					EventQueue::getInstance().pushEvent(event);
			}
			return 0;
		}
		break;

	case WM_KEYDOWN:
		{
			if(g_disableWindowsKBCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
#ifdef UNICODE
			mchar buff[10];
			mchar keyCode=0;
			ToUnicode(wParam,lParam,keys,buff,10,0);
#else
			WORD keyCode=0;
			ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
			event->Char=keyCode;

			event->key=(EKEY_CODE)wParam;
			event->shift=((keys[VK_SHIFT] & 0x80)!=0);
			event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
			event->alt=((keys[VK_MENU] & 0x80)!=0);
			event->press=1;
			event->SetOwnerRenderWindow(wnd);
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
			return 0;
		}

	case WM_KEYUP:
		{
			if(g_disableWindowsKBCallback)return 0;
			if(!EventMemoryManager::isExist())break;
			GCPtr<KeyboardEvent>event=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
			mchar keyUni=0;
			GetKeyboardState(keys);
#ifdef UNICODE
			mchar keyCode=0;
			ToUnicode(wParam,lParam,keys,&keyCode,0,0);
#else
			WORD keyCode=0;
			ToAscii(wParam,lParam,keys,&keyCode,0);
#endif
			event->key=(EKEY_CODE)wParam;
			event->Char=keyCode;
			event->shift=((keys[VK_SHIFT] & 0x80)!=0);
			event->ctrl=((keys[VK_CONTROL] & 0x80)!=0);
			event->press=0;
			event->SetOwnerRenderWindow(wnd);
			if(EventQueue::isExist())
				EventQueue::getInstance().pushEvent(event);
			return 0;
		}
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

}