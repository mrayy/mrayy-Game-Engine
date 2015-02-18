#pragma once
#include "afxwin.h"

using namespace mray;

// CShaderDialog dialog

class CShaderDialog : public CDialog
{
	DECLARE_DYNAMIC(CShaderDialog)

	void RefreshList();

	core::string m_selectedShader;
public:
	CShaderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShaderDialog();

// Dialog Data
	enum { IDD = IDD_SHADERS_DIALOG };
	const core::string& GetSelectedShader()const {return m_selectedShader;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnShowWindow(BOOL show,UINT nStatus);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadshader();
public:
	CListBox ShaderList;
public:
	afx_msg void OnBnClickedOk();
};
