
#include "stdafx.h"
#include "Application.h"
#include "Win32WindowUtils.h"

#include "IntroSenario.h"
#include "MenuSenario.h"
#include "AboutSenario.h"
#include "LoadingScenario.h"
#include "GameSenario.h"
#include "ScenarioExitCode.h"
#include "DemoCamera.h"

#include <CVFWVideoTexture.h>
#include <MeshAnimatorManager.h>

#include <PhysXAPI.h>

namespace mray
{

	using namespace math;
	using namespace core;

Application::Application()
{
	m_defaultSoundManagerType=mT("fmod");
	m_assetsPath=mT("../Assets/");

	m_screenShot=false;

	m_showMemoryState=false;
	m_showFPS=false;

}
Application:: ~Application()
{
	m_level=0;
}

void Application::onEvent(GCPtr<Event> event)
{
	CMRayApplication::onEvent(event);
}

void Application::init(core::array<SOptionElement> *extraOptions)
{
	traceFunction(Application);

 
	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	scene::MeshAnimatorManager::getInstance().SetGPUSkeletonAnimatorParams(3,4);

	{
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("Default.xml"));
		GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
		GUI::GUISkinManager::getInstance().setActiveSkin(mT("Default"));

		//load font
		m_font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(m_font);
	}
	{
		m_cursor=new GUI::GUIAnimatedSprite(0,gTextureResourceManager.loadTexture2D(mT("MouseCursor.png")),1,1,1,0,0,32);
		m_cursor->SetDevice(getDevice());
		gCursorControl.SetMouseMap(m_cursor);
		gCursorControl.CursorMap(1);
	}
	{
		m_soundThemeMngr=new sound::SoundThemeManager();
		m_soundThemeMngr->loadThemesFromDirs(m_assetsPath+mT("sound/themes"));
	}

	// init Senario Manager
	m_scenarioManager=new gameMod::ScenarioManager();
	m_gameLevelManager=new game::GameLevelManager();
	m_gameLevelManager->LoadLevelsFile(mT("Demos.xml"));

	m_optionManager=new GameDemoOptions(mT("config.xml"));

	{
		GCPtr<gameMod::IntroSenario> im=new gameMod::IntroSenario();
		GCPtr<video::CVFWVideoTexture> v= new video::CVFWVideoTexture(256);
		v->loadVideo(mT("logo.avi"));
		v->play();
		im->createVideoItem(v);

		im->createImageItem(gTextureResourceManager.loadTexture2D(mT("BigLogo.png")),2,3);

		gameMod::ImageIntroItem* title= im->createImageItem(gTextureResourceManager.loadTexture2D(mT("IntroTitle.png")),3,5);

		math::rectf rc;
		rc.ULPoint.set(0.35,0.35);
		rc.BRPoint.set(0.65,0.65);

		title->setRect(rc);

		m_scenarioManager->addNode(im,mT("MainIntro"));
	}
	{
		GCPtr<gameMod::MenuSenario> ms=new gameMod::MenuSenario();

		m_scenarioManager->addNode(ms,mT("MainMenu"));
	}
	{
		GCPtr<gameMod::LoadingScenario> ms=new gameMod::LoadingScenario();

		m_scenarioManager->addNode(ms,mT("Loading"));
	}
	{
		GCPtr<gameMod::GameSenario> ms=new gameMod::GameSenario();

		m_scenarioManager->addNode(ms,mT("game"));
	}
	{
		GCPtr<gameMod::AboutSenario> ms=new gameMod::AboutSenario();

		m_scenarioManager->addNode(ms,mT("About"));
	}
	//Transitions
	{
		m_scenarioManager->addTransition(mT("MainIntro"),mT("MainMenu"),SenarioGeneric_Done);
		m_scenarioManager->addTransition(mT("MainMenu"),mT("Loading"),MenuSenario_StartGame);
		m_scenarioManager->addTransition(mT("MainMenu"),mT("About"),MenuSenario_About);
		m_scenarioManager->addTransition(mT("About"),mT("MainMenu"),SenarioGeneric_Done);
		m_scenarioManager->addTransition(mT("Loading"),mT("game"),SenarioGeneric_Done);
		m_scenarioManager->addTransition(mT("game"),mT("MainMenu"),SenarioGeneric_Done);
	}

	{
		scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
		scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\boat\\"),mT("png"),mT("boat"));
	}
	//bench mark and the console

	m_console=new GUI::GUIConsole(0,200);
	m_console->SetDevice(getDevice());
	m_console->Visible=false;


	m_benchmark=new GUI::GUIBenchMark(0,math::vector2d(200,10),math::vector2d(300,100));
	m_benchmark->Dock=GUI::ED_BOTTOM;
	m_benchmark->Visible=false;
	m_benchmark->SetDevice(getDevice());
	m_benchmark->getFontAttributes()->fontSize=12;
	m_benchmark->getFontAttributes()->fontColor.Set(0,0,0,255);
	m_benchmark->getFontAttributes()->hasShadow=false;

