// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "XEditor.h"
#include "ChildView.h"
#include "LevelInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView():m_isInited(0)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()

	ON_COMMAND(ID_FILE_NEWSCENE,&OnFileNew)
	ON_COMMAND(ID_FILE_LOADSCENE,&OnFileLoadScene)
	ON_COMMAND(ID_FILE_SAVESCENE,&OnFileSaveScene)
	ON_COMMAND(ID_FILE_LOADMODEL,&OnFileLoadModel)
	ON_COMMAND(ID_FILE_SAVEMODEL,&OnFileSaveModel)
	ON_COMMAND(ID_FILE_ADDSEARCHPATH,&OnFileAddSearchPath)
	ON_COMMAND(ID_FILE_LEVELINFO,&OnFileLevelInfo)

	ON_COMMAND(ID_TB_NEWSCENE,&OnFileNew)
	ON_COMMAND(ID_TB_LOADSCENE,&OnFileLoadScene)
	ON_COMMAND(ID_TB_SAVESCENE,&OnFileSaveScene)

	ON_COMMAND(ID_TOOLS_ENTITYPROPERTIES, &OnToolsEntity)
	ON_COMMAND(ID_TOOLS_ANIMATION, &OnToolsAnimation)

	ON_COMMAND(ID_SETTINGS_GAMEMODE,OnSettingGameMode)

	ON_COMMAND(ID_TB_SELECT, &OnTBSelect)
	ON_COMMAND(ID_TB_TRANSLATE, &OnTBTranslate)
	ON_COMMAND(ID_TB_ROTATE, &OnTBRotate)
	ON_COMMAND(ID_TB_SCALE, &OnTBScale)


	ON_UPDATE_COMMAND_UI(ID_TB_SELECT, OnUpdateTBSelect)
	ON_UPDATE_COMMAND_UI(ID_TB_TRANSLATE, OnUpdateTBTranslate)
	ON_UPDATE_COMMAND_UI(ID_TB_ROTATE, OnUpdateTBRotate)
	ON_UPDATE_COMMAND_UI(ID_TB_SCALE, OnUpdateTBScale)

	ON_UPDATE_COMMAND_UI(ID_SETTINGS_GAMEMODE, OnUpdateStgGameMode)

END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	m_loadingDialog.Create(m_loadingDialog.IDD,0);
	m_loadingDialog.ShowWindow(SW_SHOW);
	m_loadingDialog.UpdateStatus(mT("Creating windows"));

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int res=CWnd::OnCreate(lpCreateStruct);


	m_materialEditor.Create(m_materialEditor.IDD,this);
	//m_materialEditor.ShowWindow(0);
	m_materialEditor.SetEditor(&m_engine);

	m_terrainToolbox.Create(m_terrainToolbox.IDD,this);
	m_terrainToolbox.ShowWindow(0);
	m_terrainToolbox.SetEditor(&m_engine);

	m_environmentSettings.Create(m_environmentSettings.IDD,this);
	//m_environmentSettings.Construct(m_environmentSettings.IDD);//,this);
	//m_environmentSettings.ShowWindow(0);
	m_environmentSettings.SetEditor(&m_engine);

	m_navMeshToolbox.Create(m_navMeshToolbox.IDD,this);
	m_navMeshToolbox.ShowWindow(0);
	m_navMeshToolbox.SetEditor(&m_engine);

	m_entDialog.Create(m_entDialog.IDD,this);
	m_entDialog.ShowWindow(0);
	m_entDialog.SetEditor(&m_engine);

	m_animDialog.Create(m_animDialog.IDD,this);
	m_animDialog.ShowWindow(0);
	m_animDialog.SetEditor(&m_engine);

	return res;
}

void CChildView::CreateEngine()
{
	CRect rect;
	GetWindowRect(rect);

	ScreenToClient(rect);


	m_loadingDialog.UpdateStatus(mT("Init Engine"));
	m_engine.Init(this,rect);
	m_engine.unpTimer=m_engine.SetTimer(1,1,0);
	m_isInited=true;

	m_loadingDialog.ShowWindow(0);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	if(!m_isInited)
		return;
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);

	m_engine.OnSize(rect);
}
void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.Ellipse(5,5,200,200);
	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	m_engine.OnKeyDown(nChar, nRepCnt,nFlags);
}
void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	m_engine.OnKeyUp(nChar, nRepCnt,nFlags);
}

