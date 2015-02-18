#include "stdafx.h"

#include "TacticalPointManager.h"
#include "INavWorld.h"
#include "IRenderDevice.h"
#include "DefaultColors.h"
#include <map>
#include <algorithm>

namespace mray
{
namespace AI
{

TacticalPointManager::TacticalPointManager(INavWorld*world):m_world(world),m_isVisible(false)
{
	GenerateTacticalPoints();
	GenerateVisibilityInfo();
}

TacticalPointManager::~TacticalPointManager()
{
}

void TacticalPointManager::GenerateTacticalPoints()
{
	m_tacticalPoints.clear();
	m_nodePointsMap.clear();
	m_nodesID.clear();
	float distanceFromWall=3;	//how far the points should be from the wall

	int lastID=0;
	TacticalPoint point;
	const std::vector<WorldWall>& worldWalls= m_world->GetWorldWalls();
	for (int i=0;i<worldWalls.size();++i)
	{
		int nodeID=0;
		NodeIDMap::iterator it= m_nodesID.find(worldWalls[i].owner);
		if(it==m_nodesID.end())
		{
			nodeID=m_nodesID.size();
			m_nodesID[worldWalls[i].owner]=m_nodesID.size();
			m_nodePointsMap.push_back(std::vector<int>());
		}else
		{
			nodeID=it->second;
		}
		math::vector3d vec=(worldWalls[i].owner->GetPosition()-worldWalls[i].wallLine.getMiddle()).Normalize();
		point.owner=&(worldWalls[i]);
		point.pos=vec*distanceFromWall+worldWalls[i].wallLine.getMiddle();
		point.id=lastID;
		m_tacticalPoints.push_back(point);
		m_nodePointsMap[nodeID].push_back(lastID);
		++lastID;
	}
	//TODO: optimize tactical points depending on wall information
}

void TacticalPointManager::GetVisibleNodesVec(INavWorldNode*node,const math::vector3d&pos,core::BitVector&vec)
{
	NodeIDMap::iterator it= m_nodesID.find(node);
	if (it==m_nodesID.end())
	{
		return;
	}

	//get connected points for the node
	const std::vector<int>&nodesIDs=m_nodePointsMap[it->second];
	core::BitVector extendVec,surrondVec;
	core::BitVector immediateVec;
	core::BitVector tmp;

	//visibility vector is already calculated
	immediateVec=m_visibility[nodesIDs[0]];

	//check surronding points
	//1st , calc surronding points vector
	extendVec=ExtendVisibility(immediateVec);
	//surrondVec=(!immediateVec) & extendVec;

	//2nd , do line of sight for those surronding points

	for(int i=0;i<extendVec.getBitsUsed();++i)
	{
		if(extendVec[i])
		{
			extendVec.set(i,m_world->lineOfSightTest(pos,node,m_tacticalPoints[i].pos,m_tacticalPoints[i].owner->owner,0));
		}
	}

	vec=extendVec;
}


core::BitVector TacticalPointManager::ExtendVisibility(const core::BitVector&vec)
{
	core::BitVector res(vec);
	for(int i=0;i<vec.getBitsUsed();++i)
	{
		if(vec[i])
		{
			res|=m_visibility[i];
		}
	}
	return res;
}

void TacticalPointManager::GenerateVisibilityInfo()
{
	int cnt=m_tacticalPoints.size();
	m_visibility.resize(cnt);
	//create the vectors
	for (int i=0;i<cnt;++i)
	{
		m_visibility[i].resize(cnt);
	}
	//fill the vectors
	for (int i=0;i<cnt;++i)
	{
		TacticalPoint& start=m_tacticalPoints[i];
		m_visibility[i].set(i,true);//points can see their selfs
		for (int j=0;j<i;++j)
		{
			TacticalPoint& end=m_tacticalPoints[j];
			bool see=m_world->lineOfSightTest(start.pos,start.owner->owner,end.pos,end.owner->owner,0);
			if(see)
			{
				start.visiblePoints++;//increase visibility count
				end.visiblePoints++;//increase visibility count
			}
			m_visibility[i].set(j,see);
			m_visibility[j].set(i,see);
		}
	}

	//calculate sniping weights
	for (int i=0;i<cnt;++i)
	{
		int n=0;
		int cost=m_tacticalPoints[i].visiblePoints;//cost of this node (higher it is , better it is)
		float neightborCost=0;
		//scan neighbors ,lower cost and nearer it is, better it is
		for(int j=0;j<cnt;++j)
		{
			if(i!=j && m_visibility[i][j])
			{
				neightborCost+=m_tacticalPoints[j].visiblePoints*(m_tacticalPoints[i].pos-m_tacticalPoints[j].pos).LengthSQ()*0.0001f;
				++n;
			}
		}
		if(neightborCost==0)
		{
			m_tacticalPoints[i].snipingCost=0;
		}else
		{
			m_tacticalPoints[i].snipingCost=(float)cost/(neightborCost);
		}
	}
	//std::sort(m_snipWeights.begin(),m_snipWeights.end());
}
void TacticalPointManager::GetNodesFromVector(const core::BitVector&vec,std::vector<TacticalPoint*>&points)
{
	int cnt=math::Min<int>(vec.getBitsUsed(),m_tacticalPoints.size());
	for(int i=0;i<cnt;++i)
	{
		if(vec[i])
		{
			points.push_back(&m_tacticalPoints[i]);
		}
	}
}

void TacticalPointManager::Visualize(IRenderDevice* dev)
{

	for (int i=0;i<m_tacticalPoints.size();++i)
	{
		math::line3d l(m_tacticalPoints[i].pos,m_tacticalPoints[i].pos+math::vector3d(0,4,0));
		dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Yellow);
	}
}

}
}