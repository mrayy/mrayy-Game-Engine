

#include "stdafx.h"
#include "MenuSenario.h"
#include "CharacterDemoGlobals.h"
#include "SoundThemeManager.h"
#include "ScenarioExitCode.h"
#include "GameLevelManager.h"
#include "AnimationController.h"

#include "GUIPanel.h"



namespace mray{
namespace gameMod{

	enum EMenuButtonsID
	{
		STARTDEMO_BUTTON_ID=1	,
		SELECTDEMO_BUTTON_ID,
		DEMOBACK_BUTTON_ID,
		DEMOSCENES_LIST_ID,
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


MenuSenario::MenuSenario()
{

	m_cameraAngle=0;

	m_guiMngr=new GUI::GUIManager(CharacterDemoGlobals::getInstance()->getVideoDevice());
	m_smngr=new scene::SceneManager(CharacterDemoGlobals::getInstance()->getVideoDevice());

	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(CharacterDemoGlobals::getInstance()->getSoundManagerType().c_str());
	
	
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
	createLevelMenu();

	createSceneManager();


	m_fadeOutImg=m_guiMngr->addImage(0,1);
	m_fadeOutImg->Dock=GUI::ED_FILL;
	m_fadeOutImg->Visible=false;
	m_fadeOutImg->Color=video::SColor(0,0,0,0);

	//////////////////////////////////////////////////////////////////////////

// 	GCPtr<GUI::GUIImage> img=m_guiMngr->addImage(0,300);
// 	img->setTexture(gTextureResourceManager.loadTexture2D(mT("loadingScreen.png")));
// 	img->setDock(GUI::ED_FILL);
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




void MenuSenario::createLevelMenu()
{

	m_levelPanel=new GUI::GUIPanel(m_guiMngr,math::vector2d(40,100),math::vector2d(500,400));
	m_guiMngr->addChild(m_levelPanel);
	math::vector2d offset=0;
	float height=40;
	math::vector2d eoff(0,10);
	m_levelPanel->Visible=false;
	{
		GCPtr<GUI::GUIStaticText> t=m_guiMngr->addStaticText(mT("Demos"),math::vector2d(10,10)+offset,
			math::vector2d(200,height),0,m_levelPanel);
		t->Background=false;
		t->getFontAttributes()->fontSize=20;
		t->getFontAttributes()->fontColor.Set(255,255,0,255);
		offset+=eoff;
		offset.y+=height+10;

	}
	{
		m_levelsList=m_guiMngr->addList(math::vector2d(10,10)+offset,
			math::vector2d(200,300),this,m_levelPanel,255,DEMOSCENES_LIST_ID);
		m_levelsList->getFontAttributes()->fontSize=20;
		m_levelsList->getFontAttributes()->fontColor.Set(200,200,255,255);
		offset+=eoff;
		offset.y+=300+10;
		core::array<game::GameLevelInfo>& levels=game::GameLevelManager::getInstance().GetLoadedLevelsInfo();
		
		for(int i=0;i<levels.size();++i)
		{
			m_levelsList->addItem(levels[i].name.c_str());
		}
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Start Demo"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_levelPanel,240,STARTDEMO_BUTTON_ID);

		offset+=eoff;
		offset.y+=height;
	}
	{
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Back"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_levelPanel,240,DEMOBACK_BUTTON_ID);
		offset+=eoff;
		offset.y+=height;
	}

	{
		m_levelThumbnail=m_guiMngr->addImage(math::vector2d(320,40),math::vector2d(200),this,m_levelPanel);
		m_levelDescription=m_guiMngr->addStaticText(mT(""),math::vector2d(320,60+200),
			math::vector2d(200,200),0,m_levelPanel);
		m_levelDescription->Background=true;
		m_levelDescription->Border=true;
		m_levelDescription->getFontAttributes()->fontSize=14;
		m_levelDescription->getFontAttributes()->fontAligment=GUI::EFA_TopLeft;
		m_levelDescription->getFontAttributes()->fontColor.Set(255,255,0,255);
	}

	m_levelPanel->MainSize=math::vector2d(600,offset.y+20);
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
		GCPtr<GUI::GUIButton> b=m_guiMngr->addButton(mT("Demos"),math::vector2d(10,10)+offset,math::vector2d(200,height),this,
			m_mainMenuPanel,240,SELECTDEMO_BUTTON_ID);

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

	m_mainMenuPanel->MainSize=math::vector2d(20+200,offset.y+20);
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


		slider->Value=CharacterDemoGlobals::getInstance()->getGameDemoOptions()->viewDistance;
	}
	{
		GCPtr<GUI::GUICheckBox> b=m_guiMngr->addCheckBox(mT("PostEffect"),math::vector2d(10,10)+offset,math::vector2d(200,25),this,
			m_optionPanel,240,OPTION_POST_EFFECT_ID);
		b->BGOpaque=false;
		offset+=eoff;
		offset.y+=25;
		b->Checked=CharacterDemoGlobals::getInstance()->getGameDemoOptions()->postEffect;
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

	m_optionPanel->MainSize=math::vector2d(20+200,offset.y+20);

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

	m_videoPanel->MainSize=math::vector2d(20+200,offset.y+20);

}

void MenuSenario::createSceneManager()
{
	m_camera=m_smngr->addCamera(math::vector3d(-1,0.3,1.7),math::vector3d(-0.9,0.8,0));
	m_smngr->addAnimatedMesh(mT("DemoMenuBk.mdx"));
	scene::ViewPort*vp= m_smngr->createViewport(mT("MainVP"),0);
	{
		video::IVideoDevice*device=CharacterDemoGlobals::getInstance()->getVideoDevice();

		GCPtr<video::ITexture> rtTex=device->createTexture2D(vp->getAbsViewPort().getSize(),video::EPixel_R8G8B8A8);
		GCPtr<video::IRenderTarget> rt=device->createRenderTarget(mT(""),rtTex,0,0,0);
		vp->enablePostProcessing(true);
		
		GCPtr<video::ParsedShaderPP> pp=new video::ParsedShaderPP(device);
		pp->LoadXML(gFileSystem.openFile(mT("BloomSky.peff"),OS::TXT_READ));

		vp->setPostProcessing(pp);
		vp->setRenderTarget(rt);
	}

	GCPtr<scene::IAnimatedMesh>male=m_smngr->addAnimatedMesh(mT("Male.mdx"));
	if(male){
		animation::AnimationController*  track=male->getMesh()->getSkeleton()->getAnimationMixer()->getAnimationController(mT("Idle1"));
		track->getGroup()->playAll();
		track->play(10,animation::EAM_Cycle);
	}
	m_smngr->update(0);
}

void MenuSenario::playRandomSound(){
	if(m_bgSound){
		m_bgSound->stop();
	}
	/*
	core::string sname= CharacterDemoGlobals::getInstance()->GetSoundThemeManager()->getRandomSoundName();
	if(sname!=mT("")){
		m_bgSound = m_sndMngr->loadSound(sname.c_str(),1,sound::ESNDT_2D);
		if(m_bgSound){
			m_bgSound->setVolume(CharacterDemoGlobals::getInstance()->GetSoundThemeManager()->getBGVolume());
			m_bgSound->play();
		}
		}*/
	m_bgSound = m_sndMngr->loadSound(mT("Menu.mp3"),1,sound::ESNDT_2D);
	if(m_bgSound){
		m_bgSound->setVolume(CharacterDemoGlobals::getInstance()->GetSoundThemeManager()->getBGVolume());
		m_bgSound->play();
	}
}

void MenuSenario::onEnter(){
	m_oldGuiMngr=CharacterDemoGlobals::getInstance()->getActiveGUIManager();
	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=CharacterDemoGlobals::getInstance()->getActiveSoundManager();
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(m_sndMngr);


	m_oldSmngr=CharacterDemoGlobals::getInstance()->getSceneManager();
	CharacterDemoGlobals::getInstance()->setSceneManager(m_smngr);

	playRandomSound();

	m_state=0;
	m_currentState=3;
	m_fadeOutImg->Visible=true;
	m_fadeOutImg->Color=video::SColor(0,0,0,255);
	m_bgAlpha=255;
}

void MenuSenario::onExit(){
	CharacterDemoGlobals::getInstance()->setActiveGUIManager(m_oldGuiMngr);
	CharacterDemoGlobals::getInstance()->setActiveSoundManager(m_oldSndMngr);
	CharacterDemoGlobals::getInstance()->setSceneManager(m_oldSmngr);
	m_oldGuiMngr=0;
	m_oldSndMngr=0;

}
void MenuSenario::update(float dt){

	if(m_currentState==3)//enter
	{
		if(m_bgAlpha>0){
			m_bgAlpha-=150*dt;
			if(m_bgAlpha<0)
				m_bgAlpha=0;
			video::SColor color=m_fadeOutImg->Color;
			color.A=m_bgAlpha;
			m_fadeOutImg->Color=color;
		}else{
			m_fadeOutImg->Visible=false;
			m_currentState=0;
		}
	}else
	if(m_currentState==1){//Exit
		if(m_bgAlpha<255){
			m_bgAlpha+=150*dt;
			if(m_bgAlpha>255)
				m_bgAlpha=255;
			video::SColor color=m_fadeOutImg->Color;
			color.A=m_bgAlpha;
			m_fadeOutImg->Color=color;
		}else
			m_currentState=2;
	}else if(m_currentState==2){
		m_state=MenuSenario_Exit;
		CharacterDemoGlobals::getInstance()->exitApplication();
	}

	m_cameraAngle+=dt;
	
}

void MenuSenario::onEvent(GCPtr<Event> event){
	switch (event->getType())
	{
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e=event;
			switch (e->Caller->Id)
			{
			case STARTDEMO_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					int selected=m_levelsList->getSelectedItem();
					if(selected==-1)
						break;
					const game::GameLevelInfo& info= game::GameLevelManager::getInstance().GetLoadedLevelsInfo()[selected];
					m_state=MenuSenario_StartGame;
					CharacterDemoGlobals::getInstance()->SetRequestedLevel(info.path);
				}
				break;
			case SELECTDEMO_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					//m_state=MenuSenario_StartGame;
					//GameLevelManager::getInstance().setCurrentLevel(mT("testLevel"));
					m_mainMenuPanel->Visible=false;
					m_levelPanel->Visible=true;
				}
				break;
			case DEMOSCENES_LIST_ID:
				if(e->Event==GET_Mouse_UP){
					int selected=m_levelsList->getSelectedItem();
					if(selected==-1)
						break;
					const game::GameLevelInfo& info= game::GameLevelManager::getInstance().GetLoadedLevelsInfo()[selected];
					m_levelDescription->Caption=info.description;
					m_levelThumbnail->setTexture(gTextureResourceManager.loadTexture2D(info.thumbnail));
				}break;
			case DEMOBACK_BUTTON_ID:
				if(e->Event==GET_Mouse_UP){
					//m_state=MenuSenario_StartGame;
					//GameLevelManager::getInstance().setCurrentLevel(mT("testLevel"));
					m_mainMenuPanel->Visible=true;
					m_levelPanel->Visible=false;
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
			case OPTION_POST_EFFECT_ID:
				if(e->Event==GET_Mouse_UP){
					GUI::GUICheckBox* g=(GUI::GUICheckBox*) e->Caller;

					//GameOptionManager::getInstance().postEffect=g->Checked;
				}
				break;
			case OPTION_VIEW_DIST_ID:
				if(e->Event==GET_CHANGED){
					GUI::GUISlider* g=(GUI::GUISlider*)e->Caller;

					//GameOptionManager::getInstance().viewDistance=g->Value;
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
