#pragma once

#include "EditorEventMessages.h"
#include "IToolBox.h"
#include "afxwin.h"

// CMaterialEditor dialog
using namespace mray;

namespace mray
{
class CEngineWindow;
}

class CMaterialEditor : public CDialog,public IToolBox
{
	DECLARE_DYNAMIC(CMaterialEditor)

	mray::CEngineWindow* m_editor;

	void OnModelSelected(const mray::PVOID&);
	void OnEngineInit(const mray::PVOID&);

	int m_selectedMesh;
	video::SMaterial* m_selectedMtrl;
	int m_selectedTextureUnit;


	void OnMeshSelected(int i);
	void OnTextureUnitChange(int i);
	void OnTextureChange(const core::string&name);
	void OnShaderChange(const core::string&name);
	void OnDepthShaderChange(const core::string&name);
	void OnMaterialChange(const core::string&name);
	void OnAddTextureUnit();
	void OnStatusChanged(video::RenderStates st,bool v);
public:
	CMaterialEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialEditor();

	void SetEditor(mray::CEngineWindow*e);

// Dialog Data
	enum { IDD = IDD_MATERIAL_EDITOR_DLG};

	void ClearSettings();

	virtual void OnExportLevelXML(xml::XMLElement*e);
	virtual bool OnImportLevelXML(xml::XMLElement*e);

	virtual void ShowToolBox(bool s);
	virtual bool IsToolBoxVisible();
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	video::SColor ConvertToColor(COLORREF c);
	COLORREF ConvertToDWORD(const video::SColor&c);
public:
	CXTFlatComboBox SubmeshList;
	CEdit MaterialName;

	CButton WireframeState;
	CButton SmoothState;
	CButton ZTestState;
	CButton ZWriteState;
	CButton FogState;
	CButton PointsState;
	CButton CullfaceState;
	CButton AlphaTestState;
	CButton BlendState;
	CButton CWCullState;
	CButton LightingState;

	CComboBox TexturesList;
	CStatic TextureNameTxt;

	CComboBox ValuesList;
	CComboBox ValueTypeList;

	CEdit ShaderName;
	CEdit ShaderShadowName;
public:
	afx_msg void OnCbnSelchangeSubmeshList();
	afx_msg void OnCbnSelchangeTexturesList();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedMtrlWireframe();
	afx_msg void OnBnClickedMtrlSmooth();
	afx_msg void OnBnClickedMtrlZtest();
	afx_msg void OnBnClickedMtrlZwrite();
	afx_msg void OnBnClickedMtrlFog();
	afx_msg void OnBnClickedMtrlLighting();
	afx_msg void OnBnClickedMtrlPoints();
	afx_msg void OnBnClickedMtrlCullface();
	afx_msg void OnBnClickedMtrlAlphatest();
	afx_msg void OnBnClickedMtrlBlend();
	afx_msg void OnBnClickedMtrlCwcull();
	afx_msg void OnBnClickedSelcetMaterialBtn();
	afx_msg void OnBnClickedAddTextureBtn();
public:
	afx_msg void OnBnClickedDepthShaderBtn();
public:
	CEdit Shininess;
public:
	afx_msg void OnBnClickedAmbientBtn();
public:
	afx_msg void OnBnClickedDiffuseBtn();
public:
	afx_msg void OnBnClickedSpecularBtn();
public:
	afx_msg void OnBnClickedEmissionBtn();
public:
	afx_msg void OnEnChangeShininess();
public:
	CComboBox SrcBlendLst;
public:
	CComboBox DstBlendLst;
public:
	CComboBox AlphaFuncLst;
public:
	CXTButton AmbientClrBtn;
public:
	CXTButton DiffuseClrBtn;
public:
	CXTButton SpecularClrBtn;
public:
	CXTButton EmissionClrBtn;
};
