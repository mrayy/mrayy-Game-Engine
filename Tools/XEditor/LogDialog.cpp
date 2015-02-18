// LogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XEditor.h"
#include "LogDialog.h"


// CLogDialog dialog

IMPLEMENT_DYNAMIC(CLogDialog, CXTResizeDialog)

CLogDialog::CLogDialog(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CLogDialog::IDD, pParent)
{

}

CLogDialog::~CLogDialog()
{
}

void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LIST, LogLines);
}

BOOL CLogDialog::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	SetResize(IDC_LOG_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	LogLines.InsertColumn(0, _T("Level"),   LVCFMT_LEFT, 125);
	LogLines.InsertColumn(1, _T("Message"),   LVCFMT_LEFT, 125);

	HWND hWndHeader = LogLines.GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);
	m_header.EnableAutoSize(TRUE);
	m_header.ResizeColumnsToFit();

	LogLines.ModifyExtendedStyle( 0,	LVS_EX_FULLROWSELECT  );
	LogLines.ModifyExtendedStyle( 0,LVS_EX_GRIDLINES );
	LogLines.EnableUserRowColor(true);
	return true;
}

BEGIN_MESSAGE_MAP(CLogDialog, CXTResizeDialog)
END_MESSAGE_MAP()


// CLogDialog message handlers
