#include "stdafx.h"

#include "FSLListener.h"

#include <FreeSL.h>

namespace mray{
namespace sound{


FSLListener::FSLListener(){
	m_node=0;
	m_distanceModel=InverseDistance;
}
FSLListener::~FSLListener(){
}

void FSLListener::setPosition(const math::vector3d& p){
	fslSetListenerPosition(p.x,p.y,p.z);
	m_position=p;
}
const math::vector3d& FSLListener::getPosition(){
	return m_position;
}

void FSLListener::setVelocity(const math::vector3d& p){
	fslSetListenerVelocity(p.x,p.y,p.z);
	m_velocity=p;
}
const math::vector3d& FSLListener::getVelocity(){
	return m_velocity;
}

void FSLListener::setOreintation(const math::quaternion& o){

	m_orient=o;
	math::vector3d face=m_orient*math::vector3d::ZAxis;
	math::vector3d up=m_orient*math::vector3d::YAxis;

	fslSetListenerOrientation(face.x,face.y,face.z,up.x,up.y,up.z);
}
const math::quaternion& FSLListener::getOreintation(){
	return m_orient;
}

void FSLListener::attachToListener(scene::IMovable*node){
	m_node=node;
}

scene::IMovable* FSLListener::getListenerNode(){
	return m_node;
}

void FSLListener::setDistanceModel(EListenerDistanceModel m)
{ 
	m_distanceModel=m;
	FSLenum e=FSL_INVERSE_DISTANCE;
	switch(m)
	{
	case InverseDistance:e=FSL_INVERSE_DISTANCE;break;
	case InverseDistanceClamped:e=FSL_INVERSE_DISTANCE_CLAMPED;break;
	case LinearDistance:e=FSL_LINEAR_DISTANCE;break;
	case LinearDistanceClamped:e=FSL_LINEAR_DISTANCE_CLAMPED;break;
	case ExponentDistance:e=FSL_EXPONENT_DISTANCE;break;
	case ExponentDistanceClamped:e=FSL_EXPONENT_DISTANCE_CLAMPED;break;
	}
	fslSetListenerDistanceModel(e);
}
EListenerDistanceModel FSLListener::getDistanceModel()
{
	return m_distanceModel;
}


void FSLListener::update(float dt){
	if(m_node){
		setPosition(m_node->getAbsolutePosition());
		setOreintation(m_node->getOrintation());
		m_velocity=(m_position-m_lastPos)*dt;
		m_lastPos=m_position;
		setVelocity(m_velocity);
	}

}


}
}
