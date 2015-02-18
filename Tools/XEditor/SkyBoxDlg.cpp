// SkyBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "SkyBoxDlg.h"


// CSkyBoxDlg dialog

IMPLEMENT_DYNAMIC(CSkyBoxDlg, CDialog)

CSkyBoxDlg::CSkyBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkyBoxDlg::IDD, pParent)
{

}

CSkyBoxDlg::~CSkyBoxDlg()
{
}

void CSkyBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKYBOX_NAME, NameTxt);
	DDX_Control(pDX, IDC_SKYBOXS_LIST, SkyboxLst);
}


BEGIN_MESSAGE_MAP(CSkyBoxDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSkyBoxDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOADSKYBOX, &CSkyBoxDlg::OnBnClickedLoadskybox)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSkyBoxDlg message handlers

void CSkyBoxDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	int i=SkyboxLst.GetCurSel();
	if(i==-1)
		selectedSky=mT("");
	else
	{
		CString tex;
		SkyboxLst.GetText(i,tex);
		selectedSky=tex.GetString();
	}
}

void CSkyBoxDlg::OnBnClickedLoadskybox()
{
	// TODO: Add your control notification handler code here
	CString str;
	NameTxt.GetWindowTextA(str);
	if(str==mT(""))
	{
		MessageBox(mT("Set SkyBox Name first"),mT("Skybox"),0);
		return;
	}
	CFileDialog fd(true,0,0,OFN_EXPLORER,mT("Image Files |*.*||"));
	if(fd.DoModal()==IDOK)
	{
		core::string path=fd.GetPathName().GetString();
		int i=path.findlast('\\');
		if(i!=-1){
			path=path.substr(0,i+1);
		}
		core::string ext=fd.GetFileExt().GetString();
		scene::SkyBoxManager::getInstance().loadSkyFromFolder(path,ext,str.GetString());
		SkyInfo ifo;
		ifo.name=str.GetString();
		ifo.path=path;
		ifo.ext=ext;
		m_skiesInfo[ifo.name]=ifo;
		RefreshList();
	}
}


void CSkyBoxDlg::OnExportLevelXML(xml::XMLElement*e)
{
	xml::XMLElement*elem=new xml::XMLElement(mT("Skies"));
	e->addSubElement(elem);
	std::map<core::string,SkyInfo>::iterator it=m_skiesInfo.begin();
	for(;it!=m_skiesInfo.end();++it)
	{
		xml::XMLElement* info=new xml::XMLElement(mT("SkyInfo"));
		SkyInfo&ifo=it->second;
		elem->addSubElement(info);
		elem->addAttribute(mT("Name"),ifo.name);
		elem->addAttribute(mT("Path"),ifo.path);
		elem->addAttribute(mT("Ext"),ifo.ext);
	}
}
bool CSkyBoxDlg::OnImportLevelXML(xml::XMLElement*e)
{
	xml::XMLElement*skies=e->getSubElement(mT("Skies"));
	if(skies)
	{
		xml::xmlSubElementsMapIT it= skies->getElementsBegin();
		for (;it!=skies->getElementsEnd();++it)
		{
			xml::XMLElement*el=*it;
			SkyInfo ifo;
			ifo.name=el->getAttribute(mT("Name"))->value;
			ifo.ext=el->getAttribute(mT("Ext"))->value;
			ifo.path=el->getAttribute(mT("Path"))->value;
			m_skiesInfo[ifo.name]=ifo;

			scene::SkyBoxManager::getInstance().loadSkyFromFolder(ifo.path,ifo.ext,ifo.name);
		}
	}
	return true;
}

void CSkyBoxDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	RefreshList();

	// TODO: Add your message handler code here
}



void CSkyBoxDlg::RefreshList()
{
	core::array<core::string> resources;
	SkyboxLst.ResetContent();

	scene::SkyBoxManager::getInstance().listResources(resources);
	for(int i=0;i<resources.size();++i)
	{
		SkyboxLst.AddString(resources[i].c_str());
	}
}
