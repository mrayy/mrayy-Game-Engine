// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"
#include "LogDialog.h"

class CMainFrame : public CXTPFrameWnd
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CChildView		m_wndView;
	CLogDialog		m_loggerDialog;
	CXTPDockingPaneManager m_paneManager;
	CXTPOfficeBorder<CStatic> m_wndOptions;
	CXTPOfficeBorder<CEdit> m_wndProperties;

	CEdit         m_wndXAxisEditCtrl;
	CEdit         m_wndYAxisEditCtrl;
	CEdit         m_wndZAxisEditCtrl;

	void InitStatusBar();

	void OnNodeTrasnformingDone(const mray::PVOID&);

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


	void OnToolsMaterial();
	void OnToolsEnvironment();
	void OnToolsTerrain();

	void OnAINavmesh();

	void OnSettingsPlugins();
protected:
	void OnStatusBarXAxisChange();
	void OnStatusBarYAxisChange();
	void OnStatusBarZAxisChange();

};
