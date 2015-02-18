// TestProfile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GCCollector.h"
#include "SOptionElement.h"
#include "MeshAnimatorManager.h"
#include "MeshFileCreatorManager.h"
#include "GUIBatchRenderer.h"
#include "MeshBufferData.h"
#include "../common/DemoCamera.h"
#include "mrayNet.h"
#include "IWebRequest.h"
#include "WebURL.h"
#include "AnimationController.h"
#include "Environment.h"
#include "CharacterManager.h"
#include "Character.h"
#include "AISystem.h"
#include "AIRenderDevice.h"
#include "DebugDrawManager.h"
#include "INavWorld.h"
#include "SimplePhysics.h"
#include "AIAgent.h"
#include "PathFindPlanner.h"
#include "ParsedShaderPP.h"
#include "BoneNode.h"
#include "ViewPort.h"
#include "IGUISliderBar.h"
#include "IGUIPanelElement.h"
#include "GUIManager.h"
#include "PhysXManager.h"
#include "XMLTree.h"
#include "IGUICheckBox.h"
#include "IGUIButton.h"

#include "StereoRenderer.h"
#include "PhysicalModel/Segment.h"

#include "AtmosphericCubeMap.h"
//#include "TheoraManager.h"
#include "MDLWriter.h"
#include "MDLLoaderV0.h"
#include "TraceManager.h"
#include "ImageSetResourceManager.h"
#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"
#include "FontResourceManager.h"
#include "JobPool.h"
#include "GUIThemeManager.h"
#include "ArchiveManager.h"
#include "SoundResourceManager.h"
#include "ResourceGroup.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "IGameComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"
#include "SceneComponent.h"
#include "ModelCollisionComponent.h"
#include "VehicleGameComponent.h"
#include "VehicleWheelComponent.h"

#include "XMLSceneLoader.h"
#include "IPhysicalForceField.h"
#include "PhysicalForceFieldDef.h"
#include "IPhysicalForceFieldShapes.h"
#include "IPhysicalForceFieldShapeGroup.h"

#include "PhysicalVehicle.h"
#include "VehicleDesc.h"
#include "VehicleGearDesc.h"
#include "VehicleMotorDesc.h"
#include "VehicleWheelDesc.h"
#include "VehicleManager.h"
#include "VehicleMotor.h"
#include "VehicleGear.h"
#include "VehicleXMLParser.h"
#include "IVehicleWheel.h"

#include "mrayNVPerfSDK.h"
#include "GUILineStream.h"
#include "GameComponentCreator.h"
#include "IGameComponentFactory.h"

#include "TransformManipulator.h"

//#include "vld.h"

using namespace mray;
using namespace core;
using namespace math;

class LeakyBucket
{
	ulong m_tick;
	float m_rate;
	long m_credit;
	ulong m_burstSize;
	uint m_fillRate;
public:
	LeakyBucket(ulong fillRate,ulong burst):m_fillRate(fillRate),m_credit(0),m_burstSize(burst),m_rate(0)
	{
	}
	void Start()
	{
		m_tick=gTimer.getActualTime();
		m_credit=0;
		m_rate=0;
	}
	uint Update(uint sz)
	{
		ulong tick=gTimer.getActualTime();
		ulong dt=(tick-m_tick);

		if(dt>0)
		{
			uint kbits=(sz*8/1024);
			m_rate=0.5*m_rate + 0.5*kbits/dt;
			m_tick=tick;
		}
		m_credit+=m_fillRate*dt*1024*1024/1000/8;
		if(m_credit>m_burstSize)
			m_credit=m_burstSize;
		m_credit-=sz;
		if(m_credit>=0)
			return 0;
		else
			return (-m_credit)*8*1000/(1024*1024)/m_fillRate;
	}
};

class WebJobRequest:public JobOrder
{
	network::WebURL m_url;
	core::string m_fileName;
public:
	WebJobRequest(const network::WebURL& r,const core::string& fname):m_url(r),m_fileName(fname)
	{

	}
	virtual bool ExecuteJob()
	{
		LeakyBucket bucket(1,100);
		GCPtr<network::IWebRequest> request= network::INetwork::getInstance().CreateWebRequest();
		request->SetTimeout(0xFFFFFFFF);
		request->SetURL(m_url);
		request->Connect();

		OS::IStream* stream=request->GetStream();

		uchar data[1024];
		//bucket.Start();
		OS::IStreamPtr output=gFileSystem.createBinaryFileWriter(m_fileName);
		int count=0;
		int i=0;
		do 
		{
			int r=stream->read(data,1024);
			//uint s=bucket.Update(r);
			//if(s>0)
			//	Sleep(s);
			count+=r;
			if(!r)
				break;
			output->write(data,r);
			++i;
			//page+=data;
		} while (true);

		request->Disconnect();
		output->close();

		printf(" %d Bytes\n",count);
		return true;
	}
};

class ModelLoaderJobRequest:public JobOrder
{
	scene::ISceneNode* m_owner;
	core::string m_fileName;
public:
	scene::Skeleton* skeleton;
	scene::MeshRenderableNode* modelNode;
	ModelLoaderJobRequest(scene::ISceneNode* o,const core::string& fname):m_owner(o),m_fileName(fname)
	{
		SetTargetQueue(0);
	}
	virtual bool ExecuteJob()
	{
		modelNode= scene::MeshFileCreatorManager::getInstance().LoadFromFile(m_fileName,true);
		/*loaders::mdlLoaderV0 loader;
		scene::SMesh*mesh=loader.load(mT("outModel.mdl"));
		modelNode=new scene::MeshRenderableNode(mesh);
		*/
		//modelNode->setShadowCaster(true);
		skeleton= modelNode->getMesh()->getSkeleton();
		if(skeleton)
		{
			printf("skeleton exists\n");
			//skeleton->getAnimationMixer()->getTrackGroup("Idle1")->playAll();
			//skeleton->getAnimationMixer()->getAnimationController("Walk")->play(10,animation::EAM_Cycle);
			scene::BoneNode*bone= skeleton->getBone(mT("Bip01_L_Thigh"));
			if(bone)
			{
			//	bone->rotate(20,math::vector3d::ZAxis,scene::TS_Local);
			}
		}
		m_owner->AttachNode(modelNode);
		return true;
	}
};

