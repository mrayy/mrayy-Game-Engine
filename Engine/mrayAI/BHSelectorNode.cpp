

#include "stdafx.h"
#include "BHSelectorNode.h"


namespace mray
{
namespace AI
{

BHSelectorNode::BHSelectorNode()
:IBHNode(EBHNode_Selector)
{
}
BHSelectorNode::~BHSelectorNode()
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		delete *it;
	}
	m_nodes.clear();
}
bool BHSelectorNode::CheckCondition()
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		if((*it)->CheckCondition())
			return true;
	}
	return false;
}
bool BHSelectorNode::Access(float dt)
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		(*it)->Access(dt);
	}
	return true;
}

void BHSelectorNode::AddNode(IBHNode* node)
{
	m_nodes.push_back(node);
}
void BHSelectorNode::RemoveNode(IBHNode* node)
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
void BHSelectorNode::ClearNodes()
{
	BHNodeList::iterator it= m_nodes.begin();
	for(;it!=m_nodes.end();++it)
	{
		delete *it;
	}
	m_nodes.clear();
}

const BHNodeList& BHSelectorNode::GetNodes()
{
	return m_nodes;
}

}
}
