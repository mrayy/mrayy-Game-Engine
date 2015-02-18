#pragma once

#include "CEngineWindow.h"

// CPluginsDialog dialog
using namespace mray;

class CPluginsDialog : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CPluginsDialog)

	CXTHeaderCtrl   m_header;
	CEngineWindow* m_editor;
public:
	CPluginsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPluginsDialog();

	void SetEditor(CEngineWindow* edtor);

// Dialog Data
	enum { IDD = IDD_PLUGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	void RefreshList();
public:
	CXTListCtrl PluginsList;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedInstallPluginBtn();
public:
	afx_msg void OnBnClickedUninstallPluginBtn();
public:
	afx_msg void OnBnClickedReinstallPluginBtn();
};