class HeadMount:public scene::ISceneNode
{
	InputManager* m_input;
	float m_speed;
public:
	HeadMount(scene::ISceneManager*smngr,float speed,InputManager* kb):scene::ISceneNode(mT("HeadMount"),0,smngr)
	{
		m_speed=speed;
		m_input=kb;
	}
	virtual void update(float dt){
		if(!this->isVisible())
			return;
		float dx=m_speed*(m_input->getKeyboard()->getKeyState(KEY_A)-
			m_input->getKeyboard()->getKeyState(KEY_D))*dt;
		float dz=m_speed*(m_input->getKeyboard()->getKeyState(KEY_W)-
			m_input->getKeyboard()->getKeyState(KEY_S))*dt;

		// 			dx*=(1+2*gKeyboardController.isLShiftPress());
		// 			dz*=(1+2*gKeyboardController.isLShiftPress());

		if(m_input->getMouse()->isPressed(controllers::EMB_Right)){
			this->rotate(-80*m_input->getMouse()->getDX()*dt,math::vector3d(0,1,0),scene::TS_World);
			this->rotate(80*m_input->getMouse()->getDY()*dt,math::vector3d(1,0,0),scene::TS_Local);
		}
		this->translate(math::vector3d(dx,0,dz),scene::TS_Local);
		ISceneNode::update(dt);
	}
};

class CharacterListener:public PhysicsBasedCharacter::ICharacterListener
{
public:
	void OnCharacterContact(PhysicsBasedCharacter::Character* ch,physics::IPhysicalNode* node)
	{
		//ch->deactivate();
		game::GameEntity* ent=dynamic_cast<game::GameEntity*>(node->GetUserData());
		//if(!ent)
		//	std::cout<<"Character Contacted with physical node"<<std::endl;
		//else
		//	std::cout<<"Character Contacted with :"<<ent->GetName().c_str() <<std::endl;
	};
}g_characterListener;


class GernadeComponent:public game::IGameComponent
{
	DECLARE_RTTI
protected:
	enum EStatus
	{
		EIdle,
		EExploded,
		EDone
	};
	physics::IPhysicalForceField* m_forceField;
	physics::IPhysicalForceFieldSphereShape* m_shape;
	physics::IPhysicalForceFieldLinearKernal* m_kernal;
	EStatus m_status;
	float m_timeToExplode;
	float m_time;

	bool m_explodable;

public:
	DECLARE_PROPERTY_TYPE(TimeToExplode,float,)
	DECLARE_PROPERTY_TYPE(Explodable,bool,)
public:

	bool SetTimeToExplode(float t)
	{
		m_timeToExplode=t;
		return true;
	}
	float GetTimeToExplode()const
	{
		return m_timeToExplode;
	}

	bool SetExplodable(bool t)
	{
		m_explodable=t;
		return true;
	}
	bool GetExplodable()const
	{
		return m_explodable;
	}

	bool CreateForceField()
	{
		game::PhysicsComponent* phNode=(game::PhysicsComponent*)m_ownerEntity->GetFirstComponent(game::PhysicsComponent::getClassType());
		if(!phNode)
			return false;

		physics::PhysicalForceFieldDesc ffDesc;
		physics::PhysicalForceFieldLinearKernalDesc lkernalDesc;
		lkernalDesc.constant.set(0,0,0);
		lkernalDesc.noise.set(1.3,1.3,1.3);
		lkernalDesc.velocityTarget.set(200,0,0);
		//lkernalDesc.positionTarget=phNode->GetPhysicalNode()->getGlobalPosition();
		lkernalDesc.positionMultiplier.f11=-1;
		lkernalDesc.positionMultiplier.f22=-1;
		lkernalDesc.positionMultiplier.f33=-1;

		m_kernal=phNode->GetPhysicalNode()->GetCreator()->createForceFieldLinearKernal(&lkernalDesc);
		ffDesc.trans.makeIdentity();
		ffDesc.node=phNode->GetPhysicalNode();
		phNode->GetPhysicalNode()->SetBodyFlag(physics::EBF_Kinematic,true);


		ffDesc.flags[physics::EFFF_VolumetricScalingCloth]=0;
		ffDesc.flags[physics::EFFF_VolumetricScalingFluid]=0;
		ffDesc.flags[physics::EFFF_VolumetricScalingRigidBody]=true;
		ffDesc.flags[physics::EFFF_VolumetricScalingSoftBody]=0;

		ffDesc.kernal=m_kernal;
		m_forceField=phNode->GetPhysicalNode()->GetCreator()->createForceField(&ffDesc);

		physics::PhsyicalForceFieldSphereShapeDesc b;
		b.radius=10;
		//b.mat.setTranslation(phNode->GetPhysicalNode()->getGlobalPosition());
		m_shape=(physics::IPhysicalForceFieldSphereShape*)m_forceField->getIncludeShapeGroup()->createShape(&b);

		return true;;
	}

	GernadeComponent(game::GameEntityManager*)
	{
	}
	GernadeComponent(float timeToExplode,bool explode)
	{
		m_explodable=explode;
		m_status=EIdle;
		m_forceField=0;
		m_timeToExplode=timeToExplode;
		m_shape=0;
		m_time=0;

		CPropertieDictionary* dic;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeTimeToExplode::instance);
			dic->addPropertie(&PropertyTypeExplodable::instance);
		}
	}
	~GernadeComponent()
	{
		if(m_forceField)
		{
			m_forceField->getCreator()->removeForceFieldLinearKernal(m_kernal);
			m_forceField->getCreator()->removeForceField(m_forceField);
		}
	}

	virtual void Update(float dt)
	{
		if(!m_explodable)
			return;
		game::SceneComponent* node=(game::SceneComponent*)m_ownerEntity->GetFirstComponent(game::SceneComponent::getClassType());

		switch(m_status)
		{
		case EIdle:
			m_time+=dt;

			if(m_time>=m_timeToExplode)
			{
				m_status=EExploded;
				m_time=0;
				if(!CreateForceField())
					m_status=EDone;
			}
			break;
		case EExploded:
			m_time+=dt;
			m_shape->setRadius(50+pow(2,m_time*10));
			node->GetSceneNode()->setScale(m_shape->getRadius());
			if(m_time>=1)
			{
				m_forceField->getCreator()->removeForceField(m_forceField);
				m_forceField=0;
				m_status=EDone;
				((scene::ISceneNode*)node->GetSceneNode())->setVisible(false);
				m_ownerEntity->GetGameEntityManager()->RequestToRemove(m_ownerEntity);
			}
			break;
		}
	}
};

IMPLEMENT_RTTI(GernadeComponent,game::IGameComponent);

IMPLEMENT_PROPERTY_TYPE_HEADER(TimeToExplode,GernadeComponent,float,"TimeToExplode",EPBT_Basic,"",3)
IMPLEMENT_PROPERTY_TYPE_GENERIC(TimeToExplode,GernadeComponent,float,SetTimeToExplode,GetTimeToExplode,core::StringConverter::toString,core::StringConverter::toFloat,false)

