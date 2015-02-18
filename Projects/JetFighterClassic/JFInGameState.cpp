

#include "stdafx.h"
#include "JFInGameState.h"
#include "JFGameLevelManager.h"


namespace mray
{
namespace game
{

const core::string JFInGameState::StateName(mT("InGameState"));

JFInGameState::JFInGameState()
{
	m_gameManager=new JFGameLevelManager();
	m_exitCode=0;
}
JFInGameState::~JFInGameState()
{
	delete m_gameManager;
}

void JFInGameState::OnEvent(Event* e)
{
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* evt=(KeyboardEvent*)e;
	}
}

void JFInGameState::OnEnter()
{
	m_exitCode=0;
}
void JFInGameState::Update(float dt)
{
}
void JFInGameState::OnExit()
{
}

uint JFInGameState::GetExitCode()
{
	return m_exitCode;
}

}
}


