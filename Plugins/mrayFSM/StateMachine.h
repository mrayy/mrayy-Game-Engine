

#ifndef __StateMachine__
#define __StateMachine__

#include "StateTransition.h"
#include "ICondition.h"
#include "IState.h"

#include <vector>
#include <string>
#include <map>
#include <ListenerContainer.h>


namespace mray
{
class StateMachine;

class IStateMachineListener
{
public:
	virtual void OnStateAdded(StateMachine*,IState* s){}
	virtual void OnConditionAdded(StateMachine*,ICondition* c){}
	virtual void OnStateChanged(StateMachine*,IState* oldS,IState* newS){}
};

class StateMachine:public ListenerContainer<IStateMachineListener*>
{
protected:
	typedef std::map<core::string,int> NameIDMap;
	typedef std::vector<IState*> StateMap;
	typedef std::vector<ICondition*> ConditionMap;
	typedef std::vector<StateTransition*> TransitionList;
	struct StateTransitionGroup
	{
	public:

		virtual ~StateTransitionGroup();

		IState*state;
		TransitionList transitions;
	};
	typedef std::map<IState*,StateTransitionGroup*> StateTransitionMap;

	NameIDMap m_stateNames;
	NameIDMap m_condNames;

	StateMap m_states;
	ConditionMap m_conditions;
	StateTransitionMap m_transitions;
	StateTransitionGroup* m_activeState;

	StateTransitionGroup*getTransitionGroup(IState*state);

	DECLARE_FIRE_METHOD(OnStateAdded,(StateMachine*m,IState* s),(m,s));
	DECLARE_FIRE_METHOD(OnConditionAdded,(StateMachine*m,ICondition* c),(m,c));
	DECLARE_FIRE_METHOD(OnStateChanged,(StateMachine*m,IState* oldS,IState* newS),(m,oldS,newS));
public:
	StateMachine();
	virtual~StateMachine();

	virtual void setActiveState(IState* s);
	virtual void setActiveStateByName(const core::string& name);
	virtual IState* getActiveState()const;

	virtual void addState(IState*state);
	virtual bool addCondition(ICondition*cond);
	virtual bool addTransition(IState*from, IState*to, ICondition*condition);
	virtual bool addTransitionByName(const core::string& from, const core::string& to, const core::string&condition);

	const std::vector<IState*>& getStates()const { return m_states; }
	IState*getState(const core::string&name)const;
	ICondition*getCondition(const core::string&name)const;

	virtual void onUpdate();
};

}


#endif
