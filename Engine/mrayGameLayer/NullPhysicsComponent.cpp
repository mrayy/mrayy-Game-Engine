

#include "stdafx.h"
#include "NullPhysicsComponent.h"
#include "GameEntityManager.h"
#include "ISceneManager.h"



namespace mray
{
namespace game
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(Position,NullPhysicsComponent,math::vector3d,mT("Position"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Position,NullPhysicsComponent,math::vector3d,SetPosition,GetPosition,core::StringConverter::toString,core::StringConverter::toVector3d,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Rotation,NullPhysicsComponent,math::vector3d,mT("Rotation"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Rotation,NullPhysicsComponent,math::vector3d,SetRotation,GetRotation,core::StringConverter::toString,core::StringConverter::toVector3d,false);

NullPhysicsComponent::NullPhysicsComponent(game::GameEntityManager*m)
{
	m_root=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePosition::instance);
		dic->addPropertie(&PropertyTypeRotation::instance);
	}
}
NullPhysicsComponent::~NullPhysicsComponent()
{
}

bool NullPhysicsComponent::InitComponent()
{
	if(!IPhysicsComponent::InitComponent())
		return false;
	m_root=GetOwnerEntity()->GetCreator()->GetSceneManager()->createSceneNode();
	m_root->setPosition(m_position);
	m_root->setOrintation(m_rotation);
	m_root->setNodeName(GetName()+"#ParentNode");
	std::list<game::PhysicalComponentAttachment*>::iterator it= m_attachments.begin();
	m_root->updateAbsoluteTransformation();
	for (;it!=m_attachments.end();++it)
	{
		if((*it)->GetTargetObject())
		{
			(*it)->GetTargetObject()->updateAbsoluteTransformation();
			m_root->addChild((*it)->GetTargetObject(),true);
		}
	}

	return true;
}
void NullPhysicsComponent::AttachNode(game::PhysicalComponentAttachment*a)
{
	IPhysicsComponent::AttachNode(a);
	if(m_root && a->GetTargetObject())
		m_root->addChild(a->GetTargetObject(),true);
}

bool NullPhysicsComponent::SetPosition(const math::vector3d& v)
{
	m_position=v;
	if(m_root)
	{
		scene::IMovable* m= m_root->getParent();
		math::vector3d pos=v;
		if(m)
			pos=m->getAbsoluteOrintation().inverse()*v-m->getAbsolutePosition();
		m_root->setPosition(pos);
	}
	return true;
}
const math::vector3d& NullPhysicsComponent::GetPosition()
{
	return m_position;
}

bool NullPhysicsComponent::SetRotation(const math::vector3d& v)
{
	m_rotation=v;
	if(!m_root)
		return true;
	scene::IMovable* m= m_root->getParent();
	math::quaternion q=v;
	//if(m)
	//	q=m->getAbsoluteOrintation().inverse()*q;
	if(m_root)
		m_root->setOrintation(q);
	return true;
}

const math::vector3d& NullPhysicsComponent::GetRotation()
{
	return m_rotation;
}
}
}


