

#include "stdafx.h"
#include "VApplication.h"


#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "TextureResourceManager.h"
#include "CommunicationManager.h"

#include "XMLTree.h"

#include "ICommunicatorComponent.h"
#include "CommunicationData.h"
#include "CommunicatorScheme.h"
#include "VTLib.h"
#include "SceneComponent.h"
#include "ShaderResourceManager.h"

#include "ParsedShaderPP.h"
#include "DebugDrawManager.h"
#include "Noise1D.h"
#include "IGUIPanel.h"
#include "ISoundListener.h"
#include "ISound.h"

#include "IVTComponent.h"



#include "MeshResourceManager.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "MeshBufferData.h"
#include "CameraComponent.h"

namespace mray
{

	class MotorSound:public VT::IVTComponent
	{
	protected:
		sound::ISoundPtr m_sound;
		float m_currentV;
		bool m_playing;
		float m_freq;
	public:
		MotorSound(game::GameEntityManager* mngr)
		{
		}
		MotorSound(sound::ISoundPtr snd):m_sound(snd)
		{
			m_currentV=0;
			m_playing=false;
			m_freq=0;
		}

		void Play()
		{
			//m_currentV=0;
			m_playing=true;
			//m_sound->setPitch(m_currentV);
			m_sound->play();
			if(m_freq==0)
				m_freq=m_sound->getPitch();
		}
		void Stop()
		{
			m_playing=false;
		}

		void Update(float dt)
		{
			if(!m_playing )
			{
				if( m_sound->isPlaying())
				{
					m_currentV-=10*dt;
					m_sound->setPitch(m_currentV*m_freq);
					if(m_currentV<=0)
					{
						m_sound->stop();
					}
				}
			}else
			{
				if(m_currentV<1)
				{
					m_currentV+=10*dt;
					m_currentV=math::Min(m_currentV,1.0f);
					m_sound->setPitch(m_currentV*m_freq);
				}
			}
		}
	};

	class GraphPlotListener:public VT::ICommunicationLayerListener
	{
	public:
		GUI::GUIGraphPlotElement* graphP;
		GraphPlotListener()
		{
			graphP=0;
		}

		virtual void OnDataArrived(VT::ICommunicationLayer* layer,const VT::CommunicationData*data)
		{
			if(!graphP)
				return;

			const std::list<VT::CommunicationData::DataPair>&lst=data->GetData();
			std::list<VT::CommunicationData::DataPair>::const_iterator it=lst.begin();
			for(;it!=lst.end();++it)
			{
				if(!graphP->HasGraph((*it).name))
					graphP->AddGraph((*it).name,video::SColor(math::Randomizer::rand01(),math::Randomizer::rand01(),math::Randomizer::rand01(),1));
				graphP->AddValue((*it).name,(*it).values[0]);
			}
		}

	}g_graphPlotListener;

	class HeadMount:public scene::ISceneNode
	{
	protected:
		InputManager* m_input;
		float m_speed;
		bool m_disable;
	public:
		void SetDisabled(bool d)
		{
			m_disable=d;
		}
		HeadMount(scene::ISceneManager*smngr,float speed,InputManager* kb):scene::ISceneNode(mT("HeadMount"),0,smngr)
		{
			m_speed=speed;
			m_input=kb;
			m_disable=false;
		}
		virtual void update(float dt){
			ISceneNode::update(dt);
			if(!this->isVisible() || m_disable)
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
		}
	};
	void SendData(float angle)
	{

		const VT::CommunicationManager::CommunicatorList& lst=VT::CommunicationManager::getInstance().GetCommunicators();
		VT::CommunicationManager::CommunicatorList::const_iterator it=lst.begin();
		VT::CommunicatorScheme* scheme=0;
		VT::CommunicationData data;
		for(;it!=lst.end();++it)
		{
			scheme=(*it)->GetScheme();
			data.SetTargetName(scheme->GetName());

			const VT::CommunicatorScheme::ItemsList& itmLst=scheme->GetComponents();

			data.ClearData();

			VT::CommunicatorScheme::ItemsList::const_iterator it2=itmLst.begin();
			for(;it2!=itmLst.end();++it2)
				data.AddData((*it2)->name,&angle,1);

			//VT::CommunicationManager::getInstance().OnDataArrived(0,&data);

		}
	}

