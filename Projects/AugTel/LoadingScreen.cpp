
#include "stdafx.h"
#include "LoadingScreen.h"
#include "VideoRenderElement.h"
#include "GUIManager.h"
#include "IGUIPanelElement.h"
#include "GUIThemeManager.h"
#include "GUIOverlayManager.h"
#include "GUIOverlay.h"

namespace mray
{
namespace AugTel
{


LoadingScreen::LoadingScreen()
{
	m_guiManager = new GUI::GUIManager(gEngine.getDevice());

	m_video = new VideoRenderElement();
}

LoadingScreen::~LoadingScreen()
{
	delete m_video;
	delete m_guiManager;
}


void LoadingScreen::Init()
{
	m_video->LoadVideo("NeonWormhole.ogv");


	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
	m_guiroot = (GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""), m_guiManager);
	m_guiroot->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(m_guiroot);

	GUI::GUIOverlay* screenOverlay = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUILoadingScreenLayout.gui");
	m_screenLayout = new GUI::GUILoadingScreen(m_guiManager);
	screenOverlay->CreateElements(m_guiManager, m_guiroot, 0, m_screenLayout);
	m_video->SetVisible(true);

}


void LoadingScreen::Start()
{
	m_video->Start();
	m_video->SetVisible(true);

	//m_screenLayout->SetInfo("");
	m_screenLayout->SetInfo("Location: Living Lab Tokyo");
	m_screenLayout->SetText("Connecting Robot", true);
	math::vector2d pos = m_screenLayout->Status->GetPosition();
	pos.x = 100;
	m_screenLayout->Status->SetPosition(pos);
	m_done = false;

	m_startTime = gEngine.getTimer()->getSeconds();

}


void LoadingScreen::Draw(const math::rectf& rc)
{

	m_video->Render(rc, 1);/*
	if (!m_video->IsVisible())
	{
		gEngine.getDevice()->draw2DRectangle(rc, video::SColor(0, 0, 0, 1));
	}*/
	m_guiManager->DrawAll(&rc);
}


void LoadingScreen::Update(float dt)
{
	m_video->Update(dt);
	m_guiManager->Update(dt);
	/*
	float t=m_video->GetTime();
	if (t >8)
	{
		m_done = true;
		m_video->SetVisible(false);
		math::vector2d pos=m_screenLayout->Status->GetPosition();
		pos.x = 0;
		//m_screenLayout->Status->SetPosition(pos);
		//m_screenLayout->SetText("Connected",false);

	}*/
}


void LoadingScreen::End()
{
	m_video->Stop();
}


bool LoadingScreen::IsDone()
{
	float dt = gEngine.getTimer()->getSeconds() - m_startTime;
	//return true;
	return m_done || m_video->IsDone() || dt>4000;
}



}
}



