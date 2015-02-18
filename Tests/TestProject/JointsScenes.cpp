
#include "stdafx.h"
#include "JointsScene.h"
#include "Application.h"
#include "GameEntityManager.h"
#include "GameEntity.h"
#include "XMLSkeletonParser.h"
#include "SegmentDesc.h"

namespace mray{

using namespace math;
using namespace core;

JointsScene::JointsScene(Application*app)
{
	m_app=app;
}
JointsScene::~JointsScene(){
}

const mchar* JointsScene::getName(){
	return mT("Joints");
}
const mchar* JointsScene::getDescription(){
	return mT("Scene contains physical Joints");
}

void JointsScene::createScene(){

	// load simple BoneLink File
	XMLSkeletonParser parser;
	if(!parser.LoadFile(mT("joints.xml"),m_desc)){
		gLogManager.log(mT("Couldn't load joints file."),ELL_WARNING);
	}
	setupNodes();
}
void JointsScene::destroyScene(){
	scene::SceneManager* smngr=m_app->getSceneManager();
	physics::IPhysicManager* phMngr=physics::IPhysicManager::getInstancePtr();
	GameEntityManager* gameEntMngr=m_app->getEntityManager();


	gameEntMngr->clear();
	phMngr->clearNodes();
	smngr->removeChild(m_root);
	m_root=0;	

	gMeshResourceManager.removeAll();

	m_rootBone=0;
}

void JointsScene::update(float dt){
	if(m_rootBone)
		m_rootBone->update(dt);
}


void JointsScene::setupNodes(){
	GCPtr<video::SMaterial> mat;
	GCPtr<physics::IPhysicalNode> phNode;
	GCPtr<scene::SMesh> mesh;
	GCPtr<scene::MeshSceneNode> node;

	scene::SceneManager* smngr=m_app->getSceneManager();
	physics::IPhysicManager* phMngr=physics::IPhysicManager::getInstancePtr();
	GameEntityManager* gameEntMngr=m_app->getEntityManager();

	physics::PhysicMaterialDesc mdesc;
	mdesc.dynamicFriction=0.5;
	mdesc.staticFriction=0.3;
	mdesc.restitution=0.5;

	

	GCPtr<physics::IPhysicMaterial> material=phMngr->createMaterial(&mdesc);

	//create Root node
	m_root=new scene::ISceneNode(mT("Root"),0);
	smngr->addSceneNode(m_root);

	mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));

	quaternion q;
	q.fromAngle(90,vector3d(1,0,0));
	node=smngr->addPlane(1,1,0,q,vector3d(10,1,10),mT(""),m_root);
	node->setMaterial(mat,0);
	q.fromAngle(90,vector3d(0,0,1));
	node=smngr->addPlane(1,1,0,q,vector3d(10,1,10),mT(""),m_root);
	node=smngr->addSphere(2,20,20,vector3d(+10,0,0),quaternion::Identity,vector3d(1,1,1),mT(""),m_root);
	node->setMaterial(mat,0);
	smngr->addSphere(2,20,20,vector3d(0,0,+10),quaternion::Identity,vector3d(1,1,1),mT(""),m_root);
	node=smngr->addPlane(1,1,0,quaternion::Identity,vector3d(1000,1,1000),mT(""),m_root);
	node->ShadowCaster=true;
	node->ShadowReciver=true;
	node->setMaterial(mat,0);

	physics::PlaneShapeDesc pdesc;
	physics::PhysicalNodeDesc nodeDesc;
	pdesc.mass=0;
	pdesc.plane.set(math::vector3d::YAxis,0);
	pdesc.material=material;
	nodeDesc.shapes.push_back(&pdesc);
	phNode=phMngr->createNode(&nodeDesc);

	gameEntMngr->addEntity(new GameEntity(node,phNode));



	m_rootBone=new Segment(0);
	m_rootBone->loadFromDesc(&m_desc,smngr,m_root);

}

}