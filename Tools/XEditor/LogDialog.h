#pragma once
#include "afxwin.h"


// CLogDialog dialog

class CLogDialog : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CLogDialog)
	CXTHeaderCtrl   m_header;

public:
	CLogDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogDialog();

// Dialog Data
	enum { IDD = IDD_LOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
public:
	CXTListCtrl LogLines;
};
