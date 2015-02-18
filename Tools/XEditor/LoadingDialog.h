#pragma once


// LoadingDialog dialog

class LoadingDialog : public CDialog
{
	DECLARE_DYNAMIC(LoadingDialog)

public:
	LoadingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~LoadingDialog();

// Dialog Data
	enum { IDD = IDD_LOADING_DIALOG };

	void UpdateStatus(const CString&state);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString StatusTxt;
};
