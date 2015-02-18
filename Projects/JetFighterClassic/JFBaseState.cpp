

#include "stdafx.h"
#include "JFBaseState.h"
#include "IGameState.h"


namespace mray
{
namespace game
{

JFBaseState::~JFBaseState()
{
	delete m_state;
}


void JFBaseState::onEnter(IState*prev)
{
	if(m_state)
		m_state->OnEnter();
}

void JFBaseState::onUpdate()
{
	if(m_state)
		m_state->Update(gFPS.dt());
}

void JFBaseState::onExit()
{
	if(m_state)
		m_state->OnExit();
}

}
}

