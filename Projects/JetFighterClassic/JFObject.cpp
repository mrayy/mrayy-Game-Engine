

#include "stdafx.h"
#include "JFObject.h"
#include "PhysicsComponent.h"
#include "SceneComponent.h"

namespace mray
{
namespace game
{

JFObject::JFObject()
{
	m_immune=false;
}
JFObject::~JFObject()
{
}

void JFObject::SetImmune(bool immune)
{
	m_immune=immune;
}

bool JFObject::IsImmune()const
{
	return m_immune;
}


void JFObject::ReceivedDamage(float power)
{
	if(m_immune)
		return;

	float defenseFactor=pow(1.2f,m_defense);
	float damage=power/defenseFactor;
	m_health=math::Max(m_health-damage,0.0f);
	if(m_health==0)
	{
		OnJFDestroy();
	}

	FIRE_LISTENR_METHOD(OnDamageReceived,(this,damage));
}

void JFObject::OnJFCreate()
{
}

void JFObject::OnJFDestroy()
{
	FIRE_LISTENR_METHOD(OnDestroyed,(this));

}

void JFObject::Update(float dt)
{
	IGameComponent::Update(dt);

	if(m_Kinematic)
	{
		math::vector3d pos=GetPosition();
		pos+=m_speed*dt;
		SetPosition(pos);
	}
}


math::vector3d JFObject::GetPosition()
{
	math::vector3d pos=0;
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(PhysicsComponent::getClassRTTI());

	if(!lst.empty())
		pos=((PhysicsComponent*)*lst.begin())->GetPhysicalNode()->getGlobalPosition();
	return pos;
}

void JFObject::SetPosition(const math::vector3d& pos)
{
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(PhysicsComponent::getClassRTTI());

	if(!lst.empty())
		((PhysicsComponent*)*lst.begin())->GetPhysicalNode()->setGlobalPosition(pos);
}


math::quaternion JFObject::GetOrientation()
{
	math::quaternion o=0;
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(PhysicsComponent::getClassRTTI());

	if(!lst.empty())
		o=((PhysicsComponent*)*lst.begin())->GetPhysicalNode()->getGlobalOrintation();
	return o;
}

void JFObject::SetOrientation(const math::quaternion& ori)
{
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(PhysicsComponent::getClassRTTI());

	if(!lst.empty())
		((PhysicsComponent*)*lst.begin())->GetPhysicalNode()->setGlobalOrintation(ori);
}


math::vector3d JFObject::GetScale()
{
	math::vector3d scale=1;
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(SceneComponent::getClassRTTI());

	if(!lst.empty())
		scale=((SceneComponent*)*lst.begin())->GetSceneNode()->getScale();
	return scale;
}

void JFObject::SetScale(const math::vector3d& s)
{
	std::list<IGameComponent*> lst=GetOwnerEntity()->GetGameComponents(SceneComponent::getClassRTTI());

	if(!lst.empty())
		((SceneComponent*)*lst.begin())->GetSceneNode()->setScale(s);
}

void JFObject::OnAttachedToComponent(IObjectComponent* owner)
{
	IGameComponent::OnAttachedToComponent(owner);
	GameEntity* ent=dynamic_cast<GameEntity*>(owner);
	if(!ent)
		return ;


	std::list<IGameComponent*> lst;
	std::list<IGameComponent*>::iterator it;

	//convert physical objects to kinematic
	lst=ent->GetGameComponents(PhysicsComponent::getClassRTTI());
	for(it=lst.begin();it!=lst.end();++it)
	{
		((PhysicsComponent*)*it)->GetPhysicalNode()->SetBodyFlag(physics::EBF_Kinematic,true);
	}
}


}
}
