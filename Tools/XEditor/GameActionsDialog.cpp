// GameActionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "GameActionsDialog.h"
#include "GameActionFactory.h"


// CGameActionsDialog dialog

IMPLEMENT_DYNAMIC(CGameActionsDialog, CDialog)

CGameActionsDialog::CGameActionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGameActionsDialog::IDD, pParent)
{

}

CGameActionsDialog::~CGameActionsDialog()
{
}

void CGameActionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_ACTIONS_LIST, ActionsList);
	DDX_Control(pDX, IDC_DESCRIPTION_TXT, ActionDescription);
}


BEGIN_MESSAGE_MAP(CGameActionsDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CGameActionsDialog::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_GAME_ACTIONS_LIST, &CGameActionsDialog::OnLbnSelchangeGameActionsList)
END_MESSAGE_MAP()


// CGameActionsDialog message handlers

void CGameActionsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	RefreshList();
}

void CGameActionsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	int i=ActionsList.GetCurSel();
	if(i==-1)
		ActionName=mT("");
	else
	{
		CString m;
		ActionsList.GetText(i,m);
		ActionName=m.GetString();
	}
}


void CGameActionsDialog::RefreshList()
{
	core::array<core::string> actions;
	game::GameActionFactory::getInstance().ListFactories(actions);

	ActionsList.ResetContent();
	for(int i=0;i<actions.size();++i)
	{
		ActionsList.AddString(actions[i].c_str());
	}
}

void CGameActionsDialog::OnLbnSelchangeGameActionsList()
{
	// TODO: Add your control notification handler code here
	int i=ActionsList.GetCurSel();
	if(i==-1)
	{
		ActionDescription.SetWindowText("");
	}else
	{
		CString str;
		ActionsList.GetText(i,str);
		GCPtr<game::IGameActionCreator> c=game::GameActionFactory::getInstance().GetCreater(str.GetString());
		if(c)
		{
			ActionDescription.SetWindowText(c->GetDescription().c_str());
		}
	}
}
