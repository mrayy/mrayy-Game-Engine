///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxEngineEditor__
#define __wxEngineEditor__

#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainForm
///////////////////////////////////////////////////////////////////////////////
class MainForm : public wxFrame 
{
	private:
	
	protected:
		wxTreeCtrl* m_treeCtrl1;
		wxBoxSizer* m_szRenderWindow;
		wxBoxSizer* m_szPropGrid;
		wxStatusBar* m_statusBar2;
		wxMenuBar* m_menubar2;
		wxMenu* m_mFile;
		wxMenu* m_mEdit;
		wxMenu* m_mTools;
		wxMenu* m_mHelp;
		wxToolBar* m_toolBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNewFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOpenFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSaveFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1065,678 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainForm();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyDialog1
///////////////////////////////////////////////////////////////////////////////
class MyDialog1 : public wxDialog 
{
	private:
	
	protected:
	
	public:
		MyDialog1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 505,360 ), long style = wxDEFAULT_DIALOG_STYLE );
		~MyDialog1();
	
};

#endif //__wxEngineEditor__
