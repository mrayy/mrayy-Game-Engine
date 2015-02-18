
#include "stdafx.h"
#include "IMovable.h"
#include "ISceneVisitor.h"
#include "AnimationController.h"
#include "TransformKeyFrame.h"
#include "TransformationAnimationTrack.h"


namespace mray{
namespace scene{

	

IMovable::IMovable():m_isAnimationManual(false)
{
}
IMovable::~IMovable()
{
}

void IMovable::_UpdateAnimationTrackState()
{
	if(m_isAnimationManual)
		return;

	//calculate the total weight
	float totalWeight=0;
	std::list<AttachedAnimationTrack>::iterator it= m_attachedTracks.begin();
	for (;it!=m_attachedTracks.end();++it)
	{
		if((*it).controller->isPlaying())
		{
			totalWeight+=(*it).controller->getWeight();
		}
	}
	if(totalWeight==0)
		return;
	float invTotalWeight=1.0f/totalWeight;

	animation::TransformKeyFrame kf;
	math::vector3d resultPos;
	math::vector3d resultScale(1);

	math::quaternion resultRot;
	bool first=true;
	for (it= m_attachedTracks.begin();it!=m_attachedTracks.end();++it)
	{
		animation::AnimationController* controller=(*it).controller;
		if(!controller->isPlaying())
			continue;
		float w=controller->getWeight()*invTotalWeight;
		it->track->interpolate(controller->getTime(),kf);

		resultPos+=kf.getPos()*w;
		resultScale*=math::lerp(math::vector3d(1),kf.getScale(),w);
		if(first)
		{
			first=false;
			resultRot=kf.getOrintation();
		}else
		{
			resultRot.Slerp(resultRot,kf.getOrintation(),w,true);
		}
	}
	setPosition(resultPos);
	setScale(resultScale);
	setOrintation(resultRot);
}


void IMovable::SetManualAnimation(bool m)
{
	m_isAnimationManual=m;
}
bool IMovable::IsManualAnimation()
{
	return m_isAnimationManual;
}

void IMovable::AttachAnimationTrack(animation::TransformationAnimationTrack* track,animation::AnimationController*controller)
{
	m_attachedTracks.push_back(AttachedAnimationTrack(track,controller));
}
void IMovable::DeattachAnimationTrack(animation::TransformationAnimationTrack* track)
{
	std::list<AttachedAnimationTrack>::iterator it= m_attachedTracks.begin();
	for (;it!=m_attachedTracks.end();++it)
	{
		if((*it).track==track)
		{
			m_attachedTracks.erase(it);
			break;
		}
	}

}
 
bool IMovable::rotate(float angle,const math::vector3d& axis,ETransformSpace space){
	math::quaternion q(angle,axis);
	return rotate(q,space);
}
bool IMovable::rotate(const math::quaternion &q,ETransformSpace space){

	math::quaternion o;
	switch(space){
	case TS_Local:
		o=getOrintation()*q;
		//o.Normalize();
		break;
	case TS_Parent:
		{
			if(getParent()){
				o=getOrintation()*getAbsoluteOrintation().inverse()*q*getAbsoluteOrintation();
			}else
				o=getOrintation()*q;
			o.Normalize();
		}break;
	case TS_World:
		{
			o=q*getOrintation();
		//	o.Normalize();
		}
		break;
	}
	return setOrintation(o);

}
bool IMovable::pitch(float angle,ETransformSpace space){
	math::quaternion q(angle,math::vector3d(1,0,0));
	return rotate(q,space);
}
bool IMovable::yaw(float angle,ETransformSpace space){
	math::quaternion q(angle,math::vector3d(0,1,0));
	return rotate(q,space);
}
bool IMovable::roll(float angle,ETransformSpace space){
	math::quaternion q(angle,math::vector3d(0,0,1));
	return rotate(q,space);
}


bool IMovable::translate(const math::vector3d& p,ETransformSpace space){
	switch(space){
	case TS_Local:
		return setPosition(getPosition()+getOrintation()*p);
		break;
	case TS_World:
		if(getParent()){
			return setPosition(getPosition()+(getParent()->getAbsoluteOrintation().inverse()*p)/getParent()->getAbsoluteScale());
		}else{
			return setPosition(getPosition()+p);
		}
		break;
	case TS_Parent:
		return setPosition(getPosition()+p);
		break;
	}
	return false;
}


void IMovable::preRender(){
/*	core::IteratorPair<NodeList> chP=getChildren();
	for(;!chP.done();chP++){
		(*chP)->preRender();
	}*/
}
void IMovable::update(float dt){
	_UpdateAnimationTrackState();
	updateAbsoluteTransformation();
	const MovableNodeList& lst= getChildren();
	MovableNodeList::const_iterator it = lst.begin();
	for (; it != lst.end();++it)
	{
		(*it)->update(dt);
	}
/*
	core::IteratorPair<NodeList> chP=getChildren();
	for(;!chP.done();chP++){
		(*chP)->update(dt);
	}*/
}
/*
IMovable* IMovable::getElementById(int id,bool searchChildren)
{
	IMovable*et;
	core::IteratorPair<NodeList> it(getChildren());
	for(;!it.done();++it)
	{
		IMovable* node=*it;
		if(!node)
			continue;
		if(node->getID()==id)
			return (*it);
		if(searchChildren)
			et=node->getElementById(id,searchChildren);
		if(et)return et;
	}
	return 0;
}*/

void IMovable::OnVisit(ISceneVisitor*visitor)
{
	visitor->Visit(this);
}

}
}
