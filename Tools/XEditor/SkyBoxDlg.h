#pragma once
#include "afxwin.h"


// CSkyBoxDlg dialog
using namespace mray;

class CSkyBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkyBoxDlg)

	struct SkyInfo
	{
		core::string name;
		core::string path;
		core::string ext;
	};
	std::map<core::string,SkyInfo> m_skiesInfo;

	void RefreshList();
public:
	CSkyBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkyBoxDlg();

// Dialog Data
	enum { IDD = IDD_SKYBOX_DLG };

	core::string selectedSky;

	virtual void OnExportLevelXML(xml::XMLElement*e);
	virtual bool OnImportLevelXML(xml::XMLElement*e);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedLoadskybox();
public:
	CEdit NameTxt;
public:
	CListBox SkyboxLst;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