	m_soundTrackUI=new GUI::GUISoundTrack(getGUIManager(),10,0,math::vector2d(300,150));
	//m_soundTrackUI->addEffector();


	m_viewPort=getSceneManager()->createViewport(mT("MainVP"),0);


	//////////////////////////////////////////////////////////////////////////
	m_levelGUIManager=new GUI::GUIManager(CharacterDemoGlobals::getInstance()->getVideoDevice());
	m_levelSceneManager=getSceneManager();
	m_level=new game::GameLevel();

	{

		GCPtr<video::ITexture> rtTex=getDevice()->createTexture2D(m_viewPort->getAbsViewPort().getSize(),video::EPixel_R8G8B8A8);
		GCPtr<video::IRenderTarget> rt=getDevice()->createRenderTarget(mT(""),rtTex,0,0,0);
		m_viewPort->enablePostProcessing(true);

		m_viewPort->setPostProcessing(m_level->GetEnvironment()->GetPostProcessingEffect());
		m_viewPort->setRenderTarget(rt);
	}

	m_level->SetSceneManager(m_levelSceneManager);
	physics::PhysicsSystemDesc phDesc;
	phDesc.gravity.set(0,-9.8,0);
	m_level->SetPhysicsManager(physics::createPhysXManager(&phDesc));
	m_level->SetGUIManager(m_levelGUIManager);
	scene::SCameraNode* camera=new DemoCamera(m_levelSceneManager,50);
	m_levelSceneManager->addSceneNode(camera);
	m_level->SetCamera(camera);

	m_levelSoundManager=sound::SoundManagerFactory::getInstance().
		createSoundManager(getSoundManagerType().c_str());
	m_level->SetSoundManager(m_levelSoundManager);

	m_level->InitManagers();

	setSceneManager(0);

	//m_enableSceneManager=false;

}
void Application::update(float dt)
{
	CMRayApplication::update(dt);
	m_scenarioManager->update(dt);

}
void Application::draw()
{
	CMRayApplication::draw();

	m_scenarioManager->draw();
	getDevice()->set2DMode();
	if(m_font && false){
		GUI::FontAttributes attr;
		attr.fontColor.Set(20,255,128,255);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=24;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,255);
		attr.shadowOffset=vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg=mT("FPS= ");
		msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
		m_font->print(math::rectf(10,10,10,10),&attr,0,msg);
		attr.fontSize=18;
		msg=mT("Primitive drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
		m_font->print(math::rectf(10,30,10,10),&attr,0,msg);
		msg=mT("Batches drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
		m_font->print(math::rectf(10,50,10,10),&attr,0,msg);
	}
}
void Application::onDone()
{
}


core::string Application::getAssestsPath()
{
	return m_assetsPath;
}

video::IVideoDevice* Application::getVideoDevice()
{
	return getDevice();
}

core::string Application::getSoundManagerType()
{
	return m_defaultSoundManagerType;
}

GUI::GUIManager* Application::getActiveGUIManager()
{
	return gmngr;
}
void Application::setActiveGUIManager(GUI::GUIManager* mngr)
{
	if(this->gmngr){
		this->gmngr->removeChild(m_console);
		this->gmngr->removeChild(m_benchmark);
		//this->gmngr->removeChild(m_soundTrackUI);
		this->gmngr->unregisterBenchmark();
	}
	this->gmngr=mngr;
	if(this->gmngr){
		this->gmngr->addCustomChild(m_console);
		this->gmngr->addCustomChild(m_benchmark);
		//this->gmngr->addCustomChild(m_soundTrackUI);
		//m_soundTrackUI->show(mT("Music.mp3"),mT("Game Music"));
		this->gmngr->registerBenchmark();
	}
	m_cursor->SetManager(mngr);
}

sound::ISoundManager* Application::getActiveSoundManager()
{
	return sndMngr;
} 
void Application::setActiveSoundManager(sound::ISoundManager* mngr)
{
	if(this->sndMngr){
		this->sndMngr->stopAllSounds();
	}
	this->sndMngr=mngr;
} 

GameDemoOptions* Application::getGameDemoOptions()
{
	return m_optionManager;
}

sound::SoundThemeManager* Application::GetSoundThemeManager()
{
	return m_soundThemeMngr;
}

void Application::redrawFrame()
{
	CMRayApplication::drawFrame();
}

void Application::exitApplication()
{
	terminate();
}

core::array<video::VMode>& Application::getVideoModesList()
{
	return window->getVideoModesList();
}

scene::SceneManager* Application::getSceneManager()
{
	return CMRayApplication::getSceneManager();
}

void Application::setSceneManager(scene::SceneManager*smngr)
{
	this->smngr=smngr;
}

void Application::SetRequestedLevel(const core::string& path)
{
	m_requestLevel=path;
}
void Application::LoadRequestedLevel(IResLoadListener*listener)
{
	m_level->LoadLevel(m_requestLevel,listener);
	game::GameLevelManager::getInstance().SetActiveLevel(m_level);
}

}