// MaterialToolBox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MaterialEditor.h"
#include "CEngineWindow.h"
#include "TexturesListDlg.h"
#include "ShaderDialog.h"
#include "MaterialDialog.h"

#include "EditorEventMessages.h"

// CMaterialEditor dialog

IMPLEMENT_DYNAMIC(CMaterialEditor, CDialog)

CMaterialEditor::CMaterialEditor(CWnd* pParent )
	: CDialog(CMaterialEditor::IDD,pParent)
{
	m_editor=0;
	m_selectedMtrl=0;
	m_selectedTextureUnit=-1;
	m_selectedMesh=-1;

	EditorEventManager::getInstance().AddHandler(EEE_OnModelSelected,newClassDelegate1<CMaterialEditor,void,mray::PVOID>(mT(""),this,&CMaterialEditor::OnModelSelected));
	EditorEventManager::getInstance().AddHandler(EEE_OnEngineInitiated,newClassDelegate1<CMaterialEditor,void,mray::PVOID>(mT(""),this,&CMaterialEditor::OnEngineInit));
}

CMaterialEditor::~CMaterialEditor()
{
}


void CMaterialEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUBMESH_LIST, SubmeshList);
	DDX_Control(pDX, IDC_MATERIAL_NAME, MaterialName);
	DDX_Control(pDX, IDC_MTRL_WIREFRAME, WireframeState);
	DDX_Control(pDX, IDC_MTRL_SMOOTH, SmoothState);
	DDX_Control(pDX, IDC_MTRL_ZTEST, ZTestState);
	DDX_Control(pDX, IDC_MTRL_ZWRITE, ZWriteState);
	DDX_Control(pDX, IDC_MTRL_FOG, FogState);
	DDX_Control(pDX, IDC_MTRL_POINTS, PointsState);
	DDX_Control(pDX, IDC_MTRL_CULLFACE, CullfaceState);
	DDX_Control(pDX, IDC_MTRL_ALPHATEST, AlphaTestState);
	DDX_Control(pDX, IDC_MTRL_BLEND, BlendState);
	DDX_Control(pDX, IDC_MTRL_CWCULL, CWCullState);
	DDX_Control(pDX, IDC_TEXTURES_LIST, TexturesList);
	DDX_Control(pDX, IDC_VALUES_LIST, ValuesList);
	DDX_Control(pDX, IDC_VALUETYPE_LIST, ValueTypeList);
	DDX_Control(pDX, IDC_MTRL_LIGHTING, LightingState);
	DDX_Control(pDX, IDC_SHADERNAME_TXT, ShaderName);
	DDX_Control(pDX, IDC_SHADOWSHADER_TXT, ShaderShadowName);
	DDX_Control(pDX, IDC_TEXTURENAME_TXT, TextureNameTxt);
	DDX_Control(pDX, IDC_SHININESS, Shininess);;
	DDX_Control(pDX, IDC_SOURCE_BLEND_LST, SrcBlendLst);
	DDX_Control(pDX, IDC_DEST_BLEN_LIST, DstBlendLst);
	DDX_Control(pDX, IDC_ALPHA_FUNCTION_LIST, AlphaFuncLst);
	DDX_Control(pDX, IDC_AMBIENT_BTN, AmbientClrBtn);
	DDX_Control(pDX, IDC_DIFFUSE_BTN, DiffuseClrBtn);
	DDX_Control(pDX, IDC_SPECULAR_BTN, SpecularClrBtn);
	DDX_Control(pDX, IDC_EMISSION_BTN, EmissionClrBtn);
}

