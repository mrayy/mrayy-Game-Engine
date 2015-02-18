

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   17:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameEntityManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameEntityManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameEntityManager_h__
#define GameEntityManager_h__

#include "GameEntity.h"

#include "ISingleton.h"
#include "IDGenerator.h"
#include "ListenerContainer.h"
#include <list>

namespace mray
{
	namespace scene{
		class ISceneManager;
	}
	namespace sound
	{
		class ISoundManager;
	}
	namespace physics{
		class IPhysicManager;
		class VehicleManager;
	}
namespace game
{
	class GameLevel;

	typedef std::list<GameEntity*> EntityList;


	class GameEntityManager;

	class IGameEntityManagerListener
	{
	public:
		virtual void OnAddGameEntity(GameEntityManager*m, GameEntity*e){}
		virtual void OnRemoveGameEntity(GameEntityManager*m, GameEntity*e){}
		virtual void OnGamePreUpdate(GameEntityManager*m){}
		virtual void OnGameUpdate(GameEntityManager*m,float dt){}
		virtual void OnGameDebugRender(GameEntityManager*m, scene::IDebugDrawManager* renderer){}
		virtual void OnGameGUIRender(GameEntityManager*m, GUI::IGUIRenderer* renderer, const math::rectf& vp){}
	};

class MRAY_GAME_DLL GameEntityManager:public ListenerContainer<IGameEntityManagerListener*>
{
private:
protected:
	typedef std::map<uint, uint> SceneIDEntID;
	typedef std::map<core::string, uint> GameEntityNameMap;

	std::list<GameEntity*> m_deleteList;

	IDGenerator<GameEntity*> m_entitiesID;
	EntityList m_entities;
	GameEntityNameMap m_nameMap;
	SceneIDEntID m_sceneToEntity;

	sound::ISoundManager* m_sndManager;
	scene::ISceneManager* m_sceneMngr;
	GameLevel* m_level;
	physics::IPhysicManager* m_phManager;
	physics::VehicleManager* m_vehManager;

	void _PerformDelete();

	DECLARE_FIRE_METHOD(OnAddGameEntity, (GameEntityManager*m, GameEntity*e), (m, e))
	DECLARE_FIRE_METHOD(OnRemoveGameEntity, (GameEntityManager*m, GameEntity*e), (m, e))
	DECLARE_FIRE_METHOD(OnGamePreUpdate, (GameEntityManager*m), (m))
	DECLARE_FIRE_METHOD(OnGameUpdate, (GameEntityManager*m, float dt), (m, dt))
	DECLARE_FIRE_METHOD(OnGameDebugRender, (GameEntityManager*m, scene::IDebugDrawManager* renderer), (m, renderer))
	DECLARE_FIRE_METHOD(OnGameGUIRender, (GameEntityManager*m, GUI::IGUIRenderer* renderer, const math::rectf& vp), (m, renderer,vp))
public:
	GameEntityManager();
	virtual ~GameEntityManager();

	void Clear();
	
	void SetGameLevel(GameLevel*level);
	GameLevel* GetGameLevel();

	void SetSceneManager(scene::ISceneManager* smngr);
	scene::ISceneManager*GetSceneManager();

	void SetPhysicsManager(physics::IPhysicManager* smngr);
	physics::IPhysicManager*GetPhysicsManager();

	void SetVehicleManager(physics::VehicleManager* smngr);
	physics::VehicleManager* GetVehicleManager();

	void SetSoundManager(sound::ISoundManager* m);
	sound::ISoundManager* GetSoundManager();

	void RequestToRemove(GameEntity*ent);

	virtual void AddGameEntity(GameEntity* ent);
	virtual GameEntity* CreateGameEntity(const core::string&name);
	virtual void RemoveGameEntity(uint id);
	virtual GameEntity* GetGameEntityByID(uint id);
	virtual GameEntity* GetGameEntityByName(const core::string& name);

	void SetSceneNodeEntID(uint node,uint ent);
	void RemoveSceneNodeID(uint node);
	uint GetSceneNodeEntID(uint node);
	const EntityList& GetEntities();

	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void GUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);

	virtual bool loadFromFile(const core::string& path,std::vector<game::GameEntity*>* ents=0);
	virtual void loadXMLSettings(xml::XMLElement* elem, std::vector<game::GameEntity*>* ents = 0);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // GameEntityManager_h__
