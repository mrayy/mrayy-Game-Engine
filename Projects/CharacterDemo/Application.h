

/********************************************************************
	created:	2010/06/30
	created:	30:6:2010   11:36
	filename: 	i:\Programing\GameEngine\mrayEngine\CharacterDemo\Application.h
	file path:	i:\Programing\GameEngine\mrayEngine\CharacterDemo
	file base:	Application
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef Application_h__
#define Application_h__

#include "CharacterDemoGlobals.h"

#include "GUIConsole.h"
#include "ScenarioManager.h"
#include "SoundThemeManager.h"
#include "GameDemoOptions.h"
#include "GUISoundTrack.h"
#include "GameLevelManager.h"

namespace mray
{

class Application:public CMRayApplication,public CharacterDemoGlobals
{
private:
protected:
	GCPtr<scene::ViewPort> m_viewPort;
	GCPtr<GUI::IFont> m_font;
	GCPtr<scene::SCameraNode> m_camera;

	GCPtr<GUI::GUIConsole> m_console;
	GCPtr<GUI::GUIBenchMark> m_benchmark;
	GCPtr<GUI::GUIAnimatedSprite> m_cursor;
	GCPtr<GameDemoOptions> m_optionManager;
	GCPtr<game::GameLevelManager> m_gameLevelManager;

	GCPtr<GUI::GUISoundTrack> m_soundTrackUI;

	GCPtr<gameMod::ScenarioManager> m_scenarioManager;
	GCPtr<sound::SoundThemeManager> m_soundThemeMngr;

	core::string m_defaultSoundManagerType;
	core::string m_assetsPath;

	bool m_showMemoryState;
	bool m_showFPS;

	bool m_screenShot;

	core::string m_requestLevel;


//////////////////////////////////////////////////////////////////////////
	// Level Objects
	GCPtr<game::GameLevel> m_level;
	GCPtr<GUI::GUIManager> m_levelGUIManager;
	GCPtr<scene::SceneManager> m_levelSceneManager;
	GCPtr<sound::ISoundManager> m_levelSoundManager;


public:
	Application();
	virtual ~Application();

	virtual void onEvent(GCPtr<Event> event);

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void update(float dt);
	virtual void draw();
	virtual void onDone();


	virtual core::string getAssestsPath();
	virtual video::IVideoDevice* getVideoDevice();

	virtual core::string getSoundManagerType();

	virtual GUI::GUIManager* getActiveGUIManager();
	virtual void setActiveGUIManager(GUI::GUIManager* mngr);

	virtual sound::ISoundManager* getActiveSoundManager(); 
	virtual void setActiveSoundManager(sound::ISoundManager* mngr); 

	virtual GameDemoOptions* getGameDemoOptions();

	virtual sound::SoundThemeManager* GetSoundThemeManager();

	virtual void redrawFrame();

	virtual void exitApplication();

	virtual core::array<video::VMode>& getVideoModesList();

	virtual scene::SceneManager* getSceneManager();
	virtual void setSceneManager(scene::SceneManager*smngr);

	virtual void SetRequestedLevel(const core::string& path);
	virtual void LoadRequestedLevel(IResLoadListener*listener);
};

}
#endif // Application_h__
