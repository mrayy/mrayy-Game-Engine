
/********************************************************************
	created:	2008/10/26
	created:	26:10:2008   18:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\SplashScreen.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	SplashScreen
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___SplashScreen___
#define ___SplashScreen___

#define WIN32_LEAN_AND_MEAN		// don't use rarely used parts of Windows

#include <windows.h>
#include <Point2d.h>
#include "compileConfig.h"

namespace mray{

class MRAY_WinApp_DLL SplashScreen
{

	// **********************************************************************

	HINSTANCE m_hInstance;
	int		m_iTimerNo;						//!<
	HWND	m_hWindow;						//!< window handle to the splash window
	HWND	m_pWnd;
	HBITMAP	m_hBitmap;						//!< handle to the bitmap		
	HBITMAP m_hMask;
	int		m_iXBitmapSize;					//!<
	int		m_iYBitmapSize;					//!<
	//!
	int		m_pastTime;
	float	m_step;
	int		m_timeMS;
	void Draw(  );

	//! windows message callback
	static LRESULT CALLBACK		WndProc	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	//! constructor
	SplashScreen(bool internalRes);

	//! destructor
	virtual ~SplashScreen();

	void Close();

	bool Create( HWND inhWnd, WORD inBitmapResourceID,  int maskID, int inTimeInMS );
	bool Create( HWND inhWnd, WORD inBitmapResourceID,  int maskID );
};

}

#endif //___SplashScreen___

