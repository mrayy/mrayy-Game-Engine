
#include <gl/EXTGL.h>
#include <Windows.h>
#include <mrayEngine.h>
#include <mrayAI.h>
#include <mrayPhysX.h>
#include <XMLSceneLoader.h>
#include <mrayParticles.h>
#include <IObject.h>
#include <cmrayapplication.h>
#include <ResourceQueue.h>
#include <ScreenNodeSelector.h>
#include <CatagoryManager.h>
#include <Noise1D.h>
#include <FractalTypes.h>
#include <NoiseTexture.h>
#include <MorphAnimator.h>
#include <SkeletonAnimator.h>
#include <mrayLUA.h>
#include <XMLNodeReader.h>

#include <HeightMapRaw16.h>
#include <HeightMapRaw8.h>
#include <HeightMapTexture.h>
#include <ViewPort2D.h>
#include <SkyBoxManager.h>
#include <GUIConsole.h>
#include "LuaFunc.h"
#include "DemoCamera.h"
#include "PostProcessing.h"
#include "RocketTest.h"
#include "stackDump.h"
#include "EnumManager.h"

using namespace std;
using namespace mray;
using namespace math;
using namespace core;

void printH(){
}


enum EColorEnum{
	ERed,
	EGreen,
	EBlue
};


class Vegetation:public scene::ISceneNode{

public:
	GCPtr<scene::ISceneNode> m_node;
	array<GCPtr<video::ITexture>> m_billboards;

	Vegetation(video::IVideoDevice*dev):ISceneNode(mT(""),dev){
	}


	void createTextures(int count,scene::SCameraNode*cam){
		GCPtr<video::IRenderTarget> rt;
		m_billboards.resize(count);
		float step=360.0f/(float)count;
		m_node->position=0;
		cam->position().y=m_node->position().y;
		float angle=0;
		cam->useTarget=true;
		cam->target=m_node->position;
		m_node->updateAbsoluteTransformation();
		float rad=2*m_node->getTransformedBoundingBox().getHeight()/cos(cam->fovY);
		getDevice()->set3DMode();
		for(int i=0;i<count;++i){
			rt=getDevice()->createRenderTarget(core::string(mT("Vegetation"))+i,256,video::EPixelFormat::EPixel_R8G8B8,true);
			getDevice()->setRenderTarget(m_billboards[i],1,1,video::SColor(0,0,255,0));
			cam->position().x=m_node->position().x+rad*cosd(angle);
			cam->position().z=m_node->position().z+rad*sind(angle);

			cam->position().x=100;
			cam->position().z=100;
			cam->position().y=100;


			cam->preRender();
			cam->render();
			m_node->render();
			cam->update(0.001);
			getDevice()->setRenderTarget(0);
			m_billboards[i]=rt->getDepthTexture();
			angle+=step;
		}
		cam->useTarget=false;
	}
};



class GameEntity
{
public:

	GCPtr<scene::ISceneNode> m_sceneNode;
	GCPtr<AI::IAgent> m_agent;
	GCPtr<physics::IPhysicalNode> m_physicNode;

	void update(float dt){
		if(!m_physicNode->isSleeping()){
			m_sceneNode->setPosition(m_physicNode->getGlobalPosition());
			m_sceneNode->setOrintation(m_physicNode->getGlobalOrintation());
		}
	}
};

class TextureResourceOrder:public ResourceOrder
{
	core::string m_name;
public:
	TextureResourceOrder(const core::string&name){
		m_name=name;
	}
	virtual~TextureResourceOrder(){
	}
	GCPtr<IResource> visit(){
		return gTextureResourceManager.loadTexture2D(m_name.c_str());
	}
};

class TestResourceListener:public ResourceListner
{
public:
	virtual void loadComplete(IResource*r){
		if(r)
			gLogManager.log(mT("loading ")+r->getResourceName()+mT(" Completed!"),ELL_SUCCESS);
		else
			gLogManager.log(mT("loading TempResource Failed!"),ELL_WARNING);
	}
	virtual void unloadComplete(IResource*){

	}
};


class Splash2D
{
protected:
	GCPtr<video::IGPUShaderProgram> m_shader;
	GCPtr<video::IShaderConstantsCallback> m_callback;
	GCPtr<video::ITexture> m_texture0;
	vector2d m_pos;
	float m_size;
	video::IVideoDevice*m_device;
public:
	Splash2D(video::IVideoDevice*device,GCPtr<video::IGPUShaderProgram> shader,GCPtr<video::IShaderConstantsCallback> callback,
		GCPtr<video::ITexture> texture0,vector2d pos,float size){
		m_shader=shader;
		m_texture0=texture0;
		m_pos=pos;
		m_size=size;
		m_device=device;
		m_callback=callback;
	}
	virtual~Splash2D(){}

	void draw(float dt){
		m_device->useShader(m_shader);
		m_device->unuseShader();
	}
};

stackDump sDump;

class Application:public CMRayApplication
{

	GCPtr<scene::SCameraNode> camera;
	GCPtr<scene::SLightNode> light;

	GCPtr<physics::PhysXManager> m_phManager;

	array<GCPtr<GameEntity>> m_entitys;
	GCPtr<video::IRenderTarget> m_rt;
	GCPtr<ResourceQueue> m_resQueue;
	GCPtr<scene::ISceneNode> m_selectedNode;

	GCPtr<scene::SAnimatedMesh>m_model;
	GCPtr<scene::BoneNode> root;

	GCPtr<scene::ScreenNodeSelector> m_selector;

	GCPtr<scene::Skeleton> m_skeleton;
	GCPtr<CatagoryManager> m_skeletonCatagory;
	GCPtr<video::ITexture> m_noise;

