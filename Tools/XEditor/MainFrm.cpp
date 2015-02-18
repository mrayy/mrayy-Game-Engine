// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XEditor.h"

#include "MainFrm.h"
#include "PluginsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)

	ON_COMMAND(ID_TB_MATERIAL,&OnToolsMaterial)
	ON_COMMAND(ID_TB_TERRAIN,&OnToolsTerrain)

	ON_COMMAND(ID_TOOLS_MATERIALEDITOR,&OnToolsMaterial)
	ON_COMMAND(ID_TOOLS_ENVIRONMENT,&OnToolsEnvironment)
	ON_COMMAND(ID_TOOLS_TERRAINGENERATOR,&OnToolsTerrain)

	ON_COMMAND(ID_AI_NAVMESHGENERATOR,&OnAINavmesh)

	ON_COMMAND(ID_SETTINGS_PLUGINS,&OnSettingsPlugins)

	ON_EN_CHANGE(ID_XAXIS_STATUS_BAR, &OnStatusBarXAxisChange)
	ON_EN_CHANGE(ID_YAXIS_STATUS_BAR, &OnStatusBarYAxisChange)
	ON_EN_CHANGE(ID_ZAXIS_STATUS_BAR, &OnStatusBarZAxisChange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

	//XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	//bool res=XTPSkinManager()->LoadSkin(_T("Styles\\Office2007.cjstyles"));	
	//bool res=XTPSkinManager()->LoadSkin(_T("Styles\\Le5"), mT("NormalBlue.INI"));	
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
/*
	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockLeft);
	CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane1);

*/
	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
	//	m_paneManager.SetLayout(&layoutNormal);
	}
//	m_paneManager.ShowPane(IDR_PANE_OPTIONS);

	m_loggerDialog.Create(m_loggerDialog.IDD,this);

	CXTPDockingPane* pwndPane ;
	pwndPane=m_paneManager.CreatePane(
		IDD_ENVIRONMENT_SETTINGS_DLG, CRect(0, 0,120, 120), xtpPaneDockRight);

	pwndPane->SetTitle(mT("Environment"));

	pwndPane=m_paneManager.CreatePane(
		IDD_MATERIAL_EDITOR_DLG, CRect(0, 0,120, 120), xtpPaneDockRight);

	pwndPane->SetTitle(mT("Material"));

	pwndPane=m_paneManager.CreatePane(
		IDD_LOG_DIALOG, CRect(0, 0,120, 120), xtpPaneDockBottom);

	pwndPane->SetTitle(mT("Log"));

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDD_ENVIRONMENT_SETTINGS_DLG,IDD_MATERIAL_EDITOR_DLG};
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));


	//create status bar items
	InitStatusBar();

	m_wndView.CreateEngine();
	m_wndView.GetEngine()->getTextLogger()->SetEditBox(&m_loggerDialog.LogLines);

	EditorEventManager::getInstance().AddHandler(EEE_OnNodeTrasnformingDone,newClassDelegate1<CMainFrame,void,mray::PVOID>(mT(""),this,&CMainFrame::OnNodeTrasnformingDone));

	return 0;
}

