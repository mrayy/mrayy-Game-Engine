
#include "stdafx.h"
#include "TexturesListDlg.h"

using namespace mray;

// CTexturesListDlg dialog

IMPLEMENT_DYNAMIC(CTexturesListDlg, CDialog)

CTexturesListDlg::CTexturesListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTexturesListDlg::IDD, pParent)
{

}

CTexturesListDlg::~CTexturesListDlg()
{
}

void CTexturesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURES_LIST, TexturesList);
	DDX_Control(pDX, IDC_TEXTURE_TYPE, TextureTypeList);
}

void CTexturesListDlg::OnShowWindow(BOOL show,UINT nStatus)
{
	CDialog::OnShowWindow(show,nStatus);
	RefreshList();

	TextureTypeList.AddString(mT("1D"));
	TextureTypeList.AddString(mT("2D"));
	TextureTypeList.AddString(mT("3D"));
	TextureTypeList.AddString(mT("Cube"));

	TextureTypeList.SetCurSel(1);
}

void CTexturesListDlg::OnClose()
{
	CDialog::OnClose();
}

BEGIN_MESSAGE_MAP(CTexturesListDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOADTEXTURE, &CTexturesListDlg::OnBnClickedLoadtexture)
	ON_BN_CLICKED(IDOK, &CTexturesListDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CTexturesListDlg::RefreshList()
{
	core::array<core::string> resources;
	TexturesList.ResetContent();
	gTextureResourceManager.listResources(resources);
	for(int i=0;i<resources.size();++i)
	{
		TexturesList.AddString(resources[i].c_str());
	}
}

// CTexturesListDlg message handlers

void CTexturesListDlg::OnBnClickedLoadtexture()
{
	// TODO: Add your control notification handler code here
	CFileDialog openFileDlg(true,0,0,OFN_OVERWRITEPROMPT,mT("Image Files |*.*||"));
	int iRet=openFileDlg.DoModal();
	if(iRet==IDOK)
	{
		switch(TextureTypeList.GetCurSel())
		{
		case 0:
			gTextureResourceManager.loadTexture1D(openFileDlg.GetPathName().GetString());
			break;
		case 1:
			gTextureResourceManager.loadTexture2D(openFileDlg.GetPathName().GetString());
			break;
		case 2:
			gTextureResourceManager.loadTexture3D(openFileDlg.GetPathName().GetString());
			break;
		case 3:
			gTextureResourceManager.loadTextureCube(openFileDlg.GetPathName().GetString());
			break;
		}
		RefreshList();
	}
}

void CTexturesListDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	int i=TexturesList.GetCurSel();
	if(i==-1)
		m_selectedTex=mT("");
	else
	{
		CString tex;
		TexturesList.GetText(i,tex);
		m_selectedTex=tex.GetString();
	}
}