	GCPtr<video::IGPUShaderProgram> m_splashShader;
	GCPtr<scene::SPatchTerrain> terrain;
	struct MorphVal 
	{
		GCPtr<scene::MorphTarget> target;
		GCPtr<GUI::GUISlider> m_val;
	};
	array<MorphVal> morphValues;
	GCPtr<script::LuaScriptManager> m_luaManager;

	GCPtr<GUI::ViewPort2D> m_2dViewport;

	array<GCPtr<RocketTest>> m_rockets;

	bool m_noiseDone;

	void generateNoise(){
		video::NoiseTexture noiseTex(4,2,0.4,0.8);
		noiseTex.createImage2D(m_noise);
		m_noise->unlock(true);
	}
	void noiseTexDone(){
		m_noiseDone=true;
	}
	class NoiseResourceOrder:public ResourceOrder
	{
		Application*m_app;
	public:
		NoiseResourceOrder(Application*app){
			m_app=app;
		}
		virtual~NoiseResourceOrder(){
		}
		GCPtr<IResource> visit(){
			m_app->generateNoise();
			m_app->noiseTexDone();
			return 0;
		}
	};

	class NoiseResourceListener:public ResourceListner
	{
		Application*m_app;
	public:
		NoiseResourceListener(Application*app){
			m_app=app;
		}
		virtual void loadComplete(IResource*r){
		}
		virtual void unloadComplete(IResource*){

		}
	};

	void createParticles()
	{

		gLogManager.startSection(mT("createParticles"));
		GCPtr<scene::PAColorInterpolater> pca=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ColorInterpolater"));
		pca->addKey(0  ,video::SColor(100,100,255,0));
		pca->addKey(0.3 ,video::SColor(255,128,0,255));
		pca->addKey(0.6 ,video::SColor(255,255,255,255));
		pca->addKey(0.8  ,video::SColor(128,0,0,255));
		pca->addKey(1  ,video::SColor(0,0,0,0));


		GCPtr<scene::PAScaleInterpolater> psa=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ScaleInterpolater"));
		psa->addKey(0  ,0);
		psa->addKey(0.5  ,4,7);
		psa->addKey(1 ,0);



		GCPtr<scene::SRingParticleSystem> ps=scene::SParticleSystemFactory::getInstance().createParticleSystem(mT("ringPS"));
		getSceneManager()->addSceneNode(ps);

		GCPtr<scene::PAVelocityInterpolater> pva=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("VelocityInterpolater"));
		pva->addKey(0  ,vector3d(0,2,0),vector3d(0,8,0));
		ps->addAffector(pva);


