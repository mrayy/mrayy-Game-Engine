
#include "stdafx.h"
#include "JFActionQueue.h"


namespace mray
{
namespace game
{


JFActionQueue::JFActionQueue()
{
}
JFActionQueue::~JFActionQueue()
{
	ErruptActions();
}

void JFActionQueue::AddAction(JFIAction*a)
{
	m_actions.push_back(a);
}
void JFActionQueue::ErruptActions()
{
	std::list<JFIAction*>::iterator it=m_actions.begin();
	for(;it!=m_actions.end();++it)
	{
		(*it)->OnErrupt();
		delete *it;
	}
	m_actions.clear();
}

void JFActionQueue::OnErrupt()
{
	ErruptActions();
}
bool JFActionQueue::Execute(float dt)
{
	std::list<JFIAction*>::iterator it=m_actions.begin();
	if(it!=m_actions.end())
	{
		if((*it)->Execute(dt))
		{
			delete *it;
			m_actions.erase(it);
		}
	}
	return m_actions.empty();
}



}
}


