#include "stdafx.h"

#include "GoalPipe.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

GoalPipe::GoalPipe()
{
	m_isSequence=false;
}
GoalPipe::~GoalPipe(){
	removeAllGoals();
}

void GoalPipe::SetSequence(bool s)
{
	m_isSequence=s;
}
bool GoalPipe::IsSequence()
{
	return m_isSequence;
}

void GoalPipe::activate(){
}

bool GoalPipe::isContainSubGoals()
{
	return !m_goals.empty();
}


void GoalPipe::pause()
{
	if(!m_goals.empty()){
		m_goals.front()->pause();
	}
}
void GoalPipe::resume()
{
	if(!m_goals.empty()){
		m_goals.front()->resume();
	}
}

bool GoalPipe::isGoalPresented(int goalType)
{
	GoalList::iterator it= m_goals.begin();
	for (;it!=m_goals.end();++it)
	{
		if((*it)->GetGoalType()==goalType)
			return true;
	}
	return false;
}
EGoalStatus GoalPipe::processSubGoals(){
	//remove completed and failed goals
	bool poped=false;
	while(!m_goals.empty() && (m_goals.front()->isComplete() || m_goals.front()->isFailed()))
	{
		bool failed=m_goals.front()->isFailed();
		m_goals.front()->terminate();
		delete m_goals.front();
		m_goals.pop_front();
		poped=true;
		if(m_isSequence && failed)
		{
			removeAllGoals();
			return EGS_Failed;
		}
	}

	if(!m_goals.empty()){
		EGoalStatus s;
		if(poped){
			m_goals.front()->resume();
		}
		s=m_goals.front()->process();
		if(s==EGS_Completed && m_goals.size()>1)
			return EGS_Active;
		return s;
	}else
		return EGS_Completed;
}
EGoalStatus GoalPipe::process()
{
	return processSubGoals();
}

void GoalPipe::terminate(){
	removeAllGoals();
}
 
void GoalPipe::addSubGoal(IGoal* goal){
	if(m_goals.size()>0)
	{
		m_goals.front()->pause();
	}
	m_goals.push_front(goal);
}
IGoal* GoalPipe::getSubGoal(int goalType)
{
	GoalList::iterator it= m_goals.begin();
	for (;it!=m_goals.end();++it)
	{
		if((*it)->GetGoalType()==goalType)
			return *it;
	}
	return 0;
}
void GoalPipe::removeAllGoals(){
	if(!m_goals.empty())
	{
		m_goals.front()->terminate();
		delete m_goals.front();
	}
	m_goals.clear();
}

bool GoalPipe::OnMessageTelegram(const AIMessageTelegram&msg){
	if(!m_goals.empty()){
		return m_goals.front()->OnMessageTelegram(msg);
	}
	return true;
}

void GoalPipe::Visualize(IRenderDevice*dev,const core::string&prefix)
{
	IGoal::Visualize(dev,prefix);
	GoalList::iterator it= m_goals.begin();
	for (;it!=m_goals.end();++it)
	{
		(*it)->Visualize(dev,prefix+mT(" "));
	}
}

void GoalPipe::loadXMLSettings(xml::XMLElement*elem)
{
	IGoal::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Sequence"));
	if(attr)
		SetSequence(core::StringConverter::toBool(attr->value));
	else SetSequence(false);
}

}
}

