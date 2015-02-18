

#include "stdafx.h"
#include "DeviceWidget.h"

#include "Engine.h"
#include "OptionContainer.h"
#include "mrayDev.h"
#include "TextureResourceManager.h"

namespace mray
{

	BEGIN_EVENT_TABLE(DeviceWidget, wxWindow)
		EVT_PAINT(DeviceWidget::OnPaint)
		EVT_SIZE(DeviceWidget::OnResize)
		EVT_MOUSE_EVENTS(DeviceWidget::OnMouse)
	END_EVENT_TABLE()


DrawTimer::DrawTimer(DeviceWidget* owner)
:m_owner(owner)
{
}
void DrawTimer::Notify()
{
	m_owner->Refresh(false);
}

DeviceWidget::DeviceWidget(const core::string& name,wxWindow *parent,const wxPoint& pos,const wxSize& sz)
:wxWindow(parent,-1), m_timer(this),m_name(name),m_selecting(false)
{
	m_window=0;
	/*
	DWORD style=wxFULL_REPAINT_ON_RESIZE;
	CreateBase(parent,0,pos,sz,style,wxDefaultValidator, "");
	parent->AddChild(this);
	WXDWORD exStyle = 0;
	DWORD msflags = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	msflags |= MSWGetStyle(style, &exStyle);
	msflags=WS_CHILD | WS_VISIBLE;

	if ( !MSWCreate(wxApp::GetRegisteredClassName(wxT("MRAYEngine_Canvas"), -1, CS_OWNDC),
		NULL, wxPoint(0,0), sz, msflags, exStyle) )
		return ;
*/

	m_hDC = ::GetDC(GetHWND());

	OptionContainer params;
	params[mT("FSAA")].value="4";

	video::mrayDev* dev=(video::mrayDev*)Engine::getInstance().getDevice().pointer();
	m_window=dev->CreateRenderWindow(m_name,math::vector2d(GetClientSize().x,GetClientSize().y),false,params,(ulong)GetHandle());
	m_viewport=m_window->CreateViewport(mT("Viewport"),0,0,math::rectf(0,0,1,1),1);

	dev->setClearColor(video::SColor(1,0,0,1));

	m_window->AddListener(this);

	

	static bool inited=false;
	if(!inited)
	{
		inited=true;
		//dev->InitializeContext(m_window);
	}

	startpos.x=math::Randomizer::randRange(0,100);
	startpos.y=math::Randomizer::randRange(0,100);
	m_angle=math::Randomizer::randRange(0,100);

	m_texture=new video::TextureUnit();
	m_texture->SetTexture(gTextureResourceManager.loadTexture2D(mT("Japanese_Butterfly_and_Bonsai.jpg")));

	m_timer.Start(0);

	OnResize(wxSizeEvent());

}
DeviceWidget::~DeviceWidget()
{
}

void DeviceWidget::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
{
	video::IVideoDevice* device=Engine::getInstance().getDevice();
	device->begin();
	device->set2DMode();
	device->useTexture(0,m_texture);
	device->draw2DImageRot(math::rectf(startpos.x,startpos.y,startpos.x+80,startpos.y+80),
		40,video::SColor(1,1,1,1),m_angle);
	device->end();

	m_angle+=100*gFPS.dt();
}

void DeviceWidget::OnPaint(wxPaintEvent& (event))
{
	// This is required even though dc is not used otherwise.
	//wxPaintDC dc(this);
	//dc.Clear();
	if(m_window)
		m_window->Render(true);;

	if(m_selecting)
	{
		wxScreenDC dc;

		dc.SetLogicalFunction(wxINVERT);
		dc.SetPen(wxPen(*wxBLACK, 2, wxSOLID));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);

		wxPoint newFrom(m_selectRC.ULPoint.x,m_selectRC.ULPoint.y);
		wxPoint newTo(m_selectRC.BRPoint.x,m_selectRC.BRPoint.y);

		newFrom = ClientToScreen(newFrom);
		newTo = ClientToScreen(newTo);

		wxSize rectSize(newTo.x - newFrom.x, newTo.y - newFrom.y);

		dc.DrawRectangle(newFrom, rectSize);

		SetCursor(wxCursor(wxCURSOR_SIZING));
	}
	
}
void DeviceWidget::OnResize(wxSizeEvent& event)
{
	wxSize sz;
	sz=GetSize();
	if(m_window)
	{
		m_window->Resize(math::vector2di(400));
		m_window->OnMoveResize();
	}
	Refresh();
	//if(m_device)
	//	m_device->setViewportRect(math::recti(0,0,event.GetSize().x,event.GetSize().y));

}
void DeviceWidget::OnMouse(wxMouseEvent& event)
{
	if(event.LeftDown())
	{
		m_selectRC.ULPoint.x=event.GetX();
		m_selectRC.ULPoint.y=event.GetY();
	}
	if(event.LeftIsDown())
	{
		m_selecting=true;
		m_selectRC.BRPoint.x=event.GetX();
		m_selectRC.BRPoint.y=event.GetY();

		startpos=m_selectRC.BRPoint;
	}else
	{
		m_selecting=false;
		SetCursor(*wxSTANDARD_CURSOR);
	}

	if(event.Entering())
	{
		m_timer.Start();
	}
	if(event.Leaving())
	{
		SetCursor(*wxSTANDARD_CURSOR);
		m_timer.Stop();
	}
}
void DeviceWidget::OnInit(wxInitDialogEvent& WXUNUSED(event))
{

}

}
