
#include "stdafx.h"
#include "Win32RenderWindowUtil.h"
#include "RenderWindowUtils.h"
#include "StringConverter.h"
#include "OptionContainer.h"


namespace mray
{
namespace video
{
LRESULT CALLBACK Win32RenderWindowUtil::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	OptionContainer params;
	params[mT("hWnd")].value=core::StringConverter::toString((ulong)hWnd);
	params[mT("Msg")].value=core::StringConverter::toString((uint)uMsg);
	params[mT("wParam")].value=core::StringConverter::toString((uint)wParam);
	params[mT("lParam")].value=core::StringConverter::toString((uint)lParam);
	
	ulong ret=RenderWindowUtils::HandleWindowEvent(params);
	return ret;

/*
	
		if(uMsg==WM_CREATE)
		{
			SetWindowLongPtr(hWnd,GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 0;
		}
		RenderWindow* wnd= (RenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!wnd)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
		RenderWindowListenerMap::iterator index,
			start = m_listeners.lower_bound(wnd),
			end = m_listeners.upper_bound(wnd);
	
		switch(uMsg)
		{
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
			}
			break;
		case WM_SYSKEYDOWN:
			switch( wParam )
			{
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU: //ALT
				//return zero to bypass defProc and signal we processed the message
				return 0;
			}
			break;
		case WM_SYSKEYUP:
			switch( wParam )
			{
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU: //ALT
			case VK_F10:
				//return zero to bypass defProc and signal we processed the message
				return 0;
			}
			break;
		case WM_SYSCHAR:
			// return zero to bypass defProc and signal we processed the message, unless it's an ALT-space
			if (wParam != VK_SPACE)
				return 0;
			break;
		case WM_ENTERSIZEMOVE:
			//log->logMessage("WM_ENTERSIZEMOVE");
			break;
		case WM_EXITSIZEMOVE:
			//log->logMessage("WM_EXITSIZEMOVE");
			break;
		case WM_MOVE:
			//log->logMessage("WM_MOVE");
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowMoved(wnd);
			break;
		case WM_DISPLAYCHANGE:
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowResized(wnd);
			break;
		case WM_SIZE:
			//log->logMessage("WM_SIZE");
			wnd->OnMoveResize();
			for(index = start; index != end; ++index)
				(index->second)->WindowResized(wnd);
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
	
				for(index = m_listeners.lower_bound(wnd); index != end; ++index)
					(index->second)->WindowClosed(wnd);
				wnd->Destroy();
				return 0;
			}
		}
	
		return DefWindowProc( hWnd, uMsg, wParam, lParam );*/
	
}


}
}