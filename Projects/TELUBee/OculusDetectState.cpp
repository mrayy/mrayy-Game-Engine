

#include "stdafx.h"
#include "OculusDetectState.h"
#include "GUIManager.h"
#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "GUIThemeManager.h"
#include "Application.h"
#include "TBAppGlobals.h"

namespace mray
{

namespace TBee
{
	OculusDetectState* OculusDetectState::s_instance=0;
OculusDetectState::OculusDetectState()
:IRenderingState("Oculus")
{
	s_instance=this;
	m_guiManager=0;
	m_statePanel=new GUIOculusDetectImpl();
}

OculusDetectState::~OculusDetectState()
{
	delete m_guiManager;
	delete m_statePanel;
}


void OculusDetectState::InitState()
{
	IRenderingState::InitState();
	m_guiManager=new GUI::GUIManager(Engine::getInstance().getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	GUI::IGUIPanelElement* root=(GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""),m_guiManager);
	root->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(root);

	GUI::GUIOverlay* o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUIOculusDetect.gui");
	if(o)
	{
		o->CreateElements(m_guiManager,root,0,m_statePanel);
	}
}


void OculusDetectState::OnEvent(Event* e)
{
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* evt=(KeyboardEvent*)e;
		if(evt->press && evt->key==KEY_SPACE)
		{
			if(m_statePanel->GetState()==GUIOculusDetectImpl::EFailedToConnect)
			{
				exit(0);
			}else
				m_exitCode=STATE_EXIT_CODE;
		}
	}
}

void OculusDetectState::OnEnter(IRenderingState*prev)
{
	if(m_statePanel->GetState()==GUIOculusDetectImpl::EFound)
	{
		m_exitCode=STATE_EXIT_CODE;
	}
}

void OculusDetectState::OnExit()
{
	IRenderingState::OnExit();
}

void OculusDetectState::Update(float dt)
{
	IRenderingState::Update(dt);
	m_guiManager->Update(dt);
}

video::IRenderTarget* OculusDetectState::Render(const math::rectf& rc,ETargetEye eye)
{
	IRenderingState::Render(rc,eye);

	video::IVideoDevice* dev=Engine::getInstance().getDevice();
	dev->set2DMode();
	dev->setRenderTarget(m_renderTarget[GetEyeIndex(eye)]);
	Engine::getInstance().getDevice()->draw2DRectangle(rc,video::DefaultColors::Black);

	m_guiManager->DrawAll(&rc);// m_renderTarget[GetEyeIndex(eye)]);

	dev->setRenderTarget(0);
	return m_renderTarget[GetEyeIndex(eye)];
}

void OculusDetectState::LoadFromXML(xml::XMLElement* e)
{
}
OculusDetectState* OculusDetectState::Instance()
{
	if(!s_instance)
		s_instance=new OculusDetectState();
	return s_instance;
}

}
}