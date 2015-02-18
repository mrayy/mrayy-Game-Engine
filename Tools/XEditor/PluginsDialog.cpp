// PluginsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "PluginsDialog.h"
#include "GameGlobals.h"


// CPluginsDialog dialog

IMPLEMENT_DYNAMIC(CPluginsDialog, CXTResizeDialog)

CPluginsDialog::CPluginsDialog(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CPluginsDialog::IDD, pParent),m_editor(0)
{

}

CPluginsDialog::~CPluginsDialog()
{
}

void CPluginsDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGINS_LIST, PluginsList);
}


BEGIN_MESSAGE_MAP(CPluginsDialog, CXTResizeDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_INSTALL_PLUGIN_BTN, &CPluginsDialog::OnBnClickedInstallPluginBtn)
	ON_BN_CLICKED(IDC_UNINSTALL_PLUGIN_BTN, &CPluginsDialog::OnBnClickedUninstallPluginBtn)
	ON_BN_CLICKED(IDC_REINSTALL_PLUGIN_BTN, &CPluginsDialog::OnBnClickedReinstallPluginBtn)
END_MESSAGE_MAP()


void CPluginsDialog::SetEditor(CEngineWindow* edtor)
{
	m_editor=edtor;
}

// CPluginsDialog message handlers
BOOL CPluginsDialog::OnInitDialog()
{

	CXTResizeDialog::OnInitDialog();

	SetResize(IDC_PLUGINS_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	PluginsList.InsertColumn(0, _T("ID"),   LVCFMT_LEFT, 125);
	PluginsList.InsertColumn(1, _T("Name"),   LVCFMT_LEFT, 125);
	PluginsList.InsertColumn(2, _T("Status"), LVCFMT_LEFT, 125);

	HWND hWndHeader = PluginsList.GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);
	m_header.EnableAutoSize(TRUE);
	m_header.ResizeColumnsToFit();

	PluginsList.ModifyExtendedStyle( 0,	LVS_EX_FULLROWSELECT  );
	PluginsList.ModifyExtendedStyle( 0,LVS_EX_GRIDLINES );
	PluginsList.EnableUserRowColor(true);
	return true;
}
void CPluginsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTResizeDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	RefreshList();
}

void CPluginsDialog::RefreshList()
{
	core::array<int> arr;
	PluginManager::getInstance().listPlugins(arr);

	PluginsList.DeleteAllItems();
	for(int i=0;i<arr.size();++i)
	{
		PluginsList.InsertItem(i, core::StringConverter::toString(arr[i]).c_str(), 0);
		IPlugin*p=PluginManager::getInstance().getPlugin(arr[i]);
		if(p)
		{
			PluginsList.SetItem(i,1,LVIF_TEXT, p->getName(), 0, NULL, NULL, NULL);
			PluginsList.SetItem(i,2,LVIF_TEXT, p->isInstalled()? mT("Installed") : mT("Not Installed"), 0, NULL, NULL, NULL);
			if(p->isInstalled())
			{
				PluginsList.SetRowColor(i, 0, RGB(0,255,0));
			}else
				PluginsList.SetRowColor(i, 0, RGB(255,0,0));
		}
	}

}
void CPluginsDialog::OnBnClickedInstallPluginBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,mT("Plugin files *.dll|*.dll||"));

	if(dlg.DoModal()==IDOK)
	{
		core::string fileName= dlg.GetFileName().GetString();
		m_editor->GetLevel()->GetGameGlobals()->AddRequiredPlugin(fileName);
		PluginManager::getInstance().addPlugin(dlg.GetPathName().GetString());
	}
	RefreshList();
}

void CPluginsDialog::OnBnClickedUninstallPluginBtn()
{
	// TODO: Add your control notification handler code here
	POSITION p=PluginsList.GetFirstSelectedItemPosition();
	if(p){
		int i=PluginsList.GetNextSelectedItem(p);
		int id=core::StringConverter::toInt(PluginsList.GetItemText(i,0).GetString());
		IPlugin*p=PluginManager::getInstance().getPlugin(id);
		if(p)
		{
			p->uninstall();
		}
	}
	RefreshList();
}

void CPluginsDialog::OnBnClickedReinstallPluginBtn()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	POSITION p=PluginsList.GetFirstSelectedItemPosition();
	if(p){
		int i=PluginsList.GetNextSelectedItem(p);
		int id=core::StringConverter::toInt(PluginsList.GetItemText(i,0).GetString());
		IPlugin*p=PluginManager::getInstance().getPlugin(id);
		if(p)
		{
			p->install();
		}
	}
	RefreshList();
}
