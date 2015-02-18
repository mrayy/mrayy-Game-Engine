

#include "stdafx.h"
#include "GSTVideoState.h"
#include "RobotInfoManager.h"
#include "FontResourceManager.h"
#include "CRobotConnector.h"
#include "AppData.h"
#include "IGUIRenderer.h"
#include "GstFileVideoSource.h"

namespace mray
{
namespace TBee
{


GSTVideoState::GSTVideoState()
:IEyesRenderingBaseState("VideoState")
{
	m_videoSource = new GstFileVideoSource("","");

	SetVideoSource(m_videoSource);

}


GSTVideoState::~GSTVideoState()
{
	delete m_videoSource;
}



void GSTVideoState::InitState()
{
	IEyesRenderingBaseState::InitState();
	m_videoSource->Init();

}



bool GSTVideoState::OnEvent(Event* e, const math::rectf &rc)
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


void GSTVideoState::OnEnter(IRenderingState*prev)
{

	IEyesRenderingBaseState::OnEnter(prev);
	m_videoSource->Open();
	m_fps = 0;
	m_timeAcc = 0;

}


void GSTVideoState::OnExit()
{
	IEyesRenderingBaseState::OnExit();
	m_videoSource->Close();
}


void GSTVideoState::Update(float dt)
{
	IEyesRenderingBaseState::Update(dt);

	m_timeAcc += dt;
	m_videoSource->Blit();
}

void GSTVideoState::_RenderUI(const math::rectf& rc, math::vector2d& pos)
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

		m_guiRenderer->Flush();
	}

}

video::IRenderTarget* GSTVideoState::Render(const math::rectf& rc, ETargetEye eye)
{
	video::IRenderTarget*ret = IEyesRenderingBaseState::Render(rc, eye);
	//draw a grid
	video::IVideoDevice* dev = Engine::getInstance().getDevice();


	//	_RenderUI(rc);
	//	dev->setRenderTarget(0);
	// 	dev->useTexture(0, 0);


	return 0;
}



void GSTVideoState::LoadFromXML(xml::XMLElement* e)
{
	IEyesRenderingBaseState::LoadFromXML(e);
	xml::XMLAttribute* attr;

	m_videoSource->LoadFromXML(e);

}



}
}

