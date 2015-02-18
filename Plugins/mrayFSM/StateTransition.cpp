
#include "stdafx.h"
#include "StateTransition.h"


namespace mray{


StateTransition::StateTransition(IState*from,IState*to,ICondition*cond):
	m_from(from),m_to(to),m_condition(cond)
{

}
StateTransition::~StateTransition(){

}

bool StateTransition::evaluate(){
	if(m_condition){
		return m_condition->checkCondition();
	}
	return false;
}

IState*StateTransition::getFromState(){
	return m_from;
}
IState*StateTransition::getToState(){
	return m_to;

}
ICondition*StateTransition::getCondition(){
	return m_condition;

}


}
