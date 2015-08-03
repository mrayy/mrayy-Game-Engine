

#include "stdafx.h"
#include "PhManager.h"
#include "PhNode.h"
#include "PhSpring.h"


namespace mray
{
namespace physics
{


PhManager::PhManager()
{
}
PhManager::~PhManager()
{
	Clear();
}


PhNode* PhManager::CreateNode()
{
	PhNode* node = new PhNode();
	m_nodes.push_back(node);
	return node;
}
PhSpring* PhManager::CreateSpring(PhNode* a, PhNode* b, float s,float l)
{
	PhSpring* node = new PhSpring(a,b,s,l);
	m_springs.push_back(node);
	return node;
}

void PhManager::Clear()
{
	{
		std::list<PhSpring*>::iterator it = m_springs.begin();
		for (; it != m_springs.end(); ++it)
		{
			delete *it;
		}
		m_springs.clear();
	}
	{
		std::list<PhNode*>::iterator it = m_nodes.begin();
		for (; it != m_nodes.end(); ++it)
		{
			delete *it;
		}
		m_nodes.clear();
	}
}
void PhManager::_CheckCollision(PhNode*a, PhNode*b)
{

	if (a->IsFixed() && b->IsFixed())
		return;
	math::vector2d p1 = a->GetPosition();
	math::vector2d p2 = b->GetPosition();
	math::vector2d dir = (p1-p2);
	float len = dir.Length();
	if (len == 0)
		len = 1;
	dir /= len;
	float r = a->GetRadius() + b->GetRadius();
	float diff = r - len;
	if (diff<0)
		return;
	if (a->IsFixed())
	{
		b->SetPosition(p2 - dir*(diff));
		b->AddForce(-diff*dir);
	}
	else
	if (b->IsFixed())
	{
		a->SetPosition(p1 + dir*(diff));
		a->AddForce(diff*dir);
	}
	else
	{
		a->SetPosition(p1 + dir*(diff)*0.5f);
		b->SetPosition(p2 - dir*(diff)*0.5f);
		a->AddForce(dir*diff*0.5f);
		b->AddForce(-diff*dir*0.5f);
	}
}
void PhManager::Update(float dt)
{
	{
		std::list<PhSpring*>::iterator it = m_springs.begin();
		for (; it != m_springs.end(); ++it)
		{
			(*it)->Update(dt);
		}
	}
	{
		std::list<PhNode*>::iterator it = m_nodes.begin();
		for (; it != m_nodes.end(); ++it)
		{
			(*it)->Update(dt);
		}
	}
	if (true)
	{
		std::list<PhNode*>::iterator it = m_nodes.begin();
		for (; it != m_nodes.end(); ++it)
		{
			std::list<PhNode*>::iterator it2 = it;
			++it2;
			for (; it2 != m_nodes.end(); ++it2)
			{
				_CheckCollision(*it, *it2);
			}
		}
	}
}

}
}

