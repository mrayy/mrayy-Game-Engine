
#include "stdafx.h"
#include "LoadingScenario.h"
#include "CharacterDemoGlobals.h"
#include "ScenarioExitCode.h"

namespace mray{
namespace gameMod{

#define PROCEED_BTN_ID 100

LoadingScenario::LoadingScenario(){


	m_done=0;
	m_guiMngr=new GUI::GUIManager(CharacterDemoGlobals::getInstance()->getVideoDevice());

	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(CharacterDemoGlobals::getInstance()->getSoundManagerType().c_str());

	{
		GUI::GUIOverlayManager::getInstance().loadOverlay(mT("LoadingOverlay.xml"));
		GCPtr<GUI::GUIOverlay> o= GUI::GUIOverlayManager::getInstance().getOverlay(mT("LoadingOverlay"));
		if(o){
			core::IteratorPair<GUI::ElementList> p( o->createElements());
			for(;!p.done();p++){
				m_guiMngr->addCustomChild(*p);
			}
		}
	}

	m_loadingBar=m_guiMngr->addLoadingBar(0,100,math::vector2d(0,0),math::vector2d(400,10));
	m_loadingBar->addEffector(new GUI::GUIPercentAnchorEff(GUI::EPAnchor_U,math::vector2d(0,0.75)));
	m_loadingBar->setLoadingColor(video::DefaultColors::White);

	m_info=m_guiMngr->addStaticText(mT(""),math::vector2d(0,0),math::vector2d(400,40));
	m_info->addEffector(new GUI::GUIPercentAnchorEff(GUI::EPAnchor_U,math::vector2d(0,0.65)));
	m_info->Background=false;

	m_proceedBtn=m_guiMngr->addButton(mT("Press Any Key"),math::vector2d(0,0),math::vector2d(200,30),this,0,240,PROCEED_BTN_ID);
	m_proceedBtn->addEffector(new GUI::GUIPercentAnchorEff(GUI::EPAnchor_U,math::vector2d(0,0.65)));
	m_proceedBtn->Visible=false;


}
LoadingScenario::~LoadingScenario(){
	m_oldGuiMngr=0;
}


void LoadingScenario::onEnter(){
	m_done=0;

	m_proceedBtn->Visible=false;
	m_info->Caption=mT("");
	m_info->Visible=true;
	m_loadingBar->Visible=true;

	m_loadingBar->reset();

	m_oldGuiMngr=CharacterDemoGlobals::getInstance()->getActiveGUIManager();
	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=CharacterDemoGlobals::getInstance()->getActiveSoundManager();
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(m_sndMngr);

	gCursorControl.setVisible(0);

	CharacterDemoGlobals::getInstance()->redrawFrame();
	CharacterDemoGlobals::getInstance()->LoadRequestedLevel(this);
	gCursorControl.setVisible(1);
}

void LoadingScenario::onExit(){
	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_oldGuiMngr);
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(m_oldSndMngr);
	m_oldGuiMngr=0;
	m_oldSndMngr=0;

}
void LoadingScenario::update(float dt){
	//m_loadingBar->addSteps(1);
}

void LoadingScenario::onEvent(GCPtr<Event> event){
	switch (event->getType())
	{
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent> e=event;
			if(e->press){
				m_done=true;
			}
		}
		break;
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e= event;
			if(e->Caller->Id()==PROCEED_BTN_ID &&
				e->Event==GET_Mouse_UP){
				m_done=true;
			} 
		}break;
	}
}

int LoadingScenario::isDone(){
	if(m_done)
		return SenarioGeneric_Done;
	return 0;
}


void LoadingScenario::beginLoadResources(const core::string& category,int count){
	m_loadingBar->setStepCount(count);
	CharacterDemoGlobals::getInstance()->redrawFrame();
}

void LoadingScenario::onResourceLoadStart(const core::string&name){
	m_info->Caption=mT("Loading \"")+name+mT("\"");
	CharacterDemoGlobals::getInstance()->redrawFrame();
//	Sleep(1000);
}

void LoadingScenario::onResourceLoadDone(const core::string&name){
	m_loadingBar->addSteps(1);
	CharacterDemoGlobals::getInstance()->redrawFrame();
}

void LoadingScenario::resourceInfo(const core::string&info){
}

void LoadingScenario::loadDone(){
	m_info->Caption=mT("Done");
	m_info->Visible=false;
	m_loadingBar->Visible=false;
	m_proceedBtn->Visible=true;
}

}
}