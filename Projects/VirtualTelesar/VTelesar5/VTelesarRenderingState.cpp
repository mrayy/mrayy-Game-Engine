

#include "stdafx.h"
#include "VTelesarRenderingState.h"
#include "Application.h"
#include <ShaderResourceManager.h>

#include <VTLib.h>
#include <CommunicationManager.h>
#include <SceneManager.h>
#include <XMLTree.h>
#include <ParsedShaderPP.h>
#include <CameraComponent.h>
#include "UDPCommunicationLayer.h"

#include "VTAppGlobals.h"
#include "DemoCamera.h"
#include "IGUIPanel.h"
#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "MotorConnection.h"

#include "IControllableComponent.h"
#include "TextureResourceManager.h"
#include "IThreadManager.h"

#include "PluginManager.h"
#include "DebugDrawManager.h"
#include "DebugRenderSettings.h"
#include "VTDebugManager.h"
#include "IGUIStackPanel.h"

#include "GameComponentCreator.h"
#include "StreamReader.h"

#include "sparticlesystem.h"
#include "ParticleEmitter.h"
#include "CPUParticleBillboardRenderer.h"
#include "FontResourceManager.h"

#include "OculusLensParams.h"
#include "OculusCameraComponent.h"
#include "VTSharedMemory.h"

//#define USE_3RD_VIEW

namespace mray
{
namespace VT
{


	class VTelesarPhysicsListener:public physics::IPhysicalManagerListener
	{
	protected:
		game::GhostProxyManager* m_ghostManager;
		uint m_autoID;
	public:

		VTelesarPhysicsListener(game::GhostProxyManager *gm)
		{
			m_autoID=0;
			m_ghostManager=gm;
		}

		virtual void OnNodeCreated(physics::IPhysicManager* c,physics::IPhysicalNode* node)
		{
			m_ghostManager->AddMasterObject(++m_autoID,node);
		}
		virtual void OnNodeRemoved(physics::IPhysicManager* c,physics::IPhysicalNode* node)
		{
			//m_ghostManager->RemoveMasterObject();
		}

	};

	class PhysicsProcessFunction:public OS::IThreadFunction
	{
	protected:
		VTelesarRenderingState* m_phManager;
	public:
		PhysicsProcessFunction(VTelesarRenderingState* m)
		{
			m_phManager=m;
		}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			float t0=gEngine.getTimer()->getSeconds();
			float acc=0;
			const float physicsUpdateSpeed=1/60.0f;
			while(caller->isActive() && !m_phManager->IsClosed())
			{
				float t1 = gEngine.getTimer()->getSeconds();
				float dt=physicsUpdateSpeed;//t1-t0;
				//printf("t0=%f,t1=%f,dt=%f",t0,t1,dt);
				t0=t1;
				acc+=dt;
				//while(acc>physicsUpdateSpeed)
				//{
					m_phManager->_Update(dt);
					acc-=physicsUpdateSpeed;
				//}
				OS::IThreadManager::getInstance().sleep(0.01);
			}
		}

	};
	class HeadMount:public scene::ISceneNode
	{
	protected:
		
		float m_speed;
		bool m_disable;
		video::OculusDevice* m_oculus;
		math::quaternion m_orientation;
	public:
		//TrackIRCamera* m_trackCam;
		void SetDisabled(bool d)
		{
			m_disable=d;
		}

		void SetOculus(video::OculusDevice* d)
		{
			m_oculus=d;
		}
		HeadMount(scene::ISceneManager*smngr,float speed):scene::ISceneNode(mT("HeadMount"),0,smngr)
		{
			m_oculus=0;
			m_speed=speed;
			m_disable=false;
		}
		virtual void update(float dt){
			ISceneNode::update(dt);
			if(!this->isVisible() || m_disable)
				return;
			math::quaternion resultQ;

			InputManager* m_input=VTAppGlobals::Instance()->App->GetInputManager();
			{

				// 			dx*=(1+2*gKeyboardController.isLShiftPress());
				// 			dz*=(1+2*gKeyboardController.isLShiftPress());

				if(m_input->getMouse()->isPressed(controllers::EMB_Right)){
					m_orientation*=math::quaternion(-80*m_input->getMouse()->getDX()*dt,math::vector3d(0,1,0));
					m_orientation*=math::quaternion(80*m_input->getMouse()->getDY()*dt,math::vector3d(1,0,0));
				}

			}
			if(m_oculus)
			{
				math::quaternion q=m_oculus->GetOrientation();
// 				q.x= q.x;
// 				q.y= q.y;
// 				q.z= q.z;
				//this->setOrintation(q);
				math::vector3d angles;
				q.toEulerAngles(angles);
				resultQ.fromEulerAngles(angles.x,-angles.y,-angles.z);
			}
			resultQ*=m_orientation;
			this->setOrintation(resultQ);
			float dx=m_speed*(m_input->getKeyboard()->getKeyState(KEY_A)-
				m_input->getKeyboard()->getKeyState(KEY_D))*dt;
			float dz=m_speed*(m_input->getKeyboard()->getKeyState(KEY_W)-
				m_input->getKeyboard()->getKeyState(KEY_S))*dt;
			this->translate(math::vector3d(dx,0,dz),scene::TS_Local);
			/*
			math::vector3d pos,rot;
			if(m_trackCam->ReceiveData(pos,rot))
			{
				rot*=90.0f/5000.0f;
				this->setOrintation(rot);
			}
			*/

		}
	};

	class UDPMessageListener:public IUDPCommunicationLayerListener
	{
	public:


		enum EUDPCommand
		{
			LoadScene1=0x10,
			LoadScene2,
			LoadScene3,
			LoadScene4,
			LoadScene5
		};
		GCPtr<OS::IMutex> dataMutex;
		std::list<EUDPCommand> commandQueue;
		VTelesarRenderingState* m_owner;
		UDPMessageListener()
		{
		}

		void Prepare()
		{
			dataMutex=OS::IThreadManager::getInstance().createMutex();
		}

		EUDPCommand PopCommand()
		{
			int i=0;
			dataMutex->lock();
			if(commandQueue.size()==0)
				i=-1;
			else
			{
				i=*commandQueue.begin();
				commandQueue.erase(commandQueue.begin());
			}
			dataMutex->unlock();
			return (EUDPCommand)i;
		}

