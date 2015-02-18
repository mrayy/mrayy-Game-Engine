

#include "stdafx.h"
#include "CustomNodeScene.h"
#include "CustomNode.h"
#include "Application.h"
#include "GameEntityManager.h"
#include "GameEntity.h"

namespace mray{

using namespace math;
using namespace core;

CustomNodeScene::CustomNodeScene(Application*app)
{
	m_app=app;
}
CustomNodeScene::~CustomNodeScene(){
}

const mchar* CustomNodeScene::getName(){
	return mT("Custom Node");
}
const mchar* CustomNodeScene::getDescription(){
	return mT("Creating Custom node");
}

void CustomNodeScene::createScene(){
	setupNodes();
}
void CustomNodeScene::destroyScene(){
	scene::SceneManager* smngr=m_app->getSceneManager();

	m_nodes.clear();
	smngr->removeChild(m_root);
	m_root=0;	
}

void CustomNodeScene::update(float dt){
}


void CustomNodeScene::setupNodes(){
	GCPtr<CustomNode> node;

	scene::SceneManager* smngr=m_app->getSceneManager();

	//create Root node
	m_root=new scene::ISceneNode(mT("Root"),0);
	smngr->addSceneNode(m_root);

	for(int i=0;i<50;++i){
		node=new CustomNode(mT(""),m_app->getDevice(),math::Randomizer::rand01()+0.1);
		m_root->addChild(node);
		smngr->addSceneNode(node);

		math::vector3d p;
		p.x=(math::Randomizer::rand01()*2-1)*100;
		p.y=(math::Randomizer::rand01()*2-1)*100;
		p.z=(math::Randomizer::rand01()*2-1)*100;

		node->position=p;
	}

}

}

