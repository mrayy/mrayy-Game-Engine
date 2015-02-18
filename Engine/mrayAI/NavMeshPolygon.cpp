#include "stdafx.h"

#include "NavMeshPolygon.h"


namespace mray{
namespace AI{



NavMeshPolygon::~NavMeshPolygon()
{
	for (int i=0;i<adjacent.size();++i)
	{
		if(adjacent[i]){
			adjacent[i]->poly->removeAdjacent(this);
			delete adjacent[i];
		}
	}
}

void NavMeshPolygon::createAdjList()
{
	adjList.clear();
	for (int i=0;i<adjacent.size();++i)
	{
		if(adjacent[i]){
			adjList.push_back(adjacent[i]->poly);
		}
	}
}


void NavMeshPolygon::removeAdjacent(NavMeshPolygon*p)
{
	for(int i=0;i<adjacent.size();++i){
		if(!adjacent[i])continue;
		if(adjacent[i]->poly==p){
			delete adjacent[i];
			adjacent[i]=0;
			return;
		}
	}
}

NavMeshPolygon::NavMeshPolyAdj* NavMeshPolygon::getAdjacent(NavMeshPolygon*p)
{

	for(int i=0;i<adjacent.size();++i){
		if(!adjacent[i])continue;
		if(adjacent[i]->poly==p)
			return adjacent[i];
	}
	return 0;
}
void NavMeshPolygon::setNumberOfEdges(int n)
{
	indicies.resize(n);
	adjacent.resize(n);
	for(int i=0;i<adjacent.size();++i)
		adjacent[i]=0;
}

bool NavMeshPolygon::isPointInside(const math::vector3d&p,const math::vector3d*points)
{
	uint i1,i2;
	for(int i=0;i<indicies.size();++i)
	{
		i1=indicies[i];
		i2=indicies[(i+1)%indicies.size()];
		if(math::vector3d(points[i2].x-points[i1].x,0,points[i2].z-points[i1].z).
			crossProduct(math::vector3d(p.x-points[i1].x,0,p.z-points[i1].z))
			.dotProduct(math::vector3d::YAxis)<0)
		{
			return false;
		}
	}
	return true;
}


bool NavMeshPolygon::isLineIntersect(const math::line3d&l,const math::vector3d*points,math::vector3d*intersection)
{
	math::vector3d dir=l.getVector();
	math::vector3d inter;
	if(!plane.getIntersectionWithLine(l.pStart,dir,inter))
		return false;
	if(!isPointInside(inter,points))
		return false;

	if(intersection)
		*intersection=inter;
	return true;
}

NavMeshPolygon* NavMeshPolygon::classifyPathToPoly(const math::line2d&path,const math::vector3d*points
								   ,int &edgeNum,math::vector2d*intersection)

{
	math::line2d edge;
	math::vector2d resP;
	for(int i=0;i<indicies.size();++i)
	{
		edge.pStart.set(points[indicies[i]].x,points[indicies[i]].z);
		edge.pEnd.set(points[indicies[(i+1)%indicies.size()]].x,points[indicies[(i+1)%indicies.size()]].z);
		if(path.IntersctWithLine(edge,resP))
		{
			if(intersection)
				*intersection=resP;
			if(adjacent[i]){
				return adjacent[i]->poly;
			}
			else
				return 0;
		}
	}
	return 0;
}


bool NavMeshPolygon::canRemoveEdge(int e,const math::vector3d*points)
{
	uint e1;
	uint i1,i2,i3;
	math::vector3d v;

	if(e>0){
		e1=e-1;
		i1=indicies[e1];
	}
	else{
		e1=indicies.size()-1;
		i1=indicies[e1];
	}
	i2=indicies[e];
	i3=indicies[(e+1)%indicies.size()];

	if(adjacent[e1]!=adjacent[e]){
		if(adjacent[e1] && adjacent[e]){
			if(adjacent[e1]->poly!=adjacent[e]->poly){
				return false;
			}
		}else{
			return false;
		}
	}
	v=(points[i1]-points[i2]);
	v.Normalize();
	if(abs(v.dotProduct((points[i3]-points[i2]).Normalize()))>0.99)
		return true;
	return false;
}

int NavMeshPolygon::getEdgeIndex(uint e)
{
	for(int i=0;i<indicies.size();++i)
	{
		if(indicies[i]==e)
			return i;
	}
	return -1;
}

int NavMeshPolygon::getEdgeIndex(uint i1,uint i2)
{

	uint tmp=i1;
	i1=math::Min(tmp,i2);
	i2=math::Max(tmp,i2);
	for(int i=0;i<indicies.size()+1;++i)
	{
		tmp=indicies[i];
		uint ii2=indicies[(i+1)%indicies.size()];
		uint ii1=math::Min(tmp,ii2);
		ii2=math::Max(tmp,ii2);
		if(ii1==i1 && ii2==i2)
			return i;
	}
	return -1;
}

bool NavMeshPolygon::removeEdge(uint e){
	uint e1;
	if(e>0){
		e1=e-1;
	}
	else{
		e1=indicies.size()-1;
	}
	if(adjacent[e1]!=adjacent[e]){
		if(adjacent[e1] && adjacent[e]){
			if(adjacent[e1]->poly!=adjacent[e]->poly){
				return false;
			}
		}else{
			return false;
		}
	//	return false;
	}
	std::vector<uint>::iterator it=indicies.begin();
	std::advance(it,e);
	indicies.erase(it);
	delete adjacent[e];
	adjacent[e]=0;

	NavMeshPolyAdjArr::iterator it2=adjacent.begin();
	std::advance(it2,e);
	adjacent.erase(it2);
	return true;
}

void NavMeshPolygon::calcAdjMidPoints(const math::vector3d*points)
{

	for(int i=0;i<adjacent.size();++i)
	{
		if(adjacent[i]){
			adjacent[i]->edgeMidpoint=(points[indicies[i]]+points[indicies[(i+1)%adjacent.size()]])*0.5;
// 			adjacent[i]->distance=center.getDist(adjacent[i]->edgeMidpoint)+
// 				adjacent[i]->poly->center.getDist(adjacent[i]->edgeMidpoint);
		}
	}
}

IGraphNode* NavMeshPolygon::GetNearestGraphNode(const math::vector3d& pos)
{
	float minDist=nodeCenter->position.getDistSQ(pos);
	IGraphNode* best=nodeCenter;
	if(nodeCenter->links.size()==0){
		minDist=math::Infinity;
	}
	for(int i=0;i<adjacent.size();++i)
	{
		if(adjacent[i]){
			float l=adjacent[i]->nodeEdge->position.getDistSQ(pos);
			if(l<minDist){
				minDist=l;
				best=adjacent[i]->nodeEdge;
			}
		}
	}
	return best;
}

}
}