	video::ParsedShaderPP* s_ppShader=0;
	VT::CommunicatorScheme* scheme=0;
	VApplication::VApplication()
	{
		m_2ndWnd=0;
	}
	VApplication::~VApplication()
	{
		if(m_2ndWnd)
		{
			m_2ndWnd->Destroy();
		}
	}

	void VApplication::onEvent(Event* event)
	{
		CMRayApplication::onEvent(event);


		if(event->getType()==ET_ResizeEvent)
		{
			ResizeEvent* e=(ResizeEvent*)event;
		}
		if(event->getType()==ET_Mouse)
		{
			MouseEvent* e=(MouseEvent*)event;
		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent*e=(KeyboardEvent*) event;
			if(e->press)
			{
				if(e->key==KEY_SPACE)
				{
					_SwitchCamera(SwitchHead);
				}else if(e->key==KEY_1)
				{
					_SwitchCamera(FreeMode);
				}else if(e->key==KEY_2)
				{
					_SwitchCamera(Front);
				}else if(e->key==KEY_3)
				{
					_SwitchCamera(Back);
				}else if(e->key==KEY_4)
				{
					_SwitchCamera(Side);
				}else if(e->key==KEY_5)
				{
					_SwitchCamera(HeadFront);
				}else if(e->key==KEY_F)
				{
					GetRenderWindow()->SetFullScreen(math::vector2d(1024,768),!GetRenderWindow()->IsFullScreen());
				}
				else if(e->key==KEY_P)
				{
					if(!m_isDebugging)
					{
						for(int i=0;i<6;++i)
						{
							m_sensorData[i]->SetVisible(!m_sensorData[i]->IsVisible());
						}
					}
				}else if(e->key==KEY_F12)
				{
					
					video::LockedPixelBox box=m_screenShot->getSurface(0)->lock(math::box3d(0,0,0,GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::IHardwareBuffer::ELO_Discard);

					GetRenderWindow()->TakeScreenShot(box);
					m_screenShot->getSurface(0)->unlock();

					gTextureResourceManager.writeResourceToDist(m_screenShot,"screenShot.tga");
				}
			}
			if(e->key==KEY_L && 0)
			{
				if(e->press)
					((MotorSound*)m_motor.pointer())->Play();
				else
					((MotorSound*)m_motor.pointer())->Stop();
			}
			if(e->key==KEY_U && e->press)
			{
				m_drawUI=!m_drawUI;
			}
			if(e->key==KEY_Y && e->press)
			{
				if(s_ppShader)
					s_ppShader->setEnable(!s_ppShader->isEnabled());
			}
		}

	}
	void VApplication::_SwitchCamera(ECameraMode mode)
	{
		switch(mode)
		{
		case SwitchHead:
			{
				m_attachToHead=!m_attachToHead;
				((HeadMount*)m_headMount)->SetDisabled(m_attachToHead);
				if(m_attachToHead)
				{
// 					m_headMount->setPosition(math::vector3d(0,0.10,0.10));
// 					m_headMount->setOrintation(math::quaternion(0,math::vector3d::XAxis));
					if(m_robot && m_robot->GetCameraComponent())
					{
						m_robot->GetCameraComponent()->MountCamera(m_headMount,m_stereoRenderer);
					}
//					m_head->addChild(m_headMount);
				}else
				{
					m_headMount->setPosition(m_headMount->getAbsolutePosition());
					m_headMount->setOrintation(m_headMount->getAbsoluteOrintation());
					m_headMount->removeFromParent();
				}
			}break;/*
		case FreeMode:
			{
				((HeadMount*)m_headMount)->SetDisabled(false);
				if(m_attachToHead)
				{
					m_headMount->setPosition(m_headMount->getAbsolutePosition());
					m_headMount->setOrintation(m_headMount->getAbsoluteOrintation());
					m_head->removeChild(m_headMount);
				}
				m_attachToHead=false;
			}break;
		case Front:
			{
				((HeadMount*)m_headMount)->SetDisabled(true);
				if(m_attachToHead)
					m_head->removeChild(m_headMount);
				m_attachToHead=false;
				//front camera
				m_headMount->setPosition(math::vector3d(0,1.1,1));
				m_headMount->setOrintation(math::quaternion(180,math::vector3d::YAxis));
			}break;
		case Back:
			{
				((HeadMount*)m_headMount)->SetDisabled(true);
				if(m_attachToHead)
					m_head->removeChild(m_headMount);
				m_attachToHead=false;
				//back camera
				m_headMount->setPosition(math::vector3d(0,1,-1));
				m_headMount->setOrintation(math::quaternion(0,math::vector3d::YAxis));
			}break;
		case Side:
			{
				((HeadMount*)m_headMount)->SetDisabled(true);
				if(m_attachToHead)
					m_head->removeChild(m_headMount);
				m_attachToHead=false;
				//side camera
				m_headMount->setPosition(math::vector3d(1,1.1,0));
				m_headMount->setOrintation(math::quaternion(-90,math::vector3d::YAxis));
			}break;
		case HeadFront:
			{
				((HeadMount*)m_headMount)->SetDisabled(true);
				if(!m_attachToHead)
					m_head->addChild(m_headMount);
				m_headMount->setPosition(math::vector3d(0,-1,0.1));
				m_headMount->setOrintation(math::quaternion(180,math::vector3d::YAxis)*math::quaternion(-70,math::vector3d::XAxis));
				m_attachToHead=true;
			}break;*/
		}

	}
	void VApplication::init(const OptionContainer &extraOptions)
	{
		CMRayApplication::init(extraOptions);

		{

			core::string v=extraOptions.GetOptionValue("Debugging");
			if(v=="Yes")
				m_isDebugging=true;
			else
				m_isDebugging=false;
		}
		for(int i=0;i<10;++i)
			m_angles[i]=0;

		CMRayApplication::loadResourceFile(mT("Resources.stg"));
		_InitResources();
		CreatePhysicsSystem();

		{

			core::string v=extraOptions.GetOptionValue("Shaders");
			if(v=="Yes")
			{
				gShaderResourceManager.parseShaderXML(gFileSystem.openFile("VT_Shaders.shd"));
				gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile("VT_materials.mtrl"));
			}else
			{
				gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile("VT_materials_noShaders.mtrl"));
			}
		}
		VT::InitVTLib();
		m_debugRenderer=new scene::DebugDrawManager(getDevice());

