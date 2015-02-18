

#include "StdAfx.h"
#include "ApplicationMenuState.h"
#include "TBAppGlobals.h"
#include "Application.h"
#include "GUIThemeManager.h"
#include "IGUIPanel.h"
#include "IGUIButton.h"

#include "GUIMainMenu.h"
#include "GUIOverlay.h"


namespace mray
{
namespace TBee
{

ApplicationMenuState::ApplicationMenuState()
{

	ADD_DELEGATE(ApplicationMenuState,StartDemoEvent);
	m_guiManager=new GUI::GUIManager(TBAppGlobals::App->getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	m_mainMenu=new GUIMainMenu();

	GUI::GUIOverlay* overlay= GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIMainMenu.gui");

	GUI::IGUIPanel* root=(GUI::IGUIPanel*)m_guiManager->CreateElement(GUI::IGUIPanel::ElementType);
	root->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(root);

	overlay->CreateElements(m_guiManager,root,this,m_mainMenu);

}
ApplicationMenuState::~ApplicationMenuState()
{
}

void ApplicationMenuState::StartDemoEvent(IObjectPtr p1,mray::PVOID param)
{
	m_exitCode=1;
}

void ApplicationMenuState::LoadScenarioFile(const core::string& fname)
{
}

void ApplicationMenuState::OnEvent(Event* e)
{
	m_guiManager->OnEvent(e);
}
void ApplicationMenuState::OnEnter(IApplicationState* prev)
{
}
void ApplicationMenuState::OnExit()
{
}

void ApplicationMenuState::OnUpdate(float dt)
{
	m_guiManager->Update(dt);
}
void ApplicationMenuState::OnDraw(const math::rectf& rc,video::IRenderTarget* rt)
{

	Engine::getInstance().getDevice()->setRenderTarget(rt);
	m_guiManager->DrawAll(rt);
}

}
}


