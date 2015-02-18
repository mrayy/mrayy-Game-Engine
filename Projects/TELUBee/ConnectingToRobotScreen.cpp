

#include "stdafx.h"
#include "ConnectingToRobotScreen.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "Application.h"
#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "TBAppGlobals.h"
#include "TBRobotInfo.h"
#include "JoystickDefinitions.h"
#include "INetwork.h"
#include "LocalRobotCommunicator.h"

#include "IThreadManager.h"
#include "DVISwitcherController.h"

namespace mray
{
namespace TBee
{
//#define USE_SEETHROUGH

ConnectingToRobotScreen::ConnectingToRobotScreen()
:IRenderingState("Connecting")
{
	m_connectingPnl=new GUIConnectingToRobot();
	m_roboComm=new LocalRobotCommunicator();
	m_dviSwitcher=new DVISwitcherController();
}
ConnectingToRobotScreen::~ConnectingToRobotScreen()
{
	m_roboComm->Disconnect();
	delete m_guiManager;
	delete m_connectingPnl;
	delete m_roboComm;
	delete m_dviSwitcher;
}


void ConnectingToRobotScreen::InitState()
{
	IRenderingState::InitState();
	m_guiManager=new GUI::GUIManager(Engine::getInstance().getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	m_guiroot=(GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""),m_guiManager);
	m_guiroot->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(m_guiroot);

	GUI::GUIOverlay* o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUIConnectingToRobot.gui");
	if(o)
	{
		o->CreateElements(m_guiManager,m_guiroot,0,m_connectingPnl);
	}
//	m_dviSwitcher->Open(TBAppGlobals::Instance()->DVIPort);
}

void ConnectingToRobotScreen::_Connect()
{
	m_connectingPnl->StatusLbl->SetText(core::string(core::string("Connected! You are now at: ")+TBAppGlobals::Instance()->selectedRobot->name));
	m_connectingPnl->InstructionLbl->SetVisible(false);
	//TBAppGlobals::Instance()->seeTrough->SwitchToRemoteVision();
	m_roboComm->ConnectRobot(true);
//	m_dviSwitcher->SwitchToSource2();
}
void ConnectingToRobotScreen::_Disconnect()
{
	//TBAppGlobals::Instance()->seeTrough->SwitchToSeeThrough();
	m_roboComm->ConnectRobot(false);
	m_roboComm->ConnectUser(false);
//	m_dviSwitcher->SwitchToSource1();
	m_roboComm->Disconnect();

}

void ConnectingToRobotScreen::OnEvent(Event* e)
{
	if(e->getType()==ET_Joystick)
	{
		JoystickEvent* evt=(JoystickEvent*)e;
		if(m_status==EConnected || m_status==EEstablished)
		{
			if(evt->button==JOYSTICK_SelectButton)
			{
				m_exitCode=BackToMap_Code;
				_Disconnect();
			}else  if(evt->button==JOYSTICK_StartButton && m_status==EConnected)
			{
				m_status=EEstablished;
				_Connect();
			}
		}

	//	if(evt->button==JOYSTICK_CrossButton)
		//	m_roboComm->ConnectUser(evt->event==JET_BUTTON_PRESSED);
	}
}

void ConnectingToRobotScreen::OnEnter(IRenderingState*prev)
{
	m_status=EInitializing;
	m_time=0;
	m_targetTime=math::Randomizer::rand01()*3+3;
	m_connectingPnl->ConnectingFrame->SetVisible(true);
	m_connectingPnl->StatusLbl->SetText(core::string("Initializing"));
	m_connectingPnl->InstructionLbl->SetVisible(false);
#ifdef USE_SEETHROUGH
	TBAppGlobals::Instance()->seeTrough->ConnectToCameras(TBAppGlobals::Instance()->selectedRobot->IP,1212,1213,1214);
#endif
	m_roboComm->Connect(TBAppGlobals::Instance()->selectedRobot->IP,20000);
//	m_roboComm->Connect("127.0.0.1",3000);
	m_roboComm->SetUserID("yamens");
	m_roboComm->ConnectUser(true);
}

void ConnectingToRobotScreen::OnExit()
{
	IRenderingState::OnExit();


}
video::IRenderTarget* ConnectingToRobotScreen::Render(const math::rectf& rc,ETargetEye eye)
{
	IRenderingState::Render(rc,eye);

	video::IVideoDevice* dev=Engine::getInstance().getDevice();
	dev->set2DMode();
	dev->setRenderTarget(m_renderTarget[GetEyeIndex(eye)]);
	TBAppGlobals::Instance()->App->getDevice()->draw2DRectangle(rc,video::DefaultColors::Black);

	m_guiManager->DrawAll(&rc);// m_renderTarget[GetEyeIndex(eye)]);

	dev->setRenderTarget(0);
	return m_renderTarget[GetEyeIndex(eye)];
}

void ConnectingToRobotScreen::Update(float dt)
{
	if(m_status!=EEstablished)
	{
		if(m_status!=EConnected)
		{

			m_time+=dt;
			if(m_time>m_targetTime )
			{
				m_time=0;
				m_targetTime=math::Randomizer::rand01()*3+3;
				if(m_status==EInitializing)
				{
					m_connectingPnl->StatusLbl->SetText(core::string(core::string("Connecting To:")+TBAppGlobals::Instance()->selectedRobot->name));
					m_status=EConnectingToRobot;
				}else if(m_status==EConnectingToRobot 
#ifdef USE_SEETHROUGH
					//&& TBAppGlobals::Instance()->seeTrough->IsConnected()
#endif
					)
				{
					m_connectingPnl->StatusLbl->SetText(core::string(core::string("Connected To:")+TBAppGlobals::Instance()->selectedRobot->name
						+ mT("(")+core::StringConverter::toString((int)TBAppGlobals::Instance()->selectedRobot->lat)
						+ mT(",")+core::StringConverter::toString((int)TBAppGlobals::Instance()->selectedRobot->lng)+mT(")")));
					m_connectingPnl->InstructionLbl->SetVisible(true);
					m_status=EConnected;

				}
			}
		}
	}else
	{
		
//		m_roboComm->Update(dt);
	}
	m_guiManager->Update(dt);
}



}
}