		void OnCustomMessage(OS::IStream* stream)
		{
			OS::StreamReader r(stream);
			int i=r.binReadInt();
			dataMutex->lock();
			commandQueue.push_back((EUDPCommand)i);
			dataMutex->unlock();
		}
	}g_udpListener;

VTelesarRenderingState::VTelesarRenderingState(const core::string& name)
:IRenderingState(name)
{
	m_closed=false;
	m_collisionDebugger=new VT::ContactCollisionDebugger();
	g_udpListener.m_owner = this;
// 	m_cameraSource[0] = new CameraCorrectionGrabber(0);
// 	m_cameraSource[1] = new CameraCorrectionGrabber(1);
	m_cameraSource[0] = new video::CameraTextureSource();
	m_cameraSource[1] = new video::CameraTextureSource();
	m_previewRenderingSteps=false;
	
}
VTelesarRenderingState::~VTelesarRenderingState()
{

	m_closed=true;
	m_physicsUpdateThread->terminate();
	OS::IThreadManager::getInstance().killThread(m_physicsUpdateThread);
	m_dataRecorder=0;
	m_gameManager=0;
	m_phManager=0;
	m_sceneManager=0;
	m_telesarLayer=0;
	m_stereo=false;
	m_viewMode=StereoCamera;
	delete VT::DebugRenderSettings::DebugInterface;
	m_seeThrough = false;
	VT::ReleaseVTLib();
}

void VTelesarRenderingState::CreatePhysicsSystem()
{
	core::string maxIter=VTAppGlobals::Instance()-> GetValue("Physics","MaxIterations");
	core::string maxTimestepDiv = VTAppGlobals::Instance()->GetValue("Physics", "MaxTimeStepDiv");
	core::string simulationStepDiv = VTAppGlobals::Instance()->GetValue("Physics", "SimulationStepDiv");

	if(simulationStepDiv=="")
		m_simulationStep=1.0f/60.0f;
	else m_simulationStep=1.0f/core::StringConverter::toFloat(simulationStepDiv);

	physics::PhysicsSystemDesc desc;
	desc.gravity.set(0,-9.8*1,0);
	if(maxIter=="")
		desc.maxIter=32;
	else desc.maxIter=core::StringConverter::toInt(maxIter);

	if(maxTimestepDiv=="")
		desc.maxTimestep=1.0f/(16*60.0f);
	else desc.maxTimestep=1.0f/core::StringConverter::toFloat(maxTimestepDiv);
	desc.useFixedTimeStep=true;
	m_phManager=new physics::PhysXManager(&desc);

	//if(VTAppGlobals::IsDebugging)
		m_phManager->ConnectToRemoteDebugger();

	m_physicsUpdateThread=OS::IThreadManager::getInstance().createThread(new PhysicsProcessFunction(this));
}

void VTelesarRenderingState::_CreateGUI()
{
	GUI::IGUIPanelElement* root=new GUI::IGUIPanelElement(core::string(""),m_guiManager);
	root->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(root);

	m_connectionPanel=new GUI::GUIMotorConnectionPanel(m_guiManager);
	root->AddElement(m_connectionPanel);
	m_connectionPanel->SetPosition(math::vector2d(200,0));
	m_connectionPanel->SetSize(math::vector2d(200,300));
	m_connectionPanel->SetDocking(GUI::EED_Right);
	
	m_gyroElement=new GUI::GUIGraphPlotElement(m_guiManager);
	root->AddElement(m_gyroElement);
	m_gyroElement->SetPosition(math::vector2d(0,100));
	m_gyroElement->SetSize(math::vector2d(300,100));
	m_gyroElement->AddGraph("Contacts",video::SColor(1,0,0,1));
	m_gyroElement->AddGraph("Force",video::SColor(0,1,0,1));
	m_gyroElement->SetVisible(false);
	/*
	m_gyroElement->AddGraph("GX",video::SColor(1,0,0,1));
	m_gyroElement->AddGraph("GY",video::SColor(0,1,0,1));
	m_gyroElement->AddGraph("GZ",video::SColor(0,0,1,1));
*/
	m_gyroElement->SetMaxSamplesCount(1000);

	if(false)
	{
		VT::DebugRenderSettings::guiManager=m_guiManager;
		GUI::IGUIStackPanel* panel=(GUI::IGUIStackPanel*)m_guiManager->CreateElement(GUI::IGUIStackPanel::ElementType);
		VT::DebugRenderSettings::rootElement=panel;
		root->AddElement(panel);
		panel->SetPosition(0);
		panel->SetSize(math::vector2d(200,500));
		panel->SetStackDirection(GUI::IGUIStackPanel::EVertical);
	}

	if (!VT::VTAppGlobals::Instance()->IsDebugging)
		m_connectionPanel->SetVisible(false);

}

void VTelesarRenderingState::LoadEnvironment(const core::string& path)
{
	if(m_loaded)
		return;
	m_loaded=true;
	xml::XMLTree tree;

	if(tree.load(gFileSystem.openFile("tachilab.xml",OS::BIN_READ)) )
	{
		m_gameManager->loadXMLSettings(&tree);

	}

	core::string environmentPath=path;//VTAppGlobals::GetValue("VTelesarRenderingState","Environment");

	if(tree.load(gFileSystem.openFile(environmentPath,OS::BIN_READ)) )
	{
		m_gameManager->loadXMLSettings(&tree);

	}
	{
		const scene::ISceneManager::SceneNodeMap& nodes= m_sceneManager->GetSceneNodes();
		scene::ISceneManager::SceneNodeMap::const_iterator it= nodes.begin();
		for(;it!=nodes.end();++it)
		{
			printf("%s\n",it->first.c_str());
		}
		m_cup=0;
		it= nodes.find("Cup1");
		if(it!=nodes.end())
			m_cup=it->second;
#define ADD_OBJECT(name)\
	it= nodes.find(name);\
	if(it!=nodes.end())\
	m_viewObjects.push_back(it->second);
		for(int i=0;i<20;++i)
		{
			ADD_OBJECT("CameraView"+core::StringConverter::toString(i));
		}
		//ADD_OBJECT("LeftHand");
		//ADD_OBJECT("Head");

		m_selectedObject=0;
	}

}
void VTelesarRenderingState::_Clear()
{
	m_viewObjects.clear();
	m_gameManager->Clear();
	m_phManager->clearNodes();
	m_phManager->clearJoints();
	//m_phManager->clearMaterials();
	m_sceneManager->clearSceneNodes();
	VT::CommunicationManager::getInstance().ClearCommunicators();


}

void VTelesarRenderingState::_Reload(const core::string& scene)
{
	xml::XMLTree tree;
	m_generatingBalls=false;

	m_loaded=false;
	if (m_gameManager->loadFromFile(scene,0 ) )
	{
	}

	if(VT::CommunicationManager::getInstance().GetCommunicators().size()>0)
		m_robot=dynamic_cast<VT::RobotCommunicatorComponent*>(*VT::CommunicationManager::getInstance().GetCommunicators().begin());


	if(m_robot)
	{
		//Create dummy list of connections until we get the list of source connectors from communication layer
		const std::vector<MotorConnection*> & conn= m_robot->GetMotorConnections();
		for (int i = 0; i < conn.size();++i)
		{
			m_connectionPanel->AddConnection(conn[i]);
		}

	}
	if (true)
	{
		scene::LightNode* light= m_sceneManager->createLightNode();
		light->setPosition(math::vector3d(0,3,2));
		light->setTarget(0);
		light->setType(scene::LT_SpotLight);

		GCPtr<video::ITexture> tex=Engine::getInstance().getDevice()->createEmptyTexture2D(false);
		tex->setMipmapsFilter(false);
		tex->createTexture(math::vector3d(1024,1024,1),video::EPixel_Alpha8);//EPixel_Float16_R);
		video::IRenderTargetPtr rt=Engine::getInstance().getDevice()->createRenderTarget("",tex,0,0,0);
		light->setShadowMap(rt);
		light->setCastShadows(rt);

		math::matrix4x4 projection=math::MathUtil::CreateProjectionMatrixPerspectiveFov(45,1,1,500);

		light->setProjection(projection);
	}
	{

		m_cameraL=new scene::CameraNode(mT("CameraL"),0,m_sceneManager);//100,GetInputManager());
		m_cameraR=new scene::CameraNode(mT("CameraR"),0,m_sceneManager);//100,GetInputManager());
		m_camera3rd = new DemoCamera(m_sceneManager, 1, VTAppGlobals::Instance()->App->GetInputManager());
		m_cameraL->setZNear(0.1);
		m_cameraL->setZFar(20);
		m_cameraR->setZNear(0.1);
		m_cameraR->setZFar(20);
		m_cameraL->setFovY(62);
		m_cameraR->setFovY(62);
		m_sceneManager->addSceneNode(m_cameraL);
		m_sceneManager->addSceneNode(m_cameraR);
		m_sceneManager->addSceneNode(m_camera3rd);
		m_camera3rd->setZNear(0.1);
		m_camera3rd->setZFar(20);

		HeadMount* hm=new HeadMount(m_sceneManager,1);
		m_headMount=hm;

		m_vp[0]->setCamera(m_cameraL);
		m_vp[1]->setCamera(m_cameraR);
		m_3rdVP->setCamera(m_camera3rd);

		m_vp[0]->AddListener(this);

		m_stereoRenderer=new scene::StereoRenderer(m_vp[0],m_vp[1],hm);
		m_sceneManager->addSceneNode(hm);
		m_stereoRenderer->SetOffset(0.06);
		//m_stereoRenderer->SetMode(TBee::AppData::Instance()->stereoMode);

		m_attachToHead=false;
		_SwitchCamera(m_headMount,SwitchHead);

	}
	if(0)
	{
		scene::ISceneNode* psNode= m_sceneManager->createSceneNode("Particle System");

		scene::SParticleSystem* ps=new scene::SParticleSystem();
		psNode->AttachNode(ps);
		scene::ParticleEmitter* emitter;
		emitter=ps->CreateEmitter();
		emitter->SetRenderer(new scene::CPUParticleBillboardRenderer(1000));
		emitter->particlesRate.addKey(0,10);
		emitter->setGenerateMode(scene::EPGM_Cycle);
		emitter->setCycleLength(20);
		emitter->setParticleShapeType(scene::EPS_Billboard);
		emitter->particlesCount.addKey(0,1000);
		emitter->particlesLifeSpawn.addKey(0,10);
		emitter->setParticleVelocity(math::vector3d(10,20,0));
		emitter->setParticleAcceleration(math::vector3d(0,-5,0));
		//emitter->addAffector(scene::SParticleSystemFactory::getInstance().createParticleAffector("VelocityInterpolater"));
		emitter->setMaterial(gMaterialResourceManager.getMaterial("BaseWhiteMtrl"));
	}
	//LoadEnvironment("");


	//configure oculus
	if(false)
	{
		scene::MeshRenderableNode* node=new scene::MeshRenderableNode(0);
		scene::MeshBufferData* buff= node->getMesh()->addNewBuffer();
		scene::MeshGenerator::getInstance().generateBox(1,buff->getMeshBuffer());
		buff->setMaterial(gMaterialResourceManager.getMaterial("BaseWhiteMtrl"));
		scene::ISceneNode* snode=m_sceneManager->createSceneNode();
		snode->AttachNode(node);
	}

}
void VTelesarRenderingState::InitState()
{
	IRenderingState::InitState();
	Application* app = VTAppGlobals::Instance()->App;
	m_app=app;
	m_stereo=app->IsStereo();
	CreatePhysicsSystem();
	VT::RefVTLib();
	VT::DebugRenderSettings::DebugInterface=new VTDebugManager();

	game::GameComponentCreator::getInstance().AddAlias("CoupledJointComponent","RobotJoint");

	if (VTAppGlobals::Instance()->Physics)
	{
		game::GameComponentCreator::getInstance().AddAlias("PhysicalJointDOF6Component","PhysicalJointComponent");
		game::GameComponentCreator::getInstance().AddAlias("PhysicsComponent","RigidBodyComponent");
	}
	else
	{
		game::GameComponentCreator::getInstance().AddAlias("KinematicJointDOF6Component","PhysicalJointComponent");
		game::GameComponentCreator::getInstance().AddAlias("NullPhysicsComponent","RigidBodyComponent");
	}

	//Load Plugins
	{
		PluginManager::getInstance().loadPluginsFromDir("VTPlugins");
	}

	m_seeThrough = false;
	m_sceneManager=new scene::SceneManager(app->getDevice());
	m_guiManager=new GUI::GUIManager(app->getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());


	m_ghostProxyManager=new game::GhostProxyManager();
	if (VT::VTAppGlobals::Instance()->GhostServerIP == "")
	{
		//m_ghostProxyManager->OpenPort(1234);
	}else
	{
		//m_ghostProxyManager->Connect(VT::VTAppGlobals::GhostServerIP,1234);
	}

	//m_phManager->AddListener(new VTelesarPhysicsListener(m_ghostProxyManager));

	_CreateGUI();



	m_modelName="telesarV.xml";

	g_udpListener.Prepare();
	//VT::UDPCommunicationLayer* udpComm=new VT::UDPCommunicationLayer(1234);
	
	//udpComm->SetMsgListener(&g_udpListener);
	
	m_telesarLayer=new VT::VTSharedMemory(mT("TelesarV"));//

	CommunicationManager::getInstance().AddCommunicationLayer(mT("TelesarV"), m_telesarLayer);
	m_dataRecorder=new VT::CommunicationDataRecorder();

	m_telesarLayer->AddListener(m_connectionPanel);

	m_telesarLayer->Start();

	m_robot=0;
	m_attachToHead=false;

	m_debugRenderer=new scene::DebugDrawManager(app->getDevice());

	m_gameManager=new game::GameEntityManager();
	m_gameManager->SetPhysicsManager(m_phManager);
	m_gameManager->SetSceneManager(m_sceneManager);


 	gShaderResourceManager.parseShaderXML(gFileSystem.openFile("VT_Shaders.shd"));
 	gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile("VT_materials.mtrl"));

