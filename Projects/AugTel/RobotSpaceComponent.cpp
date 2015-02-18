

#include "stdafx.h"
#include "RobotSpaceComponent.h"
#include "ATAppGlobal.h"
#include "OptiTrackDataSource.h"
#include "IMovableComponent.h"


namespace mray
{
namespace AugTel
{
	IMPLEMENT_SETGET_PROP(RobotSpaceComponent, EyeNode, core::string, m_eyeNodeName, "", , );
	IMPLEMENT_SETGET_PROP(RobotSpaceComponent, TrackableID, int, m_trackID, -1, core::StringConverter::toInt, core::StringConverter::toString);

RobotSpaceComponent::RobotSpaceComponent(game::GameEntityManager*m)
{
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTrackableID::instance);
		dic->addPropertie(&PropertyTypeEyeNode::instance);
	}
	m_isVisible = false;
}
RobotSpaceComponent::~RobotSpaceComponent()
{
}

math::vector3d RobotSpaceComponent::TransformToWorld(const math::vector3d& pos)
{
	math::vector3d p = pos;
	if (m_eyeNode)
	{
		p=m_eyeNode->getAbsoluteTransformation().inverseTransformVector(p);
	}
	return m_pos + m_ori*p;
}
math::vector3d RobotSpaceComponent::TransformToEye(const math::vector3d& pos)
{

	math::vector3d p = pos;
	if (m_eyeNode)
	{
		p=m_eyeNode->getAbsoluteTransformation().inverseTransformVector(p);
	}
	return p;
}

bool RobotSpaceComponent::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;

	game::IMovableComponent* mcomp;
	mcomp = RetriveComponent<game::IMovableComponent>(m_ownerComponent, m_eyeNodeName);
	if (mcomp)
	{
		mcomp->InitComponent();
		m_eyeNode = mcomp->GetMovableObject();
	}
	return true;
}

void RobotSpaceComponent::Update(float dt)
{
	math::vector3d* pos = ATAppGlobal::Instance()->optiDataSource->GetPositionByID(m_trackID);
	if (!pos)
		m_isVisible = false;
	else
	{
		m_pos = *pos;
		m_ori = *ATAppGlobal::Instance()->optiDataSource->GetOrientationByID(m_trackID);
		m_isVisible = true;
	}
}

void RobotSpaceComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
}
void RobotSpaceComponent::OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp)
{
}



}
}
