#pragma once

#include "CEngineWindow.h"

// CLevelInfoDlg dialog

using namespace mray;

class CLevelInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CLevelInfoDlg)

	CEngineWindow* m_editor;

public:
	CLevelInfoDlg(CEngineWindow* e,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLevelInfoDlg();

// Dialog Data
	enum { IDD = IDD_LEVEL_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString LevelName;
public:
	CString LevelDescription;
public:
	CString LevelThumbnail;
public:
	afx_msg void OnBnClickedSelectThumbnailBtn();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	CString MainAction;
public:
	afx_msg void OnBnClickedSelectMainactionBtn();
};