		GCPtr<scene::PARotationInterpolater> pra=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("RotationInterpolater"));
		pra->addKey(0  ,0);
		pra->addKey(1  ,500,2000);
		ps->addAffector(pra);

		ps->addAffector(pca);
		ps->addAffector(psa);
		ps->setParticleAcceleration(math::vector3d(0,10,0));
		ps->setParticleVelocity(math::vector3d(0,10,0));

		//		ps->particlesCount.addKey(0,0);
		//		ps->particlesCount.addKey(1,0);
		ps->particlesCount.addKey(2,-1);
		//		ps->particlesCount.addKey(3,0);
		//		ps->setCycleLength(3);
		//		ps->setGenerateMode(scene::EPGM_Cycle);

		ps->particlesLifeSpawn.addKey(0,7);

		ps->particlesRate.addKey(0,30);

		ps->position().x=0;
		ps->position().y=0;
		ps->position().z=0;
		ps->roll(0,scene::TS_Local);
		ps->InnerRadius=20;
		ps->OutterRadius=30;
		ps->setParticleShapeType(scene::EPS_Billboard);

		ps->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke1.tga")),0);
		ps->getMaterial()->setMaterialRenderer(video::MRT_ADDALPHA);
		ps->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
		ps->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

		//	getSceneManager()->Animators.addCircleAnimator(ps,0,vector3d(0,1,0),40,20);

		GCPtr<scene::PAPlaneBlocker> planeB=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("PlaneBlocker"));
		planeB->setBlockType(scene::BT_Bounce);
		planeB->setBounce(0.5);
		planeB->setPlane(Plane::XZPlane);
		ps->addAffector(planeB);


		GCPtr<scene::PASphereBlocker> sphereB=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("SphereBlocker"));
		sphereB->setSphere(vector3d(0,20,0),15);
		sphereB->setBlockType(scene::BT_Bounce);
		sphereB->setBounce(0.7);

		ps->addAffector(sphereB);
		gLogManager.endSection(true);

	}

	void createScene(){
		gLogManager.startSection(mT("createScene"));
		traceFunction(Application);
		bool supportFP=1;


		sDump.dumpStack();

		//add Camera
		camera=new DemoCamera(getSceneManager(),100);
		camera->position=vector3d(0,100,0);
		getSceneManager()->addSceneNode(camera);

		camera->ZFar=7000;

		new scene::SkyBoxManager();
		GCPtr<video::ITexture>skyTex=scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));

		GCPtr<scene::SSkyBoxNode> skyBox=getSceneManager()->addSkyBox(skyTex);


 		light=getSceneManager()->addLight(video::SColor(255),100,0,30);
 		getSceneManager()->Animators.addCircleAnimator(light,vector3d(0,150,0),vector3d(0,1,0),100,30);

		gShaderResourceManager.setDefaultShader(gShaderResourceManager.loadShader("phong_1.cg","phong_1.cg",mT("cg")));

		scene::XMLNodeReader nodeReader;
		GCPtr<scene::SLODAnimatedMesh> plane=nodeReader.loadXML(gFileResourceManager.getFile(mT("Node1.xml")));
		if(plane){
			scene::TangentCalculater calc;
			getSceneManager()->addSceneNode(plane);

			plane->setMaterial(gMaterialResourceManager.getMaterial(mT("F15Material")),0);
			plane->getMaterial(0)->addValue(new TextureValue(mT("envTex"),skyTex));

			scene::SMeshManipulator::getInstance().recalcNormalsSmooth(plane->getMesh());
			calc.calculateTangents(plane->getMesh()->getBuffer(0));

			GCPtr<scene::PAColorInterpolater> pca=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ColorInterpolater"));
			pca->addKey(0  ,video::SColor(0,0,255,128));
			pca->addKey(0.8  ,video::SColor(255,255,255,200));
			pca->addKey(1  ,video::SColor(255,255,255,0));


			GCPtr<scene::PAScaleInterpolater> psa=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ScaleInterpolater"));
			psa->addKey(0  ,1);
			psa->addKey(1 ,0.5,0.8);

			GCPtr<scene::PARotationInterpolater> pra=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("RotationInterpolater"));
			pra->addKey(0  ,0,180);
			pra->addKey(1  ,360,600);

			{
				GCPtr<scene::SParticleSystem> ps=getSceneManager()->addParticleSystem(30,20,2);
				ps->acceleration=vector3d(0,0,-10);
				ps->setParticleShapeType(scene::EPS_Billboard);
				ps->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke1.tga")),0);
				ps->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
				ps->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
				ps->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

				ps->addAffector(pra);
				ps->addAffector(pca);
				ps->addAffector(psa);

				plane->attachToMountPoint(mT("loc_leftEngine"),ps);


			}
			{
				GCPtr<scene::SParticleSystem> ps=getSceneManager()->addParticleSystem(30,20,2);
				ps->acceleration=vector3d(0,0,-10);
				ps->setParticleShapeType(scene::EPS_Billboard);
				ps->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke1.tga")),0);
				ps->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
				ps->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
				ps->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

				ps->addAffector(pra);
				ps->addAffector(pca);
				ps->addAffector(psa);

				plane->attachToMountPoint(mT("loc_rightEngine"),ps);
			}
		}
		/*
		GCPtr<loaders::HeightMapTexture> hm=new loaders::HeightMapTexture();
		GCPtr<video::ITexture> noise=gTextureResourceManager.createTexture2D(mT("noisyHeightMap"),vector2d(256),video::EPixel_LUMINANCE8);
		video::NoiseTexture noiseTex(4,4,0.4,0.8);
		noiseTex.createImage2D(noise);
		hm->setTexture(noise);*/
		
		GCPtr<loaders::HeightMapRaw16> hm=new loaders::HeightMapRaw16();
		hm->loadHeightMap(gFileResourceManager.getFile(mT("terrain12.raw")));
		terrain=new scene::SPatchTerrain(getSceneManager(),0,quaternion::Identity,1);
		getSceneManager()->addSceneNode(terrain);
		terrain->loadTerrain(hm,15,5,0.3);
		GCPtr<video::ITexture> tex=gTextureResourceManager.loadTexture2D(mT("terrain.jpg"));
		terrain->setTexture(tex,0);
		tex=gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg"));
		terrain->setTexture(tex,1);

		
		terrain->setTransofrmation(0,quaternion::Identity,vector3d(60000,1500,60000));
		terrain->getMaterial()->setRenderState(video::RS_Fog,video::ES_Use);
		getDevice()->enableFog(false);
		getDevice()->setFog(video::SColor(132,142,147,255),false,300,500,1,0);

		if(light ){
			//add a billboard
			GCPtr<scene::SBillboardNode> bNode;
			bNode=getSceneManager()->addBillboard(vector2d(10));
			GCPtr<video::ITexture>tex=gTextureResourceManager.loadTexture2D(mT("particlewhite.bmp"));
			bNode->setTexture(tex);
			bNode->getMaterial(0)->setMaterialRenderer(video::MRT_ADDALPHA);
			bNode->getMaterial(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);
			light->addChild(bNode);
			GCPtr<scene::SBillboardChain> chain=getSceneManager()->addBillboardChain(5,5,40,0);
			chain->setRecordInterval(0.2);
			light->addChild(chain);
			chain->getMaterial()->shader=gShaderResourceManager.loadShader("phong.cg","phong.cg",mT("cg"));
			chain->getMaterial()->addValue(new TextureValue(mT("baseTex"),gTextureResourceManager.loadTexture2D(mT("smokeTrail03.tga"))));;
			chain->getMaterial()->addValue(new TextureValue(mT("bumpTex"),gTextureResourceManager.loadTexture2D(mT("smoke_trail04_line_local.tga"))));;
			chain->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
		}


		return;
		for(int j=0;j<1;j++){
		for(int i=0;i<1;++i){
			m_model=getSceneManager()->addAnimatedMesh(mT("male.mdl"),vector3d((i-2)*10,camera->position().y,(j-2)*10));
			if(m_model){
				m_model->setMaterial(gMaterialResourceManager.getMaterial(mT("characterMaterial")),0);
				scene::TangentCalculater calc;
				calc.calculateTangents(m_model->getMesh()->getBuffer(0));
				/*
				model->getMaterial(0)->shader=gShaderResourceManager.loadShader("character.cg","character.cg",mT("cg"));
				model->getMaterial(0)->addValue(new TextureValue(mT("baseTex"),gTextureResourceManager.loadTexture2D(mT("casual01.tga"))));
				model->getMaterial(0)->addValue(new TextureValue(mT("specTex"),gTextureResourceManager.loadTexture2D(mT("casual01_s.tga"))));
				model->getMaterial(0)->addValue(new TextureValue(mT("bumpTex"),gTextureResourceManager.loadTexture2D(mT("casual01_n.tga"))));
				model->getMaterial(0)->setRenderState(video::RS_CullFace,video::ES_DontUse);*/

			}
		}
		}

		scene::XMLSceneLoader loader(getSceneManager());
		loader.loadXML(gFileSystem.createBinaryFileReader(mT("scene.xml")));


		setSoundManager(sound::SoundManagerFactory::getInstance().createSoundManager(mT("fmod")));
		GCPtr<sound::ISoundManager>smgr=getSoundManager();
		//smgr->playSound(mT("sound/afterburn.mp3"),0,1,5,1,sound::ESNDT_3D);


		m_skeletonCatagory=new CatagoryManager(mT("Human"));
