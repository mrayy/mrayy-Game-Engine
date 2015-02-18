
/********************************************************************
	created:	2009/03/27
	created:	27:3:2009   23:58
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\EagleApplication.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	EagleApplication
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EagleApplication___
#define ___EagleApplication___

#include <mrayWinApp.h>
#include <GUIConsole.h>
#include <GUIBenchMark.h>
#include <ICommand.h>
#include <GUIAnimatedSprite.h>

#include "ScenarioManager.h"
#include "IEaglesManager.h"
#include "CSoundthemeManager.h"
#include "ObjectFactory.h"
#include "GameLevelManager.h"
#include "EnvironmentManager.h"
#include "TextureManager.h"
#include "GameOptionManager.h"

namespace mray{
namespace eagles{


class EagleApplication:public CMRayApplication,public IEaglesManager
{
private:
protected:
	GCPtr<gameMod::ScenarioManager> m_scenarioManager;
	GCPtr<gameMod::ObjectFactory> m_objectFactory;
	GCPtr<gameMod::CSoundthemeManager> m_soundThemeMngr;
	GCPtr<gameMod::GameLevelManager> m_gameLevelManager;
	GCPtr<gameMod::EnvironmentManager> m_environmentManager;
	GCPtr<gameMod::TextureManager> m_textureManager;
	GCPtr<gameMod::GameOptionManager> m_optionManager;
	
	GCPtr<GUI::GUIConsole> m_console;
	GCPtr<GUI::GUIBenchMark> m_benchmark;

	GCPtr<GUI::GUIAnimatedSprite> m_cursor;

	core::string m_defaultSoundManagerType;
	core::string m_assetsPath;

	bool m_showMemoryState;
	bool m_showFPS;

// 	GCPtr<ICommand> m_fpsCmd;
// 	GCPtr<ICommand> m_memstateCmd;
// 	GCPtr<ICommand> m_benchmarkCmd;

	bool m_screenShot;

public:
	EagleApplication();
	virtual~EagleApplication();

	bool startApplication();

	virtual void init(core::array<SOptionElement> *extraOptions);

	virtual void loopBody(float dt);

	virtual void onEvent(GCPtr<Event> event);

	virtual void onDone();

	virtual void redraw();

	virtual void exitApplication();

	virtual void drawFrame();

	core::array<video::VMode> &getVideoModesList();

	virtual void setActiveGUIManager(GCPtr<GUI::GUIManager> mngr);
	virtual GCPtr<GUI::GUIManager> getActiveGUIManager();

	virtual void setActiveSceneManager(GCPtr<scene::ISceneManager> mngr);
	virtual GCPtr<scene::ISceneManager> getActiveSceneManager();

	virtual void setActiveSoundManager(GCPtr<sound::ISoundManager> mngr);
	virtual GCPtr<sound::ISoundManager> getActiveSoundManager();

	virtual GCPtr<video::IVideoDevice> getVideoDevice();

	virtual mrayWin* getAppWindow(){return window;}

	//////////////////////////////////////////////////////////////////////////
	virtual const core::string &getSoundManagerType();

	//////////////////////////////////////////////////////////////////////////

	virtual void toggleFPS();
	virtual void toggleBenchmark();
	virtual void toggleMemstate();
};


}
}


#endif //___EagleApplication___
