
#include "stdafx.h"
#include "Application.h"
#include "DeviceWidget.h"
#include <wx/aui/auibook.h>

#include <wx/filesys.h>
#include "wx/treectrl.h"
#include <wx/fs_arc.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <wx/numdlg.h>
//#include <wx/wxscintilla.h>

#include "wxMRayProperties.h"
#include "JobPool.h"


namespace mray
{
	enum
	{
		UpdateTimer = wxID_HIGHEST + 1
	};

#define ID_NEW wxID_HIGHEST + 100
#define ID_OPEN wxID_HIGHEST + 101
#define ID_MERGE wxID_HIGHEST + 102
#define ID_CUT wxID_HIGHEST + 103
#define ID_COPY wxID_HIGHEST + 103
#define ID_PASTE wxID_HIGHEST + 104
#define ID_UNDO wxID_HIGHEST + 105
#define ID_REDO wxID_HIGHEST + 106
#define ID_ENTITY wxID_HIGHEST + 107
#define ID_LIGHT wxID_HIGHEST + 108
#define ID_NODE wxID_HIGHEST + 109
#define ID_CREATE_CHOICE wxID_HIGHEST + 110

#define ID_VIEW_TARGETTED_CAMERA_MODE wxID_HIGHEST + 200
#define ID_VIEW_FPS_CAMERA_MODE wxID_HIGHEST + 201
#define ID_VIEW_SELECTION_MODE wxID_HIGHEST + 202
#define ID_VIEW_EDIT_MODE wxID_HIGHEST + 204

#define PGID wxID_HIGHEST + 120

	// Define a new frame type
	class MyFrame : public wxFrame
	{
		wxToolBar* mMainToolbar;
		wxToolBar* mViewToolbar;
		wxToolBar* mEditToolbar;

		wxAuiManager* mAuiManager;
		wxImageList mTreeImageList;

		wxPropertyGridManager* m_pPropGridManager;
		wxPropertyGrid* m_grid;
		wxBoxSizer* m_topSizer;
		

		void CreatePG()
		{
			wxPropertyGridManager::RegisterAdditionalEditors();
			int style = // default style
				wxPG_BOLD_MODIFIED |
				wxPG_SPLITTER_AUTO_CENTER |
				wxPG_AUTO_SORT |
				//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
				//wxPG_TOOLTIPS |
				//wxPG_HIDE_CATEGORIES |
				//wxPG_LIMITED_EDITING |
				wxPG_TOOLBAR |
				wxPG_DESCRIPTION;
			int extraStyle = wxPG_EX_MODE_BUTTONS |
				wxPG_EX_MULTIPLE_SELECTION;
			

			 m_pPropGridManager =
				new wxPropertyGridManager(this,
				// Don't change this into wxID_ANY in the sample, or the
				// event handling will obviously be broken.
				PGID, /*wxID_ANY*/
				wxDefaultPosition,
				wxSize(100, 100), // FIXME: wxDefaultSize gives assertion in propgrid.
				// But calling SetInitialSize in manager changes the code
				// order to the grid gets created immediately, before SetExtraStyle
				// is called.
				style );
			 m_pPropGridManager->SetExtraStyle(extraStyle);
			 m_grid=m_pPropGridManager->GetGrid();
			 m_pPropGridManager->GetGrid()->SetVerticalSpacing( 2 );

			 m_topSizer = new wxBoxSizer ( wxVERTICAL );
			 m_topSizer->Add( m_pPropGridManager, 1, wxEXPAND );

			 wxPropertyGridPage* pg = m_pPropGridManager->AddPage(wxT("Standard Items"));
			 pg->Append( new wxPropertyCategory(wxT("Appearance"),wxPG_LABEL) );

			 pg->Append( new wxVector3Property(wxT("Position"),wxPG_LABEL,math::vector3df(1,2,3)) );
			 pg->Append( new wxVector2Property(wxT("Size"),wxPG_LABEL,math::vector2df(1,3)) );
			 pg->Append( new wxFontProperty(wxT("Font"),wxPG_LABEL) );
			 pg->SetPropertyHelpString ( wxT("Font"), wxT("Editing this will change font used in the property grid.") );

			 pg->Append( new wxColourProperty(wxT("Colour"),wxPG_LABEL));

			 mAuiManager->AddPane(m_pPropGridManager,wxAuiPaneInfo().
				 DefaultPane().Right().TopDockable(false).BottomDockable(false).CloseButton(false).Layer(1));
			 
		}

