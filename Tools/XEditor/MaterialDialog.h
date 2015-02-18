#pragma once
#include "afxwin.h"

using namespace mray;

// CMaterialDialog dialog

class CMaterialDialog : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDialog)

	void RefreshList();

	core::string m_selectedMtrl;
public:
	CMaterialDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialDialog();

// Dialog Data
	enum { IDD = IDD_MATERIALS_DIALOG };

	const core::string& GetSelectedMaterial()const {return m_selectedMtrl;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnShowWindow(BOOL show,UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadshader();
public:
	afx_msg void OnBnClickedOk();
	CEdit MaterialName;
	CListBox MaterialsList;
public:
	afx_msg void OnBnClickedDuplicateMtrl();
};