IMPLEMENT_PROPERTY_TYPE_HEADER(Explodable,GernadeComponent,bool,"Explodable",EPBT_Basic,"",true)
IMPLEMENT_PROPERTY_TYPE_GENERIC(Explodable,GernadeComponent,bool,SetExplodable,GetExplodable,core::StringConverter::toString,core::StringConverter::toFloat,false)

DECLARE_GAMECOMPONENT_FACTORY(GernadeComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(GernadeComponent);

class CharacterGameComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_modelPath;
	PhysicsBasedCharacter::Character* m_character;
	PhysicsBasedCharacter::CharacterManager* m_charManager;
	float m_scale;
public:
	DECLARE_PROPERTY_TYPE(ModelPath,core::string,);
public:
	CharacterGameComponent(game::GameEntityManager*)
	{
	}
	CharacterGameComponent(PhysicsBasedCharacter::CharacterManager* cm)
	{
		m_scale=1;
		m_charManager=cm;
		m_character=0;

		CPropertieDictionary* dic;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeModelPath::instance);
		}
	}
	~CharacterGameComponent()
	{
	}

	bool SetScale(float scale)
	{
		m_scale=scale;
		return true;
	}
	float GetScale()
	{
		return m_scale;
	}


	PhysicsBasedCharacter::Character* GetModel()
	{
		return m_character;
	}
	virtual IObject* GetAttachedObject()
	{
		return m_character;
	}
	bool SetModelPath(const core::string& path)
	{
		m_modelPath=path;
		game::ModelComponent* modelComp=(game::ModelComponent*)m_ownerEntity->GetFirstComponent(game::ModelComponent::getClassRTTI());
		if(!modelComp)
			return false;
		m_character= m_charManager->CreateCharacter(m_modelPath,modelComp->GetModel()->getMesh()->getSkeleton(),m_scale,m_ownerEntity->GetShape()->getPosition());
		m_character->setModel(m_ownerEntity->GetShape());
		m_character->activate();
		m_character->AddListener(&g_characterListener);

		PhysicsBasedCharacter::SegmentList segments;
		m_character->getRoot()->getAllChildren(segments);
		PhysicsBasedCharacter::SegmentList::iterator it=segments.begin();
		for(;it!=segments.end();++it)
		{
			if((*it)->getJoint())
			{
				physics::IPhysicalJointD6Desc desc;
				(*it)->getJoint()->SaveToDesc(&desc);
				desc.linearLimit.damping=0.9;
				desc.linearLimit.spring=0.9;

				desc.swing1Limit.damping=0.9;
				desc.swing1Limit.spring=0.9;

				desc.swing2Limit.damping=0.9;
				desc.swing2Limit.spring=0.9;

				desc.twistLimit.low.damping=0.9;
				desc.twistLimit.low.spring=0.9;

				desc.twistLimit.high.damping=0.9;
				desc.twistLimit.high.spring=0.9;

				(*it)->getJoint()->LoadFromDesc(&desc);
			}
		}
		PhysicsBasedCharacter::Segment*seg= m_character->getSegment(mT("Head"));
		if(seg)
		{
			seg->getActor()->SetBodyFlag(physics::EBF_Kinematic,true);
		}
		return true;

	}

	const core::string& GetModelPath()
	{
		return m_modelPath;
	}

};

IMPLEMENT_RTTI(CharacterGameComponent,game::IGameComponent);