		m_telesarLayer=new VT::VTSharedMemory(mT("TelesarV"));
		m_dataRecorder=new VT::CommunicationDataRecorder();
		m_robotCommunicator=new VT::RobotCommunicator();
		m_robotCommunicator->SetCommunicatorLayer(m_telesarLayer);


		m_robotCommunicator->AddConnection("JointBody1","JointBody1");
		m_robotCommunicator->AddConnection("JointBody2","JointBody2");
		m_robotCommunicator->AddConnection("JointBody3","JointBody3");
		m_robotCommunicator->AddConnection("JointBody4","JointBody4");
		m_robotCommunicator->AddConnection("JointBody5","JointBody5");

		VT::CommunicationManager::getInstance().AddRobotCommunication(m_robotCommunicator);
		m_telesarLayer->Start();
		m_telesarLayer->AddListener(&g_graphPlotListener);

		//m_telesarLayer->AddListener(m_dataRecorder);

		m_robot=0;
		m_attachToHead=false;

		m_drawUI=true;

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());


		m_guiManager=new GUI::GUIManager(getDevice());
		m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
		{

			GUI::IGUIPanelElement* panel=new GUI::IGUIPanelElement(GUID(""),m_guiManager);;
			m_guiManager->SetRootElement(panel);
			panel->SetSize(GetRenderWindow()->GetSize());

			m_graphGUI=new GUI::GUIGraphPlotElement(m_guiManager);
			g_graphPlotListener.graphP=m_graphGUI;
			panel->AddElement(m_graphGUI);
			m_graphGUI->SetSize(math::vector2d(600,240));

			m_graphGUI->SetMaxSamplesCount(1000);

			m_graphGUI->SetVisible(m_isDebugging);

			float offsetY=0;

			for(int i=0;i<3;++i)
			{
				m_sensorData[i]=new GUI::GUIGraphPlotElement(m_guiManager);
				panel->AddElement(m_sensorData[i]);
				m_sensorData[i]->SetPosition(math::vector2d(0,offsetY));
				m_sensorData[i]->SetSize(math::vector2d(200,100));
				m_sensorData[i]->SetMaxSamplesCount(1000);
				m_sensorData[i]->AddGraph(mT("Force"),video::SColor(1,0,0,1));
				m_sensorData[i]->AddGraph(mT("Temperature"),video::SColor(0,0,1,1));
				m_sensorData[i]->SetHorizontalAlignment(GUI::EHA_Left);

				m_sensorData[i]->SetVisible(!m_isDebugging);
				offsetY+=100;
			}
			offsetY=0;
			for(int i=3;i<6;++i)
			{
				m_sensorData[i]=new GUI::GUIGraphPlotElement(m_guiManager);
				panel->AddElement(m_sensorData[i]);
				m_sensorData[i]->SetPosition(math::vector2d(200,offsetY));
				m_sensorData[i]->SetSize(math::vector2d(200,100));
				m_sensorData[i]->SetMaxSamplesCount(1000);
				m_sensorData[i]->AddGraph(mT("Force"),video::SColor(1,0,0,1));
				m_sensorData[i]->AddGraph(mT("Temperature"),video::SColor(0,0,1,1));
				m_sensorData[i]->SetHorizontalAlignment(GUI::EHA_Right);

				m_sensorData[i]->SetVisible(!m_isDebugging);
				offsetY+=100;
			}
		}

		if(0)
		{
			 m_soundManager=sound::SoundManagerFactory::getInstance().CreateObject("FMod");
		}

		m_gameManager=new game::GameEntityManager();
		m_gameManager->SetPhysicsManager(m_phManager);
		m_gameManager->SetSceneManager(getSceneManager());


		m_screenShot=getDevice()->createEmptyTexture2D(true);
		m_screenShot->setMipmapsFilter(false);
		m_screenShot->createTexture(math::vector3d(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::EPixel_R8G8B8A8);
		if(0)
		{
			physics::IPhysicalCloth* cloth;
			physics::PhysicalClothDesc desc;

			desc.globalPos.setTranslation(math::vector3d(0,4,0.4));

			desc.bendingStiffness=0.5;
			desc.stretchingStiffness=0.5;
			desc.thickness=0.02;
			desc.friction=0.5;
			desc.tearFactor=1.5;
			desc.pressure=1;
			//	desc.flags[physics::ECF_Bending]=true;
			//	desc.flags[physics::ECF_Terrable]=true;
			desc.flags[physics::ECF_Pressure]=true;
			desc.flags[physics::ECF_CollisionTwoWay]=true;
			GCPtr<scene::SMesh> m=gMeshResourceManager.loadMesh("Head.mdl",true);
			if(m)
				desc.meshData=m->getBuffer(0);

			cloth=m_phManager->createCloth(&desc);


			scene::SMesh* mesh=new scene::SMesh();
			mesh->addBuffer(cloth->GetRenderBuffer());

			GCPtr<video::RenderMaterial> mtrl=new video::RenderMaterial();
			video::RenderPass* pass= mtrl->CreateTechnique("")->CreatePass("");
			pass->SetAmbient(1);
			pass->SetDiffuse(1);
			pass->SetEmissive(0);
			pass->setRenderState(video::RS_CullFace,video::ES_DontUse);
			pass->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
			pass->setTexture(gTextureResourceManager.loadTexture2D(mT("FieldstoneBumpDOT3.tga")),1);
 			video::GPUShader*shader= new video::GPUShader();
 
 			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("RobotShaderGBuff.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
 			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("RobotShaderGBuff.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
 			pass->setRenderShader(shader);
			mesh->getBufferData(0)->setMaterial(mtrl);


			scene::MeshRenderableNode*r=new scene::MeshRenderableNode(mesh);
			scene::ISceneNode* node=getSceneManager()->createSceneNode();
			node->AttachNode(r);
			r->setShadowCaster(true);

			//cloth->AttachNode(r,0);
		}

		if(1)
		{
			GCPtr<scene::LightNode> m_light=getSceneManager()->createLightNode(mT(""));

			m_light->setRadius(1000);
			m_light->setDiffuse(video::SColor(0.7,0.7,0.7,1));
			m_light->setAmbient(video::SColor(0.5,0.5,0.5,1));
			m_light->setType(scene::LT_PointLight);
			m_light->setPosition(math::vector3d(2,4,5));
			m_light->setOrintation(math::quaternion(-10,math::vector3d::XAxis));
			GCPtr<video::ITexture> texture=getDevice()->createEmptyTexture2D(true);
			m_screenShot=texture;
			texture->setMipmapsFilter(false);
			texture->SetNumberOfMipmaps(0);
			texture->createTexture(2048,video::EPixel_Float16_R);
			GCPtr<video::IRenderTarget> shadowRT=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);

			m_light->setCastShadows(true);
			m_light->setShadowMap(shadowRT);

			math::matrix4x4 m_projection=math::MathUtil::CreateProjectionMatrixOrtho(5,5,5,5,0.1,20);//math::MathUtil::CreateProjectionMatrixPerspectiveFov(90,1,1,20);
			m_light->setProjection(m_projection);
			m_light->update(0);

			//generate a cool skybox!
			if(1)
			{
				m_atmosphericCubeMap=new game::AtmosphericCubeMap();
				//m_atmosphericCubeMap->SetThreadManager(m_appThreadManager);
				m_atmosphericCubeMap->SetLightDirection(m_light->getDir());
				m_atmosphericCubeMap->Create(640000.0,95.0213,-0.985148,638640,637800,
					math::vector3d(2.9427e-006, 9.4954e-006, 1.40597e-005),math::vector3d(9.43648e-005, 3.36762e-005, 6.59358e-006),
					256,256,0.01f,0.02f);

				GCPtr<video::ITexture> tex= m_atmosphericCubeMap->GetSkyBoxTexture();//scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
				GCPtr<scene::IRenderable> sky=new scene::SSkyBoxNode(tex);//scene::SceneNodeCreator::addSkyBox(getSceneManager(),tex);
				sky->SetHasCustomRenderGroup(true);
				sky->SetTargetRenderGroup(scene::RGH_Skies);
				scene::ISceneNode*node= getSceneManager()->createSceneNode("Skybox");
				node->AttachNode(sky);
				node->setCullingType(scene::SCT_NONE);
			}
		}
		xml::XMLTree tree;

		if(tree.load(gFileSystem.openFile("Tachilab_.xml",OS::BIN_READ)) )
		{
			m_gameManager->loadXMLSettings(&tree);

		}
		if(tree.load(gFileSystem.openFile("TelesarV.xml",OS::BIN_READ)) )
		{
			m_gameManager->loadXMLSettings(&tree);
/*
			game::GameEntity* owner= VT::CommunicationManager::getInstance().GetCommunicator("TelesarV")->GetOwnerEntity();
			const std::list<IObjectComponent*>& lst=owner->GetComponent(game::SceneComponent::getClassRTTI());
			std::list<IObjectComponent*>::const_iterator it=lst.begin();
			for (;it!=lst.end();++it)
			{
				game::SceneComponent* comp=(game::SceneComponent*)*it;
				if(comp->GetName()=="Head")
				{
					m_head=comp->GetSceneNode();
					break;
				}
			}*/
		}

		if(VT::CommunicationManager::getInstance().GetCommunicators().size()>0)
			m_robot=dynamic_cast<VT::RobotCommunicatorComponent*>(*VT::CommunicationManager::getInstance().GetCommunicators().begin());

		std::vector<scene::ViewPort*> vpList;
		{
			scene::ViewPort* lVP,*rVP;
			//Stereoscopic
			m_cameraL=new scene::CameraNode(mT("CameraL"),0,getSceneManager());//100,GetInputManager());
			m_cameraR=new scene::CameraNode(mT("CameraR"),0,getSceneManager());//100,GetInputManager());
			m_cameraL->setZNear(0.1);
			m_cameraL->setZFar(100);
			m_cameraR->setZNear(0.1);
			m_cameraR->setZFar(100);
			lVP=GetRenderWindow(0)->CreateViewport(mT("Left"),m_cameraL,0,math::rectf(0,0,1,1),0);
			core::string v=extraOptions.GetOptionValue("Stereoscopic");
			if(v=="DVI")
			{
				OptionContainer opt=extraOptions;
				opt["Monitor"].value="1";
				m_2ndWnd= getDevice()->CreateRenderWindow(mT("Secondary"),GetRenderWindow()->GetSize(),GetRenderWindow()->IsFullScreen(),opt,0);
				AddRenderWindow(m_2ndWnd);
				rVP=GetRenderWindow(1)->CreateViewport(mT("Right"),m_cameraR,0,math::rectf(0,0,1,1),0);
			}else
			{
				rVP=GetRenderWindow(0)->CreateViewport(mT("Right"),m_cameraR,0,math::rectf(0,0,1,1),0);
			}


			m_cameraL->setFovY(62);
			m_cameraR->setFovY(62);

			getSceneManager()->addSceneNode(m_cameraL);
			getSceneManager()->addSceneNode(m_cameraR);


			HeadMount* hm=new HeadMount(getSceneManager(),1,GetInputManager());
			m_headMount=hm;

			m_stereoRenderer=new scene::StereoRenderer(lVP,rVP,hm);
			getSceneManager()->addSceneNode(hm);
			m_stereoRenderer->SetOffset(0.06);
			m_stereoRenderer->SetMode(scene::EStereoMode::EStereo_None);
			if(v=="OverUnder" || v=="DVI")
				m_stereoRenderer->SetMode(scene::EStereoMode::EStereo_TopDown);
			else if(v=="SideBySide")
				m_stereoRenderer->SetMode(scene::EStereoMode::EStereo_SideBySide);

			getDevice()->setClearColor(video::SColor(0.6,0.8,0.9,0));

			_SwitchCamera(Front);

// 			m_cameraL->setVisible(false);
// 			m_cameraR->setVisible(false);

			vpList.push_back(m_stereoRenderer->GetLeftVP());
			vpList.push_back(m_stereoRenderer->GetRightVP());
		}

		if(0)
		{
			math::quaternion q;
			for(int i=0;i<4;++i)
			{
				scene::CameraNode* cam=getSceneManager()->createCamera();
				scene::ViewPort* vp=GetRenderWindow()->CreateViewport("vpL_"+core::StringConverter::toString(i),cam,0,math::rectf(0.2*i,0,0.2*i+0.2,0.2),0);
				cam->setZNear(0.1);
				cam->setZFar(100);
				cam->setFovY(90);
				vp->setAbsViewPort(math::rectf(120*i,0,120*(i+1),120));
				cam->setAutoUpdateAspect(false);
				cam->setAspect(1);
				cam->setOrintation(q);
				cam->setPosition(math::vector3d(0.03,0,0));
				m_headMount->addChild(cam);
				//vpList.push_back(vp);
				q*=math::quaternion(0,-90,0);
			}
			for(int i=0;i<4;++i)
			{
				scene::CameraNode* cam=getSceneManager()->createCamera();
				scene::ViewPort* vp=GetRenderWindow()->CreateViewport("vpR_"+core::StringConverter::toString(i),cam,0,math::rectf(0.2*i,0,0.2*i+0.2,0.2),0);
				cam->setZNear(0.1);
				cam->setZFar(100);
				cam->setFovY(90);
				vp->setAbsViewPort(math::rectf(120*i,120,120*(i+1),240));
				cam->setAutoUpdateAspect(false);
				cam->setAspect(1);
				cam->setOrintation(q);
				cam->setPosition(math::vector3d(-0.03,0,0));
				m_headMount->addChild(cam);
				//vpList.push_back(vp);
				q*=math::quaternion(0,-90,0);
			}

		}
		if(m_soundManager && 0)
		{
			//m_soundManager->getListener()->attachToListener(m_headMount);
			m_sound=m_soundManager->loadSound("motor.wav",false);
			m_sound->setLooping(true);
			m_motor=new MotorSound(m_sound);
		}

		if(0)
		{
		//Create GBuffer{m_stereoRenderer->GetLeftVP(),m_stereoRenderer->GetRightVP()};
		for(int i=0;i<vpList.size();++i)
		{
			scene::ViewPort* vp=vpList[i];
			video::EPixelFormat pf=video::EPixel_Float32_RGBA;
			video::ITexturePtr renderTargetTex=getDevice()->createTexture2D(vp->getAbsViewPort().getSize(),pf,true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);

			video::IRenderTargetPtr rt=getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
			renderTargetTex=getDevice()->createTexture2D(vp->getAbsViewPort().getSize(),pf,true);
			rt->attachRenderTarget(renderTargetTex,1);

			renderTargetTex=getDevice()->createTexture2D(vp->getAbsViewPort().getSize(),pf,true);
			rt->attachRenderTarget(renderTargetTex,2);

			vp->setRenderTarget(rt);

		}
			for(int i=0;i<vpList.size();++i)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("VTelesarPP.peff")));
				vpList[i]->enablePostProcessing(true);
				vpList[i]->setPostProcessing(pp);
				if(!s_ppShader)
					s_ppShader=pp;
			}
		}
	}

	void VApplication::draw(scene::ViewPort* vp)
	{/*
		m_debugRenderer->StartDraw(vp->getCamera());
		CMRayApplication::draw(vp);
		
		getDevice()->set3DMode();
		if(m_isDebugging)
		{
		//	m_phManager->renderDebug();
		}
		m_debugRenderer->EndDraw();*/
		getDevice()->set2DMode();

		if(0)
		{
			static video::ParsedShaderPP* pp=0;
			if(!pp )
			{
				pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("ssao.peff")));
			
			}

			video::TextureUnit tex;
			pp->Setup(vp->getAbsViewPort());
			tex.SetTexture(pp->render(vp->getRenderTarget())->getColorTexture());
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(0,vp->getSize()),1);
		}

		if(0)
		{

			video::TextureUnit tex;
			tex.SetTexture(vp->getRenderTarget()->getColorTexture());
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(0,vp->getSize()/2),1);
		}
		if(m_drawUI)
		{
			m_guiManager->DrawAll(vp);


			GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
			if(font){
				m_guiRender->Prepare();

				float yoffset=50;


				GUI::FontAttributes attr;
				attr.fontColor.Set(0.05,1,0.5,1);
				attr.fontAligment=GUI::EFA_MiddleLeft;
				attr.fontSize=24;
				attr.hasShadow=true;
				attr.shadowColor.Set(0,0,0,1);
				attr.shadowOffset=math::vector2d(2);
				attr.spacing=2;
				attr.wrap=0;
				attr.RightToLeft=0;
				core::string msg=mT("FPS= ");
				msg+=core::StringConverter::toString(core::CFPS::getInstance().getFPS());
				font->print(math::rectf(vp->getAbsRenderingViewPort().getWidth()-250,vp->getAbsRenderingViewPort().getHeight()-50,10,10),&attr,0,msg,m_guiRender);
				yoffset+=attr.fontSize;

				{
					float xOffset=10;
					attr.fontSize=20;
					attr.hasShadow=false;
					attr.fontColor.Set(1,0,0,1);
					attr.fontAligment=GUI::EFA_MiddleCenter;
					std::vector<core::string> debugLst;
					m_telesarLayer->GetDebugString(debugLst);
					for(int i=0;i<debugLst.size();++i)
					{
						math::rectf rc;
						rc.ULPoint.set(10+xOffset,vp->getAbsRenderingViewPort().getHeight()-50);
						rc.BRPoint=rc.ULPoint+math::vector2d(font->getTextDimension(debugLst[i],&attr,0).x+20,30);
						m_guiManager->GetActiveTheme()->drawSizableBox(m_guiRender,rc,0,mT("Frame"),video::SColor(1,1,1,1));

						font->print(rc,&attr,0,debugLst[i],m_guiRender);
						xOffset+=rc.getWidth()+30;
					}

				}

				m_guiRender->Flush();
				getDevice()->useShader(0);
			}
		}
	}
	void VApplication::WindowPostRender(video::RenderWindow* wnd)
	{
		m_stereoRenderer->Render();

	}

	void VApplication::update(float dt)
	{
		m_gameManager->PreUpdate();

		CMRayApplication::update(dt);

		static float acc=0;
		float targetDt=1.0/60.0;
		acc+=dt;
		while(acc>=targetDt )
		{
			acc-=targetDt;
			m_phManager->update(targetDt);
		}
		m_guiManager->Update(dt);
		m_gameManager->Update(dt);
		m_debugRenderer->Update(dt);
		if(m_soundManager)
			m_soundManager->runSounds(dt);
		//m_motor->Update(dt);

		if(m_isDebugging)
			m_gameManager->DebugRender(m_debugRenderer);
		//SendData(180*angle);
		float s=1;
		if(GetInputManager()->getKeyboard()->isLShiftPress())
		{
			s=-1;
		}
		m_angles[0]+=s*20*dt*(GetInputManager()->getKeyboard()->getKeyState(KEY_6));
		m_angles[1]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_7);
		m_angles[2]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_8);
		m_angles[3]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_9);
		m_angles[4]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_0);
		m_angles[5]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_6);
		m_angles[6]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_7);
		m_angles[7]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_8);
		m_angles[8]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_9);
		m_angles[9]+=s*20*dt*GetInputManager()->getKeyboard()->getKeyState(KEY_0);

		//if(m_dataRecorder->IsRecording())
			m_telesarLayer->_DoReceiving(dt);
			//update sensor data
			for(int i=0;i<3;++i)
			{
				m_sensorData[i]->AddValue(mT("Force"),m_telesarLayer->GetForce(true,i));
				m_sensorData[i]->AddValue(mT("Temperature"),m_telesarLayer->GetTemperature(true,i));

				m_sensorData[i+3]->AddValue(mT("Force"),m_telesarLayer->GetForce(false,i));
				m_sensorData[i+3]->AddValue(mT("Temperature"),m_telesarLayer->GetTemperature(false,i));
			}
		//m_dataRecorder->Update(dt);

		VT::CommunicationData data;
		data.SetTargetName(mT("TelesarV"));
		data.AddData("JointLF_4_1",&m_angles[0],1);
		data.AddData("JointRF_4_1",&m_angles[0],1);
		data.AddData("JointLF_2_3",&m_angles[1],1);
		data.AddData("JointLF_3_3",&m_angles[1],1);
		data.AddData("JointLF_4_3",&m_angles[1],1);
		data.AddData("JointLF_5_3",&m_angles[1],1);
		data.AddData("JointLF_2_4",&m_angles[2],1);
		data.AddData("JointLF_3_4",&m_angles[2],1);
		data.AddData("JointLF_4_4",&m_angles[2],1);
		data.AddData("JointLF_5_4",&m_angles[2],1);
		data.AddData("LFinger4",&m_angles[4],1);
