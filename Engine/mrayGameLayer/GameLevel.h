


/********************************************************************
	created:	2010/06/28
	created:	28:6:2010   15:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameLevel.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameLevel
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameLevel_h__
#define GameLevel_h__

#include "GameEntityManager.h"
#include "GameEnvironment.h"
#include "GameGlobals.h"
#include "AISystem.h"
#include "IGUIManager.h"
#include "ISoundManager.h"
#include "IPhysicManager.h"
#include "IGameLevelAction.h"

#include "ISceneNode.h"

namespace mray
{
	class IResLoadListener;
namespace scene
{
	class ISceneManager;
	class SCameraNode;
}

namespace game
{

	enum EGameLevelState
	{
		EGLS_Normal,
		EGLS_Pause,
		EGLS_Exit
	};

	struct GameLevelInfo
	{
	public:
		core::string name;
		core::string description;
		core::string thumbnail;

		core::string path;
	};
class MRAY_GAME_DLL GameLevel
{
private:
protected:

	GCPtr<GameEntityManager> m_entManager;
	GCPtr<GameEnvironment> m_envManager;
	GCPtr<GameGlobals> m_gmGlobals;
	GCPtr<AI::AISystem> m_aiSystem;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<GUI::IGUIManager> m_guiManager;

	GCPtr<sound::ISoundManager> m_soundManager;

	GCPtr<scene::ISceneNode> m_root;

	float m_phUpdate;

	GCPtr<game::IGameLevelAction> m_mainAction;

	scene::SCameraNode* m_camera;

	GameLevelInfo m_info;

	EGameLevelState m_state;

	bool m_isEditor;
	scene::ISceneManager* m_smngr;
public:
	GameLevel();
	virtual ~GameLevel();

	void SetState(EGameLevelState state);
	EGameLevelState GetState();

	void InitManagers();

	void SetCamera(scene::SCameraNode* cam);
	scene::SCameraNode* GetCamera();

	void SetGUIManager(GCPtr<GUI::IGUIManager> guiManager);
	GUI::IGUIManager* GetGUIManager();

	void SetSoundManager(GCPtr<sound::ISoundManager> sndManager);
	sound::ISoundManager* GetSoundManager();

	void SetPhysicsManager(GCPtr<physics::IPhysicManager> phManager);
	GCPtr<physics::IPhysicManager> GetPhysicsManager();

	GameEntityManager* GetEntityManager();
	GameEnvironment* GetEnvironment();
	GameGlobals* GetGameGlobals();
	AI::AISystem* GetAISystem();

	void SetSceneManager(scene::ISceneManager* smngr);
	scene::ISceneManager* GetSceneManager();

	void SetMainAction(GCPtr<game::IGameLevelAction> a);
	game::IGameLevelAction* GetMainAction();
 
	const GameLevelInfo& GetCompleteInfo();

	const core::string& GetLevelName();
	const core::string& GetLevelDescription();
	const core::string& GetLevelThumbNailName();

	void SetLevelName(const core::string& name);
	void SetLevelDescription(const core::string& desc);
	void SetLevelThumbNailName(const core::string& name);

	GCPtr<scene::ISceneNode> GetRootSceneNode();
	
	void ClearLevel();
	bool LoadLevel(const core::string& path,IResLoadListener*listener=0);
	bool ExportLevel(const core::string& path);
	
	void SetEditorMode(bool e);
	bool IsEditorMode();

	void GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,const math::vector3d &scale,float lod,float lodDist,GCPtr<video::ITexture> base,GCPtr<video::ITexture> baseNormal,
		GCPtr<video::ITexture> detail,GCPtr<video::ITexture> detailNormal);

	void Update(float dt);
};

}
}


#endif // GameLevel_h__
