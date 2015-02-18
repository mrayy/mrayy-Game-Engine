
#include <windows.h>
#include <windowsx.h>
#include <WinErrorDialog.h>
#include <mrayWin32.h>
#include <Point2d.h>
#include <CSettingsFile.h>
#include <MaterialResourceManager.h>
#include "CMozillaPlugin.h"
#include <GUIWindow.h>

#include <ResizeEvent.h>
#include <SystemEvent.h>

#include "DemoCamera.h"
#include <mrayOISInputManager.h>

namespace mray{

	using namespace math;

////////////////////////////////////////
//
// CMozillaPlugin class implementation
//
CMozillaPlugin::CMozillaPlugin(nsPluginCreateData * aCreateDataStruct) : nsPluginInstanceBase(),
	mInstance(aCreateDataStruct->instance),
	mInitialized(FALSE)
{
	mhWnd = NULL;
}

CMozillaPlugin::~CMozillaPlugin()
{
}


void CMozillaPlugin::onEvent(GCPtr<Event> event){
	switch(event->getType()){
		case EVENT_TYPE::ET_Mouse:
		case EVENT_TYPE::ET_Keyboard:
			gmngr->onEvent(event);
			break;
		case EVENT_TYPE::ET_ResizeEvent:
			{
				GCPtr<ResizeEvent> e=event;
				device->resizeWindow(e->width,e->height);
			}
			break;
		case EVENT_TYPE::ET_SystemEvent:
			{
				GCPtr<SystemEvent> e=event;
				if(e->m_eventType==ESET_Destroy)
					device->shutdown();

			}break;
	}

}
NPBool CMozillaPlugin::init(NPWindow* aWindow)
{


	mhWnd = (HWND)aWindow->window;
	if(mhWnd == NULL)
		return FALSE;

	mInitialized = TRUE;
	SetTimer(mhWnd,             // handle to main window 
		0,						 // timer identifier 
		1,						 // 10-second interval 
		(TIMERPROC) NULL);		 // no timer callback 


	initEngine(aWindow);

	return TRUE;
}

void CMozillaPlugin::initEngine(NPWindow* aWindow){

	m_eventMemManager= new EventMemoryManager();
	m_errorDialog=new WinErrorDialog();

	m_engine=new Engine(new OS::WinOSystem());

	core::string p=gFileSystem.getCurrPath();
	m_engine->loadPlugins(mT("Resources_d.stg"));

	m_app=new mozillaWin();


	InputCreationPack pack((ulong)mhWnd);
	pack.WinSize=vector2d(aWindow->width,aWindow->height);
	pack.exclusiveMouse=true;

	m_inputManager=new mrayOISInputManager(pack);

	m_app->startup(mhWnd,math::vector2di(aWindow->width,aWindow->height),32);

	device=Engine::getInstance().createDevice(mT("OpenGL"),(uint)mhWnd,math::vector2di(800,600),
		32,0,0);

	gmngr=new GUI::GUIManager(device);
	smngr=new scene::SceneManager(device);

	//////////////////////////////////////////////////////////////////////////
	loadResourceFile(mT("Resources_d.stg"));

	if(1){
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
		GUI::GUISkinManager::getInstance().setActiveSkin(mT("VistaCG"));

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}

	GCPtr<GUI::GUIWindow> w=gmngr->addWindow(mT("Window"),vector2d(100,50),vector2d(400,300));
	gmngr->addButton(mT("Hello World"),10,math::vector2d(100,40),0,w);

	GCPtr<DemoCamera> camera=new DemoCamera(getSceneManager(),10);
	smngr->addSceneNode(camera);
	smngr->addSphere(10,10,10);
}

void CMozillaPlugin::shut()
{
	// subclass it back
	m_app->shutdown();
	mhWnd = NULL;
	mInitialized = FALSE;
}

NPBool CMozillaPlugin::isInitialized()
{
	return mInitialized;
}

const char * CMozillaPlugin::getVersion()
{
	return NPN_UserAgent(mInstance);
}



void CMozillaPlugin::init(core::array<SOptionElement> *extraOptions){
}
void CMozillaPlugin::run(){
}
void CMozillaPlugin::loopBody(float dt){

	m_app->run();

	device->begin();
	if(m_enableSceneManager && getSceneManager())
		getSceneManager()->renderAll();

	if(m_enableSoundManager && getSoundManager())
		getSoundManager()->runSounds(dt);


	if(m_enableGUIManager && getGUIManager())
		getGUIManager()->drawAll();

	device->end();

	m_inputManager->capture();
	getSceneManager()->update(dt);
	Engine::getInstance().updateEngine();
}

void CMozillaPlugin::terminate(){
}

void CMozillaPlugin::onDone(){
}

GCPtr<video::IVideoDevice>	CMozillaPlugin::getDevice(){
	return device;
}
GCPtr<scene::SceneManager> 	CMozillaPlugin::getSceneManager(){
	return smngr;
}

GCPtr<GUI::GUIManager>	CMozillaPlugin::getGUIManager(){
	return gmngr;
}

GCPtr<sound::ISoundManager>	CMozillaPlugin::getSoundManager(){
	return sndMngr;
}

void CMozillaPlugin::setSoundManager(GCPtr<sound::ISoundManager>mngr){
	sndMngr=mngr;
}


void CMozillaPlugin::loadResourceFile(const mchar*resFile){

	traceFunction(Application);
	GCPtr<OS::IStream> stream=gFileSystem.createTextFileReader(resFile);
	GCPtr<script::CSettingsFile> resources=new script::CSettingsFile(stream);
	script::SConfigTag*tag= resources->getTag(mT("SearchPath"));
	if(tag){
		script::SConfigTag::AttributesIT it= tag->getAttr(mT("Path"));
		if(it!=tag->getAttrEnd()){
			script::SConfigTag::MultiValuesIT itv=it->second.begin();
			for(;itv!=it->second.end();itv++){
				gFileSystem.addPath(itv->c_str());
			}
		}
	}
	tag= resources->getTag(mT("Archive"));
	if(tag){
		script::SConfigTag::AttributesIT it= tag->getAttr(mT("Path"));
		if(it!=tag->getAttrEnd()){
			script::SConfigTag::MultiValuesIT itv=it->second.begin();
			for(;itv!=it->second.end();itv++){
				OS::ArchiveManager::getInstance().addArchive(itv->c_str());
			}
		}
	}
	tag= resources->getTag(mT("Materials"));
	if(tag){
		script::SConfigTag::AttributesIT it= tag->getAttr(mT("Path"));
		if(it!=tag->getAttrEnd()){
			script::SConfigTag::MultiValuesIT itv=it->second.begin();
			for(;itv!=it->second.end();itv++){
				gMaterialResourceManager.parseMaterialScript(gFileSystem.createBinaryFileReader(itv->c_str()));
			}
		}
	}
}


}
NPError NS_PluginInitialize()
{
	return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

/////////////////////////////////////////////////////////////
//
// construction and destruction of our plugin instance object
//
nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
	if(!aCreateDataStruct)
		return NULL;

	mray::CMozillaPlugin * plugin = new mray::CMozillaPlugin(aCreateDataStruct);
	return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
	if(aPlugin)
		delete (mray::CMozillaPlugin *)aPlugin;
}