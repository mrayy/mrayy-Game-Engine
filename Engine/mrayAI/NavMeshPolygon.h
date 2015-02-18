

/********************************************************************
	created:	2010/02/18
	created:	18:2:2010   13:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavMeshPolygon.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavMeshPolygon
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshPolygon___
#define ___NavMeshPolygon___


#include "compileConfig.h"
#include "marray.h"

#include "Plane.h"
#include "box3d.h"
#include "Point2d.h"
#include "line2d.h"
#include "INavWorldNode.h"
#include "NavGraphNode.h"

namespace mray{
namespace AI{

class MRAY_AI_DLL NavMeshPolygon:public INavWorldNode
{
public:
	struct NavMeshPolyAdj
	{
		NavMeshPolyAdj():poly(0),nodeEdge(0)
		{
		}
		NavMeshPolyAdj(NavMeshPolygon* p):poly(p),nodeEdge(0)
		{
		}
		NavMeshPolyAdj(const NavMeshPolyAdj&o):
			poly(o.poly),edgeMidpoint(o.edgeMidpoint),nodeEdge(0)
		{
		}/*
		NavMeshPolyAdj(NavMeshPolygon* p,uint e,bool isShared=true)
		{
			poly=p;
			edge=e;
			this->isShared=isShared;
		}*/
		NavMeshPolyAdj(NavMeshPolygon* p,const math::vector3d&mid):
			poly(p),edgeMidpoint(mid),nodeEdge(0)
		{
		}

		NavMeshPolygon* poly;
		//bool isShared;
		math::vector3d edgeMidpoint;
		NavGraphNode* nodeEdge;
	};
	NavMeshPolygon():nodeCenter(0),polyID(-1),attribute(0)
	{
	}
	virtual~NavMeshPolygon();

	//typedef std::list<NavMeshPolyAdj*> NavMeshPolyAdjList;
	//typedef std::vector<NavMeshPolyAdjList> NavMeshPolyAdjArr;
	typedef std::vector<NavMeshPolyAdj*> NavMeshPolyAdjArr;

	NavMeshPolyAdjArr adjacent;
	NavWorldNodeListPtr adjList;

	int polyID;

	int attribute;//surface attribute
	NavGraphNode* nodeCenter;
	math::vector3d center;
	math::box3d boundingbox;
	std::vector<uint> indicies;
	math::Plane plane;

//methods
	const math::vector3d& GetPosition()
	{
		return center;
	}
	virtual IGraphNode* GetGraphNode()
	{
		return nodeCenter;
	}
	virtual IGraphNode* GetNearestGraphNode(const math::vector3d& pos);
	virtual NavWorldNodeListPtr& GetAdjacents()
	{
		return adjList;
	}
	void setNumberOfEdges(int n);


	inline void getEdge(int e,uint &i1,uint i2)
	{
		i1=indicies[e];
		i2=indicies[(e+1)%indicies.size()];
	}

	void createAdjList();

	void removeAdjacent(NavMeshPolygon*p);
	NavMeshPolyAdj* getAdjacent(NavMeshPolygon*p);
	bool isPointInside(const math::vector3d&p,const math::vector3d*points);
	bool isLineIntersect(const math::line3d&l,const math::vector3d*points,math::vector3d*intersection);

	NavMeshPolygon* classifyPathToPoly(const math::line2d&path,const math::vector3d*points
		,int &edgeNum,math::vector2d*intersection);
	bool canRemoveEdge(int e,const math::vector3d*points);
	int getEdgeIndex(uint e);
	int getEdgeIndex(uint i1,uint i2);
	bool removeEdge(uint e);
	void calcAdjMidPoints(const math::vector3d*points);
	virtual int GetAttribute(){return attribute;}
};

}
}

#endif //___NavMeshPolygon___