	{
		xml::XMLTree tree;
		if (tree.load("VTelesarSettings.xml"))
			LoadFromXML(tree.getSubElement("Settings"));
	}
	{
		HeadMount* hm=new HeadMount(m_sceneManager,1);
		m_headMount=hm;
		
		m_vp[0]=new scene::ViewPort("Left",0,0,0,math::rectf(0,0,0.5,1),0);
		m_vp[1]=new scene::ViewPort("Right",0,0,0,math::rectf(0.5,0,1,1),0);

		m_vp[0]->setOnlyToRenderTarget(true);
		m_vp[1]->setOnlyToRenderTarget(true);

		m_3rdVP=new scene::ViewPort("3rdPerson",0,0,0,math::rectf(0.05,0.05,0.2,0.2),0);;

		m_3rdVP->SetAutoUpdateRTSize(true);

		m_3rdVP->setAbsViewPort(math::rectf(10,10,150,150));

	}
	if(0)
	{
		/*
		if(VTAppGlobals::FlipCameras==false)
		{
			m_cameraSource[0]->SetCameraInfo(0,640,480,30);
			m_cameraSource[1]->SetCameraInfo(1,640,480,30);
		}else
		{
			m_cameraSource[0]->SetCameraInfo(1,640,480,30);
			m_cameraSource[1]->SetCameraInfo(0,640,480,30);
		}*/
		for(int i=0;i<2;++i)
		{
			m_cameraSource[i]->Load();
			m_cameraSource[i]->Unload();
		}
	}


