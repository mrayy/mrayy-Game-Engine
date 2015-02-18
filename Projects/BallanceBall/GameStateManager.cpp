

#include "stdafx.h"
#include "GameStateManager.h"

#include "StateMachine.h"

namespace mray
{
namespace Ball
{

GameStateManager::GameStateManager()
{
	m_stateMachine=new StateMachine();
}
GameStateManager::~GameStateManager()
{
	delete m_stateMachine;
}

GameStateManager::Update(float dt)
{
	m_stateMachine->onUpdate();
	ScenarioState*s=(ScenarioState*) m_stateMachine->getActiveState();
	if(s)
	{
		s->GetGameState()->Update(dt);
	}
}

}
}

