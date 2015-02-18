
#include "stdafx.h"
#include "CoupledJointComponent.h"
#include "HeadCameraComponent.h"
#include "BoneComponent.h"


namespace mray
{
namespace AugTel
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode, HeadCameraComponent, core::string, mT("TargetNode"), EPBT_Basic, mT(""), mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode, HeadCameraComponent, core::string, SetNodeName, GetNodeName, , , false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Offset, HeadCameraComponent, math::vector3d, mT("Offset"), EPBT_Basic, mT(""), 0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Offset, HeadCameraComponent, math::vector3d, SetOffset, GetOffset, core::StringConverter::toString, core::StringConverter::toVector3d, false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Angles, HeadCameraComponent, math::vector3d, mT("Angles"), EPBT_Basic, mT(""), 0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Angles, HeadCameraComponent, math::vector3d, SetAngles, GetAngles, core::StringConverter::toString, core::StringConverter::toVector3d, false);

HeadCameraComponent::HeadCameraComponent(game::GameEntityManager*m) 
{
	m_node = 0;
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeOffset::instance);
		dic->addPropertie(&PropertyTypeAngles::instance);
	}
}

HeadCameraComponent::~HeadCameraComponent()
{
}


bool HeadCameraComponent::InitComponent()
{
	game::IGameComponent::InitComponent();

	game::IMovableComponent* comp = RetriveComponent<game::IMovableComponent>(GetOwnerComponent(), m_targetNode);
	if (comp)
	{
		comp->InitComponent();
		m_node = comp->GetMovableObject();
	}

	m_headAxis[0] = RetriveComponent<VT::CoupledJointComponent>(GetOwnerComponent(), "JointBody8");
	m_headAxis[1] = RetriveComponent<VT::CoupledJointComponent>(GetOwnerComponent(), "JointBody7");
	m_headAxis[2] = RetriveComponent<VT::CoupledJointComponent>(GetOwnerComponent(), "JointBody9");

	return true;
}


bool HeadCameraComponent::SetNodeName(const core::string &name)
{
	m_targetNode = name;
	return true;
}
const core::string& HeadCameraComponent::GetNodeName()
{
	return m_targetNode;
}

bool HeadCameraComponent::SetOffset(const math::vector3d &v)
{
	m_offset = v;
	return true;
}
const math::vector3d& HeadCameraComponent::GetOffset()
{
	return m_offset;
}

bool HeadCameraComponent::SetAngles(const math::vector3d &v)
{
	m_angles = v;
	m_orientation.fromEulerAngles(v.x, v.y, v.z);
	m_orientation.Normalize();
	return true;
}
const math::vector3d& HeadCameraComponent::GetAngles()
{
	return m_angles;
}


math::vector3d HeadCameraComponent::GetAbsolutePosition()
{
	if (!m_node)
		return m_offset;
	return m_node->getAbsolutePosition() + m_node->getAbsoluteOrintation()*m_offset;
}
math::quaternion HeadCameraComponent::GetAbsoluteOrientation()
{
	if (!m_node)
		return m_orientation;
	return m_node->getAbsoluteOrintation()*m_orientation;
}

void HeadCameraComponent::SetControlValue(const math::vector3d& r)
{
	m_headAxis[0]->SetControlValue(VT::ControlInputValues(r.x, r.x), VT::EControlSource::EControl_Realtime);
	m_headAxis[1]->SetControlValue(VT::ControlInputValues(r.y, r.y), VT::EControlSource::EControl_Realtime);
	m_headAxis[2]->SetControlValue(VT::ControlInputValues(r.z, r.z), VT::EControlSource::EControl_Realtime);
}


IMPLEMENT_RTTI(HeadCameraComponent,IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(HeadCameraComponent);

}
}

