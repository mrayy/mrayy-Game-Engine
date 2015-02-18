#include "stdafx.h"

#include "NavMeshGrid.h"
#include "ILogManager.h"
#include "Rect.h"



namespace mray{
namespace AI{

NavMeshGrid::NavMeshGrid()
{
	m_cellSize=5;
}

NavMeshGrid::~NavMeshGrid()
{
}


void NavMeshGrid::SetGrindBounds(const math::box3d&bb)
{
	m_boundingBox=bb;
}

void NavMeshGrid::SetCellSize(float sz)
{
	m_cellSize=sz;
}
float NavMeshGrid::GetCellSize()
{
	return m_cellSize;
}


void NavMeshGrid::CreateGrid()
{
	m_cells.clear();
	if(m_cellSize==0){
		gLogManager.log(mT("NavMeshGrid::CreateGrid()-cellSize=0!"),ELL_ERROR);
		return;
	}
	m_cellsPerEdge.x=m_boundingBox.getWidth();
	m_cellsPerEdge.y=m_boundingBox.getDepth();
	m_cellsPerEdge/=m_cellSize;
	m_cellsPerEdge+=1;

	m_cells.resize(m_cellsPerEdge.x*m_cellsPerEdge.y);
}


void NavMeshGrid::InsertPoly(NavMeshPolygon*p)
{
	//find the cells where p should be in
	math::rectf rc( p->boundingbox.MinP.x,p->boundingbox.MinP.z,
		p->boundingbox.MaxP.x,p->boundingbox.MaxP.z);;
	rc-=math::vector2d(m_boundingBox.MinP.x,m_boundingBox.MinP.z);
	//determine the starting and ending rows,cols which the poly bounding box is inside
	int r1=rc.ULPoint.x/m_cellSize;
	int r2=rc.BRPoint.x/m_cellSize;
	int c1=rc.ULPoint.y/m_cellSize;
	int c2=rc.BRPoint.y/m_cellSize;
	for(int i=r1;i<=r2;i++){
		for(int j=c1;j<=c2;j++){
			m_cells[i*m_cellsPerEdge.y+j].push_back(p);
		}
	}
}

void NavMeshGrid::QueryPolys(const math::vector3d&p,NavWorldNodeListPtr&outPoly)
{
	int r=(p.x-m_boundingBox.MinP.x)/m_cellSize;
	int c=(p.z-m_boundingBox.MinP.z)/m_cellSize;
	if(r<0 || r>=m_cellsPerEdge.x)return;
	if(c<0 || c>=m_cellsPerEdge.y)return;
	NavWorldNodeListPtr::iterator it=m_cells[r*m_cellsPerEdge.y+c].begin();
	NavWorldNodeListPtr::iterator end=m_cells[r*m_cellsPerEdge.y+c].end();
	for(;it!=end;++it){
		NavMeshPolygon*poly=(NavMeshPolygon*)*it;
		if(p.x>=poly->boundingbox.MinP.x&&
			p.x<=poly->boundingbox.MaxP.x&&
			p.z>=poly->boundingbox.MinP.z&&
			p.z<=poly->boundingbox.MaxP.z)
		{
			outPoly.push_back(poly);
		}
	}

}

int NavMeshGrid::getCellsCount(){
	return m_cells.size();
}

void NavMeshGrid::addToCell(int cell,NavMeshPolygon*p)
{
	m_cells[cell].push_back(p);
}
NavWorldNodeListPtr& NavMeshGrid::getPolygonsInCell(int cell)
{
	return m_cells[cell];
}
void NavMeshGrid::Clear()
{
	m_cells.clear();
}

}
}