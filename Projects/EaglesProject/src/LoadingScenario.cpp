
#include "LoadingScenario.h"
#include "IEaglesManager.h"
#include "ScenarioExitCode.h"

#include <keyboardEvent.h>
#include <GUIEvent.h>
#include <TextureResourceManager.h>
#include <SoundManagerFactory.h>
#include <GUIImage.h>
#include <GUIPercentAnchorEff.h>
#include <GUIOverlayManager.h>
#include <ILogManager.h>
#include <ICursorControl.h>
#include "GameLevelManager.h"


#include <windows.h>

namespace mray{
namespace gameMod{

#define PROCEED_BTN_ID 100

LoadingScenario::LoadingScenario(){


	m_done=0;
	m_guiMngr=new GUI::GUIManager(eagles::IEaglesManager::getInstance().getVideoDevice());

	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(eagles::IEaglesManager::getInstance().getSoundManagerType().c_str());

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

	m_info=m_guiMngr->addStaticText(mT(""),math::vector2d(0,0),math::vector2d(400,40));
	m_info->addEffector(new GUI::GUIPercentAnchorEff(GUI::EPAnchor_U,math::vector2d(0,0.65)));
	m_info->Background=false;

	m_proceedBtn=m_guiMngr->addButton(mT("Press Any Key"),math::vector2d(0,0),math::vector2d(200,30),this,0,240,PROCEED_BTN_ID);
	m_proceedBtn->addEffector(new GUI::GUIPercentAnchorEff(GUI::EPAnchor_U,math::vector2d(0,0.65)));
	m_proceedBtn->Visible=false;


	m_controllsGUI=m_guiMngr->addImage(0,math::vector2d(400,380));
	m_controllsGUI->setTexture(gTextureResourceManager.loadTexture2D(mT("controls.png")));
	m_controllsGUI->addEffector(m_guiMngr->addAnchorEffector(GUI::EPAnchor_CENTER,math::vector2d(0,-100)));
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

	m_oldGuiMngr=eagles::IEaglesManager::getInstance().getActiveGUIManager();
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=eagles::IEaglesManager::getInstance().getActiveSoundManager();
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_sndMngr);

	gCursorControl.setVisible(0);

	eagles::IEaglesManager::getInstance().redraw();
	eagles::IEaglesManager::getInstance().redraw();
	GameLevelManager::getInstance().loadCurrentLevel(this);
	gCursorControl.setVisible(1);
}

void LoadingScenario::onExit(){
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_oldGuiMngr);
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_oldSndMngr);
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


void LoadingScenario::beginLoadResources(int count){
	m_loadingBar->setStepCount(count);
	eagles::IEaglesManager::getInstance().redraw();
}

void LoadingScenario::onResourceLoadStart(const core::string&name){
	m_info->Caption=mT("Loading \"")+name+mT("\"");
	eagles::IEaglesManager::getInstance().redraw();
//	Sleep(1000);
}

void LoadingScenario::onResourceLoadDone(const core::string&name){
	m_loadingBar->addSteps(1);
	eagles::IEaglesManager::getInstance().redraw();
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