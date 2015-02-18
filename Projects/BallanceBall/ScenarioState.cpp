
#include "stdafx.h"
#include "ScenarioState.h"



namespace mray
{
namespace Ball
{

ScenarioState::ScenarioState(const core::string& name,IGameState* s)
:IState(name),m_state(s)
{
}
ScenarioState::~ScenarioState()
{
}

void ScenarioState::onEnter(IState*prev)
{
	ScenarioState*p=(ScenarioState*)prev;
	m_state->Enter(p->GetGameState());
}

void ScenarioState::onUpdate()
{
}

void ScenarioState::onExit()
{
	m_state->Exit();
}


}
}

