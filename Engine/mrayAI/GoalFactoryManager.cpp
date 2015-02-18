#include "stdafx.h"

#include "GoalFactoryManager.h"
#include "IGoalFactory.h"


namespace mray
{
namespace AI
{

GoalFactoryManager::GoalFactoryManager()
{
}
GoalFactoryManager::~GoalFactoryManager()
{
	ClearFactories();
}

void GoalFactoryManager::ClearFactories()
{
	FactoryMap::iterator it=m_factories.begin();
	for (;it!=m_factories.end();++it)
	{
		delete it->second;
	}
	m_factories.clear();
}

IGoal* GoalFactoryManager::CreateGoal(const core::string&name,IAIArchitect*arch)
{
	core::string str=name;
	str.make_lower();
	FactoryMap::iterator it=m_factories.find(str);
	if(it==m_factories.end())
		return 0;
	return it->second->CreateGoal(arch);
}

void GoalFactoryManager::AddFactory(IGoalFactory* f)
{
	core::string str=f->GetType();
	str.make_lower();
	m_factories[str]=f;
}
void GoalFactoryManager::RemoveFactory(const core::string&name)
{
	core::string str=name;
	str.make_lower();
	FactoryMap::iterator it=m_factories.find(str);
	if(it==m_factories.end())
		return;
	delete it->second;
	m_factories.erase(it);
}


}
}
