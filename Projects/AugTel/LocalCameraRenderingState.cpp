

#include "stdafx.h"
#include "LocalCameraRenderingState.h"


#include "TextureResourceManager.h"


#include "AppData.h"
#include <IThreadManager.h>

#include "OculusDevice.h"
#include "InputManager.h"
#include "GUIBatchRenderer.h"
#include "CRobotConnector.h"
#include "FontResourceManager.h"
#include "RobotInfoManager.h"
#include "LocalCameraVideoSource.h"
#include "LocalRobotCommunicator.h"


namespace mray
{
namespace AugTel
{
	int s_id=0;

LocalCameraRenderingState::LocalCameraRenderingState(const core::string& name)
	:IRobotControlState(name)
{
	m_exitCode=0;

	m_VerticalShift=0;
//	m_eyes[1].cw = true;
	m_cameraSource = new LocalCameraVideoSource();
	m_robotConnector->SetCommunicator(new LocalRobotCommunicator());

	GUI::GUIBatchRenderer*r = new GUI::GUIBatchRenderer();
	r->SetDevice(Engine::getInstance().getDevice());
	m_guiRenderer = r;

	m_capturing = false;
	m_clickCount = 0;

	SetVideoSource(m_cameraSource);
}


LocalCameraRenderingState::~LocalCameraRenderingState()
{
	delete m_cameraSource;

	delete m_guiRenderer;
}

video::ICameraVideoGrabber* LocalCameraRenderingState::GetCamera(ETargetEye eye)
{ 
	return m_cameraSource->GetCamera(GetEyeIndex(eye));
}


void LocalCameraRenderingState::InitState()
{
	IEyesRenderingBaseState::InitState();

	int c=1;
	if(AppData::Instance()->stereoMode!=ERenderStereoMode::None)
		c=2;

	m_cameraSource->Init();
}

void LocalCameraRenderingState::SetCameraInfo(ETargetEye eye,int id)
{
	m_cameraSource->SetCameraID(GetEyeIndex(eye), id);

	//_UpdateCameraParams();
}




bool LocalCameraRenderingState::OnEvent(Event* e,const math::rectf& rc)
{
	if (IEyesRenderingBaseState::OnEvent(e, rc))
		return true;
	bool ok = false;
	if(e->getType()==ET_Mouse)
	{
		MouseEvent* ev = (MouseEvent*)e;
		if(ev->event==MET_LEFTDOWN)
		{
			if (m_capturing)
			{
				if (m_clickCount == 0)
				{
					m_firstClick = ev->pos;
					m_clickCount++;
					ok = true;
				}
				else if (m_clickCount == 1)
				{
					m_BoxSize = ev->pos - m_firstClick;
					m_BoxSize.x = abs(m_BoxSize.x);
					m_BoxSize.y = abs(m_BoxSize.y);
					m_capturing = false;
					m_clickCount++;
					ok = true;
				}
			}
		}
	}
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* evt=(KeyboardEvent*)e;
		if (evt->press)
		{
			if(evt->key>=KEY_1 && evt->key<=KEY_9)
			{
				ETargetEye  eye=Eye_Left;
				if(evt->shift)
					eye=Eye_Right;
				int index=GetEyeIndex(eye);
				//change the camera
				if(evt->key-KEY_1!=m_cameraSource->GetCameraID(1-index))
				{
					m_cameraSource->SetCameraID(index,evt->key - KEY_1);
					m_cameraSource->Close();
					m_cameraSource->Open();
					_UpdateCameraParams();
					ok = true;
				}
			}
			else if (evt->key == KEY_ESCAPE)
			{
				m_exitCode = STATE_EXIT_CODE;
				ok = true;
			}
			else
			if (evt->key == KEY_B)
			{
				m_capturing = true;
				m_clickCount = 0;
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


void LocalCameraRenderingState::OnEnter(IRenderingState*prev)
{
	IEyesRenderingBaseState::OnEnter(prev);
	//VCameraType* cam=(VCameraType*)m_camera;//m_video->GetGrabber().pointer();
	
	m_cameraSource->Open();

	//cam->Start();
	//printf("Cam resolution=%dx%d@%d\n",m_camera->GetFrameSize().x,m_camera->GetFrameSize().y,m_camera->GetFrameRate());

	TBRobotInfo* ifo = AppData::Instance()->robotInfoManager->GetRobotInfo(0);
	if (ifo)
	{
//		m_robotConnector->ConnectRobotIP(ifo->IP, gAppData.TargetVideoPort, gAppData.TargetAudioPort, gAppData.TargetHandsVideoPort, gAppData.TargetCommunicationPort, gAppData.RtcpStream);
	}

}


void LocalCameraRenderingState::OnExit()
{
	IRenderingState::OnExit();
	//VCameraType* cam=(VCameraType*)m_video->GetGrabber().pointer();
	m_cameraSource->Close();

}
video::IRenderTarget* LocalCameraRenderingState::Render(const math::rectf& rc,ETargetEye eye)
{
	video::IRenderTarget*ret=IEyesRenderingBaseState::Render(rc, eye);


	//draw a grid
	//video::IVideoDevice* dev = Engine::getInstance().getDevice();



	//dev->setRenderTarget(0);
	//dev->useTexture(0,0);


	return ret;
}

void LocalCameraRenderingState::Update(float dt)
{
	IEyesRenderingBaseState::Update(dt);
}


void LocalCameraRenderingState::_RenderUI(const math::rectf& rc, math::vector2d&pos)
{
	IEyesRenderingBaseState::_RenderUI(rc,pos);
	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	if (m_clickCount == 2)
	{
		int n = 20;
		for (int i = 0; i < n; ++i)
		{
			dev->draw2DLine(math::vector2d(0, i*m_BoxSize.y), math::vector2d(rc.getWidth(), i*m_BoxSize.y), video::SColor(0, 0, 0, 255));
			dev->draw2DLine(math::vector2d(i*m_BoxSize.x, 0), math::vector2d(i*m_BoxSize.x, rc.getHeight()), video::SColor(0, 0, 0, 255));
		}
	}
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
		r.ULPoint = pos;

		/*controllers::IJoysticController* joystick = TBAppGlobals::inputMngr->getJoystick(0);

		if (joystick)
		{

			for (int i = 0; i < 4; ++i)
			{
				controllers::JoysticAxis a = joystick->getAxisState(i);
				core::string msg = mT("Axis")+core::StringConverter::toString(i)+"= "+ core::StringConverter::toString(a.abs);
				font->print(r, &attr, 0, msg, m_guiRenderer);
				r.ULPoint.y += attr.fontSize + 5;
			}
		}*/

	}
}


void LocalCameraRenderingState::LoadFromXML(xml::XMLElement* e)
{
	IEyesRenderingBaseState::LoadFromXML(e);
	m_cameraSource->LoadFromXML(e);

}


}
}