IMPLEMENT_PROPERTY_TYPE_HEADER(ModelPath,CharacterGameComponent,core::string,mT("ModelPath"),EPBT_Basic,mT("Model's path"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(ModelPath,CharacterGameComponent,core::string,SetModelPath,GetModelPath,,,false);

DECLARE_GAMECOMPONENT_FACTORY(CharacterGameComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(CharacterGameComponent);


class Application:public CMRayApplication
{

protected:
	GCPtr<scene::LightNode> m_light;
	GCPtr<scene::CameraNode> m_cameraL;
	//GCPtr<scene::ViewPort> m_viewPortL;
	GCPtr<scene::CameraNode> m_cameraR;
	//GCPtr<scene::ViewPort> m_viewPortR;
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<sound::ISoundManager> m_soundManager;
	GCPtr<network::INetwork> m_network;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<GUI::GUIManager> m_guiManager;

	GCPtr<PhysicsBasedCharacter::CharacterManager> m_charManager;

	GCPtr<AI::AIRenderDevice> m_renderDev;
	GCPtr<scene::DebugDrawManager> m_debugManager;
	GCPtr<AI::AISystem> m_aiSystem;

	GCPtr<PhysicsBasedCharacter::Character> m_character;

	GCPtr<GUI::IGUISliderBar> m_offsetGUI;

	GCPtr<video::GPUShader> m_fontShader;

	GCPtr<scene::StereoRenderer> m_stereoRenderer;

	GCPtr<game::AtmosphericCubeMap> m_atmoMap;
	//GCPtr<video::TheoraManager> m_theoraManager;
	//GCPtr<video::IVideoTexture> m_theoraVideo;

	GCPtr<game::GameEntityManager> m_gameManager;
	GCPtr<physics::VehicleManager> m_vehicleManager;
	physics::PhysicalVehicle* m_carVehicle;
	math::vector3d m_vehControling;

	GCPtr<video::mrayNVPerfSDK> m_perfSDK;
	GCPtr<GUI::GUILineStream> m_perfHUD;

	scene::SPatchTerrain*m_terrain;

	GCPtr<scene::TransformManipulator> m_manipulator;


	void CreateGernade(float radius,bool explodable)
	{
		physics::PhysicalBodyDesc bodyDesc;
		physics::PhysicalNodeDesc nodeDesc;
		physics::SphereShapeDesc shapeDesc;

		static physics::IPhysicMaterial* material=0;
		if(!material)
		{
			physics::PhysicMaterialDesc matDesc;
			matDesc.restitution=0.6;
			matDesc.dynamicFriction=0.5;
			material=m_phManager->createMaterial(&matDesc);
		}
		shapeDesc.radius=radius;
		bodyDesc.mass=10;
		shapeDesc.material=material;
		nodeDesc.shapes.push_back(&shapeDesc);
		nodeDesc.bodyDesc=&bodyDesc;

		nodeDesc.globalPos=m_stereoRenderer->GetHeadMount()->getAbsoluteTransformation();
		bodyDesc.linearVelocity=m_stereoRenderer->GetHeadMount()->getAbsoluteOrintation()*math::vector3d(0,0,50);
		physics::IPhysicalNode* node=m_phManager->createNode(&nodeDesc);

		static scene::SMeshPtr mesh;
		if(mesh.isNull())
		{
			mesh=new scene::SMesh();
			scene::MeshGenerator::getInstance().generateSphere(1,8,8, mesh->addNewBuffer()->getMeshBuffer());
		}
		scene::ISceneNode* sceneNode=getSceneManager()->createSceneNode(mT("Gernade"));
		scene::MeshRenderableNode* rendNode=new scene::MeshRenderableNode(mesh->duplicateContextOnly());
		sceneNode->AttachNode(rendNode);
		sceneNode->setScale(radius);

		game::GameEntity* ent=m_gameManager->CreateGameEntity(mT("GameEntity"));
		ent->SetShape(sceneNode);
		((game::SceneComponent*)ent->GetFirstComponent(game::SceneComponent::getClassRTTI()))->SetRemoveNodeOnDestroy(true);
		game::PhysicsComponent* phComp=new game::PhysicsComponent(m_phManager);
		phComp->SetPhysicalNode(node);
		node->EnableContactReportFlag(physics::ECRF_Touch);
		phComp->SetRemoveNodeOnDestroy(true);
		ent->AddComponent(phComp);
		GernadeComponent* gerComp=new GernadeComponent(3,explodable);
		ent->AddComponent(gerComp);

		{
			xml::XMLElement e(mT("Root"));
			ent->exportXMLSettings(&e);
			xml::XMLWriter w;
			w.addElement(&e);
			std::cout<<w.flush().c_str()<<std::endl;
		}
	}

public:


	Application():CMRayApplication()
	{
		m_carVehicle=0;
	}

	virtual ~Application()
	{
	}
	virtual void onEvent(Event* event)
	{
		CMRayApplication::onEvent(event);
		if(m_guiManager)
			m_guiManager->OnEvent(event);

		if(event->getType()==ET_ResizeEvent)
		{
			ResizeEvent* e=(ResizeEvent*)event;
			if(m_manipulator)
				m_manipulator->OnViewportResize();
		}
		if(event->getType()==ET_Mouse)
		{
			MouseEvent* e=(MouseEvent*)event;
			if(e->event==MET_LEFTDOWN)
			{
				m_manipulator->OnMousePressed(e->pos.x,e->pos.y);
			}
			if(e->event==MET_LEFTUP)
			{
				m_manipulator->OnMouseReleased();
			}
			if(e->event==MET_RIGHTDOWN)
			{
				m_manipulator->OnRMousePressed();
			}
			if(e->event==MET_MIDDLEDOWN)
			{
				m_manipulator->OnMouseMiddlePressed(e->pos.x,e->pos.y);
			}
			if(e->event==MET_MIDDLEUP)
			{
				m_manipulator->OnMouseReleased();
			}
			if(e->event==MET_MOVED)
			{
				m_manipulator->OnMouseMoved(e->pos.x,e->pos.y);
			}
		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent*e=(KeyboardEvent*) event;
			if(e->press && e->key==KEY_SPACE)
			{
				int mode=(m_stereoRenderer->GetMode()+1) % scene::EStereo_ModesCount;
				m_stereoRenderer->SetMode((scene::EStereoMode)mode);
			}else if(e->press && e->key==KEY_G)
				CreateGernade(0.3,true);
			else if(e->press && e->key==KEY_F)
				CreateGernade(2,false);
			else if(e->press && e->key==KEY_V)
			{
				math::quaternion ori=m_stereoRenderer->GetHeadMount()->getAbsoluteOrintation();
				m_carVehicle=CreateVehicle(m_stereoRenderer->GetHeadMount()->getAbsolutePosition(),ori);
			}else if(e->press && e->key==KEY_C)
			{
				math::quaternion ori=m_stereoRenderer->GetHeadMount()->getAbsoluteOrintation();
				m_character=CreateCharacter(m_stereoRenderer->GetHeadMount()->getAbsolutePosition(),ori);
			}

			if(e->press && e->key==KEY_1)
			{
				m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Translate);
			}
			if(e->press && e->key==KEY_2)
			{
				m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Rotate);
			}
			if(e->press && e->key==KEY_3)
			{
				m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Scale);
			}

			if(e->key==KEY_UP )
			{
				m_vehControling.x=10*e->press;
			}else if(e->key==KEY_DOWN)
			{
				m_vehControling.x=-10*e->press;
			}else if(e->key==KEY_LEFT)
			{
				m_vehControling.y=-0.001*e->press;
			}else if(e->key==KEY_RIGHT)
			{
				m_vehControling.y=0.001*e->press;
			}else if(e->key==KEY_E)
			{
				xml::XMLElement e(mT("Root"));
				m_gameManager->exportXMLSettings(&e);
				xml::XMLWriter w;
				w.addElement(&e);
				OS::IStreamPtr stream= gFileSystem.createBinaryFileWriter(mT("..\\Data\\GameEntities.xml"));
				OS::StreamWriter sw(stream);
				sw.writeString(w.flush());
				stream->close();
			}

		}
	}
	math::quaternion rotationArcMelax(const math::vector3d & a,const math::vector3d & b) 
	{  
		math::vector3d c  = a.crossProduct(b);
		float ss = (1.f+a.dotProduct(b))*2.f;  
		float rs = 1.0f/sqrt(ss); 
		float s  = sqrt(ss); 
		return math::quaternion(s*.5f,c.x*rs,c.y*rs,c.z*rs);
	}
	typedef JobOrder* JobOrderPtr;
	void OnDownloadDone(const JobOrderPtr& o,const bool& res)
	{
		printf("Download Done\n");

	}

	scene::SPatchTerrain* loadTerrain()
	{
		GCPtr<loaders::HeightMapRaw16> l=new loaders::HeightMapRaw16();
		l->loadHeightMap(gFileSystem.openFile(mT("terrain.raw")));
		m_terrain=new scene::SPatchTerrain(0,math::vector3d(10000,200,10000));
		m_terrain->loadTerrain(l,7,0.05,false);
		m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")),0);
		m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("detailmap.bmp")),1);

		video::GPUShader*shader= new video::GPUShader();

		shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
		shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
		m_terrain->getMaterial()->setRenderShader(shader);

		scene::ISceneNode*node=getSceneManager()->createSceneNode(mT("Terrain"));
		node->AttachNode(m_terrain);
		physics::PhysicMaterialDesc matDesc;
		matDesc.dynamicFriction=0.5;
		matDesc.staticFriction=0.7;
		physics::TerrainShapeDesc terrDesc(l->getWidth(),l->getHeight(),l->getHeightData(),m_terrain->GetScale());
		terrDesc.material=m_phManager->createMaterial(&matDesc);
		physics::PhysicalNodeDesc phNode;
		phNode.actorGroupID=1;
		phNode.shapes.push_back(&terrDesc);
		m_phManager->createNode(&phNode);

		scene::TerrainTextureLayer* layer;
 		layer=new scene::TerrainTextureLayer();
 		layer->SetBaseTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\nm_1\\gren.jpg")));
 		layer->SetDetailTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\nm_1\\nm3.jpg")));
 		layer->SetMaskTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\Layer4Mask.png")));
 
 		layer->SetBaseTilt(1000);
 		layer->SetDetailTilt(100);
 		layer->SetAlpha(2);

		m_terrain->AddTextureLayer(layer);

		layer=new scene::TerrainTextureLayer();
		layer->SetBaseTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\nm_1\\nm3.jpg")));
		layer->SetDetailTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\nm_1\\nm1.jpg")));
		layer->SetMaskTexture(gTextureResourceManager.loadTexture2D(mT("TerrTex\\Layer2Mask.png")));

		layer->SetBaseTilt(1000);
		layer->SetDetailTilt(100);
		layer->SetAlpha(1);
		m_terrain->AddTextureLayer(layer);
		return m_terrain;
	}
	PhysicsBasedCharacter::Character* CreateCharacter(const math::vector3d& pos,const math::quaternion& ori)
	{
		scene::ISceneNode* node=getSceneManager()->createSceneNode();
		node->setPosition(pos);
		float scale=10;
		node->setScale(scale);
		node->setDrawBoundingBox(true);

		game::GameEntity* ent=m_gameManager->CreateGameEntity(mT("Character"));
		ent->SetShape(node);
		game::ModelComponent* modelComp=new game::ModelComponent();
		ent->AddComponent(modelComp);
		modelComp->SetModelPath(mT("sportive03_m_mediumpoly.mdx"));

		CharacterGameComponent* chComp=new CharacterGameComponent(m_charManager);
		ent->AddComponent(chComp);

		chComp->SetScale(scale);
		chComp->SetModelPath(mT("Character\\Human2.xml"));

		chComp->GetModel()->getRoot()->setGlobalOrientation(ori);



		{
			xml::XMLElement e(mT("Root"));
			ent->exportXMLSettings(&e);
			xml::XMLWriter w;
			w.addElement(&e);
			std::cout<<w.flush().c_str()<<std::endl;
		}
		return chComp->GetModel();
	}

	physics::PhysicalVehicle* CreateVehicle(const math::vector3d& pos,const math::quaternion& ori)
	{
		float scale=10;
		scene::ISceneNode*node;
		scene::SMesh* NodeMesh=0;

		game::GameEntity* ent=m_gameManager->CreateGameEntity(mT("Car"));
		game::ModelComponent* modelComp=new game::ModelComponent();

		node=getSceneManager()->createSceneNode(mT("Car"));
		node->setScale(scale);
		ent->SetShape(node);

		ent->AddComponent(modelComp);

		modelComp->SetModelPath(mT("Buggy.mdx"));

		modelComp->GetModel()->setShadowCaster(true);

		game::ModelCollisionComponent* collComp=new game::ModelCollisionComponent();
		collComp->SetModelPath(mT("buggy_collision.mdl"));

		game::PhysicsComponent*comp=new game::PhysicsComponent(m_phManager);

		math::matrix4x4 globPos;
		globPos.setTranslation(pos);
		comp->SetGlobalPos(globPos);
		comp->SetMass(1000);
		ent->AddComponent(comp);
		comp->AddComponent(collComp);

		comp->GetPhysicalNode()->setGlobalOrintation(ori);

		game::VehicleGameComponent* vehComp=new game::VehicleGameComponent(m_vehicleManager);
		ent->AddComponent(vehComp);
		vehComp->SetScale(scale);
		vehComp->SetVehiclePath(mT("VehicleCar.xml"));

		core::string bones[]=
		{
			mT("Wheel_FR"),
			mT("Wheel_FL"),
			mT("Wheel_BR"),
			mT("Wheel_BL")
		};
		core::string wheelNames[]=
		{
			mT("FR"),
			mT("FL"),
			mT("BR"),
			mT("BL")
		};


		for(int i=0;i<4;++i)
		{
			game::VehicleWheelComponent* wc=new game::VehicleWheelComponent(0);
			vehComp->AddComponent(wc);
			wc->SetBoneName(bones[i]);
			wc->SetWheelName(wheelNames[i]);
		}
		{
			xml::XMLElement e(mT("Root"));
			ent->exportXMLSettings(&e);
			xml::XMLWriter w;
			w.addElement(&e);
			std::cout<<w.flush().c_str()<<std::endl;
		}

		return vehComp->GetModel();
	}

	virtual void init(const OptionContainer &extraOptions){
		traceFunction(Application);
		CMRayApplication::init(extraOptions);

		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		game::GameComponentCreator::getInstance().RegisterFactory(&GameComponentFactoryCharacterGameComponent::Instance);
		game::GameComponentCreator::getInstance().RegisterFactory(&GameComponentFactoryGernadeComponent::Instance);

		if(core::StringConverter::toBool(extraOptions["NVPerfHUD"]->value))
		{
			m_perfSDK=new video::mrayNVPerfSDK();
			m_perfSDK->Init();
			GetRenderWindow()->AddListener(m_perfSDK);
			m_perfSDK->AddCounter(video::EPerfHUDCounterName::EGPUIdle);
			m_perfSDK->AddCounter(video::EPerfHUDCounterName::EGPUBusy);
			m_perfSDK->AddCounter(video::EPerfHUDCounterName::EPixelShaderBusy);
			m_perfSDK->AddCounter(video::EPerfHUDCounterName::EVertexShaderBusy);
		}


		{
			gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
		}
		{
			gImageSetResourceManager.loadImageSet(mT("Le.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("Le.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("Le"));
		}
		{
			gImageSetResourceManager.loadImageSet(mT("iTunesX.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("iTunesX.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("iTunesX"));
		}
		{
			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}

		if(0)
		{
			JobLoadCompleteDelegate doneDelegate=newClassDelegate2(mT(""),this,&Application::OnDownloadDone);

			network::createWin32Network();
			for(int i=0;i<100;++i)
			{
				WebJobRequest* request=new WebJobRequest(network::WebURL(mT("http"),mT("www.iclarified.com"),mT("/images/index/iClarified-home-002.png")),mT("downloaded/iClarified-home-002")+core::StringConverter::toString(i)+mT(".png"));
				JobPool::getInstance().AddRequest(request,doneDelegate);
			}
		}

		m_debugManager=new scene::DebugDrawManager(getDevice());

		if(0)
		{
			m_aiSystem=new AI::AISystem();
			m_aiSystem->GetNavigationWorld()->Deserialize(mT("navMesh.nav"));
			//m_aiSystem->GetNavigationWorld()->EnableVisualize(true);

			GameMod::SimplePhysics* phModel=new GameMod::SimplePhysics();
			phModel->SetGlobalPosition(math::vector3d(0,10,0));
			AI::AIAgent* actor=new AI::AIAgent(phModel,mT(""));
			m_aiSystem->AddActor(actor);
			actor->UpdateWorldNode();

			AI::PathFindPlanner *planner=new AI::PathFindPlanner (actor);

			planner->RequestPathToPosition(vector3d(5,10,0));

			m_renderDev=new AI::AIRenderDevice(m_debugManager);
		}

		{
			m_gameManager=new game::GameEntityManager();
			m_gameManager->SetSceneManager(getSceneManager());
		}

		GCPtr<video::SMaterial> mtrl=new video::SMaterial();
		if(1)
		{
			physics::PhysicsSystemDesc desc;
			desc.gravity.set(0,-98,0);
			m_phManager=new physics::PhysXManager(&desc);
			m_phManager->ConnectToRemoteDebugger();

			m_charManager=new PhysicsBasedCharacter::CharacterManager();
			m_charManager->SetPhysicsManager(m_phManager);

			m_gameManager->SetPhysicsManager(m_phManager);

			if(1)
			{

				mtrl->SetAmbient(1);
				mtrl->SetDiffuse(1);
				mtrl->SetEmissive(0);
				mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
				mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
				mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),1);
				video::GPUShader*shader= new video::GPUShader();

				shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
				shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
				mtrl->setRenderShader(shader);

				scene::SMesh* NodeMesh=new scene::SMesh();
				NodeMesh->addNewBuffer();
				scene::MeshGenerator::getInstance().generatePlane(1,1,NodeMesh->getBuffer(0));
				scene::SMeshManipulator::getInstance().scaleMesh(NodeMesh->getBuffer(0),500);
				NodeMesh->getBufferData(0)->setMaterial(mtrl);

				GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(NodeMesh);
				//	Node->setShadowCaster(true);
				Node->setShadowCaster(true);

// 				scene::ISceneNode*node=new scene::ISceneNode(mT("Node_Plane"),0,0);
// 				node->setPosition(math::vector3d(0,0,0));
// 				node->setScale(100);
// 				node->updateAbsoluteTransformation();
// 				node->AttachNode(Node);


//				getSceneManager()->addSceneNode(node);
				//m_vegetation->AddObject(node);
				physics::PlaneShapeDesc planeDesc(math::Plane::XZPlane);
				physics::PhysicalNodeDesc phNodeDesc;
				physics::PhysicalBodyDesc bodyDesc;
				physics::PhysicMaterialDesc matDesc;
				matDesc.dynamicFriction=0.8;
				matDesc.staticFriction=0.8;
				planeDesc.material=m_phManager->createMaterial(&matDesc);

				phNodeDesc.shapes.push_back(&planeDesc);
//				node->updateAbsoluteTransformation();
//				phNodeDesc.globalPos=node->getAbsoluteTransformation();
				//phNodeDesc.bodyDesc=&bodyDesc;
				GCPtr<physics::IPhysicalNode> phNode= m_phManager->createNode(&phNodeDesc);

				m_charManager->GetEnvironment()->addGroundNode(phNode.pointer());

				game::GameEntity* ent=m_gameManager->CreateGameEntity(mT("Ground"));
				phNode->SetUserData(ent);
			}
		}

		/*
		if(0)
		{
			m_theoraManager=new video::TheoraManager();
			m_theoraVideo=m_theoraManager->CreateVideo();
			m_theoraVideo->loadVideo(mT("big_buck_bunny_480p_stereo.ogg"));
			
			m_theoraVideo->play();
		}*/

		if(1)
		{

			m_cameraL=new scene::CameraNode(mT("LeftEye"),0,getSceneManager());
			//m_viewPortL=GetRenderWindow()->CreateViewport(mT("MainVPL"),m_cameraL,0,math::rectf(0,0,1,1),0);
			getSceneManager()->addSceneNode(m_cameraL);
			//m_cameraL->setPosition(math::vector3d(-5,0,0));

			m_cameraR=new scene::CameraNode(mT("RightEye"),0,getSceneManager());
			//m_viewPortR=GetRenderWindow()->CreateViewport(mT("MainVPR"),m_cameraR,0,math::rectf(0,0,1,1),0);
			getSceneManager()->addSceneNode(m_cameraR);
			//m_cameraR->setPosition(math::vector3d(5,0,0));

			HeadMount* hm=new HeadMount(getSceneManager(),100,GetInputManager());
			hm->addChild(m_cameraL);
			hm->addChild(m_cameraR);
			getSceneManager()->addSceneNode(hm);

			m_stereoRenderer=new scene::StereoRenderer(GetRenderWindow(),m_cameraL,m_cameraR,hm);

		}


		m_manipulator=new scene::TransformManipulator();
		m_manipulator->Init(mT("Manipulator"),getSceneManager(),m_stereoRenderer->GetFullVP(),m_cameraL);
		{
			scene::MeshRenderableNode* node=new scene::MeshRenderableNode(new scene::SMesh());
			scene::ISceneNode* snode=getSceneManager()->createSceneNode();
			snode->AttachNode(node);
			snode->setPosition(math::vector3d(0,10,0));

			scene::MeshBufferData* bufData=node->getMesh()->addNewBuffer();

			//test vertex based displacement
			scene::MeshGenerator::getInstance().generateSphere(5,100,100,bufData->getMeshBuffer());

			video::GPUShader* s=new video::GPUShader();
			s->SetVertexShader(gShaderResourceManager.loadShader("displacement.cg",video::EShader_VertexProgram,"main_vp","cg"));
			s->SetFragmentShader(gShaderResourceManager.loadShader("displacement.cg",video::EShader_FragmentProgram,"main_fp","cg"));
			video::SMaterial* mtrl=new video::SMaterial();
			mtrl->setRenderShader(s);

			video::TextureUnit*tu=new video::TextureUnit();
			video::ImageInfo* imgIfo= gTextureResourceManager.loadImageInfo(mT("rockwall_height.jpg"),video::ETT_2DTex);
			video::ITexturePtr tex=gTextureResourceManager.loadTexture2D(mT("rockwall_height.jpg"));
			tu->SetTexture(tex);
			video::ColorConverter::createNormalMap(imgIfo,1);
			mtrl->setTextureUnit(tu,1);

			tex=getDevice()->createEmptyTexture2D(true);
			tex->loadSurfaces(&imgIfo,1);
			tu=new video::TextureUnit();
			tu->SetTexture(tex);


			bufData->setMaterial(mtrl);
			m_manipulator->AddNode(snode);
		}

		{
			scene::MeshRenderableNode* node=new scene::MeshRenderableNode(new scene::SMesh());
			scene::ISceneNode* snode=getSceneManager()->createSceneNode();
			snode->AttachNode(node);
			snode->setPosition(math::vector3d(40,15,30));

			scene::MeshBufferData* bufData=node->getMesh()->addNewBuffer();

			scene::MeshGenerator::getInstance().generateSphere(10,10,10,bufData->getMeshBuffer());

			video::GPUShader* s=new video::GPUShader();
			s->SetVertexShader(gShaderResourceManager.loadShader("phong2.cg",video::EShader_VertexProgram,"main_vp","cg"));
			s->SetFragmentShader(gShaderResourceManager.loadShader("phong2.cg",video::EShader_FragmentProgram,"main_fp","cg"));
			video::SMaterial* mtrl=new video::SMaterial();
			mtrl->setRenderShader(s);

			video::TextureUnit*tu=new video::TextureUnit();
			tu->SetTexture(gTextureResourceManager.loadTexture2D(mT("rockwall_height.jpg")));
			mtrl->setTextureUnit(tu,1);

			bufData->setMaterial(mtrl);

			m_manipulator->AddNode(snode);
		}

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());

