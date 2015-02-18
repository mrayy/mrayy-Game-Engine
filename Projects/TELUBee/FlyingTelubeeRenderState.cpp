


#include "stdafx.h"
#include "FlyingTelubeeRenderState.h"
#include "RobotInfoManager.h"
#include "FontResourceManager.h"
#include "CRobotConnector.h"
#include "AppData.h"
#include "IGUIRenderer.h"
#include "GStreamVideoProvider.h"

#include "TextureResourceManager.h"
#include "GstStereoStreamVideoSource.h"
#include "FlyingTelubeeRobotCommunicator.h"
#include "RemoteRobotCommunicator.h"

namespace mray
{
namespace TBee
{


FlyingTelubeeRenderState::FlyingTelubeeRenderState()
:IEyesRenderingBaseState("FlyingTelubee")
{

	m_fps = 0;
	m_timeAcc = 0;
	IEyesRenderingBaseState::InitState();
	m_robotConnector->SetCommunicator(new  FlyingTelubeeRobotCommunicator());

	m_cameraSource = new GstStereoStreamVideoSource();
	// 	m_eyes[0].flip90 = true;
	// 	m_eyes[1].flip90 = true;
	m_eyes[0].cw = true;
	m_eyes[1].cw = false;

	m_port1 = 5000;
	m_port2 = 5001;

	SetVideoSource(m_cameraSource);

}


FlyingTelubeeRenderState::~FlyingTelubeeRenderState()
{
	delete m_cameraSource;
}


void FlyingTelubeeRenderState::InitState()
{
	IEyesRenderingBaseState::InitState();
	m_cameraSource->Init();

}



bool FlyingTelubeeRenderState::OnEvent(Event* e, const math::rectf &rc)
{
	if (IEyesRenderingBaseState::OnEvent(e, rc))
		return true;
	bool ok = false;

	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			if (evt->key == KEY_ESCAPE)
			{
				m_exitCode = STATE_EXIT_CODE;
				m_robotConnector->EndUpdate();
				ok = true;
			}
			else if (evt->key == KEY_SPACE)
			{
				if (m_robotConnector->IsRobotConnected())
					m_robotConnector->EndUpdate();
				else
					m_robotConnector->StartUpdate();
				ok = true;
			}
		}
	}
	return ok;
}


void FlyingTelubeeRenderState::OnEnter(IRenderingState*prev)
{

	IEyesRenderingBaseState::OnEnter(prev);
	TBRobotInfo* ifo = AppData::Instance()->robotInfoManager->GetRobotInfo(0);

	if (ifo)
	{
		m_cameraSource->SetPorts(m_port1, m_port2);
		//m_cameraSource->SetPort(m_port1);
		m_cameraSource->Open();
		m_robotConnector->ConnectRobotIP(ifo->IP, 5000, 5002, 5003);
	}
	m_frameCounter = 0;
}


void FlyingTelubeeRenderState::OnExit()
{
	IEyesRenderingBaseState::OnExit();
	m_cameraSource->Close();
}


void FlyingTelubeeRenderState::Update(float dt)
{
	IEyesRenderingBaseState::Update(dt);
	if (m_cameraSource->Blit())
	{
		m_frameCounter++;
		if (m_timeAcc >= 1)
		{
			m_fps = m_frameCounter;
			m_frameCounter = 0;
			m_timeAcc -= 1;
		}
		_UpdateCameraParams();
	}
	m_timeAcc += dt;
}

void FlyingTelubeeRenderState::_RenderUI(const math::rectf& rc, math::vector2d& pos)
{
	IEyesRenderingBaseState::_RenderUI(rc,pos);

	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	if (font)
	{
		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = rc;

		core::string msg = mT("FPS:") + core::StringConverter::toString(m_fps);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		// 		msg=mT("Bytes:") + core::StringConverter::toString(m_providers->GetRecvBytesPerSecond());
		// 		font->print(r, &attr, 0, msg, m_guiRenderer);

		m_guiRenderer->Flush();
	}

}

video::IRenderTarget* FlyingTelubeeRenderState::Render(const math::rectf& rc, ETargetEye eye)
{
	video::IRenderTarget*ret = IEyesRenderingBaseState::Render(rc, eye);
	//draw a grid
	video::IVideoDevice* dev = Engine::getInstance().getDevice();


	//	_RenderUI(rc);
	//	dev->setRenderTarget(0);
	// 	dev->useTexture(0, 0);


	return 0;
}



void FlyingTelubeeRenderState::LoadFromXML(xml::XMLElement* e)
{
	IEyesRenderingBaseState::LoadFromXML(e);
	xml::XMLAttribute* attr;


	attr = e->getAttribute("VideoPort1");
	if (attr)
		m_port1 = core::StringConverter::toInt(attr->value);
	attr = e->getAttribute("VideoPort2");
	if (attr)
		m_port2 = core::StringConverter::toInt(attr->value);

}



}
}


