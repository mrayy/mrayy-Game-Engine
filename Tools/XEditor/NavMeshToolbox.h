#pragma once
#include "afxwin.h"
#include "IToolBox.h"
#include "AIRenderDevice.h"
#include "NavMesh.h"

using namespace mray;

// CNavMeshToolbox dialog
 
namespace mray
{
	class CEngineWindow;
}
class CNavMeshToolbox : public CDialog,public IToolBox
{
	DECLARE_DYNAMIC(CNavMeshToolbox)

	GCPtr<AI::NavMesh> m_navMesh;
	mray::AI::AIRenderDevice* m_renderDev;

	int m_selectedMesh;
	mray::CEngineWindow* m_editor;
	void OnMeshSelected(int i);
	core::array<int> m_navAttrs;
protected:
	void OnModelSelected(const mray::PVOID&);
	void OnEngineInitiated(const mray::PVOID&);

	void OnSetNavMesh();
public:
	CNavMeshToolbox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNavMeshToolbox();

// Dialog Data
	enum { IDD = IDD_NAVIGMESH_TOOLBOX };

	void SetEditor(mray::CEngineWindow*e);

	virtual void OnExportLevelXML(xml::XMLElement*e);
	virtual bool OnImportLevelXML(xml::XMLElement*e);

	virtual void ShowToolBox(bool s);
	virtual bool IsToolBoxVisible();


	virtual void OnRender();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGenerateNavmeshBtn();
public:
	CComboBox SubMeshList;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedImportNavmeshBtn();
public:
	afx_msg void OnBnClickedExportNavmeshBtn();
public:
	afx_msg void OnBnClickedImportNavmeshAttrsBtn();
public:
	float MaxStepSize;
public:
	float CellSize;
public:
	float MaxSlope;
public:
	afx_msg void OnBnClickedLoadAttributesBtn();
};
