
#include <KeyboardEvent.h>
#include <FileResourceManager.h>
#include <GUIThemeManager.h>
#include <HTMLLogger.h>
#include <StreamLogger.h>
#include <IteratorPair.h>
#include <SBillboardChain.h>
#include <SLightNode.h>
#include <CVFWVideoTexture.h>
#include <MaterialResourceManager.h>
#include <ShaderResourceManager.h>
#include <CommandManager.h>
#include <ICursorControl.h>

#include <StreamReader.h>
#include <StreamWriter.h>
#include <CMemoryStream.h>

#include "EagleApplication.h"
#include "IntroSenario.h"
#include "MenuSenario.h"
#include "AboutSenario.h"
#include "LoadingScenario.h"
#include "GameSenario.h"
#include "IDManager.h"
#include "DefaultObjectFactory.h"

#include "ScenarioExitCode.h"

namespace mray{
	using namespace math;
	using namespace core;
namespace eagles{

	class CToggleFPSCommand:public ICommand
	{
		bool m_enabled;
		EagleApplication*m_app;
	public:
		CToggleFPSCommand(EagleApplication*app){
			m_enabled=true;
			m_app=app;
		}

		virtual const mchar* getCommandName(){
			return mT("FPS");
		}

		virtual const mchar* getCommandInfo(){
			return mT("Toggle Show FPS on/off");
		}

		virtual int getCommandMinArgCount(){return 0;}

		virtual int getCommandMaxArgCount(){return 0;}

		virtual bool onCommand(core::array<core::string>& args){
			m_app->toggleFPS();
			return true;
		}

		virtual const mchar* getLastMessage(){return mT("");}

		virtual bool isEnabled(){
			return m_enabled;
		}
		virtual void enable(bool e){
			m_enabled=e;
		}
	};

	class CToggleBenchmarkCommand:public ICommand
	{
		bool m_enabled;
		EagleApplication*m_app;
	public:
		CToggleBenchmarkCommand(EagleApplication*app){
			m_enabled=true;
			m_app=app;
		}

		virtual const mchar* getCommandName(){
			return mT("benchmark");
		}

		virtual const mchar* getCommandInfo(){
			return mT("Toggle Show benchmark on/off");
		}

		virtual int getCommandMinArgCount(){return 0;}

		virtual int getCommandMaxArgCount(){return 0;}

		virtual bool onCommand(core::array<core::string>& args){
			m_app->toggleBenchmark();
			return true;
		}

		virtual const mchar* getLastMessage(){return mT("");}

		virtual bool isEnabled(){
			return m_enabled;
		}
		virtual void enable(bool e){
			m_enabled=e;
		}
	};

	class CToggleMemstateCommand:public ICommand
	{
		bool m_enabled;
		EagleApplication*m_app;
	public:
		CToggleMemstateCommand(EagleApplication*app){
			m_enabled=true;
			m_app=app;
		}

		virtual const mchar* getCommandName(){
			return mT("memstate");
		}

		virtual const mchar* getCommandInfo(){
			return mT("Toggle Show memstate on/off");
		}

		virtual int getCommandMinArgCount(){return 0;}

		virtual int getCommandMaxArgCount(){return 0;}

		virtual bool onCommand(core::array<core::string>& args){
			m_app->toggleMemstate();
			return true;
		}

		virtual const mchar* getLastMessage(){return mT("");}

		virtual bool isEnabled(){
			return m_enabled;
		}
		virtual void enable(bool e){
			m_enabled=e;
		}
	};

	class CExitCommand:public ICommand
	{
		bool m_enabled;
		EagleApplication*m_app;
	public:
		CExitCommand(EagleApplication*app){
			m_enabled=true;
			m_app=app;
		}

		virtual const mchar* getCommandName(){
			return mT("exit");
		}

		virtual const mchar* getCommandInfo(){
			return mT("Exit The Application");
		}

		virtual int getCommandMinArgCount(){return 0;}

		virtual int getCommandMaxArgCount(){return 0;}

		virtual bool onCommand(core::array<core::string>& args){
			m_app->terminate();
			return true;
		}

		virtual const mchar* getLastMessage(){return mT("");}

