#include "stdafx.h"

#include "GridSpacePartition.h"
#include "ILogManager.h"
#include "IAIPhysics.h"
#include "IRenderDevice.h"
#include <DefaultColors.h>


namespace mray{
namespace AI{

GridSpacePartition::GridSpacePartition()
{
	m_cellSize=5;
}
GridSpacePartition::~GridSpacePartition()
{
	m_cells.clear();
}

void GridSpacePartition::SetWorldAABB(const math::box3d& box)
{
	m_worldBB=box;
}
const math::box3d& GridSpacePartition::GetWorldAABB()
{
	return m_worldBB;
}

void GridSpacePartition::SetCellSize(float sz)
{
	m_cellSize=sz;
}

float GridSpacePartition::GetCellSize()
{
	return m_cellSize;
}


math::vector2di  GridSpacePartition::GetCellIndex(float x,float y,float z)
{
	math::vector3d p(x,y,z);

	p-=m_worldBB.MinP;
	int r=p.x/m_cellSize;
	int h=p.y/m_cellSize;
	int c=p.z/m_cellSize;

	if(r<0)
		r=0;
	else if(r>=m_cellsPerEdge.x)
		r=m_cellsPerEdge.x-1;

	if(h<0)
		h=0;
	else if(h>=m_cellsPerEdge.y)
		h=m_cellsPerEdge.y-1;
	if(c<0)
		c=0;
	else if(c>=m_cellsPerEdge.z)
		c=m_cellsPerEdge.z-1;

	return math::vector2di(r*m_cellsPerEdge.z+c,h);
}

void GridSpacePartition::RemoveFromYCell(AIActor*actor,int cell,int ycell)
{
	AIActorList&lst=m_cells[cell][ycell];
	AIActorList::iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		if(*it==actor)
		{
			lst.erase(it);
			return;
		}
	}

}
void GridSpacePartition::RemoveFromCell(AIActor*actor,int idx)
{
	std::vector<AIActorList>&cell=m_cells[idx];
	for(int i=0;i<cell.size();++i){
		AIActorList&lst=cell[i];
		AIActorList::iterator it=lst.begin();
		for (;it!=lst.end();++it)
		{
			if(*it==actor)
			{
				lst.erase(it);
				return;
			}
		}
	}
}
void GridSpacePartition::Create()
{
	m_cells.clear();
	if(m_cellSize==0){
		gLogManager.log(mT("GridSpacePartition::Create()-cellSize=0!"),ELL_ERROR);
		return;
	}
	m_cellsPerEdge.x=m_worldBB.getWidth();
	m_cellsPerEdge.y=m_worldBB.getHeight();
	m_cellsPerEdge.z=m_worldBB.getDepth();
	m_cellsPerEdge/=m_cellSize;
	m_cellsPerEdge+=1;

	m_cells.resize(m_cellsPerEdge.x*m_cellsPerEdge.z);
	for(int i=0;i<m_cells.size();++i)
	{
		m_cells[i].resize(m_cellsPerEdge.y);
	}
}

void GridSpacePartition::AddActor(AIActor*actor)
{
	if(!actor->GetPhysicalProp())return;
	math::vector3d pos=actor->GetPhysicalProp()->GetGlobalPosition();
	math::vector2di idx(GetCellIndex(pos.x,pos.y,pos.z));
	m_cells[idx.x][idx.y].push_back(actor);

	m_actorsCells.insert(ActorMapList::value_type(actor,idx));
}


void GridSpacePartition::RemoveActor(AIActor*actor)
{

	ActorMapList::iterator ac=m_actorsCells.find(actor);
	if(ac==m_actorsCells.end())
		return;

	math::vector2di idx=ac->second;
	m_actorsCells.erase(ac);

	AIActorList&lst=m_cells[idx.x][idx.y];

	AIActorList::iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		if(*it==actor)
		{
			lst.erase(it);
			break;
		}
	}
}
void GridSpacePartition::Update()
{
	ActorMapList::iterator it=m_actorsCells.begin();
	for (;it!=m_actorsCells.end();++it)
	{
		AIActor*actor=it->first;
		math::vector3d pos=actor->GetPhysicalProp()->GetGlobalPosition();
		math::vector2di idx;
		idx=GetCellIndex(pos.x,pos.y,pos.z);
		if(idx!=it->second)
		{
			RemoveFromYCell(actor,it->second.x,it->second.y);
			it->second=idx;
			m_cells[idx.x][idx.y].push_back(actor);
		}
	}
}

void GridSpacePartition::QueryActors(const math::box3d& box,AIActorList&actors)
{
	actors.clear();

	math::box3d b=box-m_worldBB.MinP;
	int r1=b.MinP.x/m_cellSize;
	int h1=b.MinP.y/m_cellSize;
	int c1=b.MinP.z/m_cellSize;
	int r2=b.MaxP.x/m_cellSize;
	int h2=b.MaxP.y/m_cellSize;
	int c2=b.MaxP.z/m_cellSize;

	if(r1<0)r1=0;
	else if(r1>=m_cellsPerEdge.x)
		r1=m_cellsPerEdge.x-1;

	if(h1<0)h1=0;
	else if(h1>=m_cellsPerEdge.y)
		h1=m_cellsPerEdge.y-1;

	if(c1<0)c1=0;
	else if(c1>=m_cellsPerEdge.z)
		c1=m_cellsPerEdge.z-1;

	if(r2<0)r2=0;
	else if(r2>=m_cellsPerEdge.x)
		r2=m_cellsPerEdge.x-1;

	if(h2<0)h2=0;
	else if(h2>=m_cellsPerEdge.y)
		h2=m_cellsPerEdge.y-1;

	if(c2<0)c2=0;
	else if(c2>=m_cellsPerEdge.z)
		c2=m_cellsPerEdge.z-1;

	for (int j=r1;j<=r2;++j)
	{
		int r=j*m_cellsPerEdge.z;
		for (int i=c1;i<=c2;++i)
		{
			std::vector<AIActorList>&cell=m_cells[r+i];
			for(int k=h1;k<=h2;++k)
			{
				AIActorList&lst=cell[k];
				AIActorList::iterator it=lst.begin();
				for (;it!=lst.end();++it)
				{
					actors.push_back(*it);
				}
			}
		}
	}
}

void GridSpacePartition::Visualize(IRenderDevice*dev)
{
	math::line3d l;
	float step=0;
	l.pStart.x=m_worldBB.MinP.x;
	l.pEnd.x=m_worldBB.MinP.x;
	l.pStart.z=m_worldBB.MinP.z;
	l.pEnd.z=m_worldBB.MaxP.z;
	for(int i=0;i<m_cellsPerEdge.x;++i)
	{
		dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Green);
		l.pStart.x+=m_cellSize;
		l.pEnd.x=l.pStart.x;
	}

	l.pStart.z=m_worldBB.MinP.z;
	l.pEnd.z=m_worldBB.MinP.z;
	l.pStart.x=m_worldBB.MinP.x;
	l.pEnd.x=m_worldBB.MaxP.x;
	for(int j=0;j<m_cellsPerEdge.z;++j)
	{
		dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Green);
		l.pStart.z+=m_cellSize;
		l.pEnd.z=l.pStart.z;
	}
}


}
}


