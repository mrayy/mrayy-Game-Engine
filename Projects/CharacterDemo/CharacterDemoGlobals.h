

/********************************************************************
	created:	2010/06/30
	created:	30:6:2010   12:01
	filename: 	i:\Programing\GameEngine\mrayEngine\CharacterDemo\CharacterDemoGlobals.h
	file path:	i:\Programing\GameEngine\mrayEngine\CharacterDemo
	file base:	CharacterDemoGlobals
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CharacterDemoGlobals_h__
#define CharacterDemoGlobals_h__

#include "ISingleton.h"
#include "GameDemoOptions.h"
#include "SoundThemeManager.h"

namespace mray
{

class CharacterDemoGlobals
{
private:
protected:
	static CharacterDemoGlobals* s_instance;
public:

	CharacterDemoGlobals(){
		s_instance=this;
	}


	static CharacterDemoGlobals* getInstance(){return s_instance;}

	virtual core::string getAssestsPath()=0;
	
	virtual video::IVideoDevice* getVideoDevice()=0;

	virtual core::string getSoundManagerType()=0;

	virtual GUI::GUIManager* getActiveGUIManager()=0;
	virtual void setActiveGUIManager(GUI::GUIManager* mngr)=0;

	virtual sound::ISoundManager* getActiveSoundManager()=0; 
	virtual void setActiveSoundManager(sound::ISoundManager* mngr)=0; 

	virtual GameDemoOptions* getGameDemoOptions()=0;

	virtual sound::SoundThemeManager* GetSoundThemeManager()=0;

	virtual void redrawFrame()=0;

	virtual void exitApplication()=0;

	virtual core::array<video::VMode>& getVideoModesList()=0;

	virtual scene::SceneManager* getSceneManager()=0;
	virtual void setSceneManager(scene::SceneManager*smngr)=0;

	virtual void SetRequestedLevel(const core::string& path)=0;
	virtual void LoadRequestedLevel(IResLoadListener*listener)=0;
};

}
#endif // CharacterDemoGlobals_h__
