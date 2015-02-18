#include "stdafx.h"
 
#include "IMovable.h"
#include "LookAtAnimation.h"
#include "MathUtil.h"
#include "IMovable.h"


namespace mray{
namespace animation{

LookAtAnimation::LookAtAnimation(const core::string& name,scene::IMovable* node,float damping,int affectedID,ulong catagoryBits,ulong priority):
	IAnimationTrack(name,affectedID,catagoryBits,priority)
{
	m_node=node;
	m_damping=damping;
}
LookAtAnimation::~LookAtAnimation(){

}
float LookAtAnimation::getDamping(){
	return m_damping;
}
void LookAtAnimation::setDamping(float d){
	m_damping=d;
}
void LookAtAnimation::animate(float time,float weight){
}
void LookAtAnimation::update(float dt)
{
	m_dt=dt;
}
bool LookAtAnimation::applyOnObject(scene::IMovable*object,int id)
{
	if(m_affectedNodeID!=id && id!=-1)
		return false;
	math::matrix4x4 m;
	m=math::MathUtil::CreateLookAtMatrix(object->getAbsolutePosition(),m_node->getAbsolutePosition(),math::vector3d::YAxis);
	math::quaternion q(m);
	scene::IMovable*parent= object->getParent();
	math::quaternion pQ=parent->getAbsoluteOrintation().inverse();
	q.w=-q.w;
	math::quaternion oldOrient=object->getOrintation();
	if(parent){
		q=q*pQ;
	}
	q.Slerp(oldOrient,q,m_damping*m_dt);
	object->setOrintation(q);
	return true;
}


IAnimationTrack* LookAtAnimation::duplicate(){
	IAnimationTrack* o=new LookAtAnimation(m_name,m_node,m_damping,m_affectedNodeID,m_catagoryBits,m_priority);
	copy(o);
	return o;
}
void LookAtAnimation::copy(IAnimationTrack* o){
	IAnimationTrack::copy(o);
	LookAtAnimation* ao=dynamic_cast<LookAtAnimation*>(o);
	if(!ao)
		return;

	ao->m_node=m_node;
	ao->m_damping=m_damping;
}

}
}