void CMainFrame::InitStatusBar()
{
	int nIndex;

	CXTPStatusBarPane* pPane = m_wndStatusBar.AddIndicator(ID_STATUSBAR_LOGO, 0);
	pPane->SetText(_T("M-RAY Editor"));
	pPane->SetStyle(pPane->GetStyle() | SBPS_POPOUT);

	CXTPPaintManager::CNonClientMetrics ncm;
	ncm.lfMenuFont.lfWeight = FW_BOLD;
	pPane->SetTextFont(&ncm.lfMenuFont);

	pPane->SetTextColor(GetXtremeColor(COLOR_3DSHADOW));

	pPane->SetWidth(0); // Best Fit
	m_wndStatusBar.SetRibbonDividerIndex(1);
//////////////////////////////////////////////////////////////////////////
	// Create the edit control and add it to the status bar
	if (!m_wndXAxisEditCtrl.CreateEx(WS_EX_STATICEDGE, _T("edit"), _T("0"),
		WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), &m_wndStatusBar, ID_XAXIS_STATUS_BAR))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}
	m_wndXAxisEditCtrl.SetFont(m_wndStatusBar.GetFont());

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_XAXIS_STATUS_BAR, 1);

	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_XAXIS_STATUS_BAR);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 75);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndXAxisEditCtrl, ID_XAXIS_STATUS_BAR, FALSE);

	// Create the edit control and add it to the status bar
	if (!m_wndYAxisEditCtrl.CreateEx(WS_EX_STATICEDGE, _T("edit"), _T("0"),
		WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), &m_wndStatusBar, ID_YAXIS_STATUS_BAR))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}
	m_wndYAxisEditCtrl.SetFont(m_wndStatusBar.GetFont());

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_YAXIS_STATUS_BAR, 2);

	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_YAXIS_STATUS_BAR);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 75);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndYAxisEditCtrl, ID_YAXIS_STATUS_BAR, FALSE);

	if (!m_wndZAxisEditCtrl.CreateEx(WS_EX_STATICEDGE, _T("edit"), _T("0"),
		WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), &m_wndStatusBar, ID_ZAXIS_STATUS_BAR))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}
	m_wndZAxisEditCtrl.SetFont(m_wndStatusBar.GetFont());

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_ZAXIS_STATUS_BAR, 3);

	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_ZAXIS_STATUS_BAR);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 75);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndZAxisEditCtrl, ID_ZAXIS_STATUS_BAR, FALSE);

}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnClose()
{

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_PROPERTIES:
				{
					if (m_wndProperties.GetSafeHwnd() == 0)
					{
						m_wndProperties.Create(WS_CHILD|
							ES_AUTOVSCROLL|ES_MULTILINE,
							CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_wndProperties);
					break;
				}
			case IDR_PANE_OPTIONS:
				{
					if (m_wndOptions.GetSafeHwnd() == 0)
					{
						m_wndOptions.Create(_T("\n\nOptions"),
							WS_CHILD|WS_CLIPCHILDREN|
							WS_CLIPSIBLINGS|SS_CENTER,
							CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_wndOptions);
					break;
				}
			case IDD_ENVIRONMENT_SETTINGS_DLG:
				{
					pPane->Attach(&m_wndView.m_environmentSettings);
					break;
				}
			case IDD_MATERIAL_EDITOR_DLG:
				{
					pPane->Attach(&m_wndView.m_materialEditor);
					break;
				}
			case IDD_LOG_DIALOG:
				{
					pPane->Attach(&m_loggerDialog);
					break;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}



void CMainFrame::OnNodeTrasnformingDone(const mray::PVOID&)
{
	GCPtr<scene::TransformManipulator> manip=m_wndView.GetEngine()->GetManipulator();
	scene::IMovable* node= manip->GetNode();
	math::vector3d p;
	if(node){
		switch(manip->GetActiveManipulator())
		{
		case scene::TransformManipulator::EMT_Translate:
			{
				p=manip->GetNode()->getPosition();
			}break;
		case scene::TransformManipulator::EMT_Rotate:
			{
				manip->GetNode()->getOrintation().toEulerAngles(p);
			}break;
		case scene::TransformManipulator::EMT_Scale:
			{
				p=manip->GetNode()->getScale();
			}break;
		
		}
	}
	m_wndXAxisEditCtrl.SetWindowTextA(core::StringConverter::toString(p.x).c_str());
	m_wndYAxisEditCtrl.SetWindowTextA(core::StringConverter::toString(p.y).c_str());
	m_wndZAxisEditCtrl.SetWindowTextA(core::StringConverter::toString(p.z).c_str());
}

void CMainFrame::OnToolsMaterial()
{
	//	m_materialEditor.ShowToolBox(!m_materialEditor.IsToolBoxVisible());
	//	m_materialEditor.ShowWindow(SW_SHOW);
	m_paneManager.ShowPane(IDD_MATERIAL_EDITOR_DLG);
}

void CMainFrame::OnToolsEnvironment()
{
	//	m_environmentSettings.ShowWindow(TRUE);
	//m_environmentSettings.DoModal();
	m_paneManager.ShowPane(IDD_ENVIRONMENT_SETTINGS_DLG);
}
void CMainFrame::OnToolsTerrain()
{
	m_wndView.m_terrainToolbox.ShowToolBox(true);
}
void CMainFrame::OnAINavmesh()
{
	m_wndView.m_navMeshToolbox.ShowToolBox(true);
}


void CMainFrame::OnSettingsPlugins()
{
	CPluginsDialog dlg;
	dlg.SetEditor(m_wndView.GetEngine());
	dlg.DoModal();
}

void CMainFrame::OnStatusBarXAxisChange()
{
	CString str;
	m_wndXAxisEditCtrl.GetWindowText(str);
	float v=core::StringConverter::toFloat(str.GetString());
	GCPtr<scene::TransformManipulator> manip=m_wndView.GetEngine()->GetManipulator();
	if(manip->GetNode())
	{
		switch(manip->GetActiveManipulator())
		{
		case scene::TransformManipulator::EMT_Translate:
			{
				math::vector3d p=manip->GetNode()->getPosition();
				p.x=v;
				manip->GetNode()->setPosition(p);
			}break;
		case scene::TransformManipulator::EMT_Rotate:
			{
				math::vector3d p;
				manip->GetNode()->getOrintation().toEulerAngles(p);
				p.x=v;
				manip->GetNode()->setOrintation(p);
			}break;
		case scene::TransformManipulator::EMT_Scale:
			{
				math::vector3d p=manip->GetNode()->getScale();
				p.x=v;
				manip->GetNode()->setScale(p);
			}break;
		}
	}
}
void CMainFrame::OnStatusBarYAxisChange()
{
	CString str;
	m_wndYAxisEditCtrl.GetWindowText(str);
	float v=core::StringConverter::toFloat(str.GetString());
	GCPtr<scene::TransformManipulator> manip=m_wndView.GetEngine()->GetManipulator();
	if(manip->GetNode())
	{
		switch(manip->GetActiveManipulator())
		{
		case scene::TransformManipulator::EMT_Translate:
			{
				math::vector3d p=manip->GetNode()->getPosition();
				p.y=v;
				manip->GetNode()->setPosition(p);
			}break;
		case scene::TransformManipulator::EMT_Rotate:
			{
				math::vector3d p;
				manip->GetNode()->getOrintation().toEulerAngles(p);
				p.y=v;
				manip->GetNode()->setOrintation(p);
			}break;
		case scene::TransformManipulator::EMT_Scale:
			{
				math::vector3d p=manip->GetNode()->getScale();
				p.y=v;
				manip->GetNode()->setScale(p);
			}break;
		}
	}
}
void CMainFrame::OnStatusBarZAxisChange()
{
	CString str;
	m_wndZAxisEditCtrl.GetWindowText(str);
	float v=core::StringConverter::toFloat(str.GetString());
	GCPtr<scene::TransformManipulator> manip=m_wndView.GetEngine()->GetManipulator();
	if(manip->GetNode())
	{
		switch(manip->GetActiveManipulator())
		{
		case scene::TransformManipulator::EMT_Translate:
			{
				math::vector3d p=manip->GetNode()->getPosition();
				p.z=v;
				manip->GetNode()->setPosition(p);
			}break;
		case scene::TransformManipulator::EMT_Rotate:
			{
				math::vector3d p;
				manip->GetNode()->getOrintation().toEulerAngles(p);
				p.z=v;
				manip->GetNode()->setOrintation(p);
			}break;
		case scene::TransformManipulator::EMT_Scale:
			{
				math::vector3d p=manip->GetNode()->getScale();
				p.z=v;
				manip->GetNode()->setScale(p);
			}break;
		}
	}
}