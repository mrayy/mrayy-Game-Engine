
#ifndef __IState__
#define __IState__

#include <mstring.h>
namespace mray{
	class StateMachine;

class IState
{
protected:
	core::string m_name;
	StateMachine* m_ownerMachine;
public:
	IState(const core::string&name):m_name(name),m_ownerMachine(0){}
	virtual~IState(){}

	virtual const core::string&getName(){return m_name;}

	void SetOwnerMachine(StateMachine*m){m_ownerMachine=m;}
	StateMachine* GetOwnerMachine(){return m_ownerMachine;}

	virtual void onEnter(IState*prev)=0;
	virtual void onUpdate()=0;
	virtual void onExit()=0;
};
}

#endif