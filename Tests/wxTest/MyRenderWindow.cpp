

#include "stdafx.h"
#include "MyRenderWindow.h"

#include <GUIManager.h>
#include <IGUIPanel.h>
#include <GUIOverlay.h>
#include <GUIThemeManager.h>
#include <EventMemoryManager.h>
#include <FontResourceManager.h>
#include <EventQueue.h>
#include <MouseEvent.h>
#include <ViewPort.h>


namespace mray
{
MyRenderWindow::MyRenderWindow(wxFrame* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxControlNameStr */)
	:wxRenderWindow(parent,id,pos,size,style,name)
{
	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(Engine::getInstance().getDevice());
	{
		m_guiManager=new GUI::GUIManager(Engine::getInstance().getDevice());
		m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
		GUI::IGUIPanel* parent=(GUI::IGUIPanel*)m_guiManager->CreateElement(GUI::IGUIPanel::ElementType);
		parent->SetDocking(GUI::EED_Fill);
		m_guiManager->SetRootElement(parent);
		GUI::GUIOverlayManager::getInstance().LoadOverlay("overlays/overlay1.gui")->CreateElements(m_guiManager,parent,0,0);
	}


	EventQueue::getInstance().addEventHandler(this,BIT(ET_Mouse));
}
MyRenderWindow::~MyRenderWindow()
{

}

void MyRenderWindow::onEvent(Event* e)
{
	m_guiManager->OnEvent(e);
}

void MyRenderWindow::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
{
	m_guiManager->DrawAll(vp);
	GUI::IFont* font=gFontResourceManager.getDefaultFont();
	if(font)
	{
		m_guiRender->Prepare();
		GUI::FontAttributes attrs;
		attrs.hasShadow=true;
		attrs.fontSize=24;
		attrs.shadowOffset=2;
		font->print(math::rectf(10,10,100,100),&attrs,0,core::UTFString("Test Text"),m_guiRender);
		m_guiRender->Flush();
	}
}

void MyRenderWindow::DestroyWindow()
{
	m_guiRender=0;
}
}
