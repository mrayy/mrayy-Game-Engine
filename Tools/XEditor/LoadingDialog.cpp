// LoadingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "LoadingDialog.h"


// LoadingDialog dialog

IMPLEMENT_DYNAMIC(LoadingDialog, CDialog)

LoadingDialog::LoadingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LoadingDialog::IDD, pParent)
	, StatusTxt(_T(""))
{

}

LoadingDialog::~LoadingDialog()
{
}

void LoadingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOADING_STATUES_TXT, StatusTxt);
}


BEGIN_MESSAGE_MAP(LoadingDialog, CDialog)
END_MESSAGE_MAP()


// LoadingDialog message handlers

void LoadingDialog::UpdateStatus(const CString&state)
{
	StatusTxt=mT("Loading: ")+state;
	UpdateData(false);
}