// EnvironmentSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "EnvironmentSettingsDlg.h"
#include "SkyBoxDlg.h"
#include "CEngineWindow.h"
#include "GameEnvironment.h"
#include "Sun.h"
#include "EditorEventMessages.h"

// EnvironmentSettingsDlg dialog

IMPLEMENT_DYNAMIC(EnvironmentSettingsDlg, CDialog)

EnvironmentSettingsDlg::EnvironmentSettingsDlg(CWnd* pParent)
	: CDialog(EnvironmentSettingsDlg::IDD,pParent),m_sunDirHoriz(0),m_sunDirVert(0)
	, ShadowMapSizeW(0)
	, ShadowMapSizeH(0)
	, ShadowFovY(0)
	, ShadowAspect(0)
	, ShadowZNear(0)
	, ShadowZFar(0)
{
	m_editor=0;

	EditorEventManager::getInstance().AddHandler(EEE_OnLevelLoaded,newClassDelegate1<EnvironmentSettingsDlg,void,mray::PVOID>(mT(""),this,&EnvironmentSettingsDlg::OnLoadLevel));
}

EnvironmentSettingsDlg::~EnvironmentSettingsDlg()
{
}

void EnvironmentSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUNDIR_HORIZ_SLD, SunDirHoriz);
	DDX_Control(pDX, IDC_SUNDIR_VERT_SLD, SunDirVert);/*
													  DDX_Control(pDX, IDC_SHADOWMAP_W_TXT, ShadowMapWidth);
													  DDX_Control(pDX, IDC_SHADOWMAP_H_TXT, ShadowMapHeight);
													  DDX_Control(pDX, IDC_SHADOW_FOVY_TXT, ShadowFovY);
													  DDX_Control(pDX, IDC_SHADOW_ASPECT_TXT, ShadowAspect);
													  DDX_Control(pDX, IDC_SHADOW_ZNEAR_TXT, ShadwoZNear);
													  DDX_Control(pDX, IDC_SHADOW_ZFAR_TXT5, ShadowZFar);*/

	DDX_Slider(pDX, IDC_SUNDIR_HORIZ_SLD, m_sunDirHoriz);
	DDX_Slider(pDX, IDC_SUNDIR_VERT_SLD, m_sunDirVert);
	DDX_Text(pDX, IDC_SHADOWMAP_W_TXT, ShadowMapSizeW);
	DDX_Text(pDX, IDC_SHADOWMAP_H_TXT, ShadowMapSizeH);
	DDX_Text(pDX, IDC_SHADOW_FOVY_TXT, ShadowFovY);
	DDX_Text(pDX, IDC_SHADOW_ASPECT_TXT, ShadowAspect);
	DDX_Text(pDX, IDC_SHADOW_ZNEAR_TXT, ShadowZNear);
	DDX_Text(pDX, IDC_SHADOW_ZFAR_TXT5, ShadowZFar);
}
BOOL EnvironmentSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	SunDirHoriz.SetScrollRange(0,360);
	SunDirVert.SetScrollRange(0,360);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(EnvironmentSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_SKYBOX_DLG_BTN, &EnvironmentSettingsDlg::OnBnClickedSkyboxDlgBtn)
	//ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SUNDIR_HORIZ_SLD, &EnvironmentSettingsDlg::OnNMReleasedcaptureSundirHorizSld)
	//ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SUNDIR_VERT_SLD, &EnvironmentSettingsDlg::OnNMReleasedcaptureSundirVertSld)
	ON_BN_CLICKED(IDC_CHANGE_SHADOWMAP_BTN, &EnvironmentSettingsDlg::OnBnClickedChangeShadowmapBtn)
	ON_BN_CLICKED(IDC_CHANGE_SHADOW_PROJ_BTN, &EnvironmentSettingsDlg::OnBnClickedChangeShadowProjBtn)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// EnvironmentSettingsDlg message handlers

void EnvironmentSettingsDlg::OnLoadLevel(const mray::PVOID&)
{
	m_sunDirVert=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetVertical()/3.6;
	m_sunDirHoriz=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetHorizontal()/3.6;
	math::vector2di v=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetShadowMapSize();
	ShadowMapSizeW=v.x;
	ShadowMapSizeH=v.y;

	ShadowFovY=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetShadowProjFov();
	ShadowAspect=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetShadowProjAspect();
	ShadowZNear=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetShadowProjNear();
	ShadowZFar=m_editor->GetLevel()->GetEnvironment()->GetSun()->GetShadowProjFar();

	UpdateData(false);
	
}

void EnvironmentSettingsDlg::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}
void EnvironmentSettingsDlg::OnBnClickedSkyboxDlgBtn()
{
	// TODO: Add your control notification handler code here
	CSkyBoxDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_editor->GetLevel()->GetEnvironment()->SetSkybox(dlg.selectedSky);
	}
}

void EnvironmentSettingsDlg::OnNMReleasedcaptureSundirHorizSld(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
//	m_editor->SetSunDirection(SunDirVert.GetPos(),SunDirHoriz.GetPos());
}

void EnvironmentSettingsDlg::OnNMReleasedcaptureSundirVertSld(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

//	m_editor->SetSunDirection(SunDirVert.GetPos(),SunDirHoriz.GetPos());
}

void EnvironmentSettingsDlg::OnBnClickedChangeShadowmapBtn()
{
	// TODO: Add your control notification handler code here
	/*
	CString str;
	int width;
	int height;
	ShadowMapWidth.GetWindowText(str);
	width=core::StringConverter::toInt(str.GetString());
	ShadowMapHeight.GetWindowText(str);
	height=core::StringConverter::toInt(str.GetString());*/
	UpdateData(true);
	if(ShadowMapSizeW<=0 || ShadowMapSizeH <=0)
	{
		MessageBox(mT("Check Width,Height Fields"),mT("Shadow Map"),0);
		return;
	}
	m_editor->GetLevel()->GetEnvironment()->GetSun()->SetShadowMapSize(math::vector2di(ShadowMapSizeW,ShadowMapSizeH));
}

void EnvironmentSettingsDlg::OnBnClickedChangeShadowProjBtn()
{
	// TODO: Add your control notification handler code here
	/*
	CString str;
	float fovY,aspect,znear,zfar;
	ShadowFovY.GetWindowText(str);
	fovY=core::StringConverter::toFloat(str.GetString());

	ShadowAspect.GetWindowText(str);
	aspect=core::StringConverter::toFloat(str.GetString());

	ShadwoZNear.GetWindowText(str);
	znear=core::StringConverter::toFloat(str.GetString());

	ShadowZFar.GetWindowText(str);
	zfar=core::StringConverter::toFloat(str.GetString());
	*/
	UpdateData(true);
	if(ShadowFovY<=0 || ShadowAspect <=0 || ShadowZNear<=0 || ShadowZFar<=0)
	{
		MessageBox(mT("Check Projection Fields Values"),mT("Shadow Projection"),0);
		return;
	}

	m_editor->GetLevel()->GetEnvironment()->GetSun()->SetShadowProjectionParams(ShadowFovY,ShadowAspect,ShadowZNear,ShadowZFar);
}

void EnvironmentSettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar==&SunDirVert || pScrollBar==&SunDirHoriz)
	{
		UpdateData();

		m_editor->GetLevel()->GetEnvironment()->GetSun()->SetHorizontal(m_sunDirHoriz*3.6);
		m_editor->GetLevel()->GetEnvironment()->GetSun()->SetVertical(m_sunDirVert*3.6);
		//UpdateData(FALSE);
	}
}