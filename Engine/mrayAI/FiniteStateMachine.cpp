#include "stdafx.h"


#include "FiniteStateMachine.h"
#include "AIComponentTypes.h"


namespace mray
{
namespace AI
{

FiniteStateMachine::FiniteStateMachine()
{
	m_activeState=-1;
}
FiniteStateMachine::~FiniteStateMachine()
{
// 	DelegateEvent1<core::string>::DelegateList::iterator p=OnTransition.getDelegates().begin();
// 	DelegateEvent1<core::string>::DelegateList::iterator end=OnTransition.end();
// 	for(;p!=end;++p)
// 	{
// 		delete *p;
// 	}
}


void FiniteStateMachine::UpdateStateTransitions(FSMState& s)
{
	s.transitions_id.clear();
	FSMState::TransitionMap::iterator it=s.transitions.begin();
	for (;it!=s.transitions.end();++it)
	{
		StatesIDMap::iterator sit= m_statesIDs.find(it->second);
		if(sit!=m_statesIDs.end())
		{
			s.transitions_id[it->first]=sit->second;
		}
	}
	s.isDirty=false;
}
void FiniteStateMachine::SetActiveState(const core::string& name)
{
	StatesIDMap::iterator it=m_statesIDs.find(name);
	if(it==m_statesIDs.end())
		m_activeState=-1;
	else{
		m_activeState=it->second;
		if(m_states[m_activeState].isDirty)
		{
		}
	}
}
core::string FiniteStateMachine::GetCurrentState()
{
	if(m_activeState==-1)
		return mT("");
	return m_states[m_activeState].name;
}

void FiniteStateMachine::AddState(const FSMState& s)
{
	int id=m_states.size();
	m_states.push_back(s);
	m_states[id].id=id;
	m_statesIDs[s.name]=id;
}

void FiniteStateMachine::Update(float dt)
{

}

void FiniteStateMachine::Tick(const core::string& symbol)
{
	if(m_activeState==-1)
		return;
	FSMState&s=m_states[m_activeState];
	FSMState::TransitionMap::iterator it= s.transitions.find(symbol);
	if(it!=s.transitions.end()){
		SetActiveState(it->second);
		if(m_activeState!=-1){
			OnTransition(m_states[m_activeState].name);
		}
	}
}


bool FiniteStateMachine::loadXMLStateElem(xml::XMLElement* elem)
{
	FSMState state;

	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("name"));
	if(!attr)
		return false;
	state.name=attr->value;
	
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Transition")))
			loadXMLTransElem(e,state);
	}
	AddState(state);
	return true;
}
void FiniteStateMachine::loadXMLTransElem(xml::XMLElement* elem,FSMState&s)
{

	xml::XMLAttribute*symbol,*state;
	symbol=elem->getAttribute(mT("symbol"));
	state=elem->getAttribute(mT("state"));
	if(!symbol || !state)
		return;
	s.transitions[symbol->value]=state->value;
}

xml::XMLElement* FiniteStateMachine::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadXMLSettings(elem);
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("State")))
			loadXMLStateElem(e);
	}
	return elem;
}


}
}

