// XEditor.h : main header file for the XEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CXEditorApp:
// See XEditor.cpp for the implementation of this class
//

class CXEditorApp : public CWinApp
{
public:
	CXEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CXEditorApp theApp;