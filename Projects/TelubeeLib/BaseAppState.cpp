

#include "stdafx.h"
#include "BaseAppState.h"
#include "IApplicationState.h"




namespace mray
{
namespace TBee
{


BaseAppState::~BaseAppState()
{
	delete m_state;
}


void BaseAppState::onEnter(IState*prev)
{
	IApplicationState *st=0;
	if(prev)
		st=((BaseAppState*)prev)->GetState();
	m_state->OnEnter(st);
}

void BaseAppState::onUpdate()
{
}

void BaseAppState::onExit()
{
	m_state->OnExit();
}


}
}