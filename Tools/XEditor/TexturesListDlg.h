
#pragma once
#include "Resource.h"

using namespace mray;

// CTexturesListDlg dialog

class CTexturesListDlg : public CDialog
{
	DECLARE_DYNAMIC(CTexturesListDlg)

	void RefreshList();

	core::string m_selectedTex;
public:
	CTexturesListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTexturesListDlg();

// Dialog Data
	enum { IDD = IDD_TEXTURES_DIALOG };

	const core::string& GetSelectedTexture()const {return m_selectedTex;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnShowWindow(BOOL show,UINT nStatus);
	virtual void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	CListBox TexturesList;
public:
	afx_msg void OnBnClickedLoadtexture();



public:
	afx_msg void OnBnClickedOk();
public:
	CComboBox TextureTypeList;
};

