

#include "MenuSenario.h"
#include "IEaglesManager.h"
#include "CSoundthemeManager.h"
#include "ScenarioExitCode.h"
#include "GameLevelManager.h"
#include "TextureManager.h"
#include "GameOptionManager.h"

#include <SoundManagerFactory.h>
#include <TextureResourceManager.h>
#include <GUIButton.h>
#include <GUIPanel.h>
#include <GUIImage.h>
#include <GUIEvent.h>

#include <GUIStaticText.h>
#include <GUIRadioButton.h>
#include <GUISlider.h>
#include <GUICheckBox.h>
#include <GUIOverlayManager.h>
#include <GUIComboList.h>

namespace mray{
namespace gameMod{

	enum EMenuButtonsID
	{
		NEWGAME_BUTTON_ID=1	,
		OPTIONS_BUTTON_ID	,
		CREDITS_BUTTON_ID	,
		OPTION_BACK_BUTTON_ID	,
		OPTION_VIDEO_BUTTON_ID	,
		OPTION_TEX_LOW_ID,
		OPTION_TEX_HI_ID,
		OPTION_VIEW_DIST_ID,
		OPTION_POST_EFFECT_ID,
		VIDEO_RESOLUTION_ID	,
		VIDEO_APPLY_BUTTON_ID	,
		VIDEO_BACK_BUTTON_ID	,
		EXIT_BUTTON_ID		
	};


MenuSenario::MenuSenario(){

	m_guiMngr=new GUI::GUIManager(eagles::IEaglesManager::getInstance().getVideoDevice());

	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(eagles::IEaglesManager::getInstance().getSoundManagerType().c_str());


	m_state=0;

	{
		GUI::GUIOverlayManager::getInstance().loadOverlay(mT("MenuOverlay.xml"));
		GCPtr<GUI::GUIOverlay> o= GUI::GUIOverlayManager::getInstance().getOverlay(mT("MenuOverlay"));
		if(o){
			core::IteratorPair<GUI::ElementList> p( o->createElements());
			for(;!p.done();p++){
				m_guiMngr->addCustomChild(*p);
			}
		}
	}
	createMainMenu();
	createOptionMenu();


	m_fadeOutImg=m_guiMngr->addImage(0,1);
	m_fadeOutImg->Dock=GUI::ED_FILL;
	m_fadeOutImg->Visible=false;
	m_fadeOutImg->Color=video::SColor(0,0,0,0);

	//////////////////////////////////////////////////////////////////////////

// 	GCPtr<GUI::GUIImage> img=m_guiMngr->addImage(0,300);
// 	img->setTexture(gTextureResourceManager.loadTexture2D(mT("loadingScreen.png")));
// 	img->setDock(GUI::ED_FILL);
}

void MenuSenario::createMainMenu(){


	m_mainMenuPanel=new GUI::GUIPanel(m_guiMngr,math::vector2d(40,100),math::vector2d(260,200));
	m_guiMngr->addChild(m_mainMenuPanel);
	math::vector2d offset=0;
	float height=40;
	math::vector2d eoff(0,10);
	{
		GCPtr<GUI::GUIStaticText> t=m_guiMngr->addStaticText(mT("Main Menu"),math::vector2d(10,10)+offset,
			math::vector2d(200,height),0,m_mainMenuPanel);
		t->Background=false;
		t->getFontAttributes()->fontSize=20;
		t->getFontAttributes()->fontColor.Set(255,255,0,255);
		offset+=eoff;
		offset.y+=height+10;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("New Game"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_mainMenuPanel,240,NEWGAME_BUTTON_ID);

		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Options"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_mainMenuPanel,240,OPTIONS_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Credits"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_mainMenuPanel,240,CREDITS_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Exit"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_mainMenuPanel,240,EXIT_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}

	m_mainMenuPanel->MainSize().x=20+200;
	m_mainMenuPanel->MainSize().y=offset.y+20;
}

void MenuSenario::createOptionMenu(){


	m_optionPanel=new GUI::GUIPanel(m_guiMngr,math::vector2d(40,100),math::vector2d(260,200));
	m_guiMngr->addChild(m_optionPanel);
	math::vector2d offset=0;
	float height=40;
	math::vector2d eoff(0,10);

	m_optionPanel->Visible=false;

	{
		GCPtr<GUI::GUIStaticText> t=m_guiMngr->addStaticText(mT("Options"),math::vector2d(10,10)+offset,
			math::vector2d(200,height),0,m_optionPanel);
		t->Background=false;
		t->getFontAttributes()->fontSize=20;
		t->getFontAttributes()->fontColor.Set(255,255,0,255);
		offset+=eoff;
		offset.y+=height+10;
	}
	{
		GCPtr<GUI::GUIStaticText> text=m_guiMngr->addStaticText(mT("Texture Quality:"),math::vector2d(10,10)+offset,math::vector2d(200,25),this,m_optionPanel);
		text->Background=false;

		offset+=eoff;
		offset.y+=25;

		GCPtr<GUI::GUIRadioButton> rdo;
		rdo=m_guiMngr->addRadioButton(mT("Low"),math::vector2d(10,10)+offset,math::vector2d(100,20),this,
			m_optionPanel,240,OPTION_TEX_LOW_ID);
		rdo->BGOpaque=false;
		rdo->getFontAttributes()->fontSize=14;

		if(GameOptionManager::getInstance().textureQuality==ETQ_Low)
			rdo->Checked=true;

		rdo=m_guiMngr->addRadioButton(mT("Hi"),math::vector2d(110,10)+offset,math::vector2d(100,20),this,
			m_optionPanel,240,OPTION_TEX_HI_ID);
		rdo->BGOpaque=false;
		rdo->getFontAttributes()->fontSize=14;
		if(GameOptionManager::getInstance().textureQuality==ETQ_High)
			rdo->Checked=true;


		offset+=eoff;
		offset.y+=20;
	}
	{
		GCPtr<GUI::GUIStaticText> text=m_guiMngr->addStaticText(mT("View Distance:"),math::vector2d(10,10)+offset,math::vector2d(200,25),this,m_optionPanel);
		text->Background=false;

		offset+=eoff;
		offset.y+=25;

		GCPtr<GUI::GUISlider> slider;
		slider=m_guiMngr->addSlider(10,100,math::vector2d(10,10)+offset,math::vector2d(200,20),this,
			m_optionPanel,240,OPTION_VIEW_DIST_ID);
		offset+=eoff;
		offset.y+=20;


		slider->Value=GameOptionManager::getInstance().viewDistance;
	}
	{
		GCPtr<GUI::GUICheckBox> b=m_guiMngr->addCheckBox(mT("PostEffect"),math::vector2d(10,10)+offset,math::vector2d(200,25),this,
			m_optionPanel,240,OPTION_POST_EFFECT_ID);
		b->BGOpaque=false;
		offset+=eoff;
		offset.y+=25;
		b->Checked=GameOptionManager::getInstance().postEffect;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Video"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_optionPanel,240,OPTION_VIDEO_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Back"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_optionPanel,240,OPTION_BACK_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}

	m_optionPanel->MainSize().x=20+200;
	m_optionPanel->MainSize().y=offset.y+20;

	createVideoMenu();

}

void MenuSenario::createVideoMenu(){


	m_videoPanel=new GUI::GUIPanel(m_guiMngr,math::vector2d(40+m_optionPanel->MainPos().x+m_optionPanel->MainSize().x,100),math::vector2d(260,200));
	m_guiMngr->addChild(m_videoPanel);
	math::vector2d offset=0;
	float height=40;
	math::vector2d eoff(0,10);

	m_videoPanel->Visible=false;

	{
		GCPtr<GUI::GUIStaticText> t=m_guiMngr->addStaticText(mT("Resolution"),math::vector2d(10,10)+offset,
			math::vector2d(200,height),0,m_videoPanel);
		t->Background=false;
		t->getFontAttributes()->fontSize=20;
		t->getFontAttributes()->fontColor.Set(255,255,0,255);
		offset+=eoff;
		offset.y+=height+10;
	}
	{
		m_resolutionList=m_guiMngr->addComboList(mT(""),math::vector2d(10,10)+offset,
			math::vector2d(200,height),0,m_videoPanel,240,VIDEO_RESOLUTION_ID);


		core::array<video::VMode> &modes=eagles::IEaglesManager::getInstance().getVideoModesList();
		for (int i=0;i<modes.size();i++)
		{
			m_videoModes.push_back(modes[i]);
			core::string txt;
			txt=core::StringConverter::toString(modes[i].dim);
			float ratio=(float)modes[i].dim.x/(float)modes[i].dim.y;
			if(!math::fEqual(ratio,4.0f/3.0f)){
				txt+=mT("(wide)");
			}
			m_resolutionList->addItem(txt.c_str());
		}

		m_resolutionList->getFontAttributes()->fontSize=14;
		offset+=eoff;
		offset.y+=height+10;
		m_resolutionList->Visible=false;
	}
	{
		m_fullScreen=m_guiMngr->addCheckBox(mT("Fullscreen"),math::vector2d(10,10)+offset,math::vector2d(200,25),this,
			m_videoPanel,240,OPTION_POST_EFFECT_ID);
		m_fullScreen->BGOpaque=false;
		offset+=eoff;
		offset.y+=25;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Apply"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_videoPanel,240,VIDEO_APPLY_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Back"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_videoPanel,240,VIDEO_BACK_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}

	m_videoPanel->MainSize().x=20+200;
	m_videoPanel->MainSize().y=offset.y+20;

}

MenuSenario::~MenuSenario(){

	m_bgSound=0;
	m_mainMenuPanel=0;
	m_optionPanel=0;

	m_guiMngr=0;
	m_oldGuiMngr=0;
	m_sndMngr=0;
	m_oldSndMngr=0;
}

void MenuSenario::playRandomSound(){
	if(m_bgSound){
		m_bgSound->stop();
	}
	core::string sname= CSoundthemeManager::getInstance().getRandomSoundName();
	if(sname!=mT("")){
		m_bgSound = m_sndMngr->loadSound(sname.c_str(),1,sound::ESNDT_2D);
		if(m_bgSound){
			m_bgSound->setVolume(CSoundthemeManager::getInstance().getBGVolume());
			m_bgSound->play();
		}
	}
}

void MenuSenario::onEnter(){
	m_oldGuiMngr=eagles::IEaglesManager::getInstance().getActiveGUIManager();
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=eagles::IEaglesManager::getInstance().getActiveSoundManager();
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_sndMngr);

	playRandomSound();

	m_state=0;
	m_currentState=0;
	m_fadeOutImg->Visible=false;
	m_fadeOutImg->Color=video::SColor(0);
	m_bgAlpha=0;
	m_fullScreen->Checked=eagles::IEaglesManager::getInstance().getAppWindow()->isFullScreen();
}

void MenuSenario::onExit(){
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_oldGuiMngr);
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_oldSndMngr);
	m_oldGuiMngr=0;
	m_oldSndMngr=0;

}
void MenuSenario::update(float dt){

	if(m_currentState==1){//Exit
		if(m_bgAlpha<255){
			m_bgAlpha+=150*dt;
			if(m_bgAlpha>255)
				m_bgAlpha=255;
			m_fadeOutImg->Color().A=m_bgAlpha;
		}else
			m_currentState=2;
	}else if(m_currentState==2){
		m_state=MenuSenario_Exit;
		eagles::IEaglesManager::getInstance().exitApplication();
	}
}

