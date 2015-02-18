
#include "stdafx.h"
#include "StateMachine.h"


namespace mray{


StateMachine::StateTransitionGroup::~StateTransitionGroup()
{
	TransitionList::iterator it=transitions.begin();
	for(;it!=transitions.end();++it){
		delete *it;
	}
}

	//////////////////////////////////////////////////////////////////////////
StateMachine::StateMachine(){
	m_activeState=0;
}
StateMachine::~StateMachine(){
	{
		StateMap::iterator it= m_states.begin();
		for(;it!=m_states.end();++it){
			delete *it;
		}
	}
	{
		ConditionMap::iterator it= m_conditions.begin();
		for(;it!=m_conditions.end();++it){
			delete *it;
		}
	}
	{
		StateTransitionMap::iterator it= m_transitions.begin();
		for(;it!=m_transitions.end();++it){
			delete it->second;
		}
	}
}


void StateMachine::setActiveState(IState* state){
	if (!state)
		return;
	if (m_activeState && m_activeState->state == state)
		return;
	StateTransitionMap::iterator it2 = m_transitions.find(state);
	if (it2 != m_transitions.end()){
		m_activeState = it2->second;
		if (it2->second)
		{
			it2->second->state->onEnter(m_activeState->state);
			FIRE_LISTENR_METHOD(OnStateChanged, (this, m_activeState->state, it2->second->state));
			m_activeState = it2->second;
		}
	}
}
void StateMachine::setActiveStateByName(const core::string& name)
{
	
	setActiveState(getState(name));
}


IState* StateMachine::getActiveState()const{
	if(!m_activeState)
		return 0;
	return m_activeState->state;
}

void StateMachine::addState(IState*state){
	m_states.push_back(state);
	state->SetOwnerMachine(this);
	m_stateNames[state->getName()]=m_states.size()-1;
	StateTransitionMap::iterator it= m_transitions.find(state);
	if(it==m_transitions.end()){
		StateTransitionGroup * group=new StateTransitionGroup();
		group->state=state;
		m_transitions.insert(StateTransitionMap::value_type(state,group));
	//	if(!m_activeState)
		//	m_activeState=group;
	}
	FIRE_LISTENR_METHOD(OnStateAdded,(this,state));

}
bool StateMachine::addCondition(ICondition*cond){
	NameIDMap::iterator it= m_condNames.find(cond->getName());
	if(it!=m_condNames.end())
		return false;

	m_conditions.push_back(cond);
	m_condNames[cond->getName()]=m_conditions.size()-1;
	FIRE_LISTENR_METHOD(OnConditionAdded,(this,cond));


	return true;
}
bool StateMachine::addTransition(IState*fromSt, IState*toSt, ICondition*cond)
{
	if (!fromSt || !toSt || !cond)
		return false;

	StateTransitionMap::iterator it= m_transitions.find(fromSt);
	if(it==m_transitions.end()){
		return false;	//state transition not found!
	}else{
		StateTransition*trans=new StateTransition(fromSt,toSt,cond);
		it->second->transitions.push_back(trans);
	}
	return true;
}
bool StateMachine::addTransitionByName(const core::string& from, const core::string& to, const core::string&condition)
{
	IState*fromSt = getState(from);
	IState*toSt = getState(to);
	ICondition*cond = getCondition(condition);
	return addTransition(fromSt, toSt, cond);
}
IState*StateMachine::getState(const core::string&name)const{
	NameIDMap::const_iterator it=m_stateNames.find(name);
	if(it==m_stateNames.end())
		return 0;
	return m_states[it->second];
}
ICondition*StateMachine::getCondition(const core::string&name)const{
	NameIDMap::const_iterator it = m_condNames.find(name);
	if(it==m_condNames.end())
		return 0;
	return m_conditions[it->second];


}
StateMachine::StateTransitionGroup*StateMachine::getTransitionGroup(IState*state){
	StateTransitionMap::iterator it= m_transitions.find(state);
	if(it!=m_transitions.end()){
		return it->second;
	}
	return 0;
}

void StateMachine::onUpdate(){
	if(m_activeState){
		IState*state=m_activeState->state;
		// update the current state
		state->onUpdate();
		TransitionList::iterator it= m_activeState->transitions.begin();
		for(;it!=m_activeState->transitions.end();++it){
			StateTransition* trans= *it;
			// is the condition true
			if(trans->evaluate()){
				// then move active state
				state->onExit();
				StateTransitionGroup* t=getTransitionGroup(trans->getToState());
				if(t)
				{
					t->state->onEnter(state);
					FIRE_LISTENR_METHOD(OnStateChanged,(this,state,t->state));
					m_activeState=t;
				}
				break;
			}
		}
	}
}

}
