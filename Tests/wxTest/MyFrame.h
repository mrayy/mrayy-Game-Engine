

/********************************************************************
	created:	2012/11/12
	created:	12:11:2012   14:21
	filename: 	d:\Development\mrayEngine\Tests\WxTest\MyFrame.h
	file path:	d:\Development\mrayEngine\Tests\WxTest
	file base:	MyFrame
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MyFrame__
#define __MyFrame__



namespace mray
{
	class wxRenderWindow;

class MyFrame:public wxFrame
{
protected:

	wxStatusBar *m_statusBar;
public:
	MyFrame(wxFrame* frame,const wxString& title);
	virtual~MyFrame();

	void UpdateEngine();

private:
	void OnQuit(wxCommandEvent&e);

	DECLARE_EVENT_TABLE();
};

}


#endif

