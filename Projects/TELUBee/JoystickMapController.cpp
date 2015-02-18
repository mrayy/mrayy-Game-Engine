

#include "stdafx.h"
#include "JoystickMapController.h"
#include "MapObject.h"
#include "MapCamera.h"
#include "GUIWebPageControl.h"
#include "BrowserWindow.h"
#include "JoystickDefinitions.h"

namespace mray
{
namespace TBee
{
JoystickMapController::JoystickMapController(InputManager* inputMngr)
{
	m_input=inputMngr;
	m_distAccel=0;
	m_mapObject=0;
}
bool JoystickMapController::OnEvent(Event* evt)
{
	if(evt->getType()==ET_Joystick)
	{
		JoystickEvent* e=(JoystickEvent*)evt;
		if(e->event==JET_BUTTON_PRESSED)
		{

			if(e->button==JOYSTICK_TriangleButton)
			{
				m_mapObject->Reset();
				return true;
			}else  if(e->button==JOYSTICK_R1Button)
			{
				m_mapObject->ZoomIn();
				return true;
			}else if(e->button==JOYSTICK_L1Button)
			{
				m_mapObject->ZoomOut();
				return true;
			}else if(e->button==JOYSTICK_CrossButton)
			{
				m_mapObject->SelectRobot(evt);
				return true;
			}
		}
	}
	return false;
}
void JoystickMapController::Update(float dt)
{
	if(!m_input->getJoystick(0))
		return;
	{
		float da=(m_input->getJoystick(0)->getAxisState(1).abs);
		da*=da*50*math::sign(da);
		float db=(m_input->getJoystick(0)->getAxisState(2).abs);
		db*=90;

		m_mapObject->SetTilt(da);
	}
	{
		//	float dy=(m_input->getJoystick(0)->getAxisState(2).abs*dt);
		m_distAccel+=(-(2*m_input->getJoystick(0)->getButtonState(10)-1)*500*dt);
		m_distAccel=math::clamp<float>(m_distAccel,-100,100);
		m_mapObject->ZoomCamera(m_distAccel*dt);
	}
	{

		//m_mapObject->GetCamera()->SetYaw(db);
		float axis1=m_input->getJoystick(0)->getAxisState(4).abs;
		float axis2=m_input->getJoystick(0)->getAxisState(5).abs;
		float dy=(pow(axis1,2)*math::sign(axis1)*50*dt);
		float dx=(pow(axis2,2)*math::sign(axis2)*50*dt);


		m_mapObject->ChangeDeltaPos(-dy,dx);
	}

	m_mapObject->MouseMoved(0);

}

}
}
