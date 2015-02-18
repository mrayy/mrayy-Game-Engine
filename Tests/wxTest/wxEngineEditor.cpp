///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxEngineEditor.h"

///////////////////////////////////////////////////////////////////////////

MainForm::MainForm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	bSizer4->SetMinSize( wxSize( 200,-1 ) ); 
	m_treeCtrl1 = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer4->Add( m_treeCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	bSizer10->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_szRenderWindow = new wxBoxSizer( wxVERTICAL );
	
	bSizer10->Add( m_szRenderWindow, 1, wxEXPAND, 5 );
	
	m_szPropGrid = new wxBoxSizer( wxVERTICAL );
	
	m_szPropGrid->SetMinSize( wxSize( 200,-1 ) ); 
	bSizer10->Add( m_szPropGrid, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer10 );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 4, wxST_SIZEGRIP, wxID_ANY );
	m_menubar2 = new wxMenuBar( 0 );
	m_mFile = new wxMenu();
	wxMenuItem* m_miNew;
	m_miNew = new wxMenuItem( m_mFile, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_miNew );
	
	wxMenuItem* m_miOpen;
	m_miOpen = new wxMenuItem( m_mFile, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_miOpen );
	
	wxMenuItem* m_miSave;
	m_miSave = new wxMenuItem( m_mFile, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_miSave );
	
	m_mFile->AppendSeparator();
	
	wxMenuItem* m_miExit;
	m_miExit = new wxMenuItem( m_mFile, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_miExit );
	
	m_menubar2->Append( m_mFile, wxT("File") );
	
	m_mEdit = new wxMenu();
	wxMenuItem* m_miUndo;
	m_miUndo = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Undo") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miUndo );
	
	wxMenuItem* m_miRedo;
	m_miRedo = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Redo") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miRedo );
	
	m_mEdit->AppendSeparator();
	
	wxMenuItem* m_miDuplicate;
	m_miDuplicate = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Duplicate") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miDuplicate );
	
	wxMenuItem* m_miCopy;
	m_miCopy = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Copy") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miCopy );
	
	wxMenuItem* m_miCut;
	m_miCut = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Cut") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miCut );
	
	wxMenuItem* m_miPaste;
	m_miPaste = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Paste") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miPaste );
	
	wxMenuItem* m_miDelete;
	m_miDelete = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Delete") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miDelete );
	
	m_mEdit->AppendSeparator();
	
	wxMenuItem* m_miFind;
	m_miFind = new wxMenuItem( m_mEdit, wxID_ANY, wxString( wxT("Find") ) , wxEmptyString, wxITEM_NORMAL );
	m_mEdit->Append( m_miFind );
	
	m_menubar2->Append( m_mEdit, wxT("Edit") );
	
	m_mTools = new wxMenu();
	m_menubar2->Append( m_mTools, wxT("Tools") );
	
	m_mHelp = new wxMenu();
	m_menubar2->Append( m_mHelp, wxT("Help") );
	
	this->SetMenuBar( m_menubar2 );
	
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_toolBar1->SetToolBitmapSize( wxSize( 22,22 ) );
/*	m_toolBar1->AddTool( wxID_ANY, wxT("New"), wxBITMAP( new ), wxBITMAP( new ), wxITEM_NORMAL, wxEmptyString, wxEmptyString );

	m_toolBar1->InsertTool(0,-1,wxBitmap( wxT("icons/new.png"), wxBITMAP_TYPE_PNG ));
	
	m_toolBar1->AddTool( wxID_ANY, wxT("Open"), wxBitmap( wxT("icons/open.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_ANY, wxT("Save"), wxBitmap( wxT("icons/save.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddSeparator();
	m_toolBar1->AddTool( wxID_ANY, wxT("Undo"), wxBitmap( wxT("icons/undo.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_ANY, wxT("Redo"), wxBitmap( wxT("icons/redo.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddSeparator();
	m_toolBar1->AddTool( wxID_ANY, wxT("Select"), wxBitmap( wxT("icons/stretch.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_ANY, wxT("Translate"), wxBitmap( wxT("icons/stretch.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_ANY, wxT("Scale"), wxBitmap( wxT("icons/stretch.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddSeparator();
	m_toolBar1->AddTool( wxID_ANY, wxT("Material Editor"), wxBitmap( wxT("icons/stretch.png"), wxBITMAP_TYPE_PNG ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );

	*/
	m_toolBar1->Realize();
	
	
	// Connect Events
	this->Connect( m_miNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnNewFile ) );
	this->Connect( m_miOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnOpenFile ) );
	this->Connect( m_miSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnSaveFile ) );
	this->Connect( m_miExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnExit ) );
}

MainForm::~MainForm()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnNewFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnOpenFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnSaveFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainForm::OnExit ) );
}

MyDialog1::MyDialog1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
}

MyDialog1::~MyDialog1()
{
}
