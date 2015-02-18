
#include "stdafx.h"
#include "PhysicalObjectAction.h"
#include "PhysicalObject.h"

#include "IPhysical3DDef.h"
#include "IPhysicManager.h"
#include "GameLevel.h"
#include "GameEntityManager.h"

namespace mray
{
namespace GameMod
{

PhysicalObjectAction::PhysicalObjectAction():IGameAction(mT("PhysicalObjectAction")),m_phNode(0)
{
	m_physics=new AI::AIPhysicsProxy(0);
	m_radius=new FloatValue(mT("Radius"),1);
	AddAttribute(m_radius);
	m_i=0;
	m_targetAngle1.fromEulerAngles(-180,-3,-150);
	m_targetAngle2.fromEulerAngles(-180,-3,-220);
	m_useShortPath=true;
}
PhysicalObjectAction::~PhysicalObjectAction()
{/*
	if(m_phNode && m_owner)
		m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->removeNode(m_phNode);*/
}

void PhysicalObjectAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;/*
	if(m_owner )
	{
		physics::PhysicalNodeDesc nDesc;
		physics::PhysicalBodyDesc bDesc;
		physics::SphereShapeDesc cDesc;
		bDesc.mass=1;
		cDesc.radius=m_radius->value;
		nDesc.shapes.push_back(&cDesc);
		nDesc.bodyDesc=&bDesc;
		m_phNode=m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->createNode(&nDesc);

		m_physics->SetPhysics(new PhysicalObject(m_phNode));
	}*/
}

void PhysicalObjectAction::Update(float dt)
{
	if(m_owner && m_owner->GetShape()){
		m_i+=50*dt;
		if(m_i>=100)
			m_i=0;
		float w=m_i*0.01f;
		math::quaternion q1,q2,q;
		
		q1=m_targetAngle1;//.Slerp(math::quaternion::Identity,m_targetAngle1,w,m_useShortPath);
		q2.Slerp(q1,m_targetAngle2,1-w,m_useShortPath);
//		q2.Slerp(math::quaternion::Identity,m_targetAngle2,1-w,m_useShortPath);
// 		q=q1+q2;
// 		q.Normalize();

		m_owner->GetShape()->setOrintation(q2);


		/*if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode())
			m_physics->SetGlobalPosition(m_owner->GetShape()->getPosition());
		else
			m_owner->GetShape()->setPosition(m_physics->GetGlobalPosition());*/
	}
}


}
}
