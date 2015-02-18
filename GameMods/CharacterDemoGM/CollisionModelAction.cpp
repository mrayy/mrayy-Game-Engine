
#include "stdafx.h"
#include "CollisionModelAction.h"
#include "PhysicalObject.h"

#include "IPhysical3DDef.h"
#include "IPhysicManager.h"
#include "GameLevel.h"
#include "GameEntityManager.h"
#include "CHouse.h"

namespace mray
{
namespace GameMod
{

CollisionModelAction::CollisionModelAction():IGameAction(mT("CollisionModelAction")),m_phNode(0)
{
	m_physics=new AI::AIPhysicsProxy(0);
}

CollisionModelAction::~CollisionModelAction()
{
	if(m_phNode && m_owner){
		CHouse::getInstance().RemoveCollisionGround(m_phNode);
		m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->removeNode(m_phNode);
	}
}


void CollisionModelAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner )
	{
		scene::ISceneNode*node=dynamic_cast<scene::ISceneNode*>(m_owner->GetShape());
		if(!node)
			return;
		scene::SMesh*mesh= node->getMesh();
		if(!mesh || !mesh->getBuffersCount())
			return;
		physics::PhysicalNodeDesc nDesc;
		core::array<physics::TriangleShapeDesc> cDesc;
		cDesc.resize(mesh->getBuffersCount());
		
		node->updateAbsoluteTransformation();
		nDesc.globalPos=node->getAbsoluteTransformation();

		physics::IPhysicManager*phMngr=m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager();
		
		for (int i=0;i<cDesc.size();++i)
		{
			cDesc[i].mesh=mesh->getBuffer(i);
		//	cDesc[i].material=phMngr->createMaterial
			nDesc.shapes.push_back(&cDesc[i]);
		}
		m_phNode=phMngr->createNode(&nDesc);

		CHouse::getInstance().AddCollisionGround(m_phNode);
		m_physics->SetPhysics(new PhysicalObject(m_phNode));
	}
}


void CollisionModelAction::Update(float dt)
{
	if(m_owner )
	{
		//m_physics->SetGlobalPosition(m_owner->GetShape()->getAbsolutePosition());
	}
}

}
}