		virtual bool isEnabled(){
			return m_enabled;
		}
		virtual void enable(bool e){
			m_enabled=e;
		}
	};
	//////////////////////////////////////////////////////////////////////////

EagleApplication::EagleApplication()
{
	m_defaultSoundManagerType=mT("fmod");
	m_assetsPath=mT("Assets/");

	m_screenShot=false;

	m_showMemoryState=false;
	m_showFPS=false;

	m_enableClearClr=false;
}
EagleApplication::~EagleApplication(){
	m_optionManager=0;
	if(IDManager::isExist())
		delete IDManager::getInstancePtr();
}

bool EagleApplication::startApplication(){

	core::string resFileName=mT("Resources.stg");
#ifdef _DEBUG
	resFileName=mT("Resources_d.stg");
#endif
	return startup(mT("Eagles"),vector2di(800,600),false,gFileSystem.getAppPath()+resFileName,0,true,false);
}

core::array<video::VMode> &EagleApplication::getVideoModesList(){
	return window->getVideoModesList();
}

void EagleApplication::init(core::array<SOptionElement> *extraOptions){
	traceFunction(EagleApplication);
	CMRayApplication::init(extraOptions);


	//Caps
	{
		GCPtr<StreamLogger> logger=new StreamLogger(0);
		logger->setStream(gFileSystem.openFile(gFileSystem.getAppPath()+mT("DeviceCaps.txt"),OS::BIN_WRITE));
		getDevice()->getCapabilities()->logInfo(logger);
		logger->close();
		if(!getDevice()->getCapabilities()->isFeatureSupported(video::EDF_RenderToTarget)){
			FATAL_ERROR(true,mT("Sorry but your VideoCard Dosn't Support RenderToTarget!"));
		}
		if(!getDevice()->getCapabilities()->isFeatureSupported(video::EDF_FragmentProgram)){
			FATAL_ERROR(true,mT("Sorry but your VideoCard Dosn't Support Shaders!"));
		}
	}

	//init Search Paths
	loadResourceFile(mT("SearchPath.stg"));

	gMaterialResourceManager.parseMaterialScript(gFileSystem.openFile(mT("GameMaterials.material")));

	{
		m_cursor=new GUI::GUIAnimatedSprite(0,gTextureResourceManager.loadTexture2D(mT("MouseCursor.png")),1,1,1,0,0,32);
		m_cursor->SetDevice(getDevice());
		gCursorControl.SetMouseMap(m_cursor);
		gCursorControl.CursorMap(1);
	}


	{
		m_optionManager=new gameMod::GameOptionManager(mT("config.xml"));
	}
	{
		m_soundThemeMngr=new gameMod::CSoundthemeManager();
		m_soundThemeMngr->loadThemesFromDirs(m_assetsPath+mT("sound/themes"));
	}
	{
		m_objectFactory = new gameMod::ObjectFactory();
		gameMod::DefaultObjectFactory::installObjects();
	}
	{
		m_environmentManager=new gameMod::EnvironmentManager();
	}
	{
		//gShaderResourceManager.setDefaultShader(gShaderResourceManager.loadShader("Phong.cg","Phong.cg",mT("cg")));
	}

	new IDManager();
	m_textureManager=new gameMod::TextureManager();

	// init Senario Manager
	m_scenarioManager=new gameMod::ScenarioManager();

	m_gameLevelManager=new gameMod::GameLevelManager();

	{
		GCPtr<gameMod::IntroSenario> im=new gameMod::IntroSenario();
		GCPtr<video::CVFWVideoTexture> v= new video::CVFWVideoTexture(256);
		v->loadVideo(mT("logo.avi"));
		v->play();
		im->createVideoItem(v);

		im->createImageItem(gTextureResourceManager.loadTexture2D(mT("BigLogo.png")),2,3);

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
		m_gameLevelManager->addLevel(mT("testLevel.xml"));
	}
	// set Default Font
	gFontResourceManager.setDefaultFont(gFontResourceManager.loadFont(mT("Tahoma_font.xml")));
	
	// set Theme
	GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("Default.xml"));
	GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
	GUI::GUISkinManager::getInstance().setActiveSkin(mT("Default"));

