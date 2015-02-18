#pragma once
#include "afxwin.h"


// CGameActionsDialog dialog
using namespace mray;

class CGameActionsDialog : public CDialog
{
	DECLARE_DYNAMIC(CGameActionsDialog)

	void RefreshList();

public:
	CGameActionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGameActionsDialog();

// Dialog Data
	enum { IDD = IDD_GAMEACTIONS_DLG };

	core::string ActionName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedOk();
public:
	CListBox ActionsList;
public:
	CStatic ActionDescription;
public:
	afx_msg void OnLbnSelchangeGameActionsList();
};
