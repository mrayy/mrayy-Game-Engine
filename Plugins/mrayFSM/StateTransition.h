
#ifndef __StateTransition__
#define __StateTransition__


#include "IState.h"
#include "ICondition.h"

namespace mray{

class StateTransition
{
protected:
	IState* m_from;
	IState* m_to;
	ICondition* m_condition;
public:
	StateTransition(IState*from,IState*to,ICondition*cond);
	virtual ~StateTransition();

	virtual bool evaluate();

	virtual IState*getFromState();
	virtual IState*getToState();
	virtual ICondition*getCondition();
};

}

#endif
