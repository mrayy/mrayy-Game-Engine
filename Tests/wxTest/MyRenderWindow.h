

/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   17:24
	filename: 	D:\Development\mrayEngine\Tests\wxTest\MyRenderWindow.h
	file path:	D:\Development\mrayEngine\Tests\wxTest
	file base:	MyRenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _MyRenderWindow_
#define _MyRenderWindow_

#include "wxRenderWindow.h"

namespace mray
{

class MyRenderWindow:public wxRenderWindow,public IEventReciver
{
protected:
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<GUI::IGUIManager> m_guiManager;
public:

	MyRenderWindow(wxFrame* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, 
		long style = 0,
		const wxString& name = wxControlNameStr);
	virtual~MyRenderWindow();
	
	virtual void onEvent(Event* e);
	
	virtual void WindowPostRender(video::RenderWindow* wnd){}
	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);

	virtual void DestroyWindow();
};


}

#endif
