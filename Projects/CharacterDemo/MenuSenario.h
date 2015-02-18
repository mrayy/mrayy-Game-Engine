
/********************************************************************
	created:	2009/04/04
	created:	4:4:2009   0:51
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\MenuSenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	MenuSenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MenuSenario___
#define ___MenuSenario___

#include "IScenarioNode.h"
#include <GUIManager.h>
#include <ISoundManager.h>
#include <SVideoMode.h>

namespace mray{
namespace gameMod{


class MenuSenario:public IScenarioNode
{
private:


	void createMainMenu();
	void createOptionMenu();
	void createVideoMenu();
	void createLevelMenu();

	void createSceneManager();

protected:

	scene::SCameraNode* m_camera;
	float m_cameraAngle;

	core::array<video::VMode> m_videoModes;

	GCPtr<GUI::GUIManager> m_guiMngr;
	GCPtr<GUI::GUIManager> m_oldGuiMngr;

	GCPtr<sound::ISoundManager> m_sndMngr;
	GCPtr<sound::ISoundManager> m_oldSndMngr;

	GCPtr<scene::SceneManager> m_smngr;
	GCPtr<scene::SceneManager> m_oldSmngr;

	GCPtr<sound::ISound> m_bgSound;

	GCPtr<GUI::GUIElement> m_mainMenuPanel;
	GCPtr<GUI::GUIElement> m_optionPanel;
	GCPtr<GUI::GUIElement> m_videoPanel;
	GCPtr<GUI::GUIElement> m_levelPanel;

	GCPtr<GUI::GUIComboList> m_resolutionList;
	GCPtr<GUI::GUICheckBox> m_fullScreen;

	GCPtr<GUI::GUIImage> m_levelThumbnail;
	GCPtr<GUI::GUIStaticText> m_levelDescription;
	GCPtr<GUI::GUIList> m_levelsList;

	int m_state;

	GCPtr<GUI::GUIImage> m_fadeOutImg;
	int m_currentState;
	float m_bgAlpha;
protected:
	void playRandomSound();
public:
	MenuSenario();
	virtual~MenuSenario();

	virtual void update(float dt);

	virtual void onEvent(GCPtr<Event> event);

	virtual int isDone();

	virtual void onEnter();

	virtual void onExit();
};

}
}


#endif //___MenuSenario___
