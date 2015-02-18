#include "stdafx.h"

#include "NavGraphLink.h"


namespace mray{
namespace AI{

NavGraphLink::NavGraphLink()
{
}
NavGraphLink::NavGraphLink(float weight,NavMeshPolygon*owner,NavGraphNode* begin,NavGraphNode* end,int id
						   /*,const core::string &name*/)
						   :IGraphLink(begin,end,id),m_ownerNode(owner)
{
	m_weight=begin->position.getDist(end->position)*weight;
}

NavGraphLink::~NavGraphLink()
{
}

bool NavGraphLink::isBidirectional()
{
	return true;
}

void NavGraphLink::setOwnerNode(NavMeshPolygon*node)
{
	m_ownerNode=node;
}
NavMeshPolygon* NavGraphLink::getOwnerNode()
{
	return m_ownerNode;
}

IGraphNode* NavGraphLink::GetNearestNode(const math::vector3d&p)
{
	float d1=p.getDistSQ(((NavGraphNode*)m_begin)->position);
	float d2=p.getDistSQ(((NavGraphNode*)m_end)->position);
	if(d1<d2)return m_begin;
	return m_end;
}
}
}
