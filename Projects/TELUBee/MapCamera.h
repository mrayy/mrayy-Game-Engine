

/********************************************************************
	created:	2013/01/31
	created:	31:1:2013   16:22
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\MapCamera.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	MapCamera
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MapCamera__
#define __MapCamera__


#include "CameraNode.h"

namespace mray
{
	class InputManager;
namespace TBee
{

	
class MapCamera:public scene::CameraNode
{
protected:
	math::vector2d m_angles;
	float m_distance;
public:
	MapCamera(scene::ISceneManager*smngr,InputManager* kb):CameraNode(mT(""),0,smngr)
	{
		this->setUseTarget(true);
		this->setTarget(math::vector3d(0,0,0));
		this->setAspect(1);
		this->setFovY(90);
		this->setAutoUpdateAspect(false);
		this->setZFar(1000);
		this->setUpVector(math::vector3d(0,-1,1));
		m_distance=100;
	}

	void UpdatePosition()
	{
		m_angles.x=math::clamp<float>(m_angles.x,1,45);
		m_angles.y=math::clamp<float>(m_angles.y,-80,80);
		m_distance=math::clamp<float>(m_distance,10,45);
		math::vector3d pos;
		pos.z=m_distance*math::sind(m_angles.x)*math::cosd(m_angles.y);
		pos.y=m_distance*math::cosd(m_angles.x);
		pos.x=m_distance*math::sind(m_angles.x)*math::sind(m_angles.y);
		float normAngle=m_angles.x/45.0f;
		float fov=90*(1-normAngle)+38*normAngle;
		setFovY(fov);
		setPosition(pos);
	}

	void SetTilt(float a)
	{
		m_angles.x=a;
		UpdatePosition();
	}
	void SetYaw(float a)
	{
		m_angles.y=a;
		UpdatePosition();
	}
	void SetDistance(float d)
	{
		m_distance=d;
		UpdatePosition();
	}
	void AddTilt(float dy)
	{
		m_angles.x+=dy;
		UpdatePosition();
	}
	void AddDist(float dy)
	{
		m_distance+=dy;
		UpdatePosition();
	}
	float getDist()
	{
		return m_distance;
	}
	void setYaw(float angle)
	{
		m_angles.y=angle;
		UpdatePosition();
	}

};


}
}

#endif
