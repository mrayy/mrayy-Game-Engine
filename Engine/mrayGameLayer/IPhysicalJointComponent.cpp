


#include "stdafx.h"
#include "IPhysicalJointComponent.h"
#include "PhysicsComponent.h"
#include "IPhysicalJoint.h"
#include "IPhysicManager.h"


namespace mray
{
namespace game
{


	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode1,IPhysicalJointComponent,core::string,mT("TargetNodeA"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode1,IPhysicalJointComponent,core::string,SetTargetNameA,GetTargetNameA,,,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode2,IPhysicalJointComponent,core::string,mT("TargetNodeB"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode2,IPhysicalJointComponent,core::string,SetTargetNameB,GetTargetNameB,,,false);


	IPhysicalJointComponent::IPhysicalJointComponent()
	{
		m_targetComp1=0;
		m_targetComp2=0;
		CPropertieDictionary* dic;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeTargetNode1::instance);
			dic->addPropertie(&PropertyTypeTargetNode2::instance);
		}
	}
	IPhysicalJointComponent::~IPhysicalJointComponent()
	{
	}

	void IPhysicalJointComponent::OnAttachedToComponent(IObjectComponent* owner)
	{
		IGameComponent::OnAttachedToComponent(owner);
		_OnTargetChanged();
	}
	void IPhysicalJointComponent::OnRemovedFromComponent(IObjectComponent* owner)
	{
		IGameComponent::OnRemovedFromComponent(owner);
		if(GetJoint())
		{
			GetJoint()->GetCreator()->removeJoint(GetJoint());
			_JointRemoved();
		}
	}

	void IPhysicalJointComponent::_OnTargetChanged()
	{
		if(!m_inited)
			return;
		if(m_ownerComponent && (!m_targetComp1 || !m_targetComp2 ||
			m_targetComp1->GetName()!=m_targetNode1 &&
			m_targetComp2->GetName()!=m_targetNode2))
		{
			const std::list<IObjectComponent*>& lst =m_ownerComponent->GetComponentsList();
			std::list<IObjectComponent*>::const_iterator it=lst.begin();
			for(;it!=lst.end();++it)
			{
				IPhysicsComponent* comp=dynamic_cast<IPhysicsComponent*>(*it);
				if(comp && comp->GetName()==m_targetNode1)
				{
					m_targetComp1=comp;
				}
				if(comp && comp->GetName()==m_targetNode2)
				{
					m_targetComp2=comp;
				}
			}
		}
	}
	bool IPhysicalJointComponent::InitComponent()
	{
		if(!IGameComponent::InitComponent())
			return false;
		_OnTargetChanged();
		return true;
	}
	IObject* IPhysicalJointComponent::GetAttachedObject()
	{
		return GetJoint();
	}
	bool IPhysicalJointComponent::SetTargetNameA(const core::string& name)
	{
		m_targetNode1=name;
		_OnTargetChanged();
		return true;
	}
	const core::string& IPhysicalJointComponent::GetTargetNameA()const
	{
		return m_targetNode1;
	}

	bool IPhysicalJointComponent::SetTargetNameB(const core::string& name)
	{
		m_targetNode2=name;
		_OnTargetChanged();
		return true;
	}
	const core::string& IPhysicalJointComponent::GetTargetNameB()const
	{
		return m_targetNode2;
	}



}
}