BOOL CMaterialEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubmeshList.SetHorizontalExtent(5);

	ValueTypeList.AddString(mT("Integer"));
	ValueTypeList.AddString(mT("Float"));

	ValueTypeList.AddString(mT("Vector2d"));
	ValueTypeList.AddString(mT("Vector3d"));
	ValueTypeList.AddString(mT("Vector4d"));

	ValueTypeList.AddString(mT("Color"));
	ValueTypeList.AddString(mT("Texture"));
	ValueTypeList.AddString(mT("Shader"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CMaterialEditor, CDialog)
	ON_CBN_SELCHANGE(IDC_SUBMESH_LIST, &CMaterialEditor::OnCbnSelchangeSubmeshList)
	ON_CBN_SELCHANGE(IDC_TEXTURES_LIST, &CMaterialEditor::OnCbnSelchangeTexturesList)
	ON_BN_CLICKED(IDC_BUTTON3, &CMaterialEditor::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CMaterialEditor::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_MTRL_WIREFRAME, &CMaterialEditor::OnBnClickedMtrlWireframe)
	ON_BN_CLICKED(IDC_MTRL_SMOOTH, &CMaterialEditor::OnBnClickedMtrlSmooth)
	ON_BN_CLICKED(IDC_MTRL_ZTEST, &CMaterialEditor::OnBnClickedMtrlZtest)
	ON_BN_CLICKED(IDC_MTRL_ZWRITE, &CMaterialEditor::OnBnClickedMtrlZwrite)
	ON_BN_CLICKED(IDC_MTRL_FOG, &CMaterialEditor::OnBnClickedMtrlFog)
	ON_BN_CLICKED(IDC_MTRL_LIGHTING, &CMaterialEditor::OnBnClickedMtrlLighting)
	ON_BN_CLICKED(IDC_MTRL_POINTS, &CMaterialEditor::OnBnClickedMtrlPoints)
	ON_BN_CLICKED(IDC_MTRL_CULLFACE, &CMaterialEditor::OnBnClickedMtrlCullface)
	ON_BN_CLICKED(IDC_MTRL_ALPHATEST, &CMaterialEditor::OnBnClickedMtrlAlphatest)
	ON_BN_CLICKED(IDC_MTRL_BLEND, &CMaterialEditor::OnBnClickedMtrlBlend)
	ON_BN_CLICKED(IDC_MTRL_CWCULL, &CMaterialEditor::OnBnClickedMtrlCwcull)
	ON_BN_CLICKED(IDC_SELCET_MATERIAL_BTN, &CMaterialEditor::OnBnClickedSelcetMaterialBtn)
	ON_BN_CLICKED(IDC_ADD_TEXTURE_BTN, &CMaterialEditor::OnBnClickedAddTextureBtn)
	ON_BN_CLICKED(IDC_DEPTH_SHADER_BTN, &CMaterialEditor::OnBnClickedDepthShaderBtn)

	
	ON_BN_CLICKED(IDC_AMBIENT_BTN, &CMaterialEditor::OnBnClickedAmbientBtn)
	ON_BN_CLICKED(IDC_DIFFUSE_BTN, &CMaterialEditor::OnBnClickedDiffuseBtn)
	ON_BN_CLICKED(IDC_SPECULAR_BTN, &CMaterialEditor::OnBnClickedSpecularBtn)
	/*ON_CPN_XT_SELENDOK*/ON_BN_CLICKED(IDC_EMISSION_BTN, &CMaterialEditor::OnBnClickedEmissionBtn)

	ON_EN_CHANGE(IDC_SHININESS, &CMaterialEditor::OnEnChangeShininess)
END_MESSAGE_MAP()

void CMaterialEditor::SetEditor(mray::CEngineWindow*e)
{
	m_editor=e;
}

void CMaterialEditor::OnExportLevelXML(xml::XMLElement*e)
{
}
bool CMaterialEditor::OnImportLevelXML(xml::XMLElement*e)
{
	return false;
}
void CMaterialEditor::ShowToolBox(bool s)
{
	this->ShowWindow(s? SW_SHOW : SW_HIDE);
}
bool CMaterialEditor::IsToolBoxVisible()
{
	return this->IsWindowVisible();
}


void CMaterialEditor::ClearSettings()
{
	MaterialName.SetWindowText(mT(""));

	AlphaTestState.SetCheck(false);
	BlendState	   .SetCheck(false);
	CullfaceState .SetCheck(false);
	FogState	   .SetCheck(false);
	LightingState .SetCheck(false);
	PointsState   .SetCheck(false);
	SmoothState   .SetCheck(false);
	WireframeState.SetCheck(false);
	ZTestState    .SetCheck(false);
	ZWriteState   .SetCheck(false);


	ShaderName.SetWindowText(mT(""));
	ShaderShadowName.SetWindowText(mT(""));

	TexturesList.ResetContent();

	m_selectedMesh=-1;
	m_selectedMtrl=0;
	m_selectedTextureUnit=-1;
}

void CMaterialEditor::OnEngineInit(const mray::PVOID&)
{

	SrcBlendLst.ResetContent();
	DstBlendLst.ResetContent();
	AlphaFuncLst.ResetContent();

	for(int i=0;i<=video::EBF_SRC_ALPHA_SATURATE;++i)
	{
		core::string str=core::StringConverter::toString(i,core::string(mT("EBlendFactor")));
		SrcBlendLst.AddString(str.c_str());
		DstBlendLst.AddString(str.c_str());
	}
	for(int i=0;i<=video::EAF_Always;++i)
	{
		core::string str=core::StringConverter::toString(i,core::string(mT("EAlphaFunc")));
		AlphaFuncLst.AddString(str.c_str());
	}
}

void CMaterialEditor::OnModelSelected(const mray::PVOID&)
{
	ClearSettings();

	scene::ISceneNode*node=m_editor->GetSelectedNode();
	SubmeshList.ResetContent();
	if(node)
	{
		scene::SMesh*mesh= node->getMesh();
		if(mesh)
		{
			for(int i=0;i<mesh->getBuffersCount();++i)
			{
				CString val=mesh->getBuffer(i)->getMeshName().c_str();
				SubmeshList.AddString(val);
			}
		}
	}

}
void CMaterialEditor::OnMeshSelected(int i)
{
	scene::ISceneNode*node=m_editor->GetSelectedNode();

	if(!node || i==-1)
		return;
	m_selectedMesh=i;

	scene::SMesh*mesh= node->getMesh();
	m_selectedMtrl= mesh->getBufferData(i)->getMaterial();
	if(m_selectedMtrl)
	{
		MaterialName.SetWindowText(m_selectedMtrl->getResourceName().c_str());

		AlphaTestState.SetCheck(m_selectedMtrl->getRenderState(video::RS_AlphaTest)==video::ES_Use);
		BlendState	   .SetCheck(m_selectedMtrl->getRenderState(video::RS_Blend)==video::ES_Use);
		CullfaceState .SetCheck(m_selectedMtrl->getRenderState(video::RS_CullFace)==video::ES_Use);
		FogState	   .SetCheck(m_selectedMtrl->getRenderState(video::RS_Fog)==video::ES_Use);
		LightingState .SetCheck(m_selectedMtrl->getRenderState(video::RS_Lighting)==video::ES_Use);
		PointsState   .SetCheck(m_selectedMtrl->getRenderState(video::RS_Points)==video::ES_Use);
		SmoothState   .SetCheck(m_selectedMtrl->getRenderState(video::RS_Smooth)==video::ES_Use);
		WireframeState.SetCheck(m_selectedMtrl->getRenderState(video::RS_Wireframe)==video::ES_Use);
		ZTestState    .SetCheck(m_selectedMtrl->getRenderState(video::RS_ZTest)==video::ES_Use);
		ZWriteState   .SetCheck(m_selectedMtrl->getRenderState(video::RS_ZWrite)==video::ES_Use);
/*
		AmbientClrBtn.SetXPFlatColors(0,ConvertToDWORD(m_selectedMtrl->ambientColor),0);
		DiffuseClrBtn.SetXPFlatColors(0,ConvertToDWORD(m_selectedMtrl->diffuseColor),0);
		SpecularClrBtn.SetXPFlatColors(0,ConvertToDWORD(m_selectedMtrl->specularColor),0);
		EmissionClrBtn.SetXPFlatColors(0,ConvertToDWORD(m_selectedMtrl->emissiveColor),0);

*/
		Shininess.SetWindowTextA(core::StringConverter::toString((int)m_selectedMtrl->shininess).c_str());

		if(m_selectedMtrl->shader)
			ShaderName.SetWindowText(m_selectedMtrl->shader->getResourceName().c_str());

		TexturesList.ResetContent();

		for(int i=0;i<m_selectedMtrl->getTexturesCount();++i)
		{
			video::ITexture*tex=m_selectedMtrl->getTexture(i);
			TexturesList.AddString(core::StringConverter::toString(i).c_str());
		}
	}
}
void CMaterialEditor::OnTextureUnitChange(int i)
{
	if(!m_selectedMtrl)
		return;

	m_selectedTextureUnit=i;

	video::TextureUnit* texUnit= m_selectedMtrl->getTextureUnit(m_selectedTextureUnit);

	if(!texUnit)
		return;
	if(texUnit->texture)
	{
		TextureNameTxt.SetWindowText(texUnit->texture->getResourceName().c_str());
	}
}



void CMaterialEditor::OnMaterialChange(const core::string&name)
{
	scene::ISceneNode*node=m_editor->GetSelectedNode();
	if(!node || m_selectedMesh==-1)
		return;
	node->getMesh()->getBufferData(m_selectedMesh)->setMaterial(gMaterialResourceManager.getMaterial(name));

	OnMeshSelected(m_selectedMesh);
}

void CMaterialEditor::OnTextureChange(const core::string&name)
{
	if(!m_selectedMtrl || m_selectedTextureUnit==-1)
		return;
	video::TextureUnit* texUnit= m_selectedMtrl->getTextureUnit(m_selectedTextureUnit);
	if(!texUnit)
		return;
	texUnit->texture=gTextureResourceManager.getResource(name);
	TextureNameTxt.SetWindowText(name.c_str());
}

void CMaterialEditor::OnShaderChange(const core::string&name)
{
	if(!m_selectedMtrl)
		return;
	m_selectedMtrl->shader=gShaderResourceManager.getResource(name);
	ShaderName.SetWindowText(name.c_str());
}

void CMaterialEditor::OnDepthShaderChange(const core::string&name)
{
	if(!m_selectedMtrl)
		return;
	m_selectedMtrl->depthShader=gShaderResourceManager.getResource(name);
	ShaderShadowName.SetWindowText(name.c_str());
}

void CMaterialEditor::OnStatusChanged(video::RenderStates st,bool v)
{
	if(!m_selectedMtrl)
		return;
	m_selectedMtrl->setRenderState(st,v?video::ES_Use : video ::ES_DontUse);
}

void CMaterialEditor::OnAddTextureUnit()
{
	if(!m_selectedMtrl)
		return;
	m_selectedMtrl->setTexture(0,m_selectedMtrl->getTexturesCount());

	OnMeshSelected(m_selectedMesh);
}

//////////////////////////////////////////////////////////////////////////



void CMaterialEditor::OnCbnSelchangeSubmeshList()
{
	// TODO: Add your control notification handler code here
	OnMeshSelected(SubmeshList.GetCurSel());
}

void CMaterialEditor::OnCbnSelchangeTexturesList()
{
	// TODO: Add your control notification handler code here
	OnTextureUnitChange(TexturesList.GetCurSel());
}

void CMaterialEditor::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CTexturesListDlg dlg;
	int iRet=dlg.DoModal();
	if(iRet==IDOK)
	{
		OnTextureChange(dlg.GetSelectedTexture());
	}
}

