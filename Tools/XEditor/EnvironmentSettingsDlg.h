#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// EnvironmentSettingsDlg dialog

namespace mray
{
	class CEngineWindow;
}
using namespace mray;

class EnvironmentSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(EnvironmentSettingsDlg)

	void OnLoadLevel(const mray::PVOID&);

	mray::CEngineWindow* m_editor;
	BOOL OnInitDialog();
	int m_sunDirHoriz;
	int m_sunDirVert;
public:
	EnvironmentSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~EnvironmentSettingsDlg();

// Dialog Data
	enum { IDD = IDD_ENVIRONMENT_SETTINGS_DLG };

	void SetEditor(mray::CEngineWindow*e);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) ;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSkyboxDlgBtn();
public:
	CScrollBar SunDirHoriz;
public:
	CScrollBar SunDirVert;
public:
	afx_msg void OnNMReleasedcaptureSundirHorizSld(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureSundirVertSld(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedChangeShadowmapBtn();
public:
	afx_msg void OnBnClickedChangeShadowProjBtn();
public:
	CXTColorPicker ClrBtn;
public:
	int ShadowMapSizeW;
public:
	int ShadowMapSizeH;
public:
	float ShadowFovY;
public:
	float ShadowAspect;
public:
	float ShadowZNear;
public:
	float ShadowZFar;
};
