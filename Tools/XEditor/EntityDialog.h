#pragma once
#include "afxwin.h"


// CEntityDialog dialog
using namespace mray;

namespace mray
{
	class CEngineWindow;
}

class CEntityDialog : public CDialog
{
	DECLARE_DYNAMIC(CEntityDialog)

	mray::CEngineWindow* m_editor;
	void OnModelSelected(const mray::PVOID&);
	
	void RefreshAttributes();
public:
	CEntityDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEntityDialog();

// Dialog Data
	enum { IDD = IDD_ENTITY_PROP_DLG };
	void SetEditor(mray::CEngineWindow*e);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedActionNameBtn();
public:
	CString EntityName;
public:
	int EntityID;
public:
	CComboBox AttributesLst;
public:
	afx_msg void OnCbnSelchangeAttributeList();
public:
	CString AttributeValue;
public:
	afx_msg void OnEnKillfocusModAttrValueTxt();
};
