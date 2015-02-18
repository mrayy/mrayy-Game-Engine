#include "stdafx.h"

#include "GameEntityManager.h"
#include "IMovable.h"
#include "XMLTree.h"


namespace mray
{
namespace game
{

GameEntityManager::GameEntityManager()
{
	m_sceneMngr=0;
	m_phManager=0;
	m_vehManager=0;
	m_level=0;
	m_sndManager=0;
	Clear();
}
GameEntityManager::~GameEntityManager()
{
	Clear();
}

const EntityList& GameEntityManager::GetEntities()
{
	return m_entities;
}

void GameEntityManager::AddGameEntity(GameEntity* ent)
{
	uint id=m_entitiesID.AddObject(ent,ent->GetID());
	ent->SetID(id);
	ent->_SetCreator(this);
	m_entities.push_back(ent);
/*

	GameEntityNameMap::iterator it= m_nameMap.find(ent->GetName());
	if (it != m_nameMap.end())
	{

	}*/
	FIRE_LISTENR_METHOD(OnAddGameEntity, (this, ent));
}

GameEntity* GameEntityManager::CreateGameEntity(const core::string&name)
{
	GameEntity* ent=new GameEntity(name,this);
	ent->SetID(0);
	AddGameEntity(ent);
	return ent;
	
}

void GameEntityManager::SetGameLevel(GameLevel*level)
{
	m_level=level;
}
GameLevel* GameEntityManager::GetGameLevel()
{
	return m_level;
}

void GameEntityManager::Clear()
{
	m_sceneToEntity.clear();

	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		(*it)->OnDestroy();
	}

	 it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		delete *it;
	}
	m_entities.clear();
	m_entitiesID.Clear();
	m_entitiesID.AddObject(0,0);
}
void GameEntityManager::SetSceneManager(scene::ISceneManager* smngr)
{
	m_sceneMngr=smngr;
}
scene::ISceneManager* GameEntityManager::GetSceneManager()
{
	return m_sceneMngr;
}

void GameEntityManager::SetPhysicsManager(physics::IPhysicManager* smngr)
{
	m_phManager=smngr;
}
physics::IPhysicManager* GameEntityManager::GetPhysicsManager()
{
	return m_phManager;
}

void GameEntityManager::SetVehicleManager(physics::VehicleManager* smngr)
{
	m_vehManager=smngr;
}
physics::VehicleManager* GameEntityManager::GetVehicleManager()
{
	return m_vehManager;
}
void GameEntityManager::SetSoundManager(sound::ISoundManager* m)
{
	m_sndManager=m;
}
sound::ISoundManager* GameEntityManager::GetSoundManager()
{
	return m_sndManager;
}

GameEntity* GameEntityManager::GetGameEntityByID(uint id)
{
	GameEntity* res=0;
	m_entitiesID.GetObjectByID(id,res);
	return res;
}
GameEntity* GameEntityManager::GetGameEntityByName(const core::string& name)
{
	GameEntity* res = 0;
	return res;
}

void GameEntityManager::RemoveGameEntity(uint id)
{
	m_entitiesID.RemoveObject(id);
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		if((*it)->GetID()==id)
		{
			m_nameMap.erase((*it)->GetName());
// 			if((*it)->GetShape())
// 				RemoveSceneNodeID((*it)->GetShape()->getID());
			FIRE_LISTENR_METHOD(OnRemoveGameEntity, (this, *it));
			delete *it;
			m_entities.erase(it);
			return;
		}
	}
	
}
void GameEntityManager::RequestToRemove(GameEntity*ent)
{
	m_deleteList.push_back(ent);
}

void GameEntityManager::_PerformDelete()
{
	std::list<GameEntity*>::iterator it= m_deleteList.begin();
	for (;it!=m_deleteList.end();++it)
	{
		RemoveGameEntity((*it)->GetID());
	}
	m_deleteList.clear();
}

void GameEntityManager::PreUpdate()
{
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		(*it)->PreUpdate();
	}
	FIRE_LISTENR_METHOD(OnGamePreUpdate, (this))
}
void GameEntityManager::Update(float dt)
{
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		(*it)->Update(dt);
	}

	for(it=m_entities.begin();it!=m_entities.end();++it)
	{
		(*it)->LateUpdate(dt);
	}
	FIRE_LISTENR_METHOD(OnGameUpdate, (this,dt))
	_PerformDelete();
}
void GameEntityManager::DebugRender(scene::IDebugDrawManager* renderer)
{
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		(*it)->DebugRender(renderer);
	}
	FIRE_LISTENR_METHOD(OnGameDebugRender, (this, renderer))
}
void GameEntityManager::GUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp)
{
	EntityList::iterator it = m_entities.begin();
	for (; it != m_entities.end(); ++it)
	{
		(*it)->OnGUIRender(renderer,vp);
	}
	FIRE_LISTENR_METHOD(OnGameGUIRender, (this, renderer, vp))

}

void GameEntityManager::SetSceneNodeEntID(uint node,uint ent)
{
	m_sceneToEntity[node]=ent;
}
void GameEntityManager::RemoveSceneNodeID(uint node)
{
	m_sceneToEntity.erase(node);
}
uint GameEntityManager::GetSceneNodeEntID(uint node)
{
	SceneIDEntID::iterator it=m_sceneToEntity.find(node);
	if(it==m_sceneToEntity.end())
		return 0;
	return it->second;
}
bool GameEntityManager::loadFromFile(const core::string& path, std::vector<game::GameEntity*>* ents)
{
	xml::XMLTree tree;
	if(!tree.load(path))
		return false;
	xml::XMLElement *e= tree.getSubElement("GameEntities");
	if (!e)
		return false;
	loadXMLSettings(e,ents);
	return true;
}

void GameEntityManager::loadXMLSettings(xml::XMLElement* elem, std::vector<game::GameEntity*>* ents )
{
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("GameEntity")))
		{
			GameEntity* gent=new GameEntity(mT(""),this);
			gent->loadXMLSettings(e);
			gent->InitComponent();
			AddGameEntity(gent);
			if (ents)
				ents->push_back(gent);
		}
	}
}
xml::XMLElement*  GameEntityManager::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* root=new xml::XMLElement(mT("GameEntityManager"));
	elem->addSubElement(root);
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it)
	{
		(*it)->exportXMLSettings(root);
	}

	return root;
}


}
}
