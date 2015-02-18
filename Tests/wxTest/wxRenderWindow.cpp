

#include "stdafx.h"
#include "wxRenderWindow.h"
#include "Engine.h"
#include "OptionContainer.h"
#include "RenderWindow.h"
#include "Viewport.h"

using namespace mray;

const long ID_RENDER_TIMER=wxNewId();

IMPLEMENT_CLASS(wxRenderWindow,wxControl);

BEGIN_EVENT_TABLE(wxRenderWindow,wxControl)
	EVT_WINDOW_DESTROY(OnDestroy)
	EVT_WINDOW_CREATE(OnCreated)
	EVT_SIZE(OnSize)
	EVT_TIMER(ID_RENDER_TIMER,OnRenderTimer)
	EVT_MOUSE_EVENTS(OnMouseEvent)
END_EVENT_TABLE();


wxRenderWindow::wxRenderWindow(wxFrame* parent,
              wxWindowID id,
              const wxPoint& pos ,
              const wxSize& size , 
			  long style ,
              const wxString& name )
:wxPanel(parent,id,pos,size,style,name),
m_timer(this,ID_RENDER_TIMER)
{
	OptionContainer devParams;

	devParams[mT("title")].value="";
	devParams[mT("left")].value=mT("-1");
	devParams[mT("top")].value=mT("-1");
	math::vector2di sz;
	GetSize(&sz.x,&sz.y);
	sz=sz/2;

	m_renderWindow=(Engine::getInstance().getDevice()->CreateRenderWindow(mT("MainWindow"),sz,false,devParams,(ulong)GetHandle()));
	m_renderWindow->AddListener(this);

// 	CreateViewPort("TL",math::rectf(0  ,0  ,0.5,0.5))->SetClearColor(video::SColor(1,0,0,1));
// 	CreateViewPort("BL",math::rectf(0  ,0.5,0.5,1  ))->SetClearColor(video::SColor(0,1,0,1));
//  	CreateViewPort("TR",math::rectf(0.5,0  ,1  ,0.5))->SetClearColor(video::SColor(0,0,1,1));
 	CreateViewPort("Main",math::rectf(0,0,1  ,1  ))->SetClearColor(video::SColor(1,1,1,1));
	m_timer.Start(20);
}

scene::ViewPort* wxRenderWindow::CreateViewPort(const core::string& name,const math::rectf& rc)
{
	scene::ViewPort* vp=m_renderWindow->CreateViewport(name,0,0,rc,0);
	m_viewPorts.push_back(vp);
	return vp;
}

wxRenderWindow::~wxRenderWindow()
{
}

void wxRenderWindow::WindowPostRender(video::RenderWindow* wnd)
{

}
void wxRenderWindow::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
{
}

void wxRenderWindow::Render()
{
	m_renderWindow->Render(true);
}
void wxRenderWindow::DestroyWindow()
{
	delete m_renderWindow;
}

void wxRenderWindow::OnPain(wxPaintEvent& e)
{
	
}
void wxRenderWindow::OnSize(wxSizeEvent& e)
{
	m_renderWindow->OnMoveResize();
	GCPtr<ResizeEvent> evt=EventMemoryManager::getInstance().createEvent(ET_ResizeEvent);
	evt->width=e.GetSize().x;
	evt->height=e.GetSize().y;
	evt->bits=32;
	EventQueue::getInstance().pushEvent(evt);
}
void wxRenderWindow::OnRenderTimer(wxTimerEvent&e)
{
	Render();
	Update(e.GetInterval()*0.001f);
}

void wxRenderWindow::OnMouseEvent(wxMouseEvent&e)
{
	GCPtr<MouseEvent> evt=EventMemoryManager::getInstance().createEvent(ET_Mouse);
	evt->pos.x=e.GetX();
	evt->pos.y=e.GetY();
	if(e.ButtonDown(wxMOUSE_BTN_LEFT))
		evt->event= MET_LEFTDOWN ;
	else if(e.ButtonUp(wxMOUSE_BTN_LEFT))
		evt->event= MET_LEFTUP;
	else if(e.ButtonDown(wxMOUSE_BTN_RIGHT))
		evt->event= MET_RIGHTDOWN;
	else if(e.ButtonUp(wxMOUSE_BTN_RIGHT))
		evt->event= MET_RIGHTUP;
	else evt->event=MET_MOVED;
	EventQueue::getInstance().pushEvent(evt);
}
void wxRenderWindow::OnDestroy(wxWindowDestroyEvent& e)
{
}
void wxRenderWindow::OnCreated(wxWindowCreateEvent& e)
{

}
void wxRenderWindow::OnKeyDown(wxKeyEvent& event)
{
	GCPtr<KeyboardEvent> evt=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
	evt->press=true;
	_FillKeyEvent(evt,event);
	EventQueue::getInstance().pushEvent(evt);

}
 void wxRenderWindow::OnKeyUp(wxKeyEvent& event)
 {
	 GCPtr<KeyboardEvent> evt=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
	 evt->press=true;
	 _FillKeyEvent(evt,event);
	 EventQueue::getInstance().pushEvent(evt);
 }
 void wxRenderWindow::_FillKeyEvent(KeyboardEvent* dst,wxKeyEvent& src)
 {
	 dst->alt=src.AltDown();
	 dst->ctrl=src.ControlDown();
	 dst->Char=src.GetKeyCode();
	 dst->key=(EKEY_CODE)src.GetKeyCode();
 }