// ShaderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "ShaderDialog.h"


// CShaderDialog dialog

IMPLEMENT_DYNAMIC(CShaderDialog, CDialog)

CShaderDialog::CShaderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShaderDialog::IDD, pParent)
{

}

CShaderDialog::~CShaderDialog()
{
}

void CShaderDialog::RefreshList()
{
	core::array<core::string> resources;
	ShaderList.ResetContent();
	gShaderResourceManager.listResources(resources);
	for(int i=0;i<resources.size();++i)
	{
		ShaderList.AddString(resources[i].c_str());
	}
}
void CShaderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHADERS_LIST, ShaderList);
}

void CShaderDialog::OnShowWindow(BOOL show,UINT nStatus)
{
	CDialog::OnShowWindow(show,nStatus);
	RefreshList();
}

BEGIN_MESSAGE_MAP(CShaderDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LOADSHADER, &CShaderDialog::OnBnClickedLoadshader)
	ON_BN_CLICKED(IDOK, &CShaderDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CShaderDialog message handlers

void CShaderDialog::OnBnClickedLoadshader()
{
	// TODO: Add your control notification handler code here
	CFileDialog openFileDlg(true,0,0,OFN_OVERWRITEPROMPT,mT("Shader Files |*.xml||"));
	int iRet=openFileDlg.DoModal();
	if(iRet==IDOK)
	{
		//gShaderResourceManager.loadShader(openFileDlg.GetPathName().GetString());
		RefreshList();
	}

}

void CShaderDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	int i=ShaderList.GetCurSel();
	if(i==-1)
		m_selectedShader=mT("");
	else
	{
		CString sh;
		ShaderList.GetText(i,sh);
		m_selectedShader=sh.GetString();
	}
}
