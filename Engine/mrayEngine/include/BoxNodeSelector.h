

/********************************************************************
	created:	2012/01/20
	created:	20:1:2012   15:14
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\BoxNodeSelector.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	BoxNodeSelector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BoxNodeSelector__
#define __BoxNodeSelector__

#include "INodeSelectQuery.h"
#include "ISceneNode.h"

namespace mray
{
namespace scene
{

class MRAY_DLL BoxNodeSelector:public INodeSelectQuery
{
private:
protected:
	math::box3d m_box;
	MovableNodeList::const_iterator m_nodesBegin;
	MovableNodeList::const_iterator m_nodesEnd;
	uint m_idMask;

	NodesQueryList m_query;

	//void getPickedNode(core::IteratorPair<MovableNodeList> nodes,
	//	NodesQueryList&outList);

public:
	BoxNodeSelector();
	virtual~BoxNodeSelector();

	virtual QueryResultNode query(NodesQueryList&outList);
	const NodesQueryList& query(QueryResultNode&best);

	void setup(const math::box3d&box,const MovableNodeList& nodes,uint idMask);
};

}
}

#endif

