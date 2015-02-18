

#include "stdafx.h"
#include "ShotingObjectAction.h"


#include "IPhysical3DDef.h"
#include "IPhysicManager.h"
#include "GameLevel.h"
#include "GameEntityManager.h"
#include "SceneManager.h"


namespace mray
{
namespace GameMod
{

ShotingObjectAction::ShotingObjectAction(float radius,const math::vector3d& force,EShotingObjectType type):game::IGameAction(mT("ShotingObjectAction"))
{
	m_age=0;
	m_phNode=0;
	m_owner=0;

	m_type=type;

	m_radius=radius;
	m_force=force;
}
ShotingObjectAction::~ShotingObjectAction()
{
	deleteBall();
}


void ShotingObjectAction::deleteBall()
{
	if(m_phNode && m_owner){
		m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->removeNode(m_phNode);
		//m_owner->GetGameEntityManager()->GetGameLevel()->GetSceneManager()->removeChild((scene::IRenderable*)m_owner->GetShape());
		m_owner->GetGameEntityManager()->GetGameLevel()->GetRootSceneNode()->removeChild(m_owner->GetShape());
	}
}

void ShotingObjectAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner )
	{
		physics::PhysicalNodeDesc nDesc;
		physics::PhysicalBodyDesc bDesc;
		physics::SphereShapeDesc cDesc;
		physics::BoxShapeDesc bsDesc;

		bDesc.mass=1;
		if(m_type==ESOT_Ball){
			cDesc.radius=m_radius;
			nDesc.shapes.push_back(&cDesc);
		}else
		{
			bsDesc.extent =m_radius;
			nDesc.shapes.push_back(&bsDesc);
		}
		nDesc.bodyDesc=&bDesc;

		m_owner->GetShape()->updateAbsoluteTransformation();
		nDesc.globalPos=m_owner->GetShape()->getAbsoluteTransformation();
		m_phNode=m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->createNode(&nDesc);
		m_phNode->setLinearVelocity(m_force);
	}
}

void ShotingObjectAction::Update(float dt)
{
	if(m_owner && m_owner->GetShape()){
		if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode())
			m_owner->GetGameEntityManager()->RequestToRemove(m_owner);
		else
			m_owner->GetShape()->setPosition(m_phNode->getGlobalPosition());
			m_owner->GetShape()->setOrintation(m_phNode->getGlobalOrintation());
	}
	m_age+=dt;

	if(m_age>10)
		m_owner->GetGameEntityManager()->RequestToRemove(m_owner);
}


}
}