void CMaterialEditor::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	CShaderDialog dlg;
	int iRet=dlg.DoModal();
	if(iRet==IDOK)
	{
		OnShaderChange(dlg.GetSelectedShader());
	}
}
// CMaterialEditor message handlers

void CMaterialEditor::OnBnClickedDepthShaderBtn()
{
	// TODO: Add your control notification handler code here
	CShaderDialog dlg;
	int iRet=dlg.DoModal();
	if(iRet==IDOK)
	{
		OnDepthShaderChange(dlg.GetSelectedShader());
	}
}
void CMaterialEditor::OnBnClickedSelcetMaterialBtn()
{
	// TODO: Add your control notification handler code here
	CMaterialDialog dlg;
	int iRet=dlg.DoModal();
	if(iRet==IDOK)
	{
		OnMaterialChange(dlg.GetSelectedMaterial());
	}
}

void CMaterialEditor::OnBnClickedMtrlWireframe()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Wireframe,WireframeState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlSmooth()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Smooth,SmoothState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlZtest()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_ZTest,ZTestState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlZwrite()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_ZWrite,ZWriteState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlFog()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Fog,FogState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlLighting()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Lighting,LightingState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlPoints()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Points,PointsState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlCullface()
{
	int st=WireframeState.GetCheck();
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_CullFace,CullfaceState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlAlphatest()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_AlphaTest,AlphaTestState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlBlend()
{
	// TODO: Add your control notification handler code here
	OnStatusChanged(video::RS_Blend,BlendState.GetCheck()==1);
}

void CMaterialEditor::OnBnClickedMtrlCwcull()
{
	// TODO: Add your control notification handler code here
}

void CMaterialEditor::OnBnClickedAddTextureBtn()
{
	// TODO: Add your control notification handler code here
	OnAddTextureUnit();
}


video::SColor CMaterialEditor::ConvertToColor(COLORREF c)
{
	video::SColor clr;
	clr.R=GetRValue(c);
	clr.G=GetGValue(c);
	clr.B=GetBValue(c);
	clr.A=255;
	return clr;
}
COLORREF CMaterialEditor::ConvertToDWORD(const video::SColor&c)
{/*
	COLORREF clr=0;
	clr=(c.A <<24 );
	clr|=(c.B<<16); 
	clr|=(c.G<<8);
	clr|=c.R;
	return clr;*/
	return RGB(c.R,c.G,c.B);
}

void CMaterialEditor::OnBnClickedAmbientBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_selectedMtrl)
		return;
	CXTColorDialog clrDlg(ConvertToDWORD(m_selectedMtrl->ambientColor),RGB(255,255,255));
	if(clrDlg.DoModal()==IDOK){
		COLORREF c=clrDlg.GetColor();
		m_selectedMtrl->ambientColor=ConvertToColor(c);
	}
	
}

