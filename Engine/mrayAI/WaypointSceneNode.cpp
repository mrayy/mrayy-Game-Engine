



#include "WaypointSceneNode.h"
#include "WaypointPath.h"

namespace mray
{
namespace scene
{

WaypointSceneNode::WaypointSceneNode(AI::Waypoint* point):EditorSceneNode(0,mT(""),0),m_point(point)
{
}
WaypointSceneNode::~WaypointSceneNode()
{
	if(m_point && m_point->GetOwner())
	{
		m_point->GetOwner()->RemoveWaypointById(m_point->GetID());
	}
}

AI::Waypoint* WaypointSceneNode::GetWaypoint()
{
	return m_point;
}
void WaypointSceneNode::SetWaypoint(AI::Waypoint*wp)
{
	m_point=wp;
}


math::vector3d WaypointSceneNode::getPosition()const
{
	if(m_point)
		return m_point->GetPosition();
	return math::vector3d::Zero;
}
math::quaternion WaypointSceneNode::getOrintation()const
{
	if(m_point)
		return m_point->GetOrintation();
	return math::quaternion::Identity;
}

void WaypointSceneNode::setPosition(const math::vector3d& v)
{
	if(m_point)
		m_point->SetPosition(v);
}
void WaypointSceneNode::setOrintation(const math::quaternion& q)
{
	if(m_point)
		m_point->SetOrintation(q);
}

}
}