
/********************************************************************
	created:	2009/03/08
	created:	8:3:2009   17:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RayNodeSelector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RayNodeSelector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RayNodeSelector___
#define ___RayNodeSelector___


#include "INodeSelectQuery.h"
#include "ISceneNode.h"

namespace mray{
namespace scene{

class MRAY_DLL RayNodeSelector:public INodeSelectQuery
{
protected:
	math::line3d m_ray;
	MovableNodeList::const_iterator m_nodesBegin;
	MovableNodeList::const_iterator m_nodesEnd;
	uint m_idMask;

	NodesQueryList m_query;

	//void getPickedNode(core::IteratorPair<MovableNodeList> nodes,QueryResultNode &bestNode,NodesQueryList&outList);
public:
	RayNodeSelector();
	virtual~RayNodeSelector();

	virtual QueryResultNode query(NodesQueryList&outList);
	const NodesQueryList& query(QueryResultNode&best);

	void setup(const math::line3d&ray,const MovableNodeList& nodes,uint idMask);
};

}
}


#endif //___RayNodeSelector___
