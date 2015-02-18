

/********************************************************************
	created:	2012/11/12
	created:	12:11:2012   14:10
	filename: 	d:\Development\mrayEngine\Tests\WxTest\wxRenderWindow.h
	file path:	d:\Development\mrayEngine\Tests\WxTest
	file base:	wxRenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __wxRenderWindow__
#define __wxRenderWindow__

#include <RenderWindow.h>
#include <GUIBatchRenderer.h>
#include <IGUIManager.h>

namespace mray
{

	class Engine;

class wxRenderWindow:public wxPanel,public video::IRenderWindowListener
{
	DECLARE_CLASS(wxRenderWindow)
protected:
public:
	wxRenderWindow(wxFrame* parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, 
			  long style = 0,
              const wxString& name = wxControlNameStr);
	virtual~wxRenderWindow();
	
	

	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);

	virtual void DestroyWindow();

	virtual void Update(float dt){}

	scene::ViewPort* CreateViewPort(const core::string& name,const math::rectf& rc);

protected:
	DECLARE_EVENT_TABLE()
protected:
	video::RenderWindow* m_renderWindow;
	std::vector<scene::ViewPort*> m_viewPorts;
	wxTimer m_timer;

	virtual void OnCreated(wxWindowCreateEvent& e);
	virtual void OnDestroy(wxWindowDestroyEvent& e);
	virtual void OnPain(wxPaintEvent& e);
	virtual void OnSize(wxSizeEvent& e);
	virtual void OnRenderTimer(wxTimerEvent&e);
	virtual void OnMouseEvent(wxMouseEvent&e);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnKeyUp(wxKeyEvent& event);

	void _FillKeyEvent(KeyboardEvent* dst,wxKeyEvent& src);

	void Render();
};

}


#endif
