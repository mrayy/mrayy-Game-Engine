

#include "stdafx.h"
#include "ContactDisablerComponent.h"
#include "GameEntityManager.h"
#include "PhysicsComponent.h"
#include "IPhysicalNode.h"



namespace mray
{
namespace VT
{

ContactDisablerComponent::ContactDisablerComponent(game::GameEntityManager*m)
{
	m_mngr=m->GetPhysicsManager();
}

ContactDisablerComponent::~ContactDisablerComponent()
{
}


bool ContactDisablerComponent::InitComponent()
{
	if(!IVTComponent::InitComponent())
		return false;

	const std::list<IObjectComponent*>& lst=GetOwnerComponent()->GetComponentsList();
	for (std::list<IObjectComponent*>::const_iterator it=lst.begin();
		it!=lst.end();++it)
	{
		game::PhysicsComponent* comp=dynamic_cast<game::PhysicsComponent*>(*it);
		if(!comp)
			continue;

		if(comp->GetPhysicalNode() && comp->GetIsDynamic() && !comp->GetBodyDesc()->kinematic)
			m_nodes.push_back(comp->GetPhysicalNode());
	}
	RecheckObjects();
	return true;
}



void ContactDisablerComponent::RecheckObjects()
{

	const std::list<physics::IPhysicalNode*>& ndoesLst=m_mngr->GetNodes();
	for(std::list<physics::IPhysicalNode*>::const_iterator it=ndoesLst.begin();
		it!=ndoesLst.end();++it)
	{
		physics::IPhysicalNode* n=*it;
		if(!n->isDynamic() || n->GetBodyFlag(physics::EBF_Kinematic))
		{
			for(int i=0;i<m_nodes.size();++i)
			{
				m_mngr->SetActorPairFlag(m_nodes[i],*it,physics::ECRF_IgnorePair,true);
			}
		}
	}
}

	

}
}


