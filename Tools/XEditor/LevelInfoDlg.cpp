// LevelInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "LevelInfoDlg.h"
#include "GameActionsDialog.h"


// CLevelInfoDlg dialog

IMPLEMENT_DYNAMIC(CLevelInfoDlg, CDialog)

CLevelInfoDlg::CLevelInfoDlg(CEngineWindow* e,CWnd* pParent /*=NULL*/)
	: CDialog(CLevelInfoDlg::IDD, pParent)
	, LevelName(_T(""))
	, LevelDescription(_T(""))
	, LevelThumbnail(_T(""))
	, m_editor(e)
	, MainAction(_T(""))
{

}

CLevelInfoDlg::~CLevelInfoDlg()
{
}

void CLevelInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LEVEL_NAME_TXT, LevelName);
	DDX_Text(pDX, IDC_LEVEL_DESCRIPTION_TXT, LevelDescription);
	DDX_Text(pDX, IDC_LEVEL_THUMPNAIL_TXT, LevelThumbnail);
	DDX_Text(pDX, IDC_MAINACTION_NAME_TXT, MainAction);
}


BEGIN_MESSAGE_MAP(CLevelInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_SELECT_THUMBNAIL_BTN, &CLevelInfoDlg::OnBnClickedSelectThumbnailBtn)
	ON_BN_CLICKED(IDOK, &CLevelInfoDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SELECT_MAINACTION_BTN, &CLevelInfoDlg::OnBnClickedSelectMainactionBtn)
END_MESSAGE_MAP()


// CLevelInfoDlg message handlers

void CLevelInfoDlg::OnBnClickedSelectThumbnailBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,0,0,OFN_OVERWRITEPROMPT,mT("Thumbnail (*.*)|*.*||"));
	if(dlg.DoModal()==IDOK)
	{
		core::string path=dlg.GetPathName().GetString();
		core::string app=gFileSystem.getAppPath();
		app.replace('/','\\');

		path=gFileSystem.makePathRelative(path,app);
		LevelThumbnail=path.c_str();
		UpdateData(false);
	}
}

void CLevelInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	UpdateData(true);
	m_editor->GetLevel()->SetLevelName(LevelName.GetString());
	m_editor->GetLevel()->SetLevelDescription(LevelDescription.GetString());
	m_editor->GetLevel()->SetLevelThumbNailName(LevelThumbnail.GetString());

	GCPtr<game::IGameAction> c=game::GameActionFactory::getInstance().CreateAction(MainAction.GetString());
	m_editor->GetLevel()->SetMainAction(c);
}
void CLevelInfoDlg::OnBnClickedSelectMainactionBtn()
{
	// TODO: Add your control notification handler code here
	CGameActionsDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		MainAction=dlg.ActionName.c_str();
		UpdateData(false);
	}
}

void CLevelInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	MainAction="";
	if(m_editor->GetLevel()->GetMainAction())
		MainAction=m_editor->GetLevel()->GetMainAction()->GetName().c_str();

	LevelName=m_editor->GetLevel()->GetLevelName().c_str();
	LevelDescription=m_editor->GetLevel()->GetLevelDescription().c_str();
	LevelThumbnail=m_editor->GetLevel()->GetLevelThumbNailName().c_str();
	UpdateData(false);
}

