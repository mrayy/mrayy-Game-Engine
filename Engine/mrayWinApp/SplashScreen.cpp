#include "stdafx.h"

#include "SplashScreen.h"
#include <mString.h>


namespace mray{

	typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
		(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer


//! windows message callback
LRESULT CALLBACK	SplashScreen::WndProc	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	SplashScreen *win=(SplashScreen *)GetWindowLong(hWnd,DWL_USER);

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_TIMER:
		{
			win->m_pastTime++;
			if(win->m_pastTime*win->m_step>=win->m_timeMS)
				win->Close();
			else
				win->Draw();
		}
		return true;

	case WM_PAINT:
		{
			PAINTSTRUCT	ps;

			win->Draw();
		}
		return true;

	case WM_ERASEBKGND:	
		return true;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//! constructor
SplashScreen::SplashScreen(bool internalRes){

	if(internalRes)
	{
#ifndef MRAY_WinApp_DLL
		m_hInstance=GetModuleHandle(0);
#else 
#	ifdef _DEBUG
		m_hInstance=GetModuleHandle(mT("mrayEngine_d.dll"));
#	else
		m_hInstance=GetModuleHandle(mT("mrayEngine.dll"));
#	endif
#endif
	}else
	{
		m_hInstance=GetModuleHandle(0);
	}

	// Var Init
	m_iTimerNo=0;
	m_hWindow=0;
	m_hBitmap=0;
	m_iXBitmapSize=0;
	m_iYBitmapSize=0;


	//  =======================================================================
	//  Keep the function pointer for the SetLayeredWindowAttributes function
	//  in User32.dll ready
	//  =======================================================================
	HMODULE hUser32 = GetModuleHandle(TEXT("USER32.DLL"));

	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
		GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}

//! destructor
SplashScreen::~SplashScreen(){
	Close();
}


void SplashScreen::Draw( ){


	if (!m_hBitmap)
		return;
	if(!m_iXBitmapSize || !m_iYBitmapSize)
		return;

	//  =======================================================================
	//  Paint the background by BitBlting the bitmap
	//  =======================================================================
	PAINTSTRUCT ps ;
	HDC hDc = BeginPaint (m_hWindow, &ps) ;

	COLORREF m_crWhite = RGB(255,255,255);
	COLORREF crOldBack = SetBkColor(hDc,m_crWhite);
	

	// Create two memory dcs for the image and the mask
	HDC dcImage=::CreateCompatibleDC(hDc);
	HDC dcTrans=::CreateCompatibleDC(hDc);

	int nWidth = m_iXBitmapSize;
	int nHeight = m_iYBitmapSize;

	// Select the image into the appropriate dc
	HBITMAP pOldBitmapImage = (HBITMAP)::SelectObject(dcImage, m_hBitmap);
	// Create the mask bitmap
	HBITMAP pOldBitmapTrans = (HBITMAP)::SelectObject(dcTrans, m_hMask);


	COLORREF crColour=RGB(0,0,0);
	// Build mask based on transparent colour
	//SetBkMode(hDc,TRANSPARENT);
	//SetBkColor(dcImage,crColour);
	
	//BitBlt(dcTrans,0, 0, nWidth, nHeight, dcImage, 0, 0, SRCCOPY);
	//BitBlt(hDc,0, 0, nWidth, nHeight, dcTrans, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(hDc,0, 0, nWidth, nHeight, dcImage, 0, 0, SRCINVERT);
	BitBlt(hDc,0, 0, nWidth, nHeight, dcTrans, 0, 0, SRCAND);
	BitBlt(hDc,0, 0, nWidth, nHeight, dcImage, 0, 0, SRCINVERT);

	// Restore settings
	SelectObject(dcImage,pOldBitmapImage);
	SelectObject(dcTrans,pOldBitmapTrans);
	SetBkColor(hDc,crOldBack);

	DeleteDC(dcImage);
	DeleteDC(dcTrans);
/*
	RECT   rect;
	::GetClientRect(m_hWindow, &rect);

	HDC hMemDC      = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);

	BitBlt(hDC, 0, 0, m_iXBitmapSize, m_iYBitmapSize, hMemDC, 0, 0, SRCCOPY);

	::SelectObject(hMemDC, hOldBmp);

	::DeleteDC(hMemDC);

	
	int mx=0,my=0;


	HDC BitmapDC=CreateCompatibleDC(hdc);
	HBITMAP oldBitmap=(HBITMAP)SelectObject(BitmapDC,m_hBitmap);

 	BitBlt(hdc,mx,my,m_iXBitmapSize,m_iYBitmapSize,
 		BitmapDC,0,0,
 		SRCCOPY);

// 	int res=MaskBlt(hdc,mx,my,m_iXBitmapSize,m_iYBitmapSize,
// 		BitmapDC,0,0,m_hMask,0,0,
// 		MAKEROP4(SRCCOPY,SRCCOPY));

	SelectObject(BitmapDC,oldBitmap);
	DeleteDC(BitmapDC);*/
	EndPaint (m_hWindow, &ps) ;
}
bool SplashScreen::Create( HWND inhWnd, WORD inBitmapResourceID, int maskID){
	if(m_hWindow)
		return false;
	int cxScreenMid = GetSystemMetrics(SM_CXSCREEN)/2;
	int cyScreenMid = GetSystemMetrics(SM_CYSCREEN)/2;

	int cxWindowSize = GetSystemMetrics(SM_CXSCREEN);
	int cyWindowSize = GetSystemMetrics(SM_CYSCREEN);


	m_hBitmap=LoadBitmap(m_hInstance,MAKEINTRESOURCE(inBitmapResourceID));
	m_hMask=LoadBitmap(m_hInstance,MAKEINTRESOURCE(maskID));

	BITMAP BitData;
	GetObject(m_hBitmap,sizeof(BITMAP),&BitData);
	m_iXBitmapSize=BitData.bmWidth;
	m_iYBitmapSize=BitData.bmHeight;

	cxWindowSize=m_iXBitmapSize;
	cyWindowSize=m_iYBitmapSize;

	WNDCLASSEX wndclass;
	wndclass.cbSize         = sizeof (wndclass);
	wndclass.style          = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wndclass.lpfnWndProc    = WndProc;
	wndclass.cbClsExtra     = 0;
	wndclass.cbWndExtra     = DLGWINDOWEXTRA;
	wndclass.hInstance      = m_hInstance;
	wndclass.hIcon          = NULL;
	wndclass.hCursor        = ::LoadCursor( NULL, IDC_WAIT );
	wndclass.hbrBackground  = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName   = NULL;
	wndclass.lpszClassName  = mT("SplashWindow");
	wndclass.hIconSm        = NULL;

	// Register the window class
// 	WNDCLASSEX wndClass = { 0, WndProc, 0, 
// 		DLGWINDOWEXTRA+4,		
// 		m_hInstance,
// 		NULL,
// 		LoadCursor( NULL, IDC_ARROW ), 
// 		(HBRUSH)(CreateSolidBrush(GetSysColor(COLOR_BTNFACE))),
// 		NULL, mT("SplashWindow") };

	if(!RegisterClassEx (&wndclass))
		return false;


	m_hWindow = ::CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW, wndclass.lpszClassName, 
		mT(""), WS_POPUP, cxScreenMid-cxWindowSize/2, cyScreenMid-cyWindowSize/2,
		cxWindowSize,
		cyWindowSize,
		inhWnd, NULL, m_hInstance, NULL	);

	if( !m_hWindow ) 
	{
		//		DWORD h=GetLastError();
		return false;
	}
	if( g_pSetLayeredWindowAttributes )
	{
		//  set layered style for the window
//		SetWindowLong(m_hWindow, GWL_EXSTYLE, GetWindowLong(m_hWindow, GWL_EXSTYLE) | WS_EX_LAYERED);
		//  call it with 0 alpha for the given color
		//g_pSetLayeredWindowAttributes(m_hWindow, 0, 0, LWA_COLORKEY);
	}    
// 
// 	SetForegroundWindow(m_hWindow);


	SetActiveWindow(m_hWindow);
	SetWindowLong(m_hWindow,DWL_USER,(long)(void *)(this));		
	ShowWindow(m_hWindow,SW_SHOW);
	UpdateWindow(m_hWindow);
	//SetFocus(m_hWindow);

	m_pWnd=inhWnd;

	return true;

}
bool SplashScreen::Create( HWND inhWnd, WORD inBitmapResourceID,  int maskID, int inTimeInMS ){

	if(Create(inhWnd,inBitmapResourceID,maskID)==false)
		return false;

	m_pastTime=0;
	m_step=20;
	m_timeMS=inTimeInMS;
	m_iTimerNo=SetTimer (
		m_hWindow,				// handle of window for timer messages
		0,								// timer identifier
		m_step,				// time-out value
		0									// address of timer procedure  (0= WM_TIMER MESSAGE)
		);

	return true;
}
//!
void SplashScreen::Close(){
	if(m_iTimerNo)KillTimer(m_hWindow,m_iTimerNo);m_iTimerNo=0;
	if(m_hWindow)DestroyWindow(m_hWindow);m_hWindow=0;
	if(m_hBitmap)DeleteObject(m_hBitmap);
}


}

