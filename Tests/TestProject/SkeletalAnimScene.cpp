

#include "stdafx.h"
#include "SkeletalAnimScene.h"
#include "Application.h"
#include "GameEntityManager.h"
#include "GameEntity.h"
#include "XMLSkeletonParser.h"
#include "SegmentDesc.h"
#include <IterativeIK.h>
#include <TJacobian.h>
#include <PsuedoJacobian.h>
#include <BonesJointQuery.h>


namespace mray{

using namespace math;
using namespace core;

class GPUSkinnedCallback:public video::IShaderConstantsCallback
{
	GCPtr<scene::GPUSkeletonAnimator> m_anim;
	int i;
protected:
	virtual void setUniform(video::IGPUShaderProgram*shader,video::GPUUniform* u,bool VertexU){
		if(VertexU && u->name.equals_ignore_case("JointPalette")){
			++i;
			matrix4x4* m=m_anim->getBonesMatricies();
			int cnt=m_anim->getBonesMatriciesCount();
			shader->setMatrixArray(u,m,cnt);
			/*
			if(i==40){
				GCPtr<OS::IStream> stream=gFileSystem.openFile(mT("Mat.dat"),OS::BIN_WRITE);
				for(int j=0;j<cnt;++j)
					stream->write(m[j].pointer(),16*sizeof(float));
			}*/
		}else{
			video::IShaderConstantsCallback::setUniform(shader,u,VertexU);
		}
	}
public:
	GPUSkinnedCallback(GCPtr<scene::GPUSkeletonAnimator> anim){
		i=0;
		m_anim=anim;
	}
};

SkeletalAnimScene::SkeletalAnimScene(Application*app)
{
	m_app=app;
}
SkeletalAnimScene::~SkeletalAnimScene(){
}


void SkeletalAnimScene::onEvent(GCPtr<Event> event)
{
	switch(event->getType()){
		case ET_Keyboard:
			{
				GCPtr<KeyboardEvent> e=event;
				if(e->press){
					if(e->key==KEY_UP)
					{
						m_target.x+=200*gFPS.dt();
					}else if(e->key==KEY_DOWN)
					{
						m_target.x-=200*gFPS.dt();
					}else if(e->key==KEY_LEFT)
					{
						m_target.z+=200*gFPS.dt();
					}else if(e->key==KEY_RIGHT)
					{
						m_target.z-=200*gFPS.dt();
					}else if(e->key==KEY_HOME)
					{
						m_target.y+=200*gFPS.dt();
					}else if(e->key==KEY_END)
					{
						m_target.y-=200*gFPS.dt();
					}else{
					}
				}
			}
	}
}

const mchar* SkeletalAnimScene::getName(){
	return mT("Skeleton");
}
const mchar* SkeletalAnimScene::getDescription(){
	return mT("Scene contains skeletal mesh");
}

GCPtr<scene::MeshSceneNode> SkeletalAnimScene::loadChar()
{
	scene::SceneManager* smngr=m_app->getSceneManager();

	GCPtr<video::SMaterial> mat;
	GCPtr<scene::MeshSceneNode> node;
	GCPtr<animation::IAnimationMixer> walkAnim= gAnimationResourceManager.getAnimation(mT("male_walk.anim"));
	GCPtr<animation::IAnimationMixer> idleAnim= gAnimationResourceManager.getAnimation(mT("Idle.anim"));

	{
		xml::XMLWriter writer(gFileSystem.openFile(mT("Anim.xml"),OS::TXT_WRITE));
		GCPtr<xml::XMLElement> elem=new xml::XMLElement(mT("Anim"));
		walkAnim->exportXMLSettings(elem);
		writer.addElement(elem);

	}

	node=smngr->addAnimatedMesh(mT("model.mdl"),0,math::quaternion::Identity,1,mT(""),m_root);
	/*node->ShadowCaster=true;
	node->ShadowReciver=true;
*/
	mat=gMaterialResourceManager.getMaterial(mT("HumanMaterial"));

	node->setMaterial(mat,0);



	scene::SMesh*mesh= node->getMesh();
	for (int i=0;i<mesh->getBuffersCount();++i)
	{
		scene::MeshBufferData*meshData= mesh->getBufferData(i);
		scene::IMeshBuffer*meshBuffer=meshData->getMeshBuffer();
		scene::ISkinCluster*cluster=meshBuffer->getSkinCluster();
		scene::Skeleton*skeleton=mesh->getSkeleton();
		GCPtr<scene::ISkeletonAnimator> anim=new scene::GPUSkeletonAnimator(meshBuffer,cluster,skeleton);
		/**//*
			for(int i=0;i<skeleton->getBonesCount();++i){
			m_bone=skeleton->getBone(i);
			if(m_bone)
			m_bone->addChild(smngr->addBox(10,10,10));
			}
			m_bone=skeleton->getBone(mT("Bip01_L_Forearm"));
			if(m_bone)
			m_bone->addChild(smngr->addBox(20,20,20));
			m_bone=skeleton->getBone(mT("joint3"));
			if(m_bone)
			m_bone->addChild(smngr->addBox(20,20,20));
			*/
		m_bone=skeleton->getBone(mT("Bip01"));

		if(walkAnim ){
			GCPtr<animation::AnimationTrackGroup> track=walkAnim->getTrackGroup(mT("walk"));
			if(track){

				m_animTrack=track->getTrack(mT("Bip01"));

				skeleton->addAnimationTrackGroup(track);
				track->playAll();
				track->setSpeed(math::Randomizer::rand01()*10+5);
				/*
				track->stopTrack(mT("Bip01_Spine1"));
				track->stopTrack(mT("Bip01_R_Clavicle"));
				track->stopTrack(mT("Bip01_R_UpperArm"));
				track->stopTrack(mT("Bip01_R_Forearm"));
				track->stopTrack(mT("Bip01_R_Hand"));*/

			}
		}
/*
		 if(idleAnim){
		 GCPtr<animation::AnimationTrackGroup> track=idleAnim->getTrackGroup(mT("idle"));
		 if(track){
		 skeleton->addAnimationTrackGroup(track);
		 track->playAll();
		 track->setSpeed(10);
			}
			}*/

		meshData->setShaderCallback(new GPUSkinnedCallback(anim));
		//mesh->setSkeleton(skeleton)	;
		meshData->setAnimator(anim);

		skeleton->update(0);
		//IK
		{
			GCPtr<animation::BonesJointQuery> jQuery=new animation::BonesJointQuery();
			jQuery->addBone(skeleton->getBone(mT("Bip01_R_UpperArm")));
			jQuery->addBone(skeleton->getBone(mT("Bip01_R_Forearm")));
			m_ef=skeleton->getBone(mT("Bip01_R_Hand"));
			jQuery->addBone(m_ef,true);

			m_target=m_ef->getAbsolutePosition();

			m_ik=new animation::IterativeIK();
			m_ik->setJointsChain(jQuery);
			m_ik->setBeta(0.02);
			m_ik->setAllowedError(0.5);
			m_ik->setMaxIterations(100);
			//m_ik->setTarget(math::vector3d(-42.624 ,137.684   ,24.88   ));
			m_ik->setJacobian(new math::TJacobian());
			//m_ik->update();
		}

	}

	return node;
}

void SkeletalAnimScene::createScene(){

	//gAnimationResourceManager.loadAnimationFolder(mT("Animations"));
	
	GCPtr<video::SMaterial> mat;
	GCPtr<scene::MeshSceneNode> node;


	scene::SceneManager* smngr=m_app->getSceneManager();

	m_text=m_app->getGUIManager()->addStaticText(mT(""),math::vector2d(40,100),math::vector2d(300,40));

	m_root=new scene::ISceneNode(mT("Root"),0);
	smngr->addSceneNode(m_root);

	GCPtr<scene::MeshSceneNode> root=smngr->addSphere(1,8,8,0,math::quaternion::Identity,1,mT(""),m_root);

	smngr->addSphere(1,8,8,math::vector3d(20,0,0),math::quaternion::Identity,1,mT(""),root)->getMaterial(0)->diffuseColor=video::DefaultColors::Red;
	smngr->addSphere(1,8,8,math::vector3d(0,20,0),math::quaternion::Identity,1,mT(""),root)->getMaterial(0)->diffuseColor=video::DefaultColors::Green;
	smngr->addSphere(1,8,8,math::vector3d(0,0,20),math::quaternion::Identity,1,mT(""),root)->getMaterial(0)->diffuseColor=video::DefaultColors::Blue;
	
	
	m_targetNode=smngr->addSphere(0.5,8,8,math::vector3d(0,0,0),math::quaternion::Identity,1,mT(""),m_root);
	m_targetNode->getMaterial(0)->diffuseColor=video::DefaultColors::Chocolate;
	m_Node=smngr->addSphere(4,8,8,math::vector3d(0,0,0),math::quaternion::Identity,1,mT(""),m_root);

	node=loadChar();
	node->position=math::vector3d(0,0,0);
	m_targetNode->position=m_target;
	/*
	node=loadChar();
	node->position=math::vector3d(20,0,0);
	node=loadChar();
	node->position=math::vector3d(-20,0,0);
	node=loadChar();
	node->position=math::vector3d(0,0,-20);
*/

	math::quaternion qq(45,vector3d(1,0.7,0.5).Normalize());
	math::matrix4x4 m;
	qq.toMatrix(m);
	math::quaternion qq2(m);

	//////////////////////////////////////////////////////////////////////////
	//test IK
	{
		GCPtr<scene::Skeleton> skeleton=new scene::Skeleton(mT(""));
		scene::BoneNode*rootB=skeleton->createBone(mT("root"),0);
		rootB->setPosition(vector3d(0 ,2,0));
		rootB->setOrintation(math::quaternion(45 ,math::vector3d(0,0,1)));
		scene::BoneNode*bone1=skeleton->createBone(mT("bone1"),rootB);
		bone1->setPosition(vector3d(2 ,0,0));
		/*bone1->setOrintation(math::quaternion(45 ,math::vector3d(0,0,1)));
		bone1->setOrintation(
			math::quaternion(-4.935 ,math::vector3d(1,0,0))*
			math::quaternion(-36.858  ,math::vector3d(0,1,0))*
			math::quaternion(8.323  ,math::vector3d(0,0,1))
			);*/
		scene::BoneNode*bone2=skeleton->createBone(mT("bone2"),bone1);
		bone2->setPosition(vector3d(2 ,0,0));
/*		bone2->setOrintation(math::quaternion(45 ,math::vector3d(0,0,1)));
		bone2->setOrintation(
			math::quaternion(0 ,math::vector3d(1,0,0))*
			math::quaternion(0  ,math::vector3d(0,1,0))*
			math::quaternion(-13.258   ,math::vector3d(0,0,1)));
*/
		scene::BoneNode*bone3=skeleton->createBone(mT("bone3"),bone2);
		bone3->setPosition(vector3d(2 ,0,0));
	/*	bone3->setOrintation(math::quaternion(45 ,math::vector3d(0,0,1)));
		bone3->setOrintation(
			math::quaternion(0 ,math::vector3d(1,0,0))*
			math::quaternion(0  ,math::vector3d(0,1,0))*
			math::quaternion(-13.258   ,math::vector3d(0,0,1)));*/
		scene::BoneNode*bone4=skeleton->createBone(mT("bone4"),bone3);
		bone4->setPosition(vector3d(2 ,0,0));

		GCPtr<animation::BonesJointQuery> jQuery=new animation::BonesJointQuery();
		jQuery->addBone(bone1);
		jQuery->addBone(bone2);
		jQuery->addBone(bone3,true);
		//jQuery->addBone(bone4,true);

		skeleton->refreshRoots();
		skeleton->update(0);

		GCPtr<animation::IterativeIK> ik=new animation::IterativeIK();
		ik->setJointsChain(jQuery);
		ik->setTarget(bone3->getAbsolutePosition()+0.1,math::quaternion(90 ,math::vector3d(0,0,1)));
		ik->setJacobian(new math::TJacobian());
		ik->update();
	}
	//////////////////////////////////////////////////////////////////////////

	mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));
	node=smngr->addPlane(1,1,0,quaternion::Identity,vector3d(1000,1,1000),mT(""),m_root);
	//	node->ShadowCaster=true;
	//	node->ShadowReciver=true;
	node->setMaterial(mat,0);
}
void SkeletalAnimScene::destroyScene(){
	scene::SceneManager* smngr=m_app->getSceneManager();
	physics::IPhysicManager* phMngr=physics::IPhysicManager::getInstancePtr();
	GameEntityManager* gameEntMngr=m_app->getEntityManager();


	gameEntMngr->clear();
	phMngr->clearNodes();
	smngr->removeChild(m_root);
	m_root=0;	

	gMeshResourceManager.removeAll();
}

void SkeletalAnimScene::update(float dt){

	if(m_ef){
		math::vector3d efp=m_ef->getAbsolutePosition();
		if((m_target-efp).LengthSQ()<0.0001){
			m_target=efp+(math::vector3d(math::Randomizer::rand01(),math::Randomizer::rand01(),math::Randomizer::rand01())*2-1)*10;
		}
		m_targetNode->position=m_target;
		m_ik->setTarget(m_target,math::quaternion(90,math::vector3d(1,0,0)));
	//	m_ik->update();

	}/**/
	if(m_bone)
	{
		m_Node->position=m_bone->getAbsolutePosition();
		if(m_animTrack){
			animation::transformationAnimationTrack* track=(animation::transformationAnimationTrack*)m_animTrack;
			animation::IKeyFrame*kf1,*kf2;
			float l=track->getKeyAtTime(0.4,&kf1,&kf2);
			//((animation::transformKeyFrame*)kf1)->getPos();
			m_text->Caption=core::StringConverter::toString(l);
		}
	//	m_bone->pitch(50*dt,scene::TS_Local);
	}
}



}