	scene::ViewPort* vplist[]={m_vp[0],m_vp[1]};

	int c=app->IsStereo()?2:1;

	video::EPixelFormat pf=video::EPixel_R8G8B8A8;//video::EPixel_Float16_RGBA;//

	int pf_I = core::StringConverter::toEnum("EPixelFormat", VTAppGlobals::Instance()->GetValue("VTelesarRenderingState", "RenderPixelFormat"));
	core::string postScreenEffect = VTAppGlobals::Instance()->GetValue("VTelesarRenderingState", "PostScreenEffect");
	if(pf_I!=-1)
		pf=(video::EPixelFormat)pf_I;
	m_shaderUseLighting[0]=m_shaderUseLighting[1]=0;
	m_useCameraFusion[0]=m_useCameraFusion[1]=0;

// 	video::OculusLensParams lensParams[2];
// 
// 	lensParams[0].LensParams.set(1,0.22,0.24,0);
// 	lensParams[1].LensParams.set(1,0.22,0.24,0);
// 	lensParams[0];


	for(int i=0;i<c;++i)
	{


		video::ITexturePtr renderTargetTex=app->getDevice()->createTexture2D(math::vector2d(1,1),pf,true);
		renderTargetTex->setBilinearFilter(false);
		renderTargetTex->setTrilinearFilter(false);
		renderTargetTex->setMipmapsFilter(false);
		vplist[i]->SetClearColor(video::SColor(1,1,1,0));

		video::IRenderTargetPtr rt=app->getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
		vplist[i]->setRenderTarget(rt);
		vplist[i]->setOnlyToRenderTarget(true);

		if(postScreenEffect!="")
		{
			renderTargetTex=app->getDevice()->createTexture2D(1,pf,true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);
			rt->attachRenderTarget(renderTargetTex,1);

			video::ParsedShaderPP* pp=new video::ParsedShaderPP(app->getDevice());
			pp->LoadXML(gFileSystem.openFile(postScreenEffect));
			vplist[i]->setPostProcessing(pp);
			vplist[i]->enablePostProcessing(true);
			if(false)
			{
				renderTargetTex=app->getDevice()->createTexture2D(1,pf,true);
				renderTargetTex->setBilinearFilter(false);
				renderTargetTex->setTrilinearFilter(false);
				renderTargetTex->setMipmapsFilter(false);
				rt->attachRenderTarget(renderTargetTex,2);
			}
			m_preProcessPass[i]= (video::ParsedShaderPP::SubPostEffect*)pp->GetPass(pp->GetPassID("FXAA"));
			m_maskExtractor[i]= (video::ParsedShaderPP::SubPostEffect*)pp->GetPass(pp->GetPassID("MaskExtractor"));

			video::ParsedShaderPP::PassValue*FP=pp->GetValue("DOF.DoFPass.focalPoint");
			if(FP)
			{
				m_dofFocalPoint.push_back(FP);
				FP->floatParam[0]=0;
				FP->floatParam[1]=0;
				FP->floatParam[2]=0;
			}
			FP=pp->GetValue("CameraComposer.final.CameraTex");
			if(FP)
			{
				FP->texParam->SetTexture(gTextureResourceManager.loadTexture2D("FPV.png"));// m_cameraSource[i]->GetTexture());
			}
			FP=pp->GetValue("CameraComposer.final.UseLighting");
			if(FP)
			{
				m_shaderUseLighting[i]=FP;
			}
			m_useCameraFusion[i]=pp->GetValue("CameraComposer.final.UseCamera");
			/*
			video::ParsedShaderPP::SubPostEffect* pass= (video::ParsedShaderPP::SubPostEffect*)pp->GetPass(pp->GetPassID("DOF"));
			if(pass)
			{
				video::ParsedShaderPP::ProcessingPass* ppass= pass->GetEffect()->GetPass(pass->GetEffect()->GetPassID("DoFPass"));
				if(ppass)
				{
					video::ParsedShaderPP::PassValue*FP=ppass->GetValue("focalPoint");
					if(FP)
					{
						m_dofFocalPoint.push_back(FP);
						FP->floatParam[0]=0;
						FP->floatParam[1]=0;
						FP->floatParam[2]=0;
					}
				}
			}
			video::ParsedShaderPP::SubPostEffect* pass=(video::ParsedShaderPP::SubPostEffect*)pp->GetPass(pp->GetPassID("CameraComposer"));
			if(pass)
			{
				video::ParsedShaderPP::ProcessingPass* ppass= pass->GetEffect()->GetPass(pass->GetEffect()->GetPassID("final"));
				if(ppass)
				{
					video::ParsedShaderPP::PassValue*FP=ppass->GetValue("CameraTex");
					if(FP)
					{
						FP->texParam->SetTexture(m_cameraSource[i]->GetTexture());
					}
					FP=ppass->GetValue("UseLighting");
					if(FP)
					{
						m_shaderUseLighting[i]=FP;
					}
				}
			}*/
		}
	}

