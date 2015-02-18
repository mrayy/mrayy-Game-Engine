
#include "GameEntity.h"


namespace mray{

GameEntity::GameEntity(){
	m_isPhysicsControlled=true;
}
GameEntity::GameEntity(scene::IMovable* sceneNode,AI::IAIPhysics* physicNode){
	m_isPhysicsControlled=true;
	setNode(sceneNode);
	setPhysicalNode(physicNode);
}
GameEntity::~GameEntity(){
}

void GameEntity::setNode(scene::IMovable* n){
	m_sceneNode=n;
}
scene::IMovable* GameEntity::getNode(){
	return m_sceneNode;
}


void GameEntity::setPhysicalNode(AI::IAIPhysics* n){
	m_physicNode=n;
}
AI::IAIPhysics* GameEntity::getPhysicalNode(){
	return m_physicNode;
}

void GameEntity::setPhysicsControlled(bool s){
	m_isPhysicsControlled=s;
}
bool GameEntity::isPhysicsControlled(){
	return m_isPhysicsControlled;
}

void GameEntity::update(float dt){
	//if(m_physicNode && m_isPhysicsControlled && !m_physicNode-*isSleeping())
	{
		m_sceneNode->setPosition(m_physicNode->GetGlobalPosition());
		m_sceneNode->setOrintation(m_physicNode->GetGlobalOrintation());
	}
}

}
