

#include "stdafx.h"
#include "MountableComponent.h"
#include "SceneComponent.h"
#include "IDebugDrawManager.h"
#include "DebugRenderSettings.h"


namespace mray
{
namespace VT
{

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode,MountableComponent,core::string,mT("TargetNode"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode,MountableComponent,core::string,SetNodeName,GetNodeName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Offset,MountableComponent,math::vector3d,mT("Offset"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Offset,MountableComponent,math::vector3d,SetOffset,GetOffset,core::StringConverter::toString,core::StringConverter::toVector3d,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Angles,MountableComponent,math::vector3d,mT("Angles"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Angles,MountableComponent,math::vector3d,SetAngles,GetAngles,core::StringConverter::toString,core::StringConverter::toVector3d,false);


MountableComponent::MountableComponent(game::GameEntityManager*m)
{
	m_node=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeOffset::instance);
		dic->addPropertie(&PropertyTypeAngles::instance);
	}
}
MountableComponent::~MountableComponent()
{
}

bool MountableComponent::InitComponent()
{
	game::IGameComponent::InitComponent();

	game::IMovableComponent* comp = RetriveComponent<game::IMovableComponent>(GetOwnerComponent(), m_targetNode);
	if(comp)
	{
		comp->InitComponent();
		m_node=comp->GetMovableObject();
	}

	return true;
}

bool MountableComponent::SetNodeName(const core::string &name)
{
	m_targetNode=name;
	return true;
}
const core::string& MountableComponent::GetNodeName()
{
	return m_targetNode;
}

bool MountableComponent::SetOffset(const math::vector3d &v)
{
	m_offset=v;
	return true;
}
const math::vector3d& MountableComponent::GetOffset()
{
	return m_offset;
}

bool MountableComponent::SetAngles(const math::vector3d &v)
{
	m_angles=v;
	m_orientation.fromEulerAngles(v.x,v.y,v.z);
	m_orientation.Normalize();
	return true;
}
const math::vector3d& MountableComponent::GetAngles()
{
	return m_angles;
}


math::vector3d MountableComponent::GetAbsolutePosition()
{
	if(!m_node)
		return m_offset;
	return m_node->getAbsolutePosition()+m_node->getAbsoluteOrintation()*m_offset;
}
math::quaternion MountableComponent::GetAbsoluteOrientation()
{
	if(!m_node)
		return m_orientation;
	return m_node->getAbsoluteOrintation()*m_orientation;
}


void MountableComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	if(m_node)
	{
		math::vector3d pos=GetAbsolutePosition();

		renderer->AddCross(pos,10*DebugRenderSettings::Scale,video::DefaultColors::Green);
	}
	game::IGameComponent::DebugRender(renderer);
}


}
}

