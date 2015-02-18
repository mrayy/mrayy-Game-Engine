
#include "GameEntity.h"
#include "GameEntityManager.h"



namespace mray{

GameEntityManager::GameEntityManager(){
}
GameEntityManager::~GameEntityManager(){
	clear();
}

void GameEntityManager::addEntity(GCPtr<GameEntity> e){
	m_entities.push_back(e);
}
void GameEntityManager::removeEntity(GCPtr<GameEntity> e){
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it){
		if(e==*it){
			m_entities.erase(it);
			return;
		}
	}
}

void GameEntityManager::clear(){
	m_entities.clear();
}

void GameEntityManager::update(float dt){
	EntityList::iterator it=m_entities.begin();
	for(;it!=m_entities.end();++it){
		(*it)->update(dt);
	}
}

}