	//Setup Final rendering viewports
	{
		for(int i=0;i<c;++i)
		{

			video::ITexturePtr renderTargetTex=app->getDevice()->createTexture2D(math::vector2d(1,1),video::EPixel_R8G8B8A8,true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);

			m_finalRT[i]=app->getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
			
		}
	}

	{
		video::ITexturePtr renderTargetTex=app->getDevice()->createTexture2D(math::vector2d(1,1),video::EPixel_R8G8B8,true);
		renderTargetTex->setBilinearFilter(false);
		renderTargetTex->setTrilinearFilter(false);
		renderTargetTex->setMipmapsFilter(false);

		video::IRenderTargetPtr rt=app->getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
		m_3rdVP->setRenderTarget(rt);
		m_3rdVP->setOnlyToRenderTarget(true);
		m_3rdVP->SetClearColor(video::SColor(1,1,1,0));
	}

	_Reload(m_modelName);

	if (VTAppGlobals::Instance()->IsDebugging)
		m_collisionDebugger->Init(m_gameManager);
	//m_physicsUpdateThread->start(0);



}

void VTelesarRenderingState::_calculateFocalPoint(float dt)
{
	struct SampleStruct
	{
		math::vector2d pos;
		float w;
	};
	static const SampleStruct samples[5]=
	{
		{
			math::vector2d(0,0),0.3
		},
		{
			math::vector2d(-0.5,0),0.1
		},
		{
			math::vector2d(0.5,0),0.1
		},
		{
			math::vector2d(0,-0.5),0.2
		},
		{
			math::vector2d(0,0.5),0.1
		},
	};
	if(m_dofFocalPoint.size()>0)
	{
		float sum=0;
		math::vector3d point;
		for(int i=0;i<5;++i)
		{
			//update focal point
			math::Ray3d ray;
			physics::PhysicsRaycaseHit hit;
			
			ray.Start=m_headMount->getAbsolutePosition()+m_headMount->getAbsoluteOrintation()*math::vector3d(0,0,0.1);
			ray.Dir=m_cameraL->ScreenToWorld((samples[i].pos*0.5+0.5)*(math::vector2d)m_cameraL->getWindowDim(),1)-ray.Start;//m_headMount->getAbsoluteOrintation()*math::vector3d(samples[i].x,samples[i].y,1);
			ray.Dir.Normalize();
			if( m_phManager->RaycastClosestShape(ray,physics::EShape_All,hit)!=0)
			{
				point+=hit.worldImpact*samples[i].w;
				sum+=samples[i].w;
			}
		}
		if(sum>0)
		{
			point/=sum;
			m_currentFocalPoint+=(point-m_currentFocalPoint)*10*dt;
			for(int i=0;i<m_dofFocalPoint.size();++i)
			{
				m_dofFocalPoint[i]->floatParam[0]=m_currentFocalPoint.x;
				m_dofFocalPoint[i]->floatParam[1]=m_currentFocalPoint.y;
				m_dofFocalPoint[i]->floatParam[2]=m_currentFocalPoint.z;
			}
		}
	}
}
void VTelesarRenderingState::_SwitchCamera(scene::IMovable* m,ECameraMode mode)
{
	switch(mode)
	{
	case SwitchHead:
		{
			scene::StereoRenderer* sr=0;
			if(dynamic_cast<HeadMount*>(m))
			{
				m_attachToHead=!m_attachToHead;
				((HeadMount*)m)->SetDisabled(m_attachToHead);
				sr=m_stereoRenderer;
			}
			if(m->getParent()==0)
			{
				if(m_robot && m_robot->GetCameraComponent())
				{
					m_robot->GetCameraComponent()->MountCamera(m,sr);
				}
				//					m_head->addChild(m_headMount);
			}else
			{
				m->setPosition(m->getAbsolutePosition());
				m->setOrintation(m->getAbsoluteOrintation());
				m->removeFromParent();
			}
		}break;
	case Front:
		{
			if(dynamic_cast<HeadMount*>(m))
			{
				m_attachToHead=false;
				((HeadMount*)m)->SetDisabled(m_attachToHead);
			}
			if(m->getParent()!=0)
			{
				m->setPosition(m->getAbsolutePosition());
				m->setOrintation(math::quaternion(0,m->getAbsoluteOrintation().getYaw(),0));
				m->removeFromParent();
			}
			if(m_robot && m_robot->GetCameraComponent())
			{
				m->setPosition(m_robot->GetCameraComponent()->GetAbsolutePosition()+m_robot->GetCameraComponent()->GetAbsoluteOrientation()*math::vector3d(0,0.3,1));
				math::quaternion q( math::MathUtil::CreateLookAtMatrix(m_robot->GetCameraComponent()->GetAbsolutePosition(),m->getPosition(),math::vector3d::YAxis));
				m->setOrintation(q);
			}
			//					m_head->addChild(m_headMount);
		}break;
	case Object:
		{
			if(!m_viewObjects.size())
				break;
			m_selectedObject=(m_selectedObject+1)%m_viewObjects.size();
			scene::StereoRenderer* sr=0;
			if(dynamic_cast<HeadMount*>(m))
			{
				m_attachToHead=false;
				((HeadMount*)m)->SetDisabled(m_attachToHead);
				sr=m_stereoRenderer;
			}
			m->removeFromParent();
			//m_viewObjects[m_selectedObject]->addChild(m);
			m->setPosition(m_viewObjects[m_selectedObject]->getAbsolutePosition());
			m->setOrintation(m_viewObjects[m_selectedObject]->getAbsoluteOrintation());
		}
		break;
	}
}
void VTelesarRenderingState::OnEvent(Event* event)
{
	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent*e=(KeyboardEvent*) event;
		if(e->press)
		{
			if(e->shift)
			{
				if(e->key==KEY_1)
				{
					_Clear();
					LoadEnvironment("");
					_Reload(m_modelName);
				}else if(e->key==KEY_2)
				{
					_Clear();
					LoadEnvironment("Cups.xml");
					_Reload(m_modelName);
				}else if(e->key==KEY_3)
				{
					_Clear();
					LoadEnvironment("RodOnly.xml");
					_Reload(m_modelName);
				}else if(e->key==KEY_4)
				{
					_Clear();
					LoadEnvironment("BigObject.xml");
					_Reload(m_modelName);
				}else if(e->key==KEY_5)
				{
					_Clear();
					LoadEnvironment("Cubes.xml");
					_Reload(m_modelName);
				}

				if (e->key == KEY_S)
				{
					m_seeThrough = !m_seeThrough;
				}
			}else		
			{
				if(e->key==KEY_SPACE)
				{
					_SwitchCamera(m_headMount,SwitchHead);
				}else if(e->key==KEY_1)
				{
					_SwitchCamera(m_headMount,FreeMode);
				}else if(e->key==KEY_2)
				{
					_SwitchCamera(m_headMount,Front);
				}else if(e->key==KEY_3)
				{
					_SwitchCamera(m_headMount,Back);
				}else if(e->key==KEY_4)
				{
					_SwitchCamera(m_headMount,Side);
				}else if(e->key==KEY_5)
				{
					_SwitchCamera(m_headMount,Object);
				}else if(e->key==KEY_F)
				{
					bool fullScreen=!m_app->GetRenderWindow()->IsFullScreen();
					m_app->GetRenderWindow()->SetFullScreen(math::vector2d(1280,768),fullScreen);
					if(m_app->Get2ndRenderWindow())
						m_app->Get2ndRenderWindow()->SetFullScreen(math::vector2d(1280,768),fullScreen);
				}else if(e->key==KEY_R)
				{
					_Clear();
					_Reload(m_modelName);
				} else if(e->key==KEY_P)
				{
					m_previewRenderingSteps=!m_previewRenderingSteps;
				}
			}
			if(e->key==KEY_L)
			{
				for(int i=0;i<2;++i)
				{
					if(m_shaderUseLighting[i])
						m_shaderUseLighting[i]->floatParam[0]=1-(m_shaderUseLighting[i]->floatParam[0]);
				}
			}
			if(e->key==KEY_C)
			{
				for(int i=0;i<2;++i)
				{
					if(m_useCameraFusion[i])
						m_useCameraFusion[i]->floatParam[0]=1-(m_useCameraFusion[i]->floatParam[0]);
				}
			}
		}
		if(e->key==KEY_B)
		{
			m_generatingBalls=e->press;
		}
		if(e->press && e->key==KEY_F12)
		{time_t rawtime;
			struct tm * timeinfo;
			char buffer [80];

			time ( &rawtime );
			timeinfo = localtime ( &rawtime );

			strftime (buffer,80,"%I_%M_%S%p_",timeinfo);
			core::string name="screenShots/ScreenShotR_";
			name+=buffer;
			name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
			name+=".png";
			gTextureResourceManager.writeResourceToDist(m_finalRT[0]->GetColorTexture(0),name);
			

			name="screenShots/ScreenShot_CameraR_";
			name+=buffer;
			name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
			name+=".png";
			gTextureResourceManager.writeResourceToDist(m_cameraSource[0]->GetTexture(),name);

			name="screenShots/ScreenShot_MaskR_";
			name+=buffer;
			name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
			name+=".png";
			gTextureResourceManager.writeResourceToDist(m_maskExtractor[0]->getOutput()->GetColorTexture(0), name);
			if(m_stereo)
			{

				name="screenShots/ScreenShotL_";
				name+=buffer;
				name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
				name+=".png";
				gTextureResourceManager.writeResourceToDist(m_finalRT[1]->GetColorTexture(0),name);

				name="screenShots/ScreenShot_CameraL_";
				name+=buffer;
				name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
				name+=".png";
				gTextureResourceManager.writeResourceToDist(m_cameraSource[1]->GetTexture(),name);

				name="screenShots/ScreenShot_MaskL_";
				name+=buffer;
				name += core::StringConverter::toString(gEngine.getTimer()->getMilliseconds());
				name+=".png";
				gTextureResourceManager.writeResourceToDist(m_maskExtractor[1]->getOutput()->GetColorTexture(0),name);
			}



		}
	}
	else if(event->getType()==ET_Mouse)
	{
		MouseEvent* e=(MouseEvent*)event;
		if(e->event==MET_LEFTDOWN)
		{
#ifdef USE_3RD_VIEW
			math::vector2d pos=e->pos;
			if(m_current3rdVPSize.IsPointInside(pos))
			{
				if(m_viewMode==StereoCamera)
					m_viewMode=ThirdCamera;
				else {
					m_viewMode=StereoCamera;
					((DemoCamera*)m_camera3rd.pointer())->disableInput=true;
				}

				m_vpUpdatetimer=0;
				e->setRecived();
			}
#endif
		}
	}

	m_guiManager->OnEvent(event);
}
void VTelesarRenderingState::OnEnter(IRenderingState*prev)
{
	m_firstrender=true;
	m_vpUpdatetimer=0;
	m_viewMode=StereoCamera;
	if(!m_cameraSource[0]->IsLoaded())
		m_cameraSource[0]->Load();
	if(m_stereo && !m_cameraSource[1]->IsLoaded())
		m_cameraSource[1]->Load();
	if(!m_attachToHead)
		_SwitchCamera(m_headMount,SwitchHead);
	((DemoCamera*)m_camera3rd.pointer())->disableInput=true;
}
void VTelesarRenderingState::OnExit()
{
	IRenderingState::OnExit();
	m_cameraSource[0]->Unload();
	if(m_stereo)
		m_cameraSource[1]->Unload();
}
void VTelesarRenderingState::Update(float dt)
{
	m_gameManager->PreUpdate();
	//printf("%d\n",i);
	_Update(dt);
	m_sceneManager->update(dt);
	m_guiManager->Update(dt);
	m_firstrender=true;

	math::rectf targetRect(0,m_finalRT[0]->GetSize());

	if(m_viewMode==ThirdCamera)
	{
		if(m_vpUpdatetimer!=1)
		{
			m_vpUpdatetimer+=10*dt;
			if(m_vpUpdatetimer>=1)
			{
				((DemoCamera*)m_camera3rd.pointer())->disableInput=false;
				m_vpUpdatetimer=1;
			}
			m_current3rdVPSize.ULPoint=math::lerp<math::vector2d>(m_current3rdVPSize.ULPoint,targetRect.ULPoint,m_vpUpdatetimer);
			m_current3rdVPSize.BRPoint=math::lerp<math::vector2d>(m_current3rdVPSize.BRPoint,targetRect.BRPoint,m_vpUpdatetimer);
		}
	}else
	{
		if(m_vpUpdatetimer!=1)
		{
			m_vpUpdatetimer+=10*dt;
			if(m_vpUpdatetimer>=1)
				m_vpUpdatetimer=1;
			m_current3rdVPSize.ULPoint=math::lerp<math::vector2d>(m_current3rdVPSize.ULPoint,10,m_vpUpdatetimer);
			m_current3rdVPSize.BRPoint=math::lerp<math::vector2d>(m_current3rdVPSize.BRPoint,((math::vector2d)targetRect.BRPoint)/4,m_vpUpdatetimer);
		}
	}


	m_cameraSource[0]->Update(dt);
	if(m_stereo)
		m_cameraSource[1]->Update(dt);

	m_gameManager->Update(dt);
	m_telesarLayer->Update(dt);

	//if(VTAppGlobals::IsDebugging)
		m_debugRenderer->Update(dt);

//	m_ghostProxyManager->Update();

		if (VTAppGlobals::Instance()->IsDebugging)
	{
		m_gyroElement->AddValue("Force",m_collisionDebugger->GetAverageForce());
		m_gyroElement->AddValue("Contacts",m_collisionDebugger->GetLastContactCount());
		m_collisionDebugger->Update(dt);
	}

	_calculateFocalPoint(dt);

	if(m_generatingBalls)
	{
		if(m_cup)
		{
			m_ballCreator.AutoGenerate(dt,m_gameManager,0.004+math::Randomizer::rand01()*0.003,m_cup->getAbsolutePosition()+m_cup->getAbsoluteOrintation()*math::vector3d(0,0.3,0));
		}
	}
	{
		int command=-1;
		if((command=g_udpListener.PopCommand())!=-1)
		{

			switch(command)
			{
			case UDPMessageListener::LoadScene1:
				_Clear();
				LoadEnvironment("Cups.xml");
				_Reload(m_modelName);
				break;
			case UDPMessageListener::LoadScene2:
				_Clear();
				LoadEnvironment("Cubes.xml");
				_Reload(m_modelName);
				break;
			case UDPMessageListener::LoadScene3:
				_Clear();
				LoadEnvironment("Sphere.xml");
				_Reload(m_modelName);
				break;
			case UDPMessageListener::LoadScene4:
				_Clear();
				LoadEnvironment("BigObject.xml");
				_Reload(m_modelName);
				break;
			}
		}
	}
}
void VTelesarRenderingState::_Update(float dt)
{


	static float acc=0;
	acc+=dt;
	int i=0;
	//while(acc>=m_simulationStep )
	{
		m_phManager->update(dt*1);//m_simulationStep);
		//acc-=m_simulationStep;
		//++i;
	}
}

