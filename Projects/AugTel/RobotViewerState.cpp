

#include "stdafx.h"
#include "RobotViewerState.h"
#include "ATAppGlobal.h"
#include "ICameraVideoSource.h"
#include "RobotInfoManager.h"
#include "CRobotConnector.h"
#include "IHeadController.h"
#include "TextureResourceManager.h"
#include "FontResourceManager.h"
#include "IGUIRenderer.h"

namespace mray
{
namespace AugTel
{

RobotViewerState::RobotViewerState(TBee::ICameraVideoSource* src, TBee::IRobotCommunicator* comm, const core::string& name)
	:IRobotControlState(name)
{

	m_camVideoSrc = src;

	SetVideoSource(m_camVideoSrc);
	m_robotConnector->SetCommunicator(comm);
	m_takeScreenShot = false;
}

RobotViewerState::~RobotViewerState()
{

}


bool RobotViewerState::OnEvent(Event* e, const math::rectf& rc)
{
	if (Parent::OnEvent(e, rc))
		return true;
	bool ok = false;
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			if (evt->Char == 43 || evt->key == KEY_SPACE)
			{
				if (m_robotConnector->IsRobotConnected())
				{
					m_robotConnector->EndUpdate();
				}
				else
				{
					//	m_vtState->CalibratePosition();

					m_robotConnector->GetHeadController()->Recalibrate();
					m_robotConnector->StartUpdate();
				}
				ok = true;
			}
			else if (evt->key == KEY_ESCAPE)// && evt->lshift)
			{
				m_exitCode = STATE_EXIT_CODE;
				ok = true;
			}
			else if (evt->key == KEY_C)
			{
				//m_vtState->CalibratePosition();

				m_robotConnector->GetHeadController()->Recalibrate();
				ok = true;
			}
			else if (evt->press && evt->key == KEY_F12 && evt->ctrl)
			{
				m_takeScreenShot = true;

				ok = true;

			}
		}
	}
	return ok;

}

void RobotViewerState::InitState()
{
	Parent::InitState();

}

void RobotViewerState::OnEnter(IRenderingState*prev)
{
	Parent::OnEnter(prev);

	m_camVideoSrc->Open();
	TBee::TBRobotInfo* ifo = AppData::Instance()->robotInfoManager->GetRobotInfo(0);
	if (ifo)		
		m_robotConnector->ConnectRobotIP(ifo->IP, gAppData.TargetVideoPort, gAppData.TargetAudioPort,  gAppData.TargetHandsVideoPort, gAppData.TargetCommunicationPort, gAppData.RtcpStream);

	m_robotConnector->SetData("depthSize", "", false);
}

void RobotViewerState::OnExit()
{
	Parent::OnExit();
	m_camVideoSrc->Close();
}

video::IRenderTarget* RobotViewerState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	video::IRenderTarget* ret = Parent::Render(rc, eye);

	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	AppData* app = AppData::Instance();

	dev->set2DMode();
	dev->unuseShader();
	if (font)
	{
		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleCenter;
		attr.fontSize = 30;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = rc;

		if (!m_robotConnector->IsRobotConnected())
		{
			dev->draw2DRectangle(rc, video::SColor(0, 0, 0, 0.8));
			core::string msg = mT("Please wait to get connected");
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		m_guiRenderer->Flush();

	}
	if (m_takeScreenShot)
	{
		ulong currTime = gEngine.getTimer()->getMilliseconds();

		core::string name = gFileSystem.getAppPath() + "screenShots/Final_";
		name += core::StringConverter::toString(currTime);
		name += ".png";
		gTextureResourceManager.writeResourceToDist(m_renderTarget[0]->GetColorTexture(0), name);
		m_takeScreenShot = false;
	}
	return ret;

}

void RobotViewerState::Update(float dt)
{
	Parent::Update(dt);
}
	
}
}