	CommandManager::getInstance().addCommand(new CToggleFPSCommand(this));
	CommandManager::getInstance().addCommand(new CToggleBenchmarkCommand(this));
	CommandManager::getInstance().addCommand(new CToggleMemstateCommand(this));
	CommandManager::getInstance().addCommand(new CExitCommand(this));


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

}

void EagleApplication::drawFrame(){
	CMRayApplication::drawFrame();
	/*
	if(m_screenShot){
		GCPtr<video::ITexture> tex= getDevice()->takeScreenShot();
		if(tex){
			gTextureResourceManager.writeResourceToDist(tex,gFileSystem.getAppPath()+mT("screenShot.jpg"));
		}
		m_screenShot=false;
	}*/
}


void EagleApplication::loopBody(float dt){
	CMRayApplication::loopBody(dt);

	m_scenarioManager->update(dt);

	GCPtr<GUI::IFont>font=gFontResourceManager.getDefaultFont();
	if(font){
		GUI::FontAttributes attrs;
		attrs.fontColor=video::SColor(255,255,255,255);
		attrs.hasShadow=2;
		attrs.shadowOffset=2;
		attrs.fontSize=14;
		attrs.fontColor=video::SColor(255,255,255,255);


		if(m_showFPS){
			font->print(math::rectf(10,30,10,10),&attrs,0,mT("FPS:%d"),(int)gFPS.getFPS());
			font->print(math::rectf(10,50,10,10),&attrs,0,mT("Primitives:%d "),(int)getVideoDevice()->getPrimitiveDrawnCount());
		}
		if(m_showMemoryState){
			OS::SMemoryState ms;
			if(gOSystem.getMemoryState(ms)){
				attrs.fontSize=12;
				attrs.fontColor.Set(255,255,255,255);
				attrs.hasShadow=true;
				font->print(math::rectf(10,110,10,10),&attrs,0,mT("Physical Memory : %d/%d"),ms.AvaliablePhysicalMemory,ms.TotalPhysicalMemory);
				font->print(math::rectf(10,120,10,10),&attrs,0,mT("Virtual Memory : %d/%d"),ms.AvaliableVirtualMemory,ms.TotalVirtualMemory);
				font->print(math::rectf(10,130,10,10),&attrs,0,mT("Page File: %d/%d"),ms.AvaliablePageFile,ms.TotalPageFile);
			}
		}
	}
}

void EagleApplication::onEvent(GCPtr<Event> event){
	CMRayApplication::onEvent(event);

	if(event->getType()==ET_Keyboard){
		GCPtr<KeyboardEvent>e=event;
		if(e->press){
			if(e->key==KEY_GRAVE){
				m_console->Visible=!m_console->Visible;
				if(m_console->Visible){
					getActiveGUIManager()->setFocus(m_console);
				}
			}
		}
	}
	if(event->isRecived())return;
	switch(event->getType()){
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent>e=event;
			if(!e->press)
				break;
			switch(e->key){
			case KEY_P:
				throw new std::exception("break");
				break;
			case KEY_F5:
				{
					m_screenShot=true;
				}
				break;
			}
			/*
			if(e->key==KEY_ESCAPE && e->press){
				getWindow()->shutdown();
				event->setRecived();	
			}*/
		}break;
	}
}

void EagleApplication::onDone(){
	traceFunction(EagleApplication);
	CMRayApplication::onDone();


	array<GCPtr<IResource>> arr;
	IteratorPair<ResManagersList> p(ResourceGroup::getInstance().begin(),ResourceGroup::getInstance().end());

	for(;!p.done();p++){
		IResourceManager*m=(*p);
		int um=m->getUsedMemory(true);
		if(!um)
			continue;
		gLogManager.startSection(m->getResourceType());
		core::string msg;
		msg=mT("Used Memory: ");
		msg+=(int)um;
		gLogManager.log(msg,ELL_INFO);
		m->listResources(arr);
		for(int i=0;i<arr.size();++i){
			msg=arr[i]->getResourceName();
			msg+=mT(" , size=");
			msg+=core::StringConverter::toString( arr[i]->getResourceSize());
			gLogManager.log(msg,ELL_INFO);
		}
		arr.clear();
		gLogManager.endSection(1);
	}
}

void EagleApplication::redraw(){
	drawFrame();
}

void EagleApplication::exitApplication(){
	this->terminate();
}

void EagleApplication::setActiveGUIManager(GCPtr<GUI::GUIManager> mngr){
	if(this->gmngr){
		this->gmngr->removeChild(m_console);
		this->gmngr->removeChild(m_benchmark);
		this->gmngr->unregisterBenchmark();
	}
	this->gmngr=mngr;
	if(this->gmngr){
		this->gmngr->addCustomChild(m_console);
		this->gmngr->addCustomChild(m_benchmark);
		this->gmngr->registerBenchmark();
	}
	m_cursor->SetManager(mngr);
}
GCPtr<GUI::GUIManager> EagleApplication::getActiveGUIManager(){
	return gmngr;
}

void EagleApplication::setActiveSceneManager(GCPtr<scene::ISceneManager> mngr){
	if(this->smngr){
		this->smngr->unregisterBenchmark();
	}
	this->smngr=mngr;
	if(this->smngr){
		this->smngr->registerBenchmark();
	}
}
GCPtr<scene::ISceneManager> EagleApplication::getActiveSceneManager(){
	return smngr;
}


void EagleApplication::setActiveSoundManager(GCPtr<sound::ISoundManager> mngr){
	if(this->sndMngr){
		this->sndMngr->stopAllSounds();
	}
	this->sndMngr=mngr;
}
GCPtr<sound::ISoundManager> EagleApplication::getActiveSoundManager(){
	return sndMngr;
}


GCPtr<video::IVideoDevice> EagleApplication::getVideoDevice(){
	return getDevice();
}


const core::string &EagleApplication::getSoundManagerType(){
	return m_defaultSoundManagerType;
}

void EagleApplication::toggleFPS(){
	m_showFPS=!m_showFPS;
}
void EagleApplication::toggleBenchmark(){
	m_benchmark->Visible=true;
}
void EagleApplication::toggleMemstate(){
	m_showMemoryState=!m_showMemoryState;
}
}
}