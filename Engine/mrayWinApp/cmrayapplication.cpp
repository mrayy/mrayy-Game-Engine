#include "stdafx.h"
#include "resource.h"

#include "CMRayApplication.h"
#include <Randomizer.h>
#include <CSettingsFile.h>
#include <ResizeEvent.h>
#include <SystemEvent.h>
#include <JobPool.h>

#include <IFileSystem.h>
#include <TraceManager.h>
#include <MaterialResourceManager.h>

#include <FPSCalc.h>
#include <ITimer.h>
#include <common.h>
#include <WinOSystem.h>
#include "WinConfigDialog.h"
#include <WinErrorDialog.h>
#include <StreamLogger.h>
#include "SplashScreen.h"
#include <Win32WindowUtils.h>
#include <MeshAnimatorManager.h>
#include <DeviceCapabilites.h>
#include <OptionContainer.h>
#include <RenderWindow.h>
#include <RenderWindowUtils.h>
#include <ILogManager.h>
#include "BenchmarkItem.h"
#include <mrayOIS.h>
#include <EventQueue.h>
#include <IDeviceFactory.h>
#include <ViewPort.h>

namespace mray{
CMRayApplication::CMRayApplication(const core::string&logFile)
{

	m_eventMemManager= new EventMemoryManager();
	m_errorDialog=new WinErrorDialog();

	m_engine=new Engine(new OS::WinOSystem());

	m_limitFpsCount=60;
	m_limitFps=false;
	m_drawTimeCounter=0;

//	window=new Win32WindowUtils();


	// set log file
	if(logFile!=mT(""))
	{
		GCPtr<StreamLogger> logger=new StreamLogger(true);
		logger->setStream(gFileSystem.createTextFileWriter( gFileSystem.getAppPath()+logFile));
		gLogManager.addLogDevice(logger);
	}

	if(EventQueue::isExist()){
		EventQueue::getInstance().addEventHandler(this,std::set<uint>());/*
			Event::getEventMaskingValue(ET_Mouse) | 
			Event::getEventMaskingValue(ET_Keyboard) | 
			Event::getEventMaskingValue(ET_SystemEvent) | 
			Event::getEventMaskingValue(ET_Joystic) ); */
	}
}


bool CMRayApplication::startup(const  mchar*title,math::vector2di size,bool fullscreen,std::vector<SOptionElement> &extraOptions,
	const core::string &pluginsConfigFile, const core::string &settingsPath, ulong  injectionWindow, bool showDialogBox, bool showSplash, bool useInternalResources)
{

	traceFunction(eEngine);

	m_engine->loadPlugins(pluginsConfigFile);

	m_enableClearClr=true;

	core::string deviceType=mT("OpenGL");
	video::IDeviceFactoryCreater* creator= video::DeviceFactory::getInstance().getCreater(deviceType);

	//core::string settingsPath=mT("Configure.stg");

	GCPtr<OS::IStream> stream=gFileSystem.createTextFileReader(settingsPath.c_str());
	script::CSettingsFile settings;
	bool settingsOK=settings.loadSettings(stream);
	GCPtr<SplashScreen> m_splashScreen;
	m_splashScreen=new SplashScreen(useInternalResources);

	if(stream)
		stream->close();

	int bits=32;
	OptionContainer options;
	OptionContainer devParams;
	if(showDialogBox){
		GCPtr<WinConfigDialog> dialogBox=new WinConfigDialog(useInternalResources);
		creator->getConfigParams(options);
		for(int i=0;i<extraOptions.size();++i)
			options.AddOption(extraOptions[i]);

		SOptionElement *opt;
		script::SConfigTag* settingTag= settings.getTag(mT("Settings"));
		if(settingTag)
		{
			script::SConfigTag::AttributesIT it=settingTag->m_attrSet.begin();
			for(;it!=settingTag->m_attrSet.end();++it)
			{
				const core::string& attrName=it->first;
				opt=options.GetOptionByName(attrName);
				if(opt)
				{
					opt->setValue(it->second);
				}
			}
		}
		settings.clear();
		if(!dialogBox->run(options)){
		//	window->shutdown();
			return false;
		}else
		{
			script::SConfigTag*tag=settings.addTag(mT("Settings"));
			OptionContainer::OptionsMap& opList= options.GetOptions();
			OptionContainer::OptionsMap::iterator it=opList.begin();
			for(;it!=opList.end();++it){
				SOptionElement&elem=it->second;
				tag->addAttr(elem.name,elem.getValue());
				
				if(elem.name==mT("Resolution")){
					size=core::StringConverter::toVector2d(elem.getValue());
					//tag->addAttr(mT("Resolution"),elem.getValue());
				}else 
				if(elem.name==mT("Bits")){
					bits=core::StringConverter::toInt(elem.getValue());
					//tag->addAttr(mT("Bits"),elem.getValue());
				}else 
				if(elem.name==mT("Fullscreen")){
					fullscreen=core::StringConverter::toBool(elem.getValue());
					//tag->addAttr(mT("Fullscreen"),elem.getValue());
				}else 
				{
					devParams[elem.name].value=elem.getValue();
					std::vector<SOptionElement>::iterator it=std::find(extraOptions.begin(),extraOptions.end(),elem);
					if(it!=extraOptions.end())
						(*it).value=elem.value;
				}
			}
			
			stream=gFileSystem.createTextFileWriter(settingsPath.c_str());
			settings.writeSettings(stream);
			stream->close();
		}
	}
	//window->startup(title,size,bits,0,injectionWindow);
	if(showSplash)
		m_splashScreen->Create(0,ID_SplashScreenID,ID_SplashMaskID);

	m_device=Engine::getInstance().createDevice(mT("OpenGL"));


	devParams[mT("title")].value=title;
	devParams[mT("left")].value=mT("-1");
	devParams[mT("top")].value=mT("-1");
	//devParams[mT("vsync")].value=core::StringConverter::toString(vsync);
	//devParams[mT("FSAA")].value=core::StringConverter::toString(multisample);

	video::RenderWindow* rwnd=(m_device->CreateRenderWindow(mT("MainWindow"),size,fullscreen,devParams,(ulong)injectionWindow));
	AddRenderWindow(rwnd);
		//(uint)window->getHWnd(),window->getSize(),
		//window->getBits(),vsync,multisample

	ulong hWnd; 
	rwnd->GetCustomParam(mT("WINDOW"),(void*)&hWnd);
	InputCreationPack pack(rwnd);
	pack.WinSize=size;
	pack.exclusiveMouse=false;
	pack.createJoystic=true;

	m_inputManager=CreateOISInputManager(pack);

	if(m_device->getCapabilities()->isFeatureSupported(video::EDF_VertexProgram))
	{
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(true);
	}else
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(false);

	//gmngr=new GUI::GUIManager(device);
	smngr=new scene::SceneManager(m_device);


	m_enableSceneManager=true;
	m_enableGUIManager=true;
	m_enablePhysicsManager=true;
	m_enableSoundManager=true;

	m_terminate=false;

	init(options);

	if(showSplash)
		m_splashScreen->Close();
//	if(fullscreen)
//		window->toFullScreen();
//	window->showWindow(true);
	
	return true;
}
CMRayApplication::~CMRayApplication()
{
	m_inputManager=0;
	for(int i=0;i<m_renderWindowLst.size();++i)
		delete m_renderWindowLst[i];
	sndMngr=0;
	smngr=0;
	gmngr=0;

//	delete window;
	m_device=0;
	m_engine=0;
	m_eventMemManager=0;

}

void CMRayApplication::onEvent(Event* event){
	if(event->getType()==ET_Mouse || event->getType()==ET_Keyboard){
			if(gmngr)
				gmngr->OnEvent(event);
	}else if(event->getType()==ET_ResizeEvent)
	{
		ResizeEvent *e=(ResizeEvent*)event;
		//if(m_deviceW)
		//	m_deviceW->resizeWindow(e->width,e->height);
	}else if(event->getType()==ET_SystemEvent)
	{
		SystemEvent* e=(SystemEvent*)event;
		if(e->m_eventType==ESET_Destroy)
			m_device->shutdown();
				
	}

}
void CMRayApplication::onWindowEvent(const EventPtr& event){
	
	if(gmngr)
		gmngr->OnEvent(event.pointer());
}

void CMRayApplication::onWindowShutdown(){
	m_device->shutdown();
}

void CMRayApplication::onWindowResizedEvent(float x,float y){
	//m_device->resizeWindow(x,y);
}

void CMRayApplication::init(const OptionContainer &extraOptions){
	math::Randomizer::srand(gEngine.getTimer()->getMilliseconds());
	JobPool::getInstance().SetupDefaultPools();
	m_drawFPS.resetTime(gEngine.getTimer()->getSeconds());
}
void CMRayApplication::doFrame()
{
	{
		_AUTO_BENCHMARK(Engine::getInstance().getRootBenchmarking());
		drawFrame();

		m_inputManager->capture();
		float dt = gEngine.getFPS()->dt();
		update(dt);

		Engine::getInstance().updateEngine();
	}
}
void CMRayApplication::drawFrame()
{
	float t=gEngine.getTimer()->getSeconds();
	if(m_limitFps && m_limitFpsCount>0 && fabs(t-m_drawTimeCounter)<1000.0f/m_limitFpsCount)
	{
		return;
	}
	m_drawFPS.regFrame(t);
	m_drawTimeCounter=t;
	for(int i=0;i<m_renderWindowLst.size();++i)
	{
		m_renderWindowLst[i]->SetActiveWindow();
		m_device->begin(m_enableClearClr);
		//draw();
		m_renderWindowLst[i]->Render(true);
		m_device->end();
	}

}

void CMRayApplication::run(){

	gEngine.getFPS()->resetTime(gEngine.getTimer()->getSeconds());
	while(Win32WindowUtils::doMessagePump() && !m_terminate){
		doFrame();
	}
	m_terminate = true;
	for(int i=0;i<m_renderWindowLst.size();++i)
		video::RenderWindowUtils::RemoveListener(m_renderWindowLst[i],this);
	onDone();
}


void CMRayApplication::onDone(){
}

void CMRayApplication::terminate(){
	m_terminate=true;
}

void CMRayApplication::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
{
	for(int i=0;i<m_renderWindowLst.size();++i)
	{
		if(wnd==m_renderWindowLst[i])
		{
			draw(vp);
			break;
		}
	}
}
void CMRayApplication::WindowClosed(video::RenderWindow* wnd)
{
	if(wnd==m_renderWindowLst[0])
	{
		//delete m_inputManager;
		m_terminate=true;
	}else
	{
		for(std::vector<video::RenderWindow*>::iterator it=m_renderWindowLst.begin();it!=m_renderWindowLst.end();++it)
		{
			if(*it==wnd)
			{
				m_renderWindowLst.erase(it);
				break;
			}
		}
	}
}

void CMRayApplication::draw(scene::ViewPort* vp){

	if (m_enableGUIManager && getGUIManager())
	{
		math::rectf rc(0, vp->GetSize());
		getGUIManager()->DrawAll(&rc);
	}

}
void CMRayApplication::update(float dt){
	if(m_enableSceneManager && getSceneManager()){
		getSceneManager()->update(dt);
	}
	if(m_enableSoundManager && getSoundManager())
		getSoundManager()->runSounds(dt);

	if(m_enableGUIManager && getGUIManager())
		getGUIManager()->Update(dt);
}


void CMRayApplication::enableClearClr(bool e){
	m_enableClearClr=e;
}
bool CMRayApplication::isEnableClearClr(){
	return m_enableClearClr;
}
video::RenderWindow* CMRayApplication::GetRenderWindow(int i)
{
	if(i<m_renderWindowLst.size())
		return m_renderWindowLst[i];
	return 0;
}
void CMRayApplication::AddRenderWindow(video::RenderWindow* wnd)
{
	m_renderWindowLst.push_back(wnd);
	wnd->AddListener(this);
	video::RenderWindowUtils::AddListener(wnd,this);

}

video::IVideoDevice*	CMRayApplication::getDevice(){
	return m_device;
}
scene::ISceneManager* 	CMRayApplication::getSceneManager(){
	return smngr;
}

GUI::IGUIManager*	CMRayApplication::getGUIManager(){
	return gmngr;
}

sound::ISoundManager*	CMRayApplication::getSoundManager(){
	return sndMngr;
}

void CMRayApplication::setSoundManager(GCPtr<sound::ISoundManager>mngr){
	sndMngr=mngr;
}


void CMRayApplication::loadResourceFile(const core::string &resFile){
	m_engine->loadResourceFile(resFile);
}

}

