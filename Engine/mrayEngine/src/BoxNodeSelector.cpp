
#include "stdafx.h"
#include "ISceneVisitor.h"
#include "BoxNodeSelector.h"



namespace mray
{
namespace scene
{


class BoxNodeSelectorVisitor:public ISceneVisitor
{
protected:
	NodesQueryList &m_outList;
	math::box3d m_selBox;
	uint m_idMask;

protected:

	void process(IMovable*curr)
	{
		math::vector3d edges[8];
		math::box3d box;
		if(curr && (!m_idMask || curr->getID()&m_idMask) )
		{
			if(curr->getTransformedBoundingBox().intersectWithBox(m_selBox))
			{
				m_outList.push_back(QueryResultNode(curr,0));
			}
		}
	}
public:
	BoxNodeSelectorVisitor(const math::box3d &box,uint idMask,NodesQueryList &outList)
		:m_outList(outList),m_selBox(box),m_idMask(idMask)
	{
	}
	virtual void Visit(IMovable*m)
	{
		process(m);
	}
	virtual void Visit(ISceneNode*node)
	{
		if(node->isVisible())
			process(node);
	}
};

BoxNodeSelector::BoxNodeSelector()
{
	m_idMask=0;
}
BoxNodeSelector::~BoxNodeSelector()
{
}
/*
void BoxNodeSelector::getPickedNode(core::IteratorPair<MovableNodeList> nodes,
				   NodesQueryList&outList)
{
	math::vector3d edges[8];
	for(;!nodes.done();nodes++)
	{
		math::box3d box;
		IMovable* curr=*nodes;
		if(curr && (!m_idMask || curr->getID()&m_idMask) )
		{
			if(curr->getTransformedBoundingBox().intersectWithBox(m_box))
			{
				outList.push_back(QueryResultNode(curr,0));
			}
		}
		getPickedNode((*nodes)->getChildren(),outList);
	}
}*/
QueryResultNode BoxNodeSelector::query(NodesQueryList&outList)
{
	QueryResultNode res;
	res.dist=math::Infinity;
	res.node=0;
	BoxNodeSelectorVisitor visitor(m_box,m_idMask,outList);
	MovableNodeList::const_iterator it=m_nodesBegin;
	for(;it!=m_nodesEnd;++it)
	{
		(*it)->OnVisit(&visitor);
	}
	if(outList.size()!=0)
		res=*outList.begin();
	return res;
}
const NodesQueryList& BoxNodeSelector::query(QueryResultNode&best)
{
	best=query(m_query);
	return m_query;
}

void BoxNodeSelector::setup(const math::box3d&box,const MovableNodeList& nodes,uint idMask)
{
	m_box=box;
	m_nodesBegin=nodes.begin();
	m_nodesEnd=nodes.end();
	m_idMask=idMask;
}

}
}