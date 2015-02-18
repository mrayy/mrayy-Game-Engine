

#include "stdafx.h"
#include "CameraOrbitComp.h"
#include "IMouseController.h"


namespace mray
{
namespace game
{


CameraOrbitComp::CameraOrbitComp()
{
	m_camera=0;
	m_inputManager=0;
	m_speed.set(20,10,10);
	m_minLimit=-20;
	m_maxLimit=80;
	m_distance=10.0f;
	m_followSpeed=0.04;
}

CameraOrbitComp::~CameraOrbitComp()
{
}


void CameraOrbitComp::SetCamera(scene::CameraNode* cam,InputManager* m)
{
	m_inputManager=m;
	m_camera=cam;

	m_camera->getAbsoluteOrintation().toEulerAngles(m_angles);
}


static float clampAngle(float a,float min,float max)
{
	if(a<-360)
		a+=360;
	if(a>360)
		a-=360;
	return math::clamp(a,min,max);
}

void CameraOrbitComp::Update(float dt)
{
	if(!m_camera || !m_inputManager)
		return ;

	controllers::IMouseController* mouse=m_inputManager->getMouse();
	if(mouse->isPressed(controllers::EMB_Left))
	{
		m_angles.y+=mouse->getDX()*m_speed.x*0.02;
		m_angles.x-=mouse->getDY()*m_speed.y*0.02;
		m_distance-=mouse->getDZ()*m_speed.z*0.02;
		m_distance=math::clamp(m_distance,1.0f,30.0f);
	}

	m_angles.y=clampAngle(m_angles.y,m_minLimit,m_maxLimit);

	math::quaternion rotation=math::quaternion(m_angles.x,m_angles.y,0);
	math::vector3d pos=rotation* math::vector3d(0,0,-m_distance);

	m_camera->setOrintation(rotation);
	m_camera->setPosition(math::lerp(m_camera->getAbsolutePosition(),pos,m_followSpeed));
}

}
}