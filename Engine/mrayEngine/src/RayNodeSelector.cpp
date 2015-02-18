#include "stdafx.h"

#include "RayNodeSelector.h"

#include "ISceneVisitor.h"


namespace mray{
namespace scene{

class RayNodeSelectorVisitor:public ISceneVisitor
{
protected:
	QueryResultNode &m_bestNode;
	NodesQueryList &m_outList;
	math::line3d m_selRay;
	uint m_idMask;

protected:

	void process(IMovable*curr)
	{
		math::vector3d edges[8];
		math::box3d box;
		if(curr && (!m_idMask || curr->getID()&m_idMask) )
		{
			math::matrix4x4 mat;

			if(curr->getAbsoluteTransformation().getInverse(mat)){
				math::line3d ln(m_selRay);
				ln.pStart=mat*(ln.pStart);
				ln .pEnd=mat*(ln .pEnd);
				box=curr->getBoundingBox();
				if(box.intersectsWithLine(ln))
				{
					box.getEdges(edges);
					float dist=0;
					for(int e=0;e<8;e++)
					{
						float t=edges[e].getDistSQ(ln.pStart);
						if(t>dist)
							dist=t;
					}
					m_outList.push_back(QueryResultNode(curr,dist));
					if(dist<m_bestNode.dist){
						m_bestNode.dist=dist;
						m_bestNode.node=curr;
					}
				}
			}
		}
	}
public:
	RayNodeSelectorVisitor(const math::line3d &selRay,uint idMask,
		QueryResultNode &bestNode,NodesQueryList &outList)
		:m_bestNode(bestNode),m_outList(outList),m_selRay(selRay),m_idMask(idMask)
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

RayNodeSelector::RayNodeSelector()
{
	m_idMask=0;
}
RayNodeSelector::~RayNodeSelector(){
}

/*
void RayNodeSelector::getPickedNode(core::IteratorPair<MovableNodeList> nodes,QueryResultNode &bestNode,NodesQueryList&outList)
{
	math::vector3d edges[8];
	for(;!nodes.done();nodes++)
	{
		math::box3d box;
		IMovable*curr=*nodes;
		//ISceneNode* curr=dynamic_cast<ISceneNode* >(*nodes);
		if(curr &&(!m_idMask || curr->getID()&m_idMask) )
		{
			math::matrix4x4 mat;

			if(curr->getAbsoluteTransformation().getInverse(mat)){
				math::line3d ln(m_ray);
				ln.pStart=mat*(ln.pStart);
				ln .pEnd=mat*(ln .pEnd);
				box=curr->getBoundingBox();
				if(box.intersectsWithLine(ln))
				{
					box.getEdges(edges);
					float dist=0;
					for(int e=0;e<8;e++)
					{
						float t=edges[e].getDistSQ(ln.pStart);
						if(t>dist)
							dist=t;
					}
					outList.push_back(QueryResultNode(curr,dist));
					if(dist<bestNode.dist){
						bestNode.dist=dist;
						bestNode.node=curr;
					}
				}
			}
		}
		getPickedNode((*nodes)->getChildren(),bestNode,outList);
	}
}
*/
QueryResultNode RayNodeSelector::query(NodesQueryList&outList){
	QueryResultNode res;
	res.dist=math::Infinity;
	res.node=0;
	RayNodeSelectorVisitor visitor(m_ray,m_idMask,res,outList);
	MovableNodeList::const_iterator it=m_nodesBegin;
	for(;it!=m_nodesEnd;++it)
	{
		(*it)->OnVisit(&visitor);
	}
	//getPickedNode(m_nodes,res,outList);
	return res;
}
const NodesQueryList& RayNodeSelector::query(QueryResultNode&best)
{
	best=query(m_query);
	return m_query;
}

void RayNodeSelector::setup(const math::line3d&ray,const MovableNodeList& nodes,uint idMask)
{	
	m_ray=ray;
	m_nodesBegin=nodes.begin();
	m_nodesEnd=nodes.end();
	m_idMask=idMask;
}


}
}