#define GET_BIT(x) m_skeletonCatagory->getBit(mT(x))

		m_skeletonCatagory->addBit(mT("Root"));

		m_skeletonCatagory->addBit(mT("Head"));
		m_skeletonCatagory->addBit(mT("Torso"));
		m_skeletonCatagory->addBit(mT("LeftArm"));
		m_skeletonCatagory->addBit(mT("RightArm"));
		m_skeletonCatagory->setCombineBits(mT("UpperBody"),GET_BIT("Head")|GET_BIT("Torso")|GET_BIT("LeftArm")|GET_BIT("RightArm"));

		m_skeletonCatagory->addBit(mT("Waist"));
		m_skeletonCatagory->addBit(mT("LeftLeg"));
		m_skeletonCatagory->addBit(mT("RightLeg"));
		m_skeletonCatagory->setCombineBits(mT("LowerBody"),GET_BIT("Waist")|GET_BIT("LeftLeg")|GET_BIT("RightLeg"));

		m_skeleton=SkeletonResourceManager::getInstance().createSkeleton(mT("Human"));
		GCPtr<scene::BoneNode> bone;
		GCPtr<scene::BoneNode> bone2;
		int boneId=0;
		root=m_skeleton->createBone(mT("Root"),boneId++,0);
 		bone=m_skeleton->createBone(mT("Back0"),boneId++,root);
 		bone->setPosition(vector3d(0,2,0));
 		bone=m_skeleton->createBone(mT("Back1"),boneId++,bone);
 		bone->setPosition(vector3d(0,2,0));
 		bone=m_skeleton->createBone(mT("Back2"),boneId++,bone);
 		bone->setPosition(vector3d(0,2,0));

 		bone2=m_skeleton->createBone(mT("Head"),boneId++,bone);
 		bone2->setPosition(vector3d(0,1,0));
 
 		bone2=m_skeleton->createBone(mT("LeftArm"),boneId++,bone);
 		bone2->setPosition(vector3d(2,0,0));
 		bone2=m_skeleton->createBone(mT("LeftShoulder"),boneId++,bone2);
 		bone2->setPosition(vector3d(2,0,0));
 		bone2=m_skeleton->createBone(mT("LeftHand"),boneId++,bone2);
 		bone2->setPosition(vector3d(2,0,0));
 
 		bone2=m_skeleton->createBone(mT("RightArm"),boneId++,bone);
 		bone2->setPosition(vector3d(-2,0,0));
 		bone2=m_skeleton->createBone(mT("RightShoulder"),boneId++,bone2);
 		bone2->setPosition(vector3d(-2,0,0));
 		bone2=m_skeleton->createBone(mT("RightHand"),boneId++,bone2);
 		bone2->setPosition(vector3d(-2,0,0));
 
 		bone=m_skeleton->createBone(mT("LeftLeg"),boneId++,root);
 		bone->setPosition(vector3d(2,-2,0));
 		bone=m_skeleton->createBone(mT("LeftKnee"),boneId++,bone);
 		bone->setPosition(vector3d(0,-3,0.5));
 		bone=m_skeleton->createBone(mT("LeftFeet"),boneId++,bone);
 		bone->setPosition(vector3d(0,-4,-0.5));
 		bone=m_skeleton->createBone(mT("LeftToe"),boneId++,bone);
 		bone->setPosition(vector3d(0.5,0,2));

		bone=m_skeleton->createBone(mT("RightLeg"),boneId++,root);
		bone->setPosition(vector3d(-2,-2,0));
		bone=m_skeleton->createBone(mT("RightKnee"),boneId++,bone);
		bone->setPosition(vector3d(0,-3,0.5));
		bone=m_skeleton->createBone(mT("RightFeet"),boneId++,bone);
		bone->setPosition(vector3d(0,-4,-0.5));
		bone=m_skeleton->createBone(mT("RightToe"),boneId++,bone);
		bone->setPosition(vector3d(-0.5,0,2));

		scene::BonesListIT it =m_skeleton->beginBone();
		for(;it!=m_skeleton->endBone();++it){
			GCPtr<scene::ISceneNode> n=getSceneManager()->addSphere(0.5,6,6);
			(*it)->addChild(n);
			n->setNodeName((*it)->getNodeName());
		}
		{
			GCPtr<animation::TransAnimationGroup> trackGroup= new animation::TransAnimationGroup();
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("bone0Track"),m_skeleton->getBone(mT("Root"))->getID(),GET_BIT("Root"),100);
				oriTrack->createKeyFrame(0,math::quaternion::Identity);
				oriTrack->createKeyFrame(10,math::quaternion(45,vector3d(0,1,0)));
				oriTrack->createKeyFrame(20,math::quaternion::Identity);
				oriTrack->createKeyFrame(30,math::quaternion(-45,vector3d(0,1,0)));
				oriTrack->createKeyFrame(40,math::quaternion::Identity);
				oriTrack->setInterpolationType(animation::EIT_Spline);
				trackGroup->addTrack(oriTrack);
			}
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("bone1Track"),m_skeleton->getBone(mT("Back1"))->getID(),GET_BIT("Torso"),100);
				oriTrack->createKeyFrame(0,quaternion::Identity);
				oriTrack->createKeyFrame(5,quaternion(45,vector3d::YAxis)*quaternion(30,vector3d::ZAxis));
				oriTrack->createKeyFrame(10,quaternion::Identity);
				oriTrack->createKeyFrame(15,quaternion(-45,vector3d::YAxis)*quaternion(-30,vector3d::ZAxis));
				oriTrack->createKeyFrame(20,quaternion::Identity);
				trackGroup->addTrack(oriTrack);
			}
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("LeftLegTrack"),m_skeleton->getBone(mT("LeftLeg"))->getID(),GET_BIT("LeftLeg"),100);
				oriTrack->createKeyFrame(0,quaternion::Identity);
				oriTrack->createKeyFrame(5,quaternion(30,vector3d::XAxis));
				oriTrack->createKeyFrame(10,quaternion::Identity);
				oriTrack->createKeyFrame(15,quaternion(-30,vector3d::XAxis));
				oriTrack->createKeyFrame(20,quaternion::Identity);
				trackGroup->addTrack(oriTrack);
			}
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("RightLegTrack"),m_skeleton->getBone(mT("RightLeg"))->getID(),GET_BIT("RightLeg"),100);
				oriTrack->createKeyFrame(0,quaternion::Identity);
				oriTrack->createKeyFrame(5,quaternion(-30,vector3d::XAxis));
				oriTrack->createKeyFrame(10,quaternion::Identity);
				oriTrack->createKeyFrame(15,quaternion(30,vector3d::XAxis));
				oriTrack->createKeyFrame(20,quaternion::Identity);
				trackGroup->addTrack(oriTrack);
			}

			trackGroup->setTargetWeight(1,5);
			trackGroup->setWeight(0);
   			trackGroup->playTrack(mT("bone1Track"));
   			trackGroup->playTrack(mT("LeftLegTrack"));
   			trackGroup->playTrack(mT("RightLegTrack"));
			m_skeleton->addAnimationTrackGroup(trackGroup);
		}
		{
			GCPtr<animation::TransAnimationGroup> trackGroup= new animation::TransAnimationGroup();
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("LeftLegTrack"),m_skeleton->getBone(mT("LeftLeg"))->getID(),GET_BIT("LeftLeg"),100);
				oriTrack->createKeyFrame(0,quaternion::Identity);
				oriTrack->createKeyFrame(10,quaternion(20,vector3d::ZAxis));
				oriTrack->createKeyFrame(20,quaternion::Identity);
				oriTrack->createKeyFrame(30,quaternion(-20,vector3d::ZAxis));
				oriTrack->createKeyFrame(40,quaternion::Identity);
				trackGroup->addTrack(oriTrack);
			}
			{
				GCPtr<animation::orintationAnimationTrack> oriTrack;
				oriTrack=new animation::orintationAnimationTrack(mT("RightLegTrack"),m_skeleton->getBone(mT("RightLeg"))->getID(),GET_BIT("RightLeg"),100);
				oriTrack->createKeyFrame(0,quaternion::Identity);
				oriTrack->createKeyFrame(10,quaternion(-20,vector3d::ZAxis));
				oriTrack->createKeyFrame(20,quaternion::Identity);
				oriTrack->createKeyFrame(30,quaternion(20,vector3d::ZAxis));
				oriTrack->createKeyFrame(40,quaternion::Identity);
				trackGroup->addTrack(oriTrack);
			}

			trackGroup->setTargetWeight(0.5,2);
			trackGroup->setWeight(0);
   			trackGroup->playTrack(mT("LeftLegTrack"));
   			trackGroup->playTrack(mT("RightLegTrack"));
			m_skeleton->addAnimationTrackGroup(trackGroup);
		}
		m_skeleton->refreshRoots();

		GCPtr<scene::MeshSceneNode> node1=getSceneManager()->addSphere(4,8,8);
		GCPtr<scene::MorphAnimator> anim;
		anim=new scene::MorphAnimator(node1->getMesh()->getBuffer(0));
		node1->getMesh()->getBuffer(0)->setAnimator(anim);
		{
			GCPtr<scene::SMesh> mesh=new scene::SMesh();
			GCPtr<scene::IMeshBuffer> buff= mesh->addNewBuffer();
			scene::MeshGenerator::getInstance().generateSphere(2,8,8,mesh);
			scene::SMeshManipulator::getInstance().scaleMesh(buff,vector3d(1,2,1));
			anim->addTarget(buff,50);
		}
		{
			GCPtr<scene::SMesh> mesh=new scene::SMesh();
			GCPtr<scene::IMeshBuffer> buff= mesh->addNewBuffer();
			scene::MeshGenerator::getInstance().generateSphere(4,8,8,mesh);
			scene::SMeshManipulator::getInstance().rotateMesh(buff,quaternion(45,vector3d::XAxis));
			anim->addTarget(buff,50);
		}


		GCPtr<GUI::GUIWindow>wind= getGUIManager()->addWindow(mT("Test"),vector2d(200),vector2d(250,100));
		for (int i=0;i<anim->getTargetCount();++i)
		{
			MorphVal v;
			v.target=anim->getTarget(i);
			v.m_val=getGUIManager()->addSlider(0,100,vector2d(30,10 + 40*i),vector2d(200,30),0,wind);
			morphValues.push_back(v);
		}


		return;

		light->active=true;
		if(supportFP && 0)
			light->active=false;



		//create AI

		new AI::SteerBehavior();

		m_phManager=new physics::PhysXManager();

		GCPtr<video::ITexture> bump=gTextureResourceManager.loadTexture2D(mT("rockwall_height.jpg"));
		//video::ColorConverter::createNormalMap(bump,10);
		scene::TangentCalculater calc;
		// create plane
		{
			GCPtr<scene::ISceneNode>node=getSceneManager()->addPlane(1,1,0,math::quaternion(),100);
			node->setMaterial(gMaterialResourceManager.getMaterial(mT("Bump_object")));
			physics::PlaneShapeDesc desc(math::Plane(math::vector3d(0),vector3d(0,1,0)));
			m_phManager->createNode(&desc);
			calc.calculateTangents(node->getMesh()->getBuffer(0));
			node->setTexture(bump,1);
		}



		
		{
			GCPtr<GameEntity> ent=new GameEntity();
			float ext=5;
			ent->m_sceneNode=getSceneManager()->addSphere(ext,20,20);
			ent->m_sceneNode->setMaterial(gMaterialResourceManager.getMaterial(mT("Bump_object")));
			physics::SphereShapeDesc desc(ext);
			desc.density=3;
			ent->m_physicNode=m_phManager->createNode(&desc);
			ent->m_physicNode->setGlobalPosition(math::vector3d(0,ext,0));//i*(ext.z+4)*2));
			ent->m_physicNode->setAngularMomentum(math::vector3d(0,0,0));
			ent->m_agent=new AI::AIAgent(ent->m_physicNode,mT(""));
			ent->m_agent->setMaxSpeed(20);
			m_entitys.push_back(ent);

			calc.calculateTangents(ent->m_sceneNode->getMesh()->getBuffer(0));

			ent->m_sceneNode->setTexture(bump,1);
		}
		for(int i=0;i<50;++i)
		{
			GCPtr<GameEntity> ent=new GameEntity();
			math::vector3d ext(15,5,15);
			ent->m_sceneNode=getSceneManager()->addBox(ext.x,ext.y,ext.z);
			ent->m_sceneNode->setMaterial(gMaterialResourceManager.getMaterial(mT("Bump_object")));
			physics::BoxShapeDesc desc(ext);
			desc.density=0.4;
			ent->m_physicNode=m_phManager->createNode(&desc);
			ent->m_physicNode->setGlobalPosition(math::vector3d(50,20+ext.y*3*i,0));//i*(ext.z+4)*2));
			ent->m_physicNode->setAngularMomentum(math::vector3d(0,0,0));
			ent->m_agent=new AI::AIAgent(ent->m_physicNode,mT(""));
			ent->m_agent->setMaxSpeed(10);
			m_entitys.push_back(ent);

			calc.calculateTangents(ent->m_sceneNode->getMesh()->getBuffer(0));
			ent->m_sceneNode->setTexture(bump,1);
		}


	}

