
#include "stdafx.h"
#include "ModelScene.h"
#include "Application.h"



namespace mray{

using namespace math;
using namespace core;

ModelScene::ModelScene(Application*app)
{
	m_app=app;
}
ModelScene::~ModelScene(){
}

const mchar* ModelScene::getName(){
	return mT("Model");
}
const mchar* ModelScene::getDescription(){
	return mT("Scene contains Skeletal Model");
}

void ModelScene::createScene(){

	scene::SceneManager* smngr=m_app->getSceneManager();

	smngr->addAnimatedMesh(mT("skeletal.mdl"));
}
void ModelScene::destroyScene(){
	scene::SceneManager* smngr=m_app->getSceneManager();

	smngr->removeChild(m_root);
	m_root=0;	

	gMeshResourceManager.removeAll();
}

void ModelScene::update(float dt){
}
}