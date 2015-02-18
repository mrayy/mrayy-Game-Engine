
#include "stdafx.h"
#include "BHSequenceNode.h"


namespace mray
{
namespace AI
{

BHSequenceNode::BHSequenceNode()
	:IBHNode(EBHNode_Selector)
{
}
BHSequenceNode::~BHSequenceNode()
{
	ClearNodes();
}
bool BHSequenceNode::CheckCondition()
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		if(!(*it)->CheckCondition())
			return false;
	}
	return true;
}
bool BHSequenceNode::Access(float dt)
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		(*it)->Access(dt);
	}
	return true;
}


void BHSequenceNode::AddNode(IBHNode* node)
{
	m_nodes.push_back(node);
}
void BHSequenceNode::RemoveNode(IBHNode* node)
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		if((*it)==node)
		{
			m_nodes.erase(it);
			break;
		}
	}
}
void BHSequenceNode::ClearNodes()
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		delete *it;
	}
	m_nodes.clear();
}

const BHNodeList& BHSequenceNode::GetNodes()
{
	return m_nodes;
}

}
}
