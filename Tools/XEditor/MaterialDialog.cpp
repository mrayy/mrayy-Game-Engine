// MaterialDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MaterialDialog.h"


// CMaterialDialog dialog

IMPLEMENT_DYNAMIC(CMaterialDialog, CDialog)

CMaterialDialog::CMaterialDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDialog::IDD, pParent)
{

}

CMaterialDialog::~CMaterialDialog()
{
}

void CMaterialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATERIAL_NAME, MaterialName);
	DDX_Control(pDX, IDC_MATERIALS_LIST, MaterialsList);
}

void CMaterialDialog::OnShowWindow(BOOL show,UINT nStatus)
{
	CDialog::OnShowWindow(show,nStatus);
	RefreshList();
}

BEGIN_MESSAGE_MAP(CMaterialDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LOADSHADER, &CMaterialDialog::OnBnClickedLoadshader)
	ON_BN_CLICKED(IDOK, &CMaterialDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DUPLICATE_MTRL, &CMaterialDialog::OnBnClickedDuplicateMtrl)
END_MESSAGE_MAP()


void CMaterialDialog::RefreshList()
{
	core::array<core::string> resources;
	MaterialsList.ResetContent();
	gMaterialResourceManager.listResources(resources);
	for(int i=0;i<resources.size();++i)
	{
		MaterialsList.AddString(resources[i].c_str());
	}
}

// CMaterialDialog message handlers

void CMaterialDialog::OnBnClickedLoadshader()
{
	// TODO: Add your control notification handler code here
	CString name;
	MaterialName.GetWindowTextA(name);
	if(name==mT(""))
	{
		MessageBox(mT("Material Name is empty!"),mT("New Material"),0);
		return;
	}
	if(gMaterialResourceManager.getResource(name.GetString())!=0)
	{
		MessageBox(mT("Material is already exists!"),mT("New Material"),0);
		return;
	}

	video::SMaterial* mtrl=new video::SMaterial();
	gMaterialResourceManager.addResource(mtrl,name.GetString());
	MaterialsList.AddString(name);
	MaterialsList.SetSel(MaterialsList.GetCount()-1);
	//RefreshList();
}

void CMaterialDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	int i=MaterialsList.GetCurSel();
	if(i==-1)
		m_selectedMtrl=mT("");
	else
	{
		CString m;
		MaterialsList.GetText(i,m);
		m_selectedMtrl=m.GetString();
	}
}

void CMaterialDialog::OnBnClickedDuplicateMtrl()
{
	// TODO: Add your control notification handler code here
	int i=MaterialsList.GetCurSel();
	if(i==-1){
		MessageBox(mT("Select material to duplicate"),mT("duplicate"));
		return;
	}
	else
	{
		CString m;
		MaterialsList.GetText(i,m);
		GCPtr<video::SMaterial> parent=gMaterialResourceManager.getResource(m.GetString());
		GCPtr<video::SMaterial> mtrl=parent->duplicate();

		CString name;
		MaterialName.GetWindowTextA(name);
		gMaterialResourceManager.addResource(mtrl,name.GetString());
		MaterialsList.AddString(name);
		MaterialsList.SetSel(MaterialsList.GetCount()-1);
		//RefreshList();
	}
}
