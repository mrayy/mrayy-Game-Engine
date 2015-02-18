

#include "stdafx.h"
#include "HierarchyComponent.h"
#include "IPhysicalJointComponent.h"
#include "PhysicsComponent.h"
#include "GameEntityManager.h"
#include "IPhysicManager.h"
#include "IGUIElement.h"
#include "IGUIManager.h"
#include "IGUIPanelElement.h"

#include "IPhysicalJointComponent.h"


namespace mray
{
namespace game
{

HierarchyComponent::HierarchyComponent(game::GameEntityManager*mngr)
{
	m_mngr=mngr;
	m_innerCollision=false;
}

HierarchyComponent::HierarchyComponent()
{
}
HierarchyComponent::~HierarchyComponent()
{
	AttachmentMap::iterator it=m_attachments.begin();
	for(;it!=m_attachments.end();++it)
		delete it->second;
	m_attachments.clear();
}

IPhysicsComponent* HierarchyComponent::InsertPair(IPhysicalJointComponent* joint)
{
	IPhysicsComponent*a=joint->GetTargetComponentA();
	IPhysicsComponent* b=joint->GetTargetComponentB();
	IPhysicsComponent* parent=0;
	AttachmentInfo* aIfo=0;
	AttachmentInfo* bIfo=0;
	AttachmentMap::iterator it;
	it=m_attachments.find(a);
	if(it==m_attachments.end())
	{
		aIfo=new AttachmentInfo();
		aIfo->comp=a;
		m_attachments[a]=aIfo;
	}else
		aIfo=it->second;
	it=m_attachments.find(b);
	if(it==m_attachments.end())
	{
		bIfo=new AttachmentInfo();
		bIfo->comp=b;
		m_attachments[b]=bIfo;
	}else
		bIfo=it->second;

	ChainInfo* c=new ChainInfo();
	if(bIfo->start==0)
	{
		c->link=bIfo;
		bIfo->start=c;
		bIfo->joint=joint;
		bIfo->parent=aIfo;
		aIfo->children.push_back(c);
		parent=a;
	}else
	{
		c->link=aIfo;
		aIfo->start=c;
		aIfo->joint=joint;
		aIfo->parent=bIfo;
		bIfo->children.push_back(c);
		parent=b;
	}
	return parent;
}

template <class T>
void HierarchyComponent_GetComponentsByType(IObjectComponent* c,std::vector<T*>& olst)
{
	const std::list<IObjectComponent*>& lst=c->GetComponentsList();
	for(std::list<IObjectComponent*>::const_iterator it=lst.begin();it!=lst.end();++it)
	{
		T* c=dynamic_cast<T*>(*it);
		if(c)
		{
			olst.push_back(c);
		}
	}
}


bool HierarchyComponent::InitComponent()
{
	if(!game::IGameComponent::InitComponent())
		return false;

	if(!m_ownerComponent)
		return false;
	{
		const std::list<IObjectComponent*>& lst=m_ownerComponent->GetComponentsList();
		for(std::list<IObjectComponent*>::const_iterator it=lst.begin();it!=lst.end();++it)
		{
			game::IPhysicalJointComponent* c=dynamic_cast<game::IPhysicalJointComponent*>(*it);
			if(c)
			{
				InsertPair(c);
		//		m_joints.push_back(c);
			}
		}
	}
	//if(false)
	{
		HierarchyComponent_GetComponentsByType<game::IPhysicsComponent>(m_ownerComponent,m_physList);
	
		if(!m_innerCollision)
		{
			physics::IPhysicManager* phMngr=m_mngr->GetPhysicsManager();
			for(int i=0;i<m_physList.size();++i)
			{
				m_physList[i]->InitComponent();
				PhysicsComponent* a=dynamic_cast<PhysicsComponent*>(m_physList[i]);
				if(!a)
					continue;

				for(int j=0;j<m_physList.size();++j)
				{
					if(i==j)
						continue;
					m_physList[j]->InitComponent();
					PhysicsComponent* b=dynamic_cast<PhysicsComponent*>(m_physList[j]);
					if(!b)
						continue;
					phMngr->SetActorPairFlag(a->GetPhysicalNode(),b->GetPhysicalNode(),physics::ECRF_IgnorePair,true);
				}
			}
		}
	}

	return true;
}
/*
void HierarchyComponent::_Update(AttachmentInfo*ifo)
{
	physics::IPhysicalNode* node= ifo->comp->GetPhysicalNode();
	math::quaternion q=ifo->comp->GetRotation();
	math::vector3d pos;
	if(node)
	{
		//q=node->getGlobalOrintation();
		pos=node->getGlobalPosition();
	}
	for (std::list<ChainInfo*>::iterator it=ifo->children.begin();it!=ifo->children.end();++it)
	{
		PhysicsComponent* comp= (*it)->link->comp;
		node=comp->GetPhysicalNode();
		IPhysicalJointComponent* joint=(*it)->link->joint;

		node->moveGlobalOrientation(q*math::quaternion(comp->GetRotation()));
		physics::IPhysicalJoint3DDesc* desc= joint->GetJointDesc();
		node->moveGlobalPosition(pos+q*desc->localAnchor[0]);

		_Update((*it)->link);
	}
}

void HierarchyComponent::Update(float dt)
{
	for (AttachmentMap::iterator it=m_attachments.begin();it!=m_attachments.end();++it)
	{
		if(it->second->parent==0)
			_Update(it->second);
	}
}*/

/*
void HierarchyComponent::ClearGUIElements()
{

	for(int i=0;i<m_guiElements.size();++i)
	{
		GUI::IGUIPanelElement*m= m_guiElements[i].elem->GetParent();
		if(m)
			m->RemoveElement(m_guiElements[i].elem);
	}
	m_guiElements.clear();
}
void HierarchyComponent::GenerateGUIElements(GUI::IGUIManager*mngr)
{
	ClearGUIElements();
	if(mngr)
	{
		const std::list<IObjectComponent*>& lst=m_ownerComponent->GetComponentsList();
		for(std::list<IObjectComponent*>::const_iterator it=lst.begin();it!=lst.end();++it)
		{
			IVTComponent* c=dynamic_cast<IVTComponent*>(*it);
			if(c)
			{
				GUI::IGUIElement*e=c->GeneratedUIElement(mngr);
				if(e)
				{
					GUIElementInfo ifo;
					ifo.comp=c;
					ifo.elem=e;
					m_guiElements.push_back(ifo);
				}
			}
		}
	}

}
*/
}
}