video::ITexture* VTelesarRenderingState::_RenderRobotVision(TBee::ETargetEye eye)
{
	int index = GetEyeIndex(eye);
	/*
	if(0 && rc.getSize()!=m_vp[index]->getSize())
	{
		m_vp[index]->setAbsViewPort(math::rectf(0,rc.getSize()));
		video::IRenderTarget *rt= m_vp[index]->getRenderTarget();
		for(int i=0;i<8;++i)
		{
			video::ITexture* tex= rt->GetColorTexture(i);
			//if(tex)
			//	tex->createTexture(math::vector3d(rc.getWidth(),rc.getHeight(),1),tex->getImageFormat());
		}
	}*/
	if(!(m_viewMode==ThirdCamera && m_vpUpdatetimer==1))
	{
		m_vp[index]->draw();
		return m_vp[index]->GetColorTexture();
	}
	return 0;

}
video::IRenderTarget* VTelesarRenderingState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	
	int index = GetEyeIndex(eye);
	if(!m_stereo)
		index=0;
	if( rc.getSize()!=m_finalRT[index]->GetSize())
	{
		m_finalRT[index]->Resize(rc.getWidth(),rc.getHeight());
	}

	scene::ViewPort* vp=Engine::getInstance().getDevice()->getViewport();
	video::IVideoDevice* device=Engine::getInstance().getDevice();

	//m_cameraSource[index]->SetTargetRect(rc.getSize());

	if (m_firstrender && VT::VTAppGlobals::Instance()->IsDebugging)
	{
		m_3rdVP->setAbsViewPort(math::rectf(0,m_current3rdVPSize.getSize()));
		m_3rdVP->draw();
		m_firstrender=false;
	}
	device->setViewport(vp);

	{
		video::ITexture* roboVis = 0;
		if (!m_seeThrough)
			roboVis = _RenderRobotVision(eye);
		device->setRenderTarget(m_finalRT[index],true,true,video::SColor(1,1,1,0));
		device->useShader(0);
		video::TextureUnit tex;

		if (m_seeThrough)
		{
		/*	if (m_oculusDevice->IsConnected())
			{
				math::matrix4x4 m, pm;
				device->getTransformationState(video::TS_PROJECTION, m);
				pm.f14 = m_oculusComponents[index]->GetPerspectiveOffset();
				pm = pm*m;
				device->setTransformationState(video::TS_PROJECTION, pm);
			}*/
			tex.SetTexture(m_cameraSource[index]->GetTexture());
			device->useTexture(0, &tex);
			device->draw2DImage(rc, video::DefaultColors::White);
		}
		else
		{

			device->set2DMode();
			if (!m_previewRenderingSteps)
			{

				if (roboVis)
				{
					tex.SetTexture(roboVis);
					device->useTexture(0, &tex);
					device->draw2DImage(rc, video::DefaultColors::White);
				}

				if (VT::VTAppGlobals::Instance()->IsDebugging)
				{
					if (m_3rdVP->getRenderTarget())
						tex.SetTexture(m_3rdVP->getRenderTarget()->GetColorTexture());
					else tex.SetTexture(0);
					math::rectf rc(m_current3rdVPSize);
					device->useTexture(0, &tex);
					device->draw2DImage(rc, video::DefaultColors::White);
				}
			}
			else 
			{
			/*	if (m_oculusDevice && m_oculusDevice->IsConnected())
				{
					math::matrix4x4 m, pm;
					device->getTransformationState(video::TS_PROJECTION, m);
					pm.f14 = m_oculusComponents[index]->GetPerspectiveOffset();
					pm = pm*m;
					device->setTransformationState(video::TS_PROJECTION, pm);
				}*/
				video::ITexturePtr textures[] = { m_cameraSource[index]->GetTexture(),
					m_maskExtractor[index]->getOutput()->GetColorTexture(0),
					m_maskExtractor[index]->getOutput()->GetColorTexture(1),
					roboVis };
				bool flipTex[] = { 0,0,0, 0 };
				core::string title[] = { "Camera Image", "Mask Image", "Color Correction", "Fused Image" };
				GUI::IFont* font = gFontResourceManager.getDefaultFont();
				GUI::FontAttributes attrs;
				attrs.fontSize = 24;
				attrs.fontColor.Set(1, 1, 1, 1);
				attrs.fontAligment = GUI::EFA_TopLeft;

				int r = 2;
				int c = 2;
				math::vector2d sz = rc.getSize();
				sz.x /= c;
				sz.y /= r;

				math::rectf pRC;

				for (int i = 0; i < r; ++i)
				{
					pRC.ULPoint.y = i*sz.y;
					for (int j = 0; j < c; ++j)
					{
						pRC.ULPoint.x = j*sz.x;
						pRC.BRPoint = pRC.ULPoint + sz;
						int idx = (i*c) + j;
						tex.SetTexture(textures[idx]);
						device->useTexture(0, &tex);
						device->draw2DImage(pRC, video::DefaultColors::White, 0, flipTex[idx] ? &math::rectf(0, 1, 1, 0) : 0);
						font->print(pRC, &attrs, 0, title[idx], m_guiManager->GetRenderQueue());
					}
				}
				m_guiManager->GetRenderQueue()->Flush();

			}
		}
		if(false)
		{
			math::vector2d points[4]=
			{
				math::vector2d(rc.ULPoint.x,rc.ULPoint.y),
				math::vector2d(rc.BRPoint.x,rc.ULPoint.y),
				math::vector2d(rc.BRPoint.x,rc.BRPoint.y),
				math::vector2d(rc.ULPoint.x,rc.BRPoint.y)
			};
			video::SColor clr[4]=
			{
				video::SColor(1,1,1,0),
				video::SColor(1,1,1,0),
				video::SColor(1,1,1,0.5),
				video::SColor(1,1,1,0),
			};
			device->draw2DShape(points,clr,4,true);
			device->draw2DRectangle(rc,video::DefaultColors::Black,false);
		}


		//m_guiManager->DrawAll(m_finalRT[index]);

		if (VTAppGlobals::Instance()->IsDebugging)
			((VTDebugManager*)VT::DebugRenderSettings::DebugInterface)->Render(rc);
		
		if(dynamic_cast<VT::VTSharedMemory*>(m_telesarLayer.pointer())!=0 && false)
		{
			GUI::IFont* font= gFontResourceManager.getDefaultFont();
			GUI::FontAttributes attrs;
			attrs.fontSize=40;
			attrs.fontColor.Set(1,1,1,1);
			attrs.fontAligment=GUI::EFA_MiddleCenter;
			std::vector<core::string> lst;
			((VT::VTSharedMemory*)m_telesarLayer.pointer())->GetDebugString(lst);
			math::rectf r=rc;
			for(int i=0;i<lst.size();++i)
			{
				font->print(r,&attrs,0,lst[i],m_guiManager->GetRenderQueue());
				r.ULPoint.y+=attrs.fontSize;
			}
			m_guiManager->GetRenderQueue()->Flush();
		}
		device->setRenderTarget(0, false, false);/*
		if (m_oculusDevice && m_oculusDevice->IsConnected())
		{
			m_oculusRenderer[index]->Setup(math::rectf(0, 0, m_finalRT[index]->GetSize().x, m_finalRT[index]->GetSize().y));
			m_oculusRenderer[index]->render(m_finalRT[index]);
		}*/

	}
	