public:
	Application():CMRayApplication()
	{
		GCPtr<HTMLLogger> l=new HTMLLogger(true);
		l->setStream(gFileSystem.createTextFileWriter(gFileSystem.getAppPath()+mT("Log.html")));
		gLogManager.addLogDevice(l);
	}

	virtual ~Application(){
		m_resQueue=0;

		delete scene::SkyBoxManager::getInstancePtr();
	}
	virtual void onEvent(GCPtr<Event>event){
		CMRayApplication::onEvent(event);
		if(event->isRecived())return;
		if(event->getType()==ET_Keyboard){
			GCPtr<KeyboardEvent> e=event;
			if(e->press){
				switch (e->key)
				{
				case KEY_ESCAPE:
					getWindow()->shutdown();
					break;
				case KEY_Q:
					{
					if(terrain){
						if(terrain->getMaterial()->getRenderState(video::RS_Wireframe)==video::ES_DontUse){
							terrain->getMaterial()->setRenderState(video::RS_Wireframe,video::ES_Use);
						}else
							terrain->getMaterial()->setRenderState(video::RS_Wireframe,video::ES_DontUse);
					}
					}
					break;
				case KEY_SPACE:
					m_rockets.push_back(new RocketTest(getSceneManager(),camera->position,camera->getOrintation()));
					break;
				}
			}
		}else if(event->getType()==ET_Mouse){
			GCPtr<MouseEvent> e=event;
			if(e->Event==MET_LEFTDOWN){
				m_selector->setup(vector2d(e->x,e->y),getSceneManager()->getActiveCamera(),
					camera->getVisibleNodes(),0);
				scene::QueryResultNode node;
				m_selector->query(node);
				m_selectedNode= node.node;
				
			}
		}
	}

	GCPtr<video::ITexture> m_blood0;


	//	GCPtr<video::ITexture> rt;
	virtual void init(core::array<SOptionElement> *extraOptions){
		traceFunction(Application);
		gLogManager.startSection(mT("Init"));
		CMRayApplication::init(extraOptions);
		loadResourceFile(mT("dataPath.stg"));

		m_luaManager=new script::LuaScriptManager();
		tolua_scene_open(m_luaManager->getLuaState());

		GCPtr<script::LuaScript> script1= m_luaManager->createScript(mT("script"),gFileResourceManager.getFile(mT("mainscript.lua")));
		if(script1){
			script1->runScript();
			ParameterMap<float> params;
			script1->callFunction("testLog",params);
		}

		m_selector=new scene::ScreenNodeSelector();

		m_splashShader=gShaderResourceManager.loadShader("splash.cg","splash.cg",mT("cg"));

		GCPtr<TestResourceListener>l=new TestResourceListener();
/*
		m_resQueue->addRequest(new TextureResourceOrder(mT("TestResource.tga")),l);
		m_resQueue->addRequest(new TextureResourceOrder(mT("terrain.bmp")),l);
		m_resQueue->addRequest(new TextureResourceOrder(mT("Test2.tga")),l);
		m_resQueue->addRequest(new TextureResourceOrder(mT("Test3.jpg")),l);
*/
		EventQueue::getInstance().addEventHandler(this,Event::getEventMaskingValue(ET_Keyboard)|
			Event::getEventMaskingValue(ET_Mouse));

		gTextureResourceManager.setDefaultTexture2D(gTextureResourceManager.loadTexture2D(mT("Default2D.jpg")));

		getDevice()->setAmbientColor(video::SColor(20));
		loadResourceFile(mT("Resources.stg"));

		if(1){
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
			GUI::GUISkinManager::getInstance().setActiveSkin(mT("VistaCG"));

			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}

		GCPtr<GUI::GUIWindow>wind= getGUIManager()->addWindow(mT("Test"),vector2d(200),400);
		getGUIManager()->addButton(mT("gds"),vector2d(215,100),vector2d(100,30),0,wind);
		getGUIManager()->addEditBox(mT("Test"),vector2d(10,100),vector2d(200,30),0,wind);

		m_rt=getDevice()->createRenderTarget(mT("RT_VP"),256,video::EPixel_R8G8B8,true);
		m_2dViewport=new GUI::ViewPort2D(wind,m_rt);
		getGUIManager()->removeChild(wind);

		createScene();
		//getSceneManager()->enablePostProcessing(true);
		getSceneManager()->enableRenderTarget(false);
		//getSceneManager()->setRenderTargetSize(256);
		
		//getSceneManager()->setPostProcessing(new GrayscalePP(getDevice()));
		//createParticles();
 		//GCPtr<scene::ViewPort> vp=new scene::ViewPort(camera,m_rt,rectf(0.5,0.5,1,1),0);
 		//setViewPort(vp);

		GCPtr<GUI::GUIAnimatedSprite>cursor= new GUI::GUIAnimatedSprite(getGUIManager(),0,1,1,1,20,0,vector2d(32));

/*		GUI::XMLAnimatedSprite loader(cursor);
		loader.loadXML(gFileSystem.createBinaryFileReader(mT("Human.xml")));
		cursor->setCycle(mT("Normal"));
		ICursorControl::getInstance().SetMouseMap(cursor);
		ICursorControl::getInstance().CursorMap(true);*/

		m_blood0=gTextureResourceManager.loadTexture2D(mT("blood1.tga"));

		m_noiseDone=0;
		m_noise=gTextureResourceManager.createTexture2D(mT("Noise1"),vector2d(256),video::EPixel_R8G8B8);

		m_resQueue=new ResourceQueue();
		ResourceQueue::getInstance().addRequest(new NoiseResourceOrder(this),new NoiseResourceListener(this));

		gLogManager.endSection(true);

		getGUIManager()->addCustomChild(new GUI::GUIConsole(getGUIManager(),200));

		return;
	}

	class SplashCallback:public video::IShaderConstantsCallback
	{
	protected:
		float m_size;

	public:
		SplashCallback(float sz){
			m_size=sz;
		}
	};

	virtual void loopBody(float dt){
		GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();


// 		getDevice()->set2DMode();


// 		getDevice()->setRenderTarget(m_rt);
// 		getGUIManager()->drawAll();
// 		getDevice()->setRenderTarget(0);

		CMRayApplication::loopBody(dt);
		if(m_phManager)
			m_phManager->update(dt);

		if(m_entitys.size()>0)
			AI::SteerBehavior::getInstance().seek(m_entitys[0]->m_agent,camera->position,dt);

		for(int i=0;i<m_entitys.size();++i)
		{
			m_entitys[i]->update(dt);
		}
		if(m_skeleton){
			m_skeleton->update(dt);
		}


// 		m_2dViewport->draw();
//  		if(m_rt)
//  			getDevice()->draw2DImage(rectf(300,300,500,500),m_rt->getColorTexture(),255);

		static float a=0;
		a+=10*dt;

		SplashCallback callback(50);
/*
		getDevice()->useShader(m_splashShader);
		callback.setConstants(m_splashShader);
		getDevice()->draw2DImage(math::rectf(100,100,300,300),m_blood0,255);
		getDevice()->unuseShader();
		if(m_noiseDone && 0){
			video::SColor c;
			c.setHSL(0.3,1,0.5);
			c.A=255;
			getDevice()->draw2DImage(math::rectf(40,40,256,256),m_noise,c);
		}
		for (int i=0;i<morphValues.size();++i)
		{
			morphValues[i].target->setPercent(morphValues[i].m_val->Value());
		}*/


		if(terrain){
			float y=terrain->getHeight(camera->position().x,camera->position().z);
			if(camera->position().y-1<y){
				camera->position().y=y+1;
				camera->updateAbsoluteTransformation();
			}
		}


		if(font){
			GUI::FontAttributes attr;
			attr.fontColor.Set(20,255,128,255);
			attr.fontAligment=GUI::EFA_MiddleLeft;
			attr.fontSize=24;
			attr.hasShadow=true;
			attr.shadowColor.Set(0,0,0,255);
			attr.shadowOffset=vector2d(2);
			attr.spacing=2;
			attr.wrap=0;
			attr.RightToLeft=0;
			font->print(math::rectf(10,10,10,10),&attr,0,mT("FPS= %f"),CFPS::getInstance().getFPS());
			attr.fontSize=18;
			font->print(math::rectf(10,30,10,10),&attr,0,mT("Primitive drawn : %d"),getDevice()->getPrimitiveDrawnCount());
			if(0)
			{
				if(m_selectedNode){
					font->print(math::rectf(10,50,10,10),&attr,0,mT("Selected node is %s"),m_selectedNode->getNodeName());
					font->print(math::rectf(10,90,10,10),&attr,0,mT("Selected Pos is %f,%f,%f"),m_selectedNode->getAbsolutePosition().x,
						m_selectedNode->getAbsolutePosition().y,m_selectedNode->getAbsolutePosition().z);
				}
				OS::SMemoryState ms;
				if(gOSystem.getMemoryState(ms)){
					attr.fontSize=12;
					attr.fontColor.Set(0,0,255,255);
					attr.hasShadow=false;
					font->print(math::rectf(10,150,10,10),&attr,0,mT("Physical Memory : %d/%d"),ms.AvaliablePhysicalMemory,ms.TotalPhysicalMemory);
					font->print(math::rectf(10,165,10,10),&attr,0,mT("Virtual Memory : %d/%d"),ms.AvaliableVirtualMemory,ms.TotalVirtualMemory);
					font->print(math::rectf(10,180,10,10),&attr,0,mT("Page File: %d/%d"),ms.AvaliablePageFile,ms.TotalPageFile);
				}
			}
		}

		for (int i=0;i<m_rockets.size();++i)
		{
			m_rockets[i]->update(dt);
		}
	}

	virtual void onDone(){
		traceFunction(Application);
		array<core::string> arr;
		ResManagersListIT it= ResourceGroup::getInstance().begin();
		ResManagersListIT end= ResourceGroup::getInstance().end();

		for(;it!=end;++it){
			IResourceManager*m=(*it);
			gLogManager.startSection(m->getResourceType());
			core::string msg;
			msg=mT("Used Memory: ");
			msg+=(int)m->getUsedMemory(true);
			gLogManager.log(msg,ELL_INFO);
			m->listResources(arr);
			for(int i=0;i<arr.size();++i){
				msg=arr[i];
				gLogManager.log(msg,ELL_INFO);
			}
			arr.clear();
			gLogManager.endSection(1);

		}
	}
};


