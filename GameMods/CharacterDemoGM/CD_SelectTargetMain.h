

/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   12:55
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CD_SelectTargetMain.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CD_SelectTargetMain
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CD_SelectTargetMain_h__
#define CD_SelectTargetMain_h__

#include "CompileConfig.h"
#include "IGameLevelAction.h"
#include "IGameActionCreator.h"
#include "GUIElement.h"

#include "IRenderDevice.h"
#include "AIVisualizeRenderNode.h"
#include "LocationArrow.h"
#include "RTPickBuffer.h"
#include "GUIList.h"
#include "GUISlider.h"
#include "GUIStaticText.h"
#include "GUIRadioButton.h"
#include "GUICheckBox.h"

#include "CharacterManager.h"

namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API CD_SelectTargetMain:public game::IGameLevelAction,public IEventReciver
{
private:
protected:

	GCPtr<GUI::GUIElement> m_guiRoot;
	GCPtr<GUI::GUIList> m_visualizeOutput;
	GCPtr<GUI::GUISlider> m_shotPowerSlider;

	GCPtr<GUI::GUIStaticText> m_characterEnergy;
	GCPtr<GUI::GUIStaticText> m_characterSpeed;
	GCPtr<GUI::GUICheckBox> m_dynamicObsChk;

// 	GCPtr<GUI::GUIRadioButton> m_BallObjectRdo;
// 	GCPtr<GUI::GUIRadioButton> m_CrateObjectRdo;

	game::GameLevel* m_level;
	scene::SCameraNode* m_camera;

	GCPtr<AI::IRenderDevice> m_aiRenderDevice;
	GCPtr<scene::AIVisualizeRenderNode> m_aiVisualizeNode;
	GCPtr<LocationArrow> m_locationArr;
	//video::RTPickBuffer *m_pickBuffer;

	game::GameEntity* m_selectedEntity;

	bool m_cameraAttached;

	float m_phUpdate;

	PhysicsBasedCharacter::CharacterManager* m_characterManager;

	void InitGUI();

	void OnLMouseDown(int x,int y);
	void OnRMouseDown(int x,int y);

	void SetSelectedEntity(game::GameEntity*e);


	void CreateShotingBall();
public:
	CD_SelectTargetMain();
	virtual ~CD_SelectTargetMain();

	virtual void AttachToEntity(game::GameEntity*ent);
	PhysicsBasedCharacter::CharacterManager* GetCharacterManager(){return m_characterManager;}

	virtual void Init(game::GameLevel*level);
	game::GameLevel* GetLevel();

	virtual void SetEditorMode(bool e);

	virtual void Update(float dt);

	virtual void onEvent(GCPtr<Event> event);

};

class CD_SelectTargetMainCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("CD_SelectTargetMain");
	}
	virtual core::string GetDescription()
	{
		return mT("Main entry point for selection demo");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new CD_SelectTargetMain();
	}
};

}
}


#endif // CD_SelectTargetMain_h__
