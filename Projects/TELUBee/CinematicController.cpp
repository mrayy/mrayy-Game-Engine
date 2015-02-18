

#include "stdafx.h"
#include "CinematicController.h"
#include "MapObject.h"

#include "JoystickMapController.h"
#include "Mapcamera.h"
#include "JoystickDefinitions.h"



namespace mray
{
namespace TBee
{

CinematicController::CinematicController(InputManager* inputMngr)
{

	KeyFrame k;
	k.time=0;
	k.x=37.579413;
	k.y=139.921875;
	k.zoom=45;
	k.tilt=0;
	k.yaw=0;
	k.blur=400;
	k.zoomLevel=8;

	m_keyframes.push_back(k);
	k.time=3;
	k.x=37.579413;
	k.y=139.921875;
	k.zoom=45;
	k.tilt=0;
	k.yaw=-90;
	k.blur=0;
	k.zoomLevel=2;

	m_keyframes.push_back(k);

	k.time=10;
	k.x=34.065174;
	k.y=-118.243103;
	k.zoom=1450;
	k.tilt=45;
	k.blur=400;
	k.yaw=-80;
	k.zoomLevel=8;
	m_keyframes.push_back(k);

	k.time=13;
	k.x=34.065174;
	k.y=-118.243103;
	k.zoom=45;
	k.tilt=0;
	k.blur=0;
	k.yaw=0;
	k.zoomLevel=2;
	m_keyframes.push_back(k);

	m_time=0;
	m_status=ECinematic;

	m_nextController=new JoystickMapController(inputMngr);
}

CinematicController::~CinematicController()
{
	delete m_nextController;
}

float CinematicController::getKeyFrames(float t,CinematicController::KeyFrame*& k1,CinematicController::KeyFrame*& k2)
{
	k1=k2=0;
	for(int i=m_keyframes.size()-1;i>=0;--i)
	{
		if(m_keyframes[i].time<t)
		{
			float r=0;
			k1=&m_keyframes[i];
			if(i<m_keyframes.size()-1)
			{
				k2=&m_keyframes[i+1];
				r=math::clamp((t-k1->time)/(k2->time-k1->time),0.0f,1.0f);	
			}
			else
			{
				k2=0;
			}
			return r;

		}
	}
	k1=&m_keyframes[0];
	k2=0;
	return 0;
}


bool CinematicController::OnEvent(Event* e)
{
	if(m_status==EControll)
	{
		return m_nextController->OnEvent(e);
	}else
	{

		if(e->getType()==ET_Joystick)
		{
			JoystickEvent* evt=(JoystickEvent*)e;
			if(evt->button==JOYSTICK_TriangleButton)
			{
				m_status=EControll;
				m_mapObject->Reset();
			}
		}
	}
	return false;
}

void CinematicController::Update(float dt)
{
	if(m_status==ECinematic)
	{

		m_time+=dt;
		KeyFrame* k1=0,*k2=0;
		float t=getKeyFrames(m_time,k1,k2);
		if(m_time>m_keyframes[m_keyframes.size()-1].time)
		{
			m_status=EControll;
		}

		KeyFrame k;
		if(k2==0)
			k=*k1;
		else
		{
			k.x=math::lerp(k1->x,k2->x,t);
			k.y=math::lerp(k1->y,k2->y,t);
			k.zoom=math::lerp(k1->zoom,k2->zoom,t);
			k.zoomLevel=math::lerp(k1->zoomLevel,k2->zoomLevel,t);
			k.tilt=math::lerp(k1->tilt,k2->tilt,t);
			k.blur=math::lerp(k1->blur,k2->blur,t);
			k.yaw=math::lerp(k1->yaw,k2->yaw,t);
		}

		m_mapObject->SetPos(k.x,k.y);
		m_mapObject->SetTilt(k.tilt);
		m_mapObject->SetZoomCamera(k.zoom);
		m_mapObject->SetBlur(k.blur);
		m_mapObject->GetCamera()->setYaw(k.yaw);
		m_mapObject->SetZoom(k.zoomLevel);
	}else
	{
		m_nextController->Update(dt);
	}
}

}
}

