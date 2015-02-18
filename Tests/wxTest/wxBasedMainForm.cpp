
#include "stdafx.h"
#include "wxBasedMainForm.h"
#include "EventMemoryManager.h"
#include "EventQueue.h"
#include "MyRenderWindow.h"
#include "ModelRenderWindow.h"
#include "GameRenderWindow.h"

#include "GUIButton.h"


BEGIN_EVENT_TABLE(wxBasedMainForm,MainForm)

	EVT_KEY_DOWN(OnKeyDown)
	EVT_KEY_UP(OnKeyUp)
END_EVENT_TABLE()


wxBasedMainForm::wxBasedMainForm( wxWindow* parent )
:
MainForm( parent )
{
	wxPanel* pan=new wxPanel(this);
	m_szRenderWindow->Add(pan,1,wxALL | wxEXPAND);
	m_auimanager=new wxAuiManager(pan);
	ModelRenderWindow*r2;
	m_renderWindow=r2=new mray::ModelRenderWindow("sportive03_m_mediumpoly.mdx",this,-1,wxDefaultPosition,wxDefaultSize,0);
	m_auimanager->AddPane(r2);

	ModelRenderWindow*r3;
	r3=new mray::ModelRenderWindow("sportive03_m_mediumpoly.mdx",this,-1,wxDefaultPosition,wxDefaultSize,0);
	//m_szRenderWindow->Add(r2,1,wxALL | wxEXPAND);
	m_auimanager->AddPane(r3);
	m_auimanager->Update();
	
	/*m_renderWindow=new mray::MyRenderWindow(this,-1,wxDefaultPosition,wxDefaultSize,0);
	m_szRenderWindow->Add(m_renderWindow,1,wxALL | wxEXPAND);
	*/
	
	InitPropGrid();

	m_propManager->SetObject(r3->GetCamera());

}


void wxBasedMainForm::InitPropGrid()
{
	int style = // default style
		wxPG_BOLD_MODIFIED |
		wxPG_SPLITTER_AUTO_CENTER |
		wxPG_AUTO_SORT |
		wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
		wxPG_TOOLTIPS |
		wxPG_LIMITED_EDITING |
		wxPG_TOOLBAR |
		wxPG_DESCRIPTION;
	int extraStyle = wxPG_EX_MODE_BUTTONS
		| wxPG_EX_MULTIPLE_SELECTION
		| wxPG_EX_ENABLE_TLP_TRACKING
		| wxPG_EX_UNFOCUS_ON_ENTER;
	m_propManager=new wxMRAYPropGrid(this,-1,wxDefaultPosition,wxDefaultSize,style);

	m_szPropGrid->Add(m_propManager,1,wxEXPAND);


}

void wxBasedMainForm::OnNewFile( wxCommandEvent& event )
{
	// TODO: Implement OnNewFile
}

void wxBasedMainForm::OnOpenFile( wxCommandEvent& event )
{
	// TODO: Implement OnOpenFile
	wxFileDialog fd(this,"","","","",wxOPEN,wxDefaultPosition);
	fd.ShowModal();
}

void wxBasedMainForm::OnSaveFile( wxCommandEvent& event )
{
	// TODO: Implement OnSaveFile
}

void wxBasedMainForm::OnExit( wxCommandEvent& event )
{
	// TODO: Implement OnExit
	Close();
}

void wxBasedMainForm::OnKeyDown(wxKeyEvent& event)
{
	GCPtr<KeyboardEvent> evt=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
	evt->press=true;
	_FillKeyEvent(evt,event);
	EventQueue::getInstance().pushEvent(evt);

}
void wxBasedMainForm::OnKeyUp(wxKeyEvent& event)
{
	GCPtr<KeyboardEvent> evt=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
	evt->press=true;
	_FillKeyEvent(evt,event);
	EventQueue::getInstance().pushEvent(evt);
}
void wxBasedMainForm::_FillKeyEvent(KeyboardEvent* dst,wxKeyEvent& src)
{
	dst->alt=src.AltDown();
	dst->ctrl=src.ControlDown();
	dst->Char=src.GetKeyCode();
	dst->key=(EKEY_CODE)src.GetKeyCode();
}