class CA:public IObject
{
	DECLARE_RTTI
	int m_val;
	void setVal(const int &	v){
		m_val=v;
	}
	int& getVal(){
		return m_val;
	}
public:
	rwProperty<CA,int> Value;
	CA():Value(this,&CA::setVal,&CA::getVal,EPT_INT,mT("Value"))
	{
		fillProperties();
		Value=10;
	}
	virtual void fillProperties(){
		addPropertie(&Value);
	}
	virtual core::string toString()const{
		return mT("My Value Is :")+core::string(Value());
	}
	virtual void parse(const core::string&str){
		core::UParseString::parse(str,Value());
	}
};
IMPLEMENT_RTTI(CA,IObject);

class CB:public IObject
{
	DECLARE_RTTI
public:
	CB(){}
};
IMPLEMENT_RTTI(CB,IObject);

uint toRGB1(float rm1, float rm2, float rh)
{
	while ( rh > 2.f * math::PI32 )
		rh -= 2.f * math::PI32;

	while ( rh < 0.f )
		rh += 2.f * math::PI32;

	if      (rh <  toRad(60.0f) ) rm1 = rm1 + (rm2 - rm1) * rh / (toRad(60.0f ));
	else if (rh < toRad(180.0f ) ) rm1 = rm2;
	else if (rh < toRad(240.0f ) ) rm1 = rm1 + (rm2 - rm1) * ( ( toRad(240.0f ) ) - rh) / (toRad(60.0f ));

	return (uint) (rm1 * 255.f);
}

