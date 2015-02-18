
/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   18:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMozillaApp\CMozillaPlugin.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMozillaApp
	file base:	CMozillaPlugin
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CMozillaPlugin___
#define ___CMozillaPlugin___

#include <IApplication.h>
#include <IVideoDevice.h>
#include <SceneManager.h>
#include <GUIManager.h>
#include <ISoundManager.h>
#include <SceneManager.h>
#include <GUIManager.h>
#include <Engine.h>
#include <CPUInfo.h>
#include <EventMemoryManager.h>
#include "pluginbase.h"
#include "mozillaWin.h"
#include <InputManager.h>

namespace mray{

class CMozillaPlugin:public nsPluginInstanceBase,public IApplication,public ISingleton<CMozillaPlugin>
{
protected:
	GCPtr<Engine> m_engine;
	GCPtr<video::IVideoDevice>device;
	GCPtr<scene::SceneManager>smngr;
	GCPtr<GUI::GUIManager>gmngr;
	GCPtr<sound::ISoundManager>sndMngr;

	GCPtr<IErrorDialog> m_errorDialog;
	GCPtr<EventMemoryManager> m_eventMemManager;
	GCPtr<mozillaWin> m_app;

	GCPtr<InputManager> m_inputManager;


	bool m_enableSceneManager;
	bool m_enableGUIManager;
	bool m_enablePhysicsManager;
	bool m_enableSoundManager;

	bool m_enableClearClr;

	bool m_terminate;
	GCPtr<CPUInfo> m_cpuInfo;


	virtual void initEngine(NPWindow* aWindow);
public:
	CMozillaPlugin(nsPluginCreateData * aCreateDataStruct);
	virtual~CMozillaPlugin();

	NPBool init(NPWindow* aWindow);
	void shut();
	NPBool isInitialized();

	// locals
	const char * getVersion();


	void onEvent(GCPtr<Event> event);

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void run();
	virtual void loopBody(float dt);

	virtual void terminate();

	virtual void onDone();

	GCPtr<video::IVideoDevice> getDevice();
	GCPtr<scene::SceneManager> getSceneManager();
	GCPtr<GUI::GUIManager> getGUIManager();
	GCPtr<sound::ISoundManager>getSoundManager();

	void setSoundManager(GCPtr<sound::ISoundManager>mngr);


	void loadResourceFile(const mchar*resFile);

private:
	NPP mInstance;
	NPBool mInitialized;
	HWND mhWnd;
};

}


#endif //___CMozillaPlugin___