void CMaterialEditor::OnBnClickedDiffuseBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_selectedMtrl)
		return;
	CXTColorDialog clrDlg(ConvertToDWORD(m_selectedMtrl->diffuseColor),RGB(255,255,255));
	if(clrDlg.DoModal()==IDOK){
		DWORD c=clrDlg.GetColor();
		m_selectedMtrl->diffuseColor=ConvertToColor(c);
	}
}
void CMaterialEditor::OnBnClickedSpecularBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_selectedMtrl)
		return;
	CXTColorDialog clrDlg(ConvertToDWORD(m_selectedMtrl->specularColor),RGB(255,255,255));
	if(clrDlg.DoModal()==IDOK){
		DWORD c=clrDlg.GetColor();
		m_selectedMtrl->specularColor=ConvertToColor(c);
	}
}


void CMaterialEditor::OnBnClickedEmissionBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_selectedMtrl)
		return;
	CXTColorDialog clrDlg(ConvertToDWORD(m_selectedMtrl->emissiveColor),RGB(255,255,255));
	if(clrDlg.DoModal()==IDOK){
		DWORD c=clrDlg.GetColor();
		m_selectedMtrl->emissiveColor=ConvertToColor(c);
	}
}

void CMaterialEditor::OnEnChangeShininess()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if(!m_selectedMtrl)
		return;

	CString str;
	Shininess.GetWindowTextA(str);
	float shin=core::StringConverter::toFloat(str.GetString());
	m_selectedMtrl->shininess=shin;
}
