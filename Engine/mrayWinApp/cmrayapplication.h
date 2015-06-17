



#ifndef ___CMRayApplication___
#define ___CMRayApplication___

#include <IApplication.h>
#include <IVideoDevice.h>
#include <SceneManager.h>
#include <IGUIManager.h>
#include <ISoundManager.h>
#include <ISceneManager.h>
#include <Engine.h>
#include <EventMemoryManager.h>
#include <IEventReciver.h>
#include <InputManager.h>
#include <RenderWindow.h>

#include "CompileConfig.h"
#include "IErrorDialog.h"
#include "FPSCalc.h"

namespace mray{

	class Win32WindowUtils;

class MRAY_WinApp_DLL CMRayApplication:public IApplication,public IEventReciver,video::IRenderWindowListener
{
protected:
	GCPtr<Engine> m_engine;
	GCPtr<video::IVideoDevice>m_device;
	GCPtr<scene::ISceneManager>smngr;
	GCPtr<GUI::IGUIManager>gmngr;
	GCPtr<sound::ISoundManager>sndMngr;

	GCPtr<IErrorDialog> m_errorDialog;
	GCPtr<EventMemoryManager> m_eventMemManager;

	GCPtr<InputManager> m_inputManager;

	std::vector<video::RenderWindow*> m_renderWindowLst;

	core::FPSCalc m_drawFPS;

	bool m_enableSceneManager;
	bool m_enableGUIManager;
	bool m_enablePhysicsManager;
	bool m_enableSoundManager;

	bool m_enableClearClr;

	bool m_terminate;

	float m_drawTimeCounter;
	bool m_limitFps;
	float m_limitFpsCount;
public:
	CMRayApplication(const core::string&logFile=mT("log.txt"));
	virtual~CMRayApplication();

	bool IsDone(){ return m_terminate; }

	virtual void init(const OptionContainer &extraOptions);
	virtual void run();

	const core::FPSCalc& GetDrawFPS(){ return m_drawFPS; }

	void drawFrame();

	virtual void draw(scene::ViewPort* vp);
	virtual void update(float dt);

	virtual void onEvent(Event* event);

	virtual void onWindowEvent(const EventPtr& event);
	virtual void onWindowResizedEvent(float x,float y);
	virtual void onWindowShutdown();

	virtual void doFrame();
	void terminate();

	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);
	virtual void WindowClosed(video::RenderWindow* wnd);

	virtual void onDone();

	void enableClearClr(bool e);
	bool isEnableClearClr();

	InputManager* GetInputManager(){return m_inputManager.pointer();}
	//Win32WindowUtils* getWindow();
	video::RenderWindow* GetRenderWindow(int i=0);
	video::IVideoDevice* getDevice();
	scene::ISceneManager* getSceneManager();
	GUI::IGUIManager* getGUIManager();
	sound::ISoundManager*getSoundManager();

	void AddRenderWindow(video::RenderWindow* wnd);

	void setSoundManager(GCPtr<sound::ISoundManager>mngr);

	void loadResourceFile(const core::string &resFile);

	bool startup(const  mchar*title, math::vector2di size, bool fullscreen, std::vector<SOptionElement> &extraOptions, const core::string &pluginsConfigFile = mT(""), 
		const core::string &settingsPath=mT("Configure.stg"),ulong injectionWindow=0,bool showDialogBox=false,bool showSplash=false,bool useInternalResources=true);


};


}


#endif




