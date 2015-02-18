
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   15:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GridSpacePartition.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GridSpacePartition
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GridSpacePartition___
#define ___GridSpacePartition___

#include "IAISpacePartition.h"
#include "Point2d.h"
#include "marray.h"
#include <map>

namespace mray{
namespace AI{

class GridSpacePartition:public IAISpacePartition
{
private:
protected:
	math::box3d m_worldBB;
	float m_cellSize;
	math::Point3di m_cellsPerEdge;


	typedef std::map<AIActor*,math::vector2di> ActorMapList;
	ActorMapList m_actorsCells;

	std::vector<std::vector<AIActorList>> m_cells;

	math::vector2di GetCellIndex(float x,float y,float z);
	void RemoveFromYCell(AIActor*actor,int cell,int ycell);
	void RemoveFromCell(AIActor*actor,int idx);
public:
	GridSpacePartition();
	virtual~GridSpacePartition();

	void Create();

	virtual void SetWorldAABB(const math::box3d& box);
	virtual const math::box3d& GetWorldAABB();

	void SetCellSize(float sz);
	float GetCellSize();

	virtual void AddActor(AIActor*actor);
	virtual void RemoveActor(AIActor*actor);

	virtual void Update();

	virtual void QueryActors(const math::box3d& box,AIActorList&actors);
	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___GridSpacePartition___
