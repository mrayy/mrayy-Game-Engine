#include "stdafx.h"

#include "GoalScenario.h"

#include "IFileSystem.h"
#include "XMLTree.h"
#include "ILogManager.h"

#include "GoalFactoryManager.h"

namespace mray
{
namespace AI
{

GoalScenario::GoalScenario(AI::IAIArchitect*arch,const core::string&scenarioToLoad):m_id(0),m_arch(arch),m_requiredScenario(mT("Scenario"),scenarioToLoad)
{
	m_status=EGS_Inactive;
	m_type=mT("Scenario");

	AddAttribute(&m_requiredScenario);
}
GoalScenario::~GoalScenario()
{
}

IGoal* GoalScenario::LoadGoal(xml::XMLElement*e)
{
	xml::XMLAttribute* attr;
	IGoal* goal=0;

	attr=e->getAttribute(mT("Type"));
	if(!attr)
		return 0;
	goal=GoalFactoryManager::getInstance().CreateGoal(attr->value,m_arch);
	if(!goal){
		gLogManager.log(mT("Couldn't find goal with name: ")+attr->value,ELL_WARNING);
		return 0;
	}
	goal->loadXMLSettings(e);

	return goal;
}
void GoalScenario::loadXMLSettings(xml::XMLElement*e)
{
	GoalPipe::loadXMLSettings(e);

	xml::XMLAttribute* attr;

	std::list<IGoal*> loadedGoals;

	attr=e->getAttribute(mT("Type"));
	if(attr)
		m_type=attr->value;
	else m_type=mT("Scenario");

	attr=e->getAttribute(mT("ID"));
	if(attr)
		m_id=core::StringConverter::toInt(attr->value);
	else m_id=0;


	xml::xmlSubElementsMapIT it=e->getElementsBegin();
	for(;it!=e->getElementsEnd();++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* elem=dynamic_cast<xml::XMLElement*>(*it);
		if(elem->getName().equals_ignore_case(mT("Goal")))
		{
			IGoal*g= LoadGoal(elem);
			if(g)
				loadedGoals.push_front(g);
		}
	}

	removeAllGoals();

	std::list<IGoal*>::iterator it2= loadedGoals.begin();
	for(;it2!=loadedGoals.end();++it2)
	{
		addSubGoal(*it2);
	}

}
bool GoalScenario::LoadScenario(const core::string& file)
{

	OS::IStream* stream=gFileSystem.openFile(file,OS::TXT_READ);
	if(!stream)
		return false;

	bool ret=false;

	xml::XMLTree tree;
	if(tree.load(stream))
	{
		xml::XMLElement *e=tree.getSubElement(mT("GoalScenario"));
		if(e)
		{
			loadXMLSettings(e);
			ret=true;
		}
	}

	stream->close();
	return ret;
}
void GoalScenario::activate()
{
	m_status=EGS_Active;
	if(m_requiredScenario.value!=mT("") && !LoadScenario(m_requiredScenario.value))
		m_status=EGS_Failed;
}
EGoalStatus GoalScenario::process()
{
	if(m_status!=EGS_Active)
		activate();
	if(m_status==EGS_Failed)
		return m_status;

	m_status=GoalPipe::process();
	return m_status;
}

core::string GoalScenario::getType()
{
	return m_type;
}
int GoalScenario::GetGoalType()
{
	return m_id;
}
EGoalStatus GoalScenario::getStatus()
{
	return m_status;
}



}
}
