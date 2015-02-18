#include "stdafx.h"

#include "WaypointPath.h"
//#include "WaypointSceneNode.h"


namespace mray
{
namespace AI
{

WaypointPath::WaypointPath(bool createWPSceneNodes)
{
	m_lastID=0;
	m_visualizable=0;
	m_createWPSceneNodes=createWPSceneNodes;
}
WaypointPath::~WaypointPath()
{
}

const core::string& WaypointPath::GetName()const
{
	return m_name;
}
void WaypointPath::SetName(const core::string&name)
{
	m_name=name;
}


void WaypointPath::AddWaypoint(const Waypoint&p)
{
	m_waypoints.push_back(p);
	m_waypoints[m_waypoints.size()-1].SetOwner(this);
	m_waypoints[m_waypoints.size()-1].SetID(++m_lastID);
}

void WaypointPath::RemoveWaypoint(int pos)
{
	if(pos>=m_waypoints.size())
		return;

	WaypointList::iterator it=m_waypoints.begin()+pos;
	m_waypoints.erase(it);
}

void WaypointPath::RemoveWaypointById(int id)
{
	WaypointList::iterator it=m_waypoints.begin();
	for(;it!=m_waypoints.end();++it){
		if(it->GetID()==id){
			m_waypoints.erase(it);
			return;
		}
	}
}

void WaypointPath::Clear()
{
	m_waypoints.clear();
	m_lastID=0;
}

int WaypointPath::GetWaypointsCount()
{
	return m_waypoints.size();
}

Waypoint* WaypointPath::GetWaypoint(int i)
{
	if(i>=m_waypoints.size())
		return 0;

	return &m_waypoints[i];
}
Waypoint* WaypointPath::GetWaypointById(int id)
{
	WaypointList::iterator it=m_waypoints.begin();
	for(;it!=m_waypoints.end();++it){
		if(it->GetID()==id)
			return &(*it);
	}
	return 0;
}


void WaypointPath::EnableVisualize(bool e)
{
	m_visualizable=e;
/*	WaypointSceneNodes::iterator it=m_wpSceneNodes.begin();
	for(;it!=m_wpSceneNodes.end();++it)
	{
		//it->second->Visible=e;
	}*/
}
bool WaypointPath::IsEnableVisualize()
{
	return m_visualizable;
}
void WaypointPath::Visualize(IRenderDevice*dev)
{

}


void WaypointPath::loadXMLSettings(xml::XMLElement* e)
{
}
xml::XMLElement* WaypointPath::exportXMLSettings(xml::XMLElement* elem)
{
	return 0;
}

}
}
