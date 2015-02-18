
#include "stdafx.h"
#include "IBehaviorTree.h"
#include "IBHNode.h"


namespace mray
{
namespace AI
{


IBehaviorTree::IBehaviorTree():m_interval(0.1),m_rootNode(0),m_time(0)
{
}
IBehaviorTree::~IBehaviorTree()
{
	delete m_rootNode;
}

void IBehaviorTree::Update(float dt)
{
	m_time+=dt;
	if(m_time>=m_interval)
	{
		m_time=0;
		if(m_rootNode && m_rootNode->CheckCondition())
			m_rootNode->Access(dt);
	}
}

void IBehaviorTree::SetRootNode(IBHNode*node)
{
	m_rootNode=node;
}
IBHNode* IBehaviorTree::GetRootNode()const
{
	return m_rootNode;
}

}
}
