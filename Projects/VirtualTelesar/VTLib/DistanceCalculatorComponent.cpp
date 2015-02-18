

#include "stdafx.h"
#include "DistanceCalculatorComponent.h"
#include "MountableComponent.h"


namespace mray
{
namespace VT
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode1,DistanceCalculatorComponent,core::string,mT("TargetNode1"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode1,DistanceCalculatorComponent,core::string,SetNode1Name,GetNode1Name,,,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode2,DistanceCalculatorComponent,core::string,mT("TargetNode2"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode2,DistanceCalculatorComponent,core::string,SetNode2Name,GetNode2Name,,,false);

DistanceCalculatorComponent::DistanceCalculatorComponent(game::GameEntityManager* mngr)
{
	m_comp1=0;
	m_comp2=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode1::instance);
		dic->addPropertie(&PropertyTypeTargetNode2::instance);
	}
}
DistanceCalculatorComponent::~DistanceCalculatorComponent()
{
}

bool DistanceCalculatorComponent::InitComponent()
{
	if(!game::IGameComponent::InitComponent())
		return false;

	m_comp1=RetriveComponent<MountableComponent>(GetOwnerComponent(),m_target1);
	m_comp2=RetriveComponent<MountableComponent>(GetOwnerComponent(),m_target2);

	return true;
}

bool DistanceCalculatorComponent::SetNode1Name(const core::string &name)
{
	m_target1=name;
	return true;
}
const core::string& DistanceCalculatorComponent::GetNode1Name()
{
	return m_target1;
}

bool DistanceCalculatorComponent::SetNode2Name(const core::string &name)
{
	m_target2=name;
	return true;
}
const core::string& DistanceCalculatorComponent::GetNode2Name()
{
	return m_target2;
}

float DistanceCalculatorComponent::GetDistance()
{
	if(!m_comp1 || !m_comp2)
		return 0;
	return m_comp1->GetAbsolutePosition().getDist(m_comp2->GetAbsolutePosition());
}


void DistanceCalculatorComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	game::IGameComponent::DebugRender(renderer);
}


}
}
