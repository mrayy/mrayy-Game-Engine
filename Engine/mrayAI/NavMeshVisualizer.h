
/********************************************************************
	created:	2010/03/08
	created:	8:3:2010   13:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavMeshVisualizer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavMeshVisualizer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshVisualizer___
#define ___NavMeshVisualizer___

#include "IWorldVisualizer.h"
#include "NavMesh.h"

namespace mray{
namespace AI{

class NavMeshVisualizer:public IWorldVisualizer
{
private:
protected:
	NavMesh*m_navMesh;
	std::vector<math::vector3d> m_points;
	std::vector<ushort> m_indicies;

public:
	NavMeshVisualizer(NavMesh*world);
	virtual~NavMeshVisualizer();

	virtual void DrawWorld(IRenderDevice*dev);
	virtual void DrawNodes(IRenderDevice*dev,const NavWorldNodeListPtr&nodes,bool drawConnections);
	virtual void DrawNode(IRenderDevice*dev,AI::INavWorldNode*p);
	virtual void DrawLinks(IRenderDevice*dev,GraphLinksList&links);

};

}
}

#endif //___NavMeshVisualizer___