void MenuSenario::onEvent(GCPtr<Event> event){
	switch (event->getType())
	{
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e=event;
			switch (e->Caller->Id)
			{
			case NEWGAME_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_state=MenuSenario_StartGame;
					GameLevelManager::getInstance().setCurrentLevel(mT("testLevel"));
				}
				break;
			case CREDITS_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_state=MenuSenario_About;
				}
				break;
			case OPTIONS_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_optionPanel->Visible=true;
					m_mainMenuPanel->Visible=false;
				}
				break;
			case EXIT_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_fadeOutImg->Visible=true;
					m_currentState=1;
					if(m_bgSound)
						m_bgSound->stop();
				}
				break;
			case OPTION_TEX_HI_ID:
				if(e->Event==GET_Mouse_UP){
					GameOptionManager::getInstance().textureQuality=ETQ_High;
				}
				break;
			case OPTION_TEX_LOW_ID:
				if(e->Event==GET_Mouse_UP){
					GameOptionManager::getInstance().textureQuality=ETQ_Low;
				}
				break;
			case OPTION_POST_EFFECT_ID:
				if(e->Event==GET_Mouse_UP){
					GUI::GUICheckBox* g=(GUI::GUICheckBox*) e->Caller;

					GameOptionManager::getInstance().postEffect=g->Checked;
				}
				break;
			case OPTION_VIEW_DIST_ID:
				if(e->Event==GET_CHANGED){
					GUI::GUISlider* g=(GUI::GUISlider*)e->Caller;

					GameOptionManager::getInstance().viewDistance=g->Value;
				}
				break;
			case OPTION_VIDEO_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_optionPanel->Enable=false;
					m_videoPanel->Visible=true;
				}
				break;
			case OPTION_BACK_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_optionPanel->Visible=false;
					m_mainMenuPanel->Visible=true;
				}
				break;
			case VIDEO_APPLY_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					int i=m_resolutionList->getSelectedItem();
					if(i!=-1){
						eagles::IEaglesManager::getInstance().getAppWindow()->changeVideoMode(m_videoModes[i]);
					}
					if( m_fullScreen->Checked())
						eagles::IEaglesManager::getInstance().getAppWindow()->toFullScreen();
					else
						eagles::IEaglesManager::getInstance().getAppWindow()->toWindowMode();
				}
				break;
			case VIDEO_BACK_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					m_optionPanel->Enable=true;
					m_videoPanel->Visible=false;
				}
				break;

			}
		}
		break;
	}
}

int MenuSenario::isDone(){
	return m_state;
}

}
}
