

#include "stdafx.h"
#include "GameSenario.h"
#include "ScenarioExitCode.h"
#include "GameLevelManager.h"
#include "CharacterDemoGlobals.h"

#include "KeyboardEvent.h"
#include "GUIEvent.h"

namespace mray{
namespace gameMod{

	const int ResumeButtonId=100;
	const int QuitButtonId=101;
GameSenario::GameSenario():m_level(0)
{

	m_state=0;
	m_pause=false;
}
GameSenario::~GameSenario(){
}

void GameSenario::update(float dt)
{
	if(m_level)
		m_level->Update(dt);
}

void GameSenario::onEvent(GCPtr<Event> event)
{
	if(event->isRecived())
		return;
	if(event->getType()==ET_Keyboard)
	{
		GCPtr<KeyboardEvent> e=event;
		if(e->press && e->key==KEY_ESCAPE){
			//			onExit();
			//m_state=SenarioGeneric_Done;
			m_menuWnd->Visible=!m_menuWnd->Visible();
			m_pause=!m_pause;
			gCursorControl.setVisible(m_menuWnd->Visible);
			gCursorControl.setToMidScreen(!m_menuWnd->Visible);
		}
	}else if(event->getType()==ET_GuiEvent){
		GCPtr<GUIEvent> e=event;
		if(e->Event==GET_Mouse_UP){
			if(e->Caller->Id==ResumeButtonId){
				m_pause=false;
				m_menuWnd->Visible=false;
				gCursorControl.setVisible(false);
				gCursorControl.setToMidScreen(false);
			}else if(e->Caller->Id==QuitButtonId){
				m_state=SenarioGeneric_Done;
				gCursorControl.setVisible(true);
				gCursorControl.setToMidScreen(false);
			}
		}
	}
}
void GameSenario::SetLevel(game::GameLevel* level)
{
	m_level=level;
}

int GameSenario::isDone(){
	return m_state;
}

void GameSenario::onEnter()
{
	SetLevel(game::GameLevelManager::getInstance().GetActiveLevel());
	if(!m_level)
		return;
	m_oldGuiMngr=CharacterDemoGlobals::getInstance()->getActiveGUIManager();
	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_level->GetGUIManager());

	m_oldSndMngr=CharacterDemoGlobals::getInstance()->getActiveSoundManager();
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(0);

	m_oldSceneMngr=CharacterDemoGlobals::getInstance()->getSceneManager();
	CharacterDemoGlobals::getInstance()->setSceneManager(m_level->GetSceneManager());


	GUI::GUIOverlayManager::getInstance().loadOverlay(mT("GameMenuOverlay.xml"));
	{
		GCPtr<GUI::GUIOverlay> o= GUI::GUIOverlayManager::getInstance().getOverlay(mT("GameMenuOverlay"));
		if(o){
			core::IteratorPair<GUI::ElementList> p(o->createElements());
			for (;!p.done();p++)
			{
				m_level->GetGUIManager()->addCustomChild(*p);
				if((*p)->Name().equals_ignore_case(mT("GameMenu"))){
					m_menuWnd=*p;
					m_menuWnd->Visible=false;
				}
			}
		}	
	}

	m_pause=false;
	m_state=0;
}

void GameSenario::onExit()
{
	m_level->GetGUIManager()->removeChild(m_menuWnd);

	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_oldGuiMngr);
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(m_oldSndMngr);
	CharacterDemoGlobals::getInstance()->setSceneManager(m_oldSceneMngr);

	m_oldGuiMngr=0;
	m_oldSndMngr=0;
	m_oldSceneMngr=0;
}


}
}

