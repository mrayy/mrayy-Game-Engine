

#include "stdafx.h"
#include "MyFrame.h"
#include "wxRenderWindow.h"

using namespace mray;


const long idMenuQuit=wxNewId();

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
	EVT_MENU(idMenuQuit,OnQuit)
END_EVENT_TABLE();

MyFrame::MyFrame(wxFrame* frame,const wxString& title):
wxFrame(frame,-1,title)
{
	m_statusBar=new wxStatusBar();

	CreateStatusBar(2);


	wxPanel* m_panel2;
	wxButton* m_button4;
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxBoxSizer* bSizer3;
		bSizer3 = new wxBoxSizer( wxVERTICAL );

		wxPanel*m_panel1 = new wxPanel( this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0 );
		m_panel1->SetSize(-1,50);
		bSizer3->Add( m_panel1, 0, wxALL | wxEXPAND, 5 );

		m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
		m_panel2->SetSize(-1,50);
		bSizer3->Add( m_panel2, 0, wxALL|wxEXPAND, 5 );

		m_button4 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
		bSizer3->Add( m_button4, 0, wxALL, 5 );

		this->SetSizer( bSizer3 );
		this->Layout();


		/*
		s_frameManager.SetFrame(this);
		wxTextCtrl * t1=new wxTextCtrl(this,-1,"Text1",wxDefaultPosition,wxSize(200,150));
		wxTextCtrl * t2=new wxTextCtrl(this,-1,"Text2",wxDefaultPosition,wxSize(200,150));
		wxTextCtrl * t3=new wxTextCtrl(this,-1,"Text3",wxDefaultPosition,wxSize(200,150));
		s_frameManager.AddPane(t1);
		s_frameManager.AddPane(t2);
		s_frameManager.AddPane(t3);
		s_frameManager.Update();*/
		Show();

}
MyFrame::~MyFrame()
{
}

void MyFrame::UpdateEngine()
{
	//m_engine->update();
}

void MyFrame::OnQuit(wxCommandEvent&e)
{
	Close();
}

