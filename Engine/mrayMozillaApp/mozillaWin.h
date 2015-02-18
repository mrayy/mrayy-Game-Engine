
/********************************************************************
created:	2009/03/06
created:	6:3:2009   21:55
filename: 	i:\Programing\GameEngine\mrayEngine\mrayMozillaApp\mozillaWin.h
file path:	i:\Programing\GameEngine\mrayEngine\mrayMozillaApp
file base:	mozillaWin
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef ___mozillaWin___
#define ___mozillaWin___

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mMath.h>
#include <Point2d.h>
#include <SVideoMode.h>
#include <ISerializable.h>
#include <IEventReciver.h>
#include <IXMLParser.h>

#include <IMouseController.h>
#include <IJoysticController.h>
#include <IKeyboardController.h>

#include <ICursorControl.h>
#include <GCPtr.h>

#include "compileConfig.h"

namespace mray{
class Event;




/*!
API window class
*/
class mozillaWin:public IEventReciver
{
	HWND hWnd;
	math::Point2di WinSize;
	int WinBits;
	
	bool windowActive;

	bool m_resized;

	WNDPROC m_oldProc;

	GCPtr<controllers::IMouseController>m_mouse;
	GCPtr<controllers::IKeyboardController>m_keyboard;

	GCPtr<ICursorControl> m_cursorControl;

	void windowResized();
	void resizeDevice();

public:

	mozillaWin();
	virtual ~mozillaWin();

	bool startup(HWND hWnd,math::Point2di size,int bits);

	bool restart();

	/*!
	run the application
	if it returned 0 thats mean the window closed
	*/
	bool run();

	void shutdown();

	math::Point2di getWindowSize(){return WinSize;}

	void onEvent(GCPtr<Event> event);
	HWND getHWnd(){return hWnd;}
	math::Point2di getSize(){return WinSize;}
	int getBits(){return WinBits;}

};


};//mray


#endif //___mozillaWin___
