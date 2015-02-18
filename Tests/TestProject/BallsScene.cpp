

#include "stdafx.h"
#include "BallsScene.h"
#include "Application.h"
#include "GameEntityManager.h"
#include "GameEntity.h"

namespace mray{

	using namespace math;
	using namespace core;

BallsScene::BallsScene(Application*app)
{
	m_app=app;
}
BallsScene::~BallsScene(){
}

const mchar* BallsScene::getName(){
	return mT("Balls");
}
const mchar* BallsScene::getDescription(){
	return mT("Scene contains physical balls");
}

void BallsScene::createScene(){
	setupNodes();
}
void BallsScene::destroyScene(){
	scene::SceneManager* smngr=m_app->getSceneManager();
	physics::IPhysicManager* phMngr=physics::IPhysicManager::getInstancePtr();
	GameEntityManager* gameEntMngr=m_app->getEntityManager();


	gameEntMngr->clear();
	phMngr->clearNodes();
	smngr->removeChild(m_root);
	m_root=0;	
}

void BallsScene::update(float dt){
}


void BallsScene::setupNodes(){
	GCPtr<scene::MeshSceneNode> node;
	GCPtr<video::SMaterial> mat;
	GCPtr<physics::IPhysicalNode> phNode;
	GCPtr<scene::SMesh> mesh;

	scene::SceneManager* smngr=m_app->getSceneManager();
	physics::IPhysicManager* phMngr=physics::IPhysicManager::getInstancePtr();
	GameEntityManager* gameEntMngr=m_app->getEntityManager();

	//create Root node
	m_root=new scene::ISceneNode(mT("Root"),0);
	smngr->addSceneNode(m_root);

	mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));

	node=smngr->addPlane(1,1,0,quaternion::Identity,vector3d(1000,1,1000),mT(""),m_root);
//	node->ShadowCaster=true;
//	node->ShadowReciver=true;
	node->setMaterial(mat,0);

	physics::PlaneShapeDesc pdesc;
	pdesc.mass=0;
	pdesc.plane.set(math::vector3d::YAxis,0);

	physics::PhysicalNodeDesc nodeDesc;
	nodeDesc.shapes.push_back(&pdesc);
	phNode=phMngr->createNode(&nodeDesc);
	nodeDesc.shapes.clear();

	gameEntMngr->addEntity(new GameEntity(node,phNode));


	mesh=new scene::SMesh();
	GCPtr<scene::MeshBufferData>bd=mesh->addNewBuffer();
	scene::MeshGenerator::getInstance().generateSphere(10,8,8,bd->getMeshBuffer());
	mesh->getBufferData(0)->setMaterial(mat);

	physics::SphereShapeDesc sdesc;

	physics::PhysicalBodyDesc bodyDesc;
	nodeDesc.shapes.push_back(&sdesc);
	nodeDesc.bodyDesc=&bodyDesc;

	for(int i=0;i<4;i++){
		for(int j=0;j<6;j++){
			for(int k=0;k<7;k++){
				node=new scene::MeshSceneNode(mesh,mT(""),m_root,0,math::quaternion::Identity,1,0);
				smngr->addSceneNode(node);

				node->setMaterial(mat,0);
				node->ShadowCaster=true;
				node->ShadowReciver=true;
				math::vector3d pos=vector3d(35*(k-5)+Randomizer::rand01()*10,
					35*(i+1)+Randomizer::rand01()*10,
					35*(j-5)+Randomizer::rand01()*10);
				node->position=pos;

				//nodeDesc.density=1;
				bodyDesc.mass=Randomizer::rand01()*2;
				sdesc.radius=10;
				nodeDesc.globalPos.setTranslation(pos);
				phNode=phMngr->createNode(&nodeDesc);

				gameEntMngr->addEntity(new GameEntity(node,phNode));

			}

		}
	}


}

}