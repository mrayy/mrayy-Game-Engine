#pragma once
#include "afxwin.h"
#include "IToolBox.h"

using namespace  mray;

// CAnimationDialog dialog
namespace mray
{
	class CEngineWindow;
}

class CAnimationDialog : public CDialog,public IToolBox
{
	DECLARE_DYNAMIC(CAnimationDialog)

	mray::CEngineWindow* m_editor;

	void OnModelSelected(const mray::PVOID&);

	void RefereshList();
	void AddAnimationTrack(GCPtr<animation::AnimationTrackGroup> track);
	void PlayTrack(const core::string&name);
	void StopTrack(const core::string&name);

public:
	CAnimationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimationDialog();

	core::array<core::string> animationFiles;

// Dialog Data
	enum { IDD = IDD_ANIMATION_BOX };

	void SetEditor(mray::CEngineWindow*e);

	virtual void ShowToolBox(bool s);
	virtual bool IsToolBoxVisible();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnShowWindow(BOOL show,UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox TracksList;
public:
	afx_msg void OnBnClickedLoadanimBtn();
public:
	afx_msg void OnBnClickedPlayanimBtn();
public:
	afx_msg void OnBnClickedStopanimBtn();
};
