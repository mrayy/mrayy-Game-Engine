#include "stdafx.h"

#include "Waypoint.h"


namespace mray
{
namespace AI
{

Waypoint::Waypoint()
{
	m_id=0;
	m_owner=0;
}
Waypoint::~Waypoint()
{
}

void Waypoint::SetOwner(WaypointPath*o)
{
	m_owner=o;
}
WaypointPath* Waypoint::GetOwner()
{
	return m_owner;
}
const core::string& Waypoint::GetName()const
{
	return m_name;
}
void Waypoint::SetName(const core::string&name)
{
	m_name=name;
}

uint Waypoint::GetID()
{
	return m_id;
}
void Waypoint::SetID(const uint&id)
{
	m_id=id;
}

const math::vector3d& Waypoint::GetPosition()const
{
	return m_pos;
}
const math::quaternion& Waypoint::GetOrintation()const
{
	return m_orintation;
}

void Waypoint::SetPosition(const math::vector3d& v)
{
	m_pos=v;
}
void Waypoint::SetOrintation(const math::quaternion& v)
{
	m_orintation=v;
}


}
}