//		getDevice()->setFogColor(video::SColor(0,0,1,1));

		{
			m_guiManager=new GUI::GUIManager(getDevice());

			GUI::IGUIPanelElement* rootElem=new GUI::IGUIPanelElement(mray::GUID(""),m_guiManager);
			rootElem->SetSize(GetRenderWindow()->GetSize());

			GUI::IGUIButton* btnElem=(GUI::IGUIButton*)m_guiManager->CreateElement(mray::GUID("Button"));
			btnElem->SetPosition(math::vector2d(50,200));
			btnElem->SetSize(math::vector2d(200,50));
			//rootElem->AddElement(btnElem);


			GUI::IGUICheckBox* chkElem=(GUI::IGUICheckBox*)m_guiManager->CreateElement(mray::GUID("CheckBox"));
			chkElem->SetPosition(math::vector2d(50,250));
			chkElem->SetSize(math::vector2d(200,50));
			//rootElem->AddElement(chkElem);

			m_offsetGUI=(GUI::IGUISliderBar*)m_guiManager->CreateElement(mray::GUID("SliderBar"));
			m_offsetGUI->SetPosition(math::vector2d(50,150));
			m_offsetGUI->SetSize(math::vector2d(200,20));
			m_offsetGUI->SetDirection(GUI::ESBDir_Horizontal);
			m_offsetGUI->SetMinValue(0);
			m_offsetGUI->SetMaxValue(1);

			rootElem->AddElement(m_offsetGUI);

			if(m_perfHUD)
			{
				m_perfHUD=new GUI::GUILineStream(m_guiManager);
				m_guiManager->AddElement(m_perfHUD);
				m_perfHUD->SetPosition(math::vector2d(50,250));
				m_perfHUD->SetSize(math::vector2d(200,100));
				video::SColor perfClrs[]=
				{
					video::DefaultColors::Red,
					video::DefaultColors::Green,
					video::DefaultColors::Blue,
					video::DefaultColors::Yellow,
				};
				core::string perfTitle[]=
				{
					mT("GPU Idle"),
					mT("Pixel Shader Busy"),
					mT("Vertex Shader Busy"),
					mT("GPU Busy")
				};
				for(int i=0;i<video::EPerfHUDCounterName::ECountersCount;++i)
				{
					if(m_perfSDK->HasCounter((video::EPerfHUDCounterName)i))
					{
						m_perfHUD->AddTrace(i,perfClrs[i],perfTitle[i]);
					}
				}
				rootElem->AddElement(m_perfHUD );
			}


			m_guiManager->SetRootElement(rootElem);

			m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

		}
		if(1)
		{
			m_light=getSceneManager()->createLightNode(mT(""));

			m_light->setPosition(math::vector3d(1,10,10));

			if(1)
			{
				GCPtr<video::ITexture> texture=getDevice()->createEmptyTexture2D(true);
				texture->setMipmapsFilter(false);
				texture->SetNumberOfMipmaps(0);
				texture->createTexture(1024,video::EPixel_Float16_R);
				GCPtr<video::IRenderTarget> shadowRT=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);

				m_light->setCastShadows(false);
				m_light->setShadowMap(shadowRT);

				math::matrix4x4 m_projection=math::MathUtil::CreateProjectionMatrixPerspectiveFov(m_cameraL->getFovY(),
					m_cameraL->getAspect(),1,200);
				m_light->setProjection(m_projection);
				m_light->update(0);

			}
		}

		{

			m_atmoMap=new game::AtmosphericCubeMap();
			m_atmoMap->SetLightDirection(m_light->getDir());
			m_atmoMap->Create(640000.0,95.0213,-0.985148,638640,637800,
				math::vector3d(2.9427e-006, 9.4954e-006, 1.40597e-005),math::vector3d(9.43648e-005, 3.36762e-005, 6.59358e-006),
				512,512,0.005f,0.02f);

			GCPtr<video::ITexture> tex=m_atmoMap->GetSkyBoxTexture();// scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
			GCPtr<scene::IRenderable> sky=new scene::SSkyBoxNode(tex);//scene::SceneNodeCreator::addSkyBox(getSceneManager(),tex);
			sky->SetHasCustomRenderGroup(true);
			sky->SetTargetRenderGroup(scene::RGH_Skies);
			scene::ISceneNode*node= getSceneManager()->createSceneNode();
			node->AttachNode(sky);
			node->setCullingType(scene::SCT_NONE);
		}

		{
			loadTerrain();
		}

		
		if(1)
		{

			OS::ArchiveManager::getInstance().addArchive(mT("Jihl.zip"));
			std::vector<JobOrderPtr> jobs;
			
		//	for(int i=0;i<jobs.size();++i)
		//		JobPool::getInstance().AddRequest(jobs[i],0);
		}

		if(1)
		{
			video::GPUShaderPtr shader= new video::GPUShader();

			//shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			//shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));

			XMLSceneLoader loader(getSceneManager(),m_phManager);
			std::vector<scene::ISceneNode*> nodes;
			std::vector<physics::IPhysicalNode*> phNodes;
			loader.Load(gFileSystem.openFile(mT("testScene.xml")),0,nodes,phNodes);
			for(int i=0;i<nodes.size();++i)
			{
				scene::IRenderable* r= nodes[i]->GetAttachedNode(0);
				if(!r)
					continue;
				r->setShadowCaster(true);
				int cnt=r->getMesh()->getBuffersCount();
				for(int j=0;j<cnt;++j)
				{
					r->getMesh()->getBufferData(j)->getMaterial()->setRenderShader(0);
					r->getMesh()->getBufferData(j)->getMaterial()->SetAmbient(video::SColor(1,1,1,1));
					r->getMesh()->getBufferData(j)->getMaterial()->SetDiffuse(video::SColor(1,1,1,1));
					r->getMesh()->getBufferData(j)->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
					r->getMesh()->getBufferData(j)->getMaterial()->setRenderState(video::RS_CullFace,video::ES_DontUse);
				}
			}
		}

		m_vehicleManager=new physics::VehicleManager(m_phManager);
		m_gameManager->SetVehicleManager(m_vehicleManager);
		if(0)
		{
			m_soundManager= sound::SoundManagerFactory::getInstance().CreateObject(mT("FreeSL"));
			sound::ISound*snd=m_soundManager->loadSound(mT("sound\\afterburn.ogg"),true);
			snd->play();
		}
		if(1)
		{
			xml::XMLTree tree;
			if(tree.load(gFileSystem.openFile(mT("GameEntities.xml"))))
			{
				xml::XMLElement* e=tree.getSubElement(mT("GameEntityManager"));
				m_gameManager->loadXMLSettings(e);
			}
		}

		if(1)
		{
			m_fontShader=new video::GPUShader();
			m_fontShader->SetVertexShader(gShaderResourceManager.loadShader(mT("FontShader.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			//m_fontShader->SetFragmentShader(gShaderResourceManager.loadShader(mT("FontShader.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
		}
		if(0)
		{
			video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
//			pp->LoadXML(gFileSystem.openFile(mT("HDR.peff")));
			scene::ViewPort* vp[]={m_stereoRenderer->GetLeftVP(),m_stereoRenderer->GetRightVP()};
			for(int i=0;i<2;++i)
			{

// 				vp[i]->enablePostProcessing(true);
 				vp[i]->setPostProcessing(pp);

				video::ITexturePtr renderTargetTex=getDevice()->createTexture2D(vp[i]->getAbsViewPort().getSize(),video::EPixel_R8G8B8,true);

				video::IRenderTargetPtr rt=getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
				renderTargetTex=getDevice()->createTexture2D(vp[i]->getAbsViewPort().getSize(),video::EPixel_R8G8B8,true);
				rt->attachRenderTarget(renderTargetTex,1);

				vp[i]->setRenderTarget(rt);

			}
		}

	}
	virtual void draw(scene::ViewPort* vp)
	{
		CMRayApplication::draw(vp);
		if(!vp->getCamera())
			return;
		//getDevice()->setRenderState(video::RS_Blend,false);
		//getDevice()->useMaterial(0);
		//getDevice()->draw2DImage(math::rectf(0,0,200,200),0,video::DefaultColors::Red);
		getDevice()->clearBuffer(video::EDB_DEPTH);

		if(m_aiSystem)
			m_aiSystem->Visualize(m_renderDev);
		if(m_character)m_character->Visualize(m_debugManager);

		m_debugManager->StartDraw(vp->getCamera());
		m_debugManager->EndDraw();


		getDevice()->set2DMode();

		//m_stereoRenderer->Render();

	}
	virtual void WindowPostRender(video::RenderWindow* wnd)
	{
		getDevice()->set2DMode();
		m_stereoRenderer->Render();
		getDevice()->setRenderTarget(0);


		if(1)
		{
			getDevice()->setRenderState(video::RS_Blend,false);
			getDevice()->useShader(0);
			video::TextureUnit videoTexture;
			videoTexture.SetTexture(m_manipulator->GetPickBuffer()->GetRenderTarget()->getColorTexture(0));
			getDevice()->useTexture(0,&videoTexture);
			getDevice()->draw2DImage(math::rectf(0,0,200,200),1);
			getDevice()->setRenderState(video::RS_Blend,true);
		}

		GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
		if(font){
			video::IShaderConstantsCallback cb;
			getDevice()->useShader(m_fontShader);
			cb.setConstants(m_fontShader);

			m_guiRender->Prepare();

			float yoffset=50;

			GUI::FontAttributes attr;
			attr.fontColor.Set(0.05,1,0.5,1);
			attr.fontAligment=GUI::EFA_MiddleLeft;
			attr.fontSize=24;
			attr.hasShadow=true;
			attr.shadowColor.Set(0,0,0,1);
			attr.shadowOffset=vector2d(2);
			attr.spacing=2;
			attr.wrap=0;
			attr.RightToLeft=0;
			core::string msg=mT("FPS= ");
			msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;
			msg=mT("Primitives= ");
			msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;

			m_guiRender->Flush();
			getDevice()->useShader(0);
		}
		m_guiManager->DrawAll();
	}
	virtual void update(float dt)
	{
		CMRayApplication::update(dt);
		if(m_soundManager)m_soundManager->runSounds(dt);
		if(m_phManager)m_phManager->update(dt);
		if(m_charManager)m_charManager->Update(dt);
		m_debugManager->Update(dt);

		//if(m_theoraVideo)
		//	m_theoraVideo->update(dt);

		m_stereoRenderer->SetOffset(m_offsetGUI->GetValue());
		if(m_gameManager)
			m_gameManager->Update(dt);

		if(m_carVehicle)
		{
			m_carVehicle->Control(m_vehControling.y,false,m_vehControling.x,false,m_vehControling.z!=0);
		}
		if(m_vehicleManager)
			m_vehicleManager->Update(dt);


		m_manipulator->Update();

		if(m_perfSDK)
		{
			for(int i=0;i<video::EPerfHUDCounterName::ECountersCount;++i)
			{
				if(m_perfSDK->HasCounter((video::EPerfHUDCounterName)i))
				{
					m_perfHUD->SetTraceValue(i,m_perfSDK->GetCounterValue((video::EPerfHUDCounterName)i));
				}
			}
		}
	}

	virtual void onDone(){
		traceFunction(Application);
		return;
		std::vector<core::string> arr;
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

void test()
{

}

APPLICATION_ENTRY_POINT
{
#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif
	
	GCPtr<Application> app=new Application();
	GCPtr<Engine> engine=Engine::getInstancePtr();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.name=mT("NVPerfHUD");
	op.valueSet.insert(mT("True"));
	op.valueSet.insert(mT("False"));
	op.value=mT("False");
	extraOptions.push_back(op);
	//VLDEnable();
		app->loadResourceFile(mT("dataPath.stg"));
		if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
		{
			app->run();
		}

	//	VLDDisable();
	app=0;
	engine=0;

	GCCollector::shutdown();
}