APPLICATION_ENTRY_POINT
{
#if defined(_DEBUG) && defined(WIN32)
	// check for memory leaks at program exit (after the 'return 0')
	// through a call to _CrtDumpMemoryLeaks 
	// note that in debug mode, objects allocated with the new operator 
	// may be destroyed *after* the end of the main function. 
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	long temp=-1;
	_crtBreakAlloc=temp;
#endif

#ifdef TEST
	core::string str;
	GCPtr<IObject>a=new CA();
	IProperty*prop= a->getPropertie(mT("Value"));
	if(prop)
		prop->parse(mT("40"));
	str=a->toString();
	GCPtr<CA>ca=a;
	a->parse(mT("50"));
	str=ca->toString();
	GCPtr<IObject>b=new CB();
	str=b->toString();
#endif


	{
		video::SColor clr(255,129,200,255);
		float v,x,f;
		float d[4];
		vector3d hsv;
		clr.FloatArray(d);
		x=math::Min(d[0],Min(d[1],d[2]));
		v=math::Max(d[0],Max(d[1],d[2]));
		if(x==v)
			hsv.set(0,0,v);
		else{
			f=(d[0]==x)? d[1]-d[2]: (d[1]==x)?d[2]-d[0] : d[0]-d[1];
			int i=(d[0]==x)? 3 : (d[1]==x) ? 5 : 1;
			hsv.set(i-f/(v-x),(v-x)/v,v);
		}
	}

	{
		video::SColor clr;
		float h,s,l;
		h=0;
		s=0.94;
		l=0.486;

		if ( s == 0.0f) // grey
		{
			uchar c = (uchar) ( l * 255.0 );
			clr.Set(c,c,c,255);
		}

		float rm1, rm2;

		if ( l <= 0.5f )
		{
			rm2 = l + l * s;  
		}
		else
		{
			rm2 = l + s - l * s;
		}

		rm1 = 2.0f * l - rm2;   

		clr.R= ( toRGB1(rm1, rm2, h + (toRad(120.0f  )) ));
		clr.G= ( toRGB1(rm1, rm2, h) );
		clr.B= ( toRGB1(rm1, rm2, h - (toRad(120.0f )) )) ;

		clr.A=255;
	}


 	Application* app=new Application();
 
 	core::string resFileName=mT("Resources.stg");
 #ifdef _DEBUG
 	resFileName=mT("Resources_d.stg");
 #endif
 
 	if(app->startup(mT("MRaY App"),vector2di(800,600),false,resFileName,0,true,false))
 		app->run();
 	delete app;


	GCCollector::shutdown();
	
}