// 		data.AddData("RShoulderY",m_angles[1]);
// 		data.AddData("RShoulderZ",m_angles[2]);
// 		data.AddData("RArmX",m_angles[3]);
// 		data.AddData("RArmZ",m_angles[4]);
// 		data.AddData("RHandY",m_angles[5]);
// 		data.AddData("RHandX",m_angles[6]);
// 		data.AddData("Body1",0);
// 		data.AddData("Body2",-m_angles[7]);
// 		data.AddData("Body3",0);
// 		data.AddData("Body4",m_angles[8]);
// 		data.AddData("Body5",0);
// 		data.AddData("Body6",0);
// 		data.AddData("Body7",0);

		float zeroV=0;
		data.AddData("JointBody1",&zeroV,1);
		data.AddData("JointBody2",&zeroV,1);
		data.AddData("JointBody3",&zeroV,1);
		data.AddData("JointBody4",&zeroV,1);
		data.AddData("JointBody5",&zeroV,1);
		data.AddData("JointBody6",&zeroV,1);
		data.AddData("JointBody7",&zeroV,1);

		//m_phManager->endUpdate();
		//VT::CommunicationManager::getInstance().OnDataArrived(0,&data);
		//g_graphPlotListener.OnDataArrived(0,&data);

	}

	void VApplication::onDone()
	{
		CMRayApplication::onDone();

		VT::ShutdownVTLib();
	}


	//////////////////////////////////////////////////////////////////////////
	void VApplication::_InitResources()
	{
		gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}
	void VApplication::CreatePhysicsSystem()
	{
		physics::PhysicsSystemDesc desc;
		desc.gravity.set(0,-9.8*1,0);
		desc.maxIter=32;
		desc.maxTimestep=1/(16*60.0);
		desc.useFixedTimeStep=true;
		m_phManager=new physics::PhysXManager(&desc);
		m_phManager->ConnectToRemoteDebugger();

	}
}