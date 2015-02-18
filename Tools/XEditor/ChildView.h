// ChildView.h : interface of the CChildView class
//


#pragma once

#include "CEngineWindow.h"
//#include "MaterialPropertySheet.h"
#include "EnvironmentSettingsDlg.h"
#include "TerrainToolbox.h"
#include "MaterialEditor.h"
#include "NavMeshToolbox.h"
#include "EntityDialog.h"
#include "AnimationDialog.h"
#include "LoadingDialog.h"

// CChildView window
using namespace mray;

class CChildView : public CWnd
{

private:
	mray::CEngineWindow m_engine;
	bool m_isInited;
public:
	
	CMaterialEditor m_materialEditor;
	TerrainToolbox m_terrainToolbox;
	EnvironmentSettingsDlg m_environmentSettings;
	CNavMeshToolbox m_navMeshToolbox;
	CEntityDialog m_entDialog;
	CAnimationDialog m_animDialog;

	LoadingDialog m_loadingDialog;
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
	void CreateEngine();

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Implementation
public:
	virtual ~CChildView();

	mray::CEngineWindow* GetEngine(){return &m_engine;}

protected:

	void OnFileNew();
	void OnFileLoadScene();
	void OnFileSaveScene();
	void OnFileLoadModel();
	void OnFileSaveModel();
	void OnFileAddSearchPath();
	void OnFileLevelInfo();

	void OnToolsEntity();
	void OnToolsAnimation();

	void OnSettingGameMode();

protected:
	void OnTBSelect();
	void OnTBTranslate();
	void OnTBRotate();
	void OnTBScale();
	void OnUpdateTBSelect(CCmdUI* pCmdUI);
	void OnUpdateTBTranslate(CCmdUI* pCmdUI);
	void OnUpdateTBRotate(CCmdUI* pCmdUI);
	void OnUpdateTBScale(CCmdUI* pCmdUI);
	void OnUpdateStgGameMode(CCmdUI* pCmdUI);

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);


	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};
