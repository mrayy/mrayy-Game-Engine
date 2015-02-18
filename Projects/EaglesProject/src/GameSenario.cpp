

#include "GameSenario.h"
#include "ScenarioExitCode.h"
#include "GameLevelManager.h"
#include "ScenarioExitCode.h"


namespace mray{
namespace gameMod{

GameSenario::GameSenario(){
}
GameSenario::~GameSenario(){
}

void GameSenario::setLevel(GCPtr<GameLevel> gameLevel){
	m_gameLevel=gameLevel;
}

void GameSenario::update(float dt){
	if(m_gameLevel)
		m_gameLevel->update(dt);
}

void GameSenario::onEvent(GCPtr<Event> event){
	if(m_gameLevel)
		m_gameLevel->onEvent(event);

}

int GameSenario::isDone(){
	if(m_gameLevel)
		return m_gameLevel->getState();
	return SenarioGeneric_Done;
}

void GameSenario::onEnter(){
	setLevel(GameLevelManager::getInstance().getCurrentLevel());
	if(m_gameLevel)
		m_gameLevel->onEnter();
}

void GameSenario::onExit(){
	if(m_gameLevel)
		m_gameLevel->onExit();
}

}
}