//////////////////////////////////////////////////////////////////////////
// Menus
void CChildView::OnFileNew()
{
	m_engine.OnNewScene();
}
void CChildView::OnFileLoadScene()
{
	CFileDialog fd(true,0,0,OFN_OVERWRITEPROMPT,mT("Game level file *.level|*.level||"));

	int iRet=fd.DoModal();
	if(iRet==IDOK)
	{
		core::string file=fd.GetPathName().GetString();

		m_engine.OnLoadScene(file);
	}
}
void CChildView::OnFileSaveScene()
{
	CFileDialog fd(false,0,0,OFN_OVERWRITEPROMPT,mT("Game level file *.level|*.level||"));

	int iRet=fd.DoModal();
	if(iRet==IDOK)
	{
		core::string file=fd.GetPathName().GetString();

		m_engine.OnExportScene(file);
	}
}
void CChildView::OnFileLoadModel()
{
	// TODO: Add your control notification handler code here
	CString filter(mT("Model Desc Files (*.mdx)|*.mdx|Model Files (*.mdl)|*.mdl||"));
	CFileDialog openFileDlg(true,0,0,OFN_OVERWRITEPROMPT,filter);

	int iRet=openFileDlg.DoModal();

	if(iRet==IDOK)
	{
		core::string ext=openFileDlg.GetFileExt().GetString();
		m_engine.OnLoadModel(openFileDlg.GetPathName().GetString(),ext);
	}
}
void CChildView::OnFileSaveModel()
{
	if(!m_engine.GetSelectedNode())
		return;
	// TODO: Add your control notification handler code here
	CFileDialog fd(false,0,0,OFN_OVERWRITEPROMPT,mT("Model MDX file *.mdx|*.mdx||"));

	int iRet=fd.DoModal();
	if(iRet==IDOK)
	{
		GCPtr<OS::IStream> stream=gFileSystem.openFile(fd.GetPathName().GetString(),OS::TXT_WRITE);
		xml::XMLWriter writer(stream);
		xml::XMLElement e(mT(""));
		
		scene::SLODAnimatedMesh* mesh=dynamic_cast<scene::SLODAnimatedMesh*>(m_engine.GetSelectedNode());
		core::string sourceMode;
		if(mesh){
			sourceMode=mesh->GetModelFileName();
			mesh->SetModelFileName(mT(""));
		}
		xml::XMLElement*e2=m_engine.GetSelectedNode()->exportXMLSettings(&e);
		if(mesh){
			mesh->SetModelFileName(sourceMode);
		}
		//clear the attributes
		xml::xmlAttributesMapIT it= e2->getAttributesBegin();
		xml::xmlAttributesMapIT end=e2->getAttributesEnd();

		xml::xmlAttributesMapIT it2;
		for(;it!=end;){
			it2=it;
			it2++;
			if(!it->first.equals_ignore_case(mT("Type")))
				e2->removeAttribute(it->first);
			it=it2;
		}
		writer.addElement(e2);
		stream->close();
	}
}


void CChildView::OnFileAddSearchPath()
{
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("Select Directory"));
	core::string app=gFileSystem.getAppPath();
	app.replace('/','\\');
	dlg.SetSelPath(app.c_str());
	dlg.SetOptions(BIF_EDITBOX);
	
	
	if (dlg.DoModal() == IDOK)
	{
		core::string path=dlg.GetSelPath();
		path=gFileSystem.makePathRelative(path,app);
		m_engine.GetLevel()->GetGameGlobals()->AddSearchPath(path);
		gFileSystem.addPath(path);
	}
}
void CChildView::OnFileLevelInfo()
{
	CLevelInfoDlg dlg(&m_engine);
	dlg.DoModal();
}


void CChildView::OnToolsEntity()
{
	m_entDialog.ShowWindow(SW_SHOW);
}
void CChildView::OnToolsAnimation()
{
	m_animDialog.ShowWindow(SW_SHOW);
}
void CChildView::OnSettingGameMode()
{
	m_engine.GetLevel()->SetEditorMode(!m_engine.GetLevel()->IsEditorMode());
}

//////////////////////////////////////////////////////////////////////////

void CChildView::OnTBSelect()
{
	m_engine.GetManipulator()->SetActiveManipulator(scene::TransformManipulator::EMT_None);
}
void CChildView::OnTBTranslate()
{
	m_engine.GetManipulator()->SetActiveManipulator(scene::TransformManipulator::EMT_Translate);
	EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
}
void CChildView::OnTBRotate()
{
	m_engine.GetManipulator()->SetActiveManipulator(scene::TransformManipulator::EMT_Rotate);
	EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
}
void CChildView::OnTBScale()
{
	m_engine.GetManipulator()->SetActiveManipulator(scene::TransformManipulator::EMT_Scale);
	EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
}
void CChildView::OnUpdateTBSelect(CCmdUI* pCmdUI)
{
	scene::TransformManipulator::EManipulatorType comp= m_engine.GetManipulator()->GetActiveManipulator();
	pCmdUI->SetCheck(comp==scene::TransformManipulator::EMT_None);
}
void CChildView::OnUpdateTBTranslate(CCmdUI* pCmdUI)
{
	scene::TransformManipulator::EManipulatorType comp= m_engine.GetManipulator()->GetActiveManipulator();
	pCmdUI->SetCheck(comp==scene::TransformManipulator::EMT_Translate);
}
void CChildView::OnUpdateTBRotate(CCmdUI* pCmdUI)
{
	scene::TransformManipulator::EManipulatorType comp= m_engine.GetManipulator()->GetActiveManipulator();
	pCmdUI->SetCheck(comp==scene::TransformManipulator::EMT_Rotate);
}
void CChildView::OnUpdateTBScale(CCmdUI* pCmdUI)
{
	scene::TransformManipulator::EManipulatorType comp= m_engine.GetManipulator()->GetActiveManipulator();
	pCmdUI->SetCheck(comp==scene::TransformManipulator::EMT_Scale);
}
void CChildView::OnUpdateStgGameMode(CCmdUI* pCmdUI)
{
	if(m_engine.GetLevel())
		pCmdUI->SetCheck(!m_engine.GetLevel()->IsEditorMode());
}



