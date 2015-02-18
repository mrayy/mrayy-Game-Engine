
/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   0:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavGraphNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavGraphNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavGraphNode___
#define ___NavGraphNode___

#include "IGraphNode.h"
#include "Point3d.h"

namespace mray{
namespace AI{

class NavMeshPolygon;

class NavGraphNode:public IGraphNode
{
private:
protected:
public:
	math::vector3d position;
	NavMeshPolygon* owner;//if owner is null,then this node is on edge

	NavGraphNode(uint id):IGraphNode(id),owner(0)
	{
	}
	virtual~NavGraphNode()
	{
	}

};

}
}


#endif //___NavGraphNode___
