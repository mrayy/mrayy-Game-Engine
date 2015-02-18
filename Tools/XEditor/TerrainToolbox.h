#pragma once

#include "IToolBox.h"
#include "afxwin.h"

// TerrainToolbox dialog

namespace mray
{
	class CEngineWindow;
}
using namespace mray;

class TerrainToolbox : public CDialog,public IToolBox
{
	DECLARE_DYNAMIC(TerrainToolbox)

	mray::CEngineWindow* m_editor;
public:
	TerrainToolbox(CWnd* pParent = NULL);   // standard constructor
	virtual ~TerrainToolbox();

// Dialog Data
	enum { IDD = IDD_TERRAIN_TOOLBOX};

	void SetEditor(mray::CEngineWindow*e);


	virtual void OnExportLevelXML(xml::XMLElement*e);
	virtual bool OnImportLevelXML(xml::XMLElement*e);

	virtual void ShowToolBox(bool s);
	virtual bool IsToolBoxVisible();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	GCPtr<loaders::IHeightMapLoader> heightMap;
	GCPtr<video::ITexture> baseTex;
	GCPtr<video::ITexture> baseNormalTex;
	GCPtr<video::ITexture> detailTex;
	GCPtr<video::ITexture> detailNormalTex;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton10();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton11();
public:
	afx_msg void OnBnClickedButton12();
public:
	CEdit NumLOD;
public:
	CEdit LODDist;
public:
	afx_msg void OnBnClickedGenNormalMapBtn();
public:
	float ScaleX;
	float ScaleY;
	float ScaleZ;
public:
	int LODCount;
public:
	float LODDistance;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
