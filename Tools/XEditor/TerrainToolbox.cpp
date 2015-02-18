// TerrainToolbox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "TerrainToolbox.h"
#include "CEngineWindow.h"
#include "EditorGlobals.h"


// TerrainToolbox dialog

IMPLEMENT_DYNAMIC(TerrainToolbox, CDialog)

TerrainToolbox::TerrainToolbox(CWnd* pParent /*=NULL*/)
	: CDialog(TerrainToolbox::IDD, pParent)
	, ScaleX(0)
	, ScaleY(0)
	, ScaleZ(0)
	, LODCount(0)
	, LODDistance(0)
{
	m_editor=0;
	EditorGlobals::RegisterToolBox(this);
}

TerrainToolbox::~TerrainToolbox()
{
}

void TerrainToolbox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TERR_NUM_LODS_TXT, NumLOD);
	DDX_Control(pDX, IDC_TERR_LOD_DISTANCE_TXT, LODDist);
	DDX_Text(pDX, IDC_TERR_SCALE_X_TXT, ScaleX);
	DDX_Text(pDX, IDC_TERR_SCALE_Y_TXT, ScaleY);
	DDX_Text(pDX, IDC_TERR_SCALE_Z_TXT, ScaleZ);
	DDX_Text(pDX, IDC_TERR_NUM_LODS_TXT, LODCount);
	DDX_Text(pDX, IDC_TERR_LOD_DISTANCE_TXT, LODDistance);
}


BEGIN_MESSAGE_MAP(TerrainToolbox, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &TerrainToolbox::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TerrainToolbox::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, &TerrainToolbox::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON3, &TerrainToolbox::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON11, &TerrainToolbox::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &TerrainToolbox::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_GEN_NORMAL_MAP_BTN, &TerrainToolbox::OnBnClickedGenNormalMapBtn)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void TerrainToolbox::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}


void TerrainToolbox::OnExportLevelXML(xml::XMLElement*e)
{
}
bool TerrainToolbox::OnImportLevelXML(xml::XMLElement*e)
{
	return false;
}

void TerrainToolbox::ShowToolBox(bool s)
{
	this->ShowWindow(s);
}
bool TerrainToolbox::IsToolBoxVisible()
{
	return this->IsWindowVisible();
}

// TerrainToolbox message handlers

void TerrainToolbox::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("Height Map Raw Files 16-bit (*.r16)|*.r16|Height Map Raw Files 8-bit (*.r8)|*.r8|All files (*.*)|*.*||"));
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=dlg.DoModal();

	if(iRet==IDOK)
	{
		core::string ext=dlg.GetFileExt().GetString();
		if(ext==mT("r16"))
		{
			heightMap=new loaders::HeightMapRaw16();
			((loaders::HeightMapRaw16*)heightMap.pointer())->loadHeightMap(gFileSystem.openFile(dlg.GetPathName().GetString()));
		}else if(ext==mT("r8"))
		{
			heightMap=new loaders::HeightMapRaw8();
			((loaders::HeightMapRaw8*)heightMap.pointer())->loadHeightMap(gFileSystem.openFile(dlg.GetPathName().GetString()));
		}else
			return;
	}
}

void TerrainToolbox::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("All files (*.*)|*.*||"));
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=dlg.DoModal();

	if(iRet==IDOK)
	{
		baseTex=gTextureResourceManager.loadTexture2D(dlg.GetPathName().GetString());
	}
}

void TerrainToolbox::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("All files (*.*)|*.*||"));
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=dlg.DoModal();

	if(iRet==IDOK)
	{
		baseNormalTex=gTextureResourceManager.loadTexture2D(dlg.GetPathName().GetString());
	}
}

void TerrainToolbox::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("All files (*.*)|*.*||"));
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=dlg.DoModal();

	if(iRet==IDOK)
	{
		detailTex=gTextureResourceManager.loadTexture2D(dlg.GetPathName().GetString());
	}
}

void TerrainToolbox::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("All files (*.*)|*.*||"));
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=dlg.DoModal();

	if(iRet==IDOK)
	{
		detailNormalTex=gTextureResourceManager.loadTexture2D(dlg.GetPathName().GetString());
	}
}

void TerrainToolbox::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
	if(heightMap){
		UpdateData(true);
		math::vector3d scale(ScaleX,ScaleY,ScaleZ);
		m_editor->GenerateTerrain(heightMap,scale,baseTex,baseNormalTex,detailTex,detailNormalTex,LODCount,LODDistance);
	}
}

void TerrainToolbox::OnBnClickedGenNormalMapBtn()
{
	// TODO: Add your control notification handler code here
	if(heightMap){
		baseNormalTex=heightMap->GenerateTexture(m_editor->GetDevice());
		//video::ColorConverter::createNormalMap(baseNormalTex->getSurface(0)->,1);
	}
}

void TerrainToolbox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	UpdateData(false);
}