/*	if (m_oculusDevice && m_oculusDevice->IsConnected())
	{
		return (video::IRenderTarget*)m_oculusRenderer[index]->getOutput();
	}
	else*/
	{
		return m_finalRT[index];
	}
}
video::IRenderTarget* VTelesarRenderingState::GetLastFrame(bool left)
{
	int index=m_app->getEyeIndex(left);

	/*if (m_oculusDevice && m_oculusDevice->IsConnected())
	{
		return (video::IRenderTarget*)m_oculusRenderer[index]->getOutput();
	}
	else*/
	{
		return m_finalRT[index];
	}
}

void VTelesarRenderingState::LoadFromXML(xml::XMLElement* e)
{
	IRenderingState::LoadFromXML(e);
	xml::XMLAttribute* attr;

	m_cameraSource[0]->LoadFromXML(e->getSubElement("CameraLeft"));
	m_cameraSource[1]->LoadFromXML(e->getSubElement("CameraRight"));
}

void VTelesarRenderingState::onRenderDone(scene::ViewPort*vp)
{
	if (!VTAppGlobals::Instance()->IsDebugging)
		return;
	m_gameManager->DebugRender(m_debugRenderer);
	m_debugRenderer->StartDraw(vp->getCamera());
	if (VTAppGlobals::Instance()->IsDebugging)
		m_collisionDebugger->Draw(vp->getCamera());
	m_debugRenderer->EndDraw();
}
}
}