		void CreateToolbar()
		{

			mMainToolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, 
				wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
			mMainToolbar->SetToolBitmapSize(wxSize(16, 16));
			mMainToolbar->AddTool(ID_NEW, _("New Scene"), 
				wxBitmap( wxT("./icons/page_world.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddTool(ID_OPEN, _("Open Scene"), 
				wxBitmap( wxT("./icons/folder_page.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddTool(ID_MERGE, _("Merge Scene"), 
				wxBitmap( wxT("./icons/arrow_join.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddSeparator();
			mMainToolbar->AddTool(ID_CUT, _("Cut"), 
				wxBitmap( wxT("./icons/cut.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddTool(ID_COPY, _("Copy"), 
				wxBitmap( wxT("./icons/page_copy.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddTool(ID_PASTE, _("Paste"), 
				wxBitmap( wxT("./icons/page_paste.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddSeparator();
			mMainToolbar->AddTool(ID_UNDO, _("Undo"), 
				wxBitmap( wxT("./icons/arrow_turn_left.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->AddTool(ID_REDO, _("Redo"), 
				wxBitmap( wxT("./icons/arrow_turn_right.png"), wxBITMAP_TYPE_PNG), 
				wxNullBitmap, wxITEM_NORMAL, 
				wxEmptyString, wxEmptyString );
			mMainToolbar->Realize();

			mAuiManager->AddPane(mMainToolbar, wxAuiPaneInfo().
				Name(wxT("tb1")).Caption(wxT("Main Toolbar")).
				ToolbarPane().Top().LeftDockable(false).RightDockable(false));
		}
		void CreateNoteBook()
		{
			// Notebook
			// Tabs at bottom not implemented yet
			wxAuiNotebook* noteBook = new wxAuiNotebook(this, wxID_ANY);
			noteBook->SetWindowStyle(wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_MOVE);

			wxTreeCtrl* tree = new wxTreeCtrl(noteBook);
			mTreeImageList.Add(wxIcon(wxT("./icons/chart_organisation.png"), wxBITMAP_TYPE_PNG));
			mTreeImageList.Add(wxIcon(wxT("./icons/brick.png"), wxBITMAP_TYPE_PNG));
			mTreeImageList.Add(wxIcon(wxT("./icons/lightbulb.png"), wxBITMAP_TYPE_PNG));
			tree->SetImageList(&mTreeImageList);
			wxTreeItemId rootid = tree->AddRoot(wxT("SceneGraph"), 0);
			wxTreeItemId id  = tree->AppendItem(rootid, wxT("Node"), 0);
			tree->AppendItem(id, wxT("Ninja1"), 1);
			tree->AppendItem(id, wxT("Ninja2"), 1);
			tree->AppendItem(id, wxT("Ninja3"), 1);
			id  = tree->AppendItem(rootid, wxT("n2"), 0);
			tree->AppendItem(id, wxT("Light1"), 2);
			tree->ExpandAll();

			noteBook->AddPage(tree, wxEmptyString, true, wxBitmap(wxT("./icons/chart_organisation.png"), wxBITMAP_TYPE_PNG));
			noteBook->AddPage(new wxPanel(noteBook), wxEmptyString, false, wxBitmap(wxT("./icons/shape_square_edit.png"), wxBITMAP_TYPE_PNG));
			noteBook->AddPage(new wxPanel(noteBook), wxEmptyString, false, wxBitmap(wxT("./icons/page_lightning.png"), wxBITMAP_TYPE_PNG));
			mAuiManager->AddPane(noteBook, wxAuiPaneInfo().
				Name(wxT("scene")).Caption(wxT("Scene")).
				DefaultPane().Left().TopDockable(false).BottomDockable(false).
				CloseButton(false).Layer(1).MinSize(150, 100));
		}
	public:
		MyFrame(const wxChar *title, int xpos, int ypos, int width, int height): 
		  wxFrame((wxFrame *) NULL, -1, title,	wxPoint(xpos, ypos),
			  wxSize(width, height))
			,m_updateTimer(this,UpdateTimer)
		{
			m_updateTimer.Start(15 );
			// Initialise AUI
			mAuiManager = new wxAuiManager(this, wxAUI_MGR_DEFAULT);

			CreateToolBar();
			this->CreateStatusBar();
			CreateNoteBook();

			CreatePG();

			SetIcon(wxICON(sample));
			for(int i=0;i<2;++i)
			{
				wxPanel *panel = new wxPanel(this, wxID_ANY,wxPoint(0,0),wxSize(200,200));
				DeviceWidget *ew=new DeviceWidget(mT("Wnd")+core::StringConverter::toString(i),panel,wxPoint(100,100),wxSize(200,200));
				mAuiManager->AddPane(panel,wxAuiPaneInfo().
					Caption(wxT("Scene")).
					DefaultPane().Center().
					Layer(1).MinSize(150, 100));
			}



			wxPanel* xformPanel = new wxPanel();
			// Load from named resource
			wxXmlResource::Get()->LoadPanel(xformPanel, this, wxT("xformPanel"));
			mAuiManager->AddPane(xformPanel,wxAuiPaneInfo().
				Name(wxT("xform")).Caption(wxT("Transform")).
				DefaultPane().Left().TopDockable(false).BottomDockable(false).CloseButton(false).Layer(1));


			Show();
			mAuiManager->Update();

		}

		  ~MyFrame()
		  {
			  mAuiManager->UnInit();
			  delete mAuiManager;
		  }

	private:
		void OnClose(wxCommandEvent& event)
		{
			// true is to force the frame to close
			Close(true);
		}
		void Idle(wxIdleEvent& event)
		{
			//for(int i=0;i<m_widgets.size();++i)
				//m_widgets[i]->
		}

		void OnUpdateTimer(wxTimerEvent& WXUNUSED(event))
		{
			Engine::getInstance().updateEngine();
		}

		void OnPropertyGridChange( wxPropertyGridEvent& event )
		{
			wxPGProperty* property = event.GetProperty();

			const wxString& name = property->GetName();
			wxAny value = property->GetValue();

			// Don't handle 'unspecified' values
			if ( value.IsNull() )
				return;

			if(name==mT("Colour"))
			{
				wxColour cpv = wxANY_AS(value, wxColour);
			}
		}


		wxTimer m_updateTimer;

		DECLARE_EVENT_TABLE()
	};
	BEGIN_EVENT_TABLE(MyFrame, wxFrame)
		EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
		EVT_IDLE(MyFrame::Idle)
		EVT_TIMER(UpdateTimer, MyFrame::OnUpdateTimer)


		// This occurs when a property value changes
		EVT_PG_CHANGED( PGID, MyFrame::OnPropertyGridChange )
	END_EVENT_TABLE()


Application::Application()
{
}


bool Application::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxGIFHandler());


	// resource system
	wxFileSystem::AddHandler(new wxArchiveFSHandler()); 


	wxXmlResource::Get()->InitAllHandlers();

	wxXmlResource::Get()->Load(wxT("*.xrc"));

	new EventMemoryManager();
	
	Engine* e=new Engine(new OS::WinOSystem());

	JobPool::getInstance().SetupDefaultPools();
	

	e->loadResourceFile(mT("dataPath.stg"));
	e->loadPlugins(mT("pluginsU.stg"));

	m_device=Engine::getInstance().createDevice(mT("OpenGL"));

	int displayWidth, displayHeight;
	wxDisplaySize(&displayWidth, &displayHeight);
	new MyFrame(wxT("Top Frame"), 0, 0, 
		std::min(1024, displayWidth), std::min(768, displayHeight));

	return true;
}
int Application::OnExit()
{
	delete Engine::getInstancePtr();
	return wxApp::OnExit();
}

IMPLEMENT_APP(Application)


}