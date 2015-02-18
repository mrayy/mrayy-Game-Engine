

/********************************************************************
	created:	2010/02/18
	created:	18:2:2010   13:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavMeshGrid.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavMeshGrid
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshGrid___
#define ___NavMeshGrid___

#include "NavMeshPolygon.h"
#include "Point2d.h"

namespace mray{
namespace AI{

class NavMeshGrid
{
private:
protected:
	float m_cellSize;
	math::box3d m_boundingBox;
	math::Point2di m_cellsPerEdge;

	std::vector<NavWorldNodeListPtr> m_cells;

public:
	NavMeshGrid();
	virtual~NavMeshGrid();

	void SetGrindBounds(const math::box3d&bb);
	void SetCellSize(float sz);
	float GetCellSize();
	void CreateGrid();

	void InsertPoly(NavMeshPolygon*p);
	void QueryPolys(const math::vector3d&p,NavWorldNodeListPtr&outPoly);

	void Clear();

	int getCellsCount();
	void addToCell(int cell,NavMeshPolygon*p);
	NavWorldNodeListPtr& getPolygonsInCell(int cell);
};

}
}


#endif //___NavMeshGrid___
