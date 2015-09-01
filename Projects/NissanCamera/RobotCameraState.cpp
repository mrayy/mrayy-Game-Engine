
#include "stdafx.h"
#include "RobotCameraState.h"
#include "LocalCameraVideoSource.h"
#include "NissanRobotCommunicator.h"
#include "CRobotConnector.h"

#include "MeshBufferData.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "GUIBatchRenderer.h"
#include "FontResourceManager.h"
#include "NCAppGlobals.h"
#include "OptiTrackHeadController.h"
#include "KeyboardHeadController.h"
#include "CalibHeadController.h"
#include "GUIThemeManager.h"
#include "ARGroupManager.h"
#include "GUIConsole.h"
#include "TextureResourceManager.h"
#include "ConsoleLogDevice.h"
#include "ICameraVideoGrabber.h"
#include "MeshResourceManager.h"
#include "TextureRTWrap.h"

#include "NCAppGlobals.h"
#include "Application.h"
#include "StringUtil.h"

namespace mray
{
namespace NCam
{

	class TestController :public TBee::IHeadController
	{
	protected:
	public:
		TestController()
		{}
		virtual~TestController()
		{}

		virtual bool GetHeadOrientation(math::quaternion& q,bool abs)
		{
			float time = gEngine.getTimer()->getSeconds();
			math::vector3d angles;
			angles.x = 70 * sin(time*0.002f);// 0 * sin(time*0.001f);
			angles.y = 0;;// 70 * sin(time*0.002f);
			angles.z = 0;

			q= math::quaternion(angles);
			return true;
		}
		virtual bool GetHeadPosition(math::vector3d& v, bool abs)
		{
			v = 0;
			return 1;
		}
	};


	enum class ERobotCameraKeyMap
	{
		Unkown = 0,
		Connect = 50,
		LockX,
		LockY,
		LockZ,
		Calibrate,
		Homing,
		Console,
		ARVisibility
	};


RobotCameraState::RobotCameraState()
	:Parent("RobotState")
{
	m_exitCode = 0;

	m_robotConnector = new TBee::CRobotConnector();
	m_robotComm = new NCam::NissanRobotCommunicator();
	m_robotConnector->SetCommunicator(m_robotComm);
	m_cameraRenderer = new CameraPlaneRenderer();

	//m_headController = new TBee::CalibHeadController(new TBee::OptiTrackHeadController(1));
	m_headController = new TBee::CalibHeadController(new TBee::KeyboardHeadController());
	//m_headController = new TBee::CalibHeadController(new TestController());

	m_robotConnector->SetHeadController(m_headController);
//	SetVideoSource(m_videoSource);

	m_lockAxis[0] = false;
	m_lockAxis[1] = false;
	m_lockAxis[2] = false;

	GUI::GUIBatchRenderer*r = new GUI::GUIBatchRenderer();
	r->SetDevice(Engine::getInstance().getDevice());
	m_guiRenderer = r;
	m_arServiceProvider = new ARServiceProvider();
	m_arServiceProvider->AddListener(this);
	m_arManager = new ARGroupManager();

	m_commandManager = new CommandManager();
	m_commandManager->MessageLog += newClassDelegate1("", this, &RobotCameraState::_OnCommandMessage);

	m_commandManager->addCommand(new ARCreateObject(m_commandManager, this));
	m_commandManager->addCommand(new ARUpdateCommand(m_commandManager, this));
	m_commandManager->addCommand(new ARRemoveCommand(m_commandManager, this));
	m_commandManager->addCommand(new ARQeuryCommand(m_commandManager, this));
	m_commandManager->addCommand(new ARMoveCommand(m_commandManager, this));
	m_commandManager->addCommand(new ARAlphaCommand(m_commandManager, this));
	m_commandManager->addCommand(new ARFovCommand(m_commandManager, this));

//	SetContentsOrigin(0.5, 0.5);

}

RobotCameraState::~RobotCameraState()
{
//	delete m_videoSource;
	m_robotConnector->DisconnectRobot();
	delete m_robotConnector;
	delete m_arServiceProvider;
	delete m_arManager;
	m_cameraRenderer = 0;

	gLogManager.removeLogDevice(m_consoleLogDevice);
}



void RobotCameraState::SetCameraConnection(TBee::EUSBCameraType type)
{
	((TBee::LocalCameraVideoSource*)m_cameraRenderer->GetVideoSource())->SetCameraConnectionType(type);
}

void RobotCameraState::SetCameraInfo(TBee::ETargetEye eye, int id)
{
	((TBee::LocalCameraVideoSource*)m_cameraRenderer->GetVideoSource())->SetCameraID(GetEyeIndex(eye), id);
}


void RobotCameraState::_OnConsoleCommand(GUI::GUIConsole*, const core::string& cmd)
{
	core::string s = core::StringUtil::Trim(cmd, "\r \t");
	m_commandManager->execCommand(s);
}
void RobotCameraState::_OnCommandMessage(const core::string& msg)
{
	m_console->AddToHistory(msg, video::SColor(1));
}


void RobotCameraState::InitState()
{
	Parent::InitState();

	m_guimngr = new GUI::GUIManager(gEngine.getDevice());
	m_guimngr->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
	{
		GUI::IGUIPanelElement* m_guiroot;
		m_guiroot = (GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""), m_guimngr);
		m_guiroot->SetDocking(GUI::EED_Fill);
		m_guimngr->SetRootElement(m_guiroot);

		GUI::GUIConsole* console = new GUI::GUIConsole(m_guimngr);
		m_guiroot->AddElement(console);
		console->SetAnchorLeft(true);
		console->SetAnchorRight(true);
		console->SetAnchorBottom(true);
		console->SetPosition(0);
		console->SetSize(math::vector2d(100, 300));
		console->SetVisible(false);
		m_console = console;

		m_console->OnCommand += newClassDelegate2("", this, &RobotCameraState::_OnConsoleCommand);

		m_consoleLogDevice = new ConsoleLogDevice(console);

		gLogManager.addLogDevice(m_consoleLogDevice);
	}

	{
		//register keys

		controllers::InputKeyMap& keys = NCAppGlobals::Instance()->keyMap;

		keys.RegisterKey((uint)ERobotCameraKeyMap::Connect, KEY_SPACE, false, false, "Connect/Disconnect Robot");
		keys.RegisterKey((uint)ERobotCameraKeyMap::Calibrate, KEY_C, false, false, "Calibrate Head Position");
		keys.RegisterKey((uint)ERobotCameraKeyMap::Homing, KEY_H, false, false, "Set Robot To Homing State");
		keys.RegisterKey((uint)ERobotCameraKeyMap::LockX, KEY_X, false, false, "Lock Robot X Axis");
		keys.RegisterKey((uint)ERobotCameraKeyMap::LockY, KEY_Y, false, false, "Lock Robot Y Axis");
		keys.RegisterKey((uint)ERobotCameraKeyMap::LockZ, KEY_Z, false, false, "Lock Robot Z Axis");
		keys.RegisterKey((uint)ERobotCameraKeyMap::Console, KEY_TAB, false, false, "Show/Hide Console");
		keys.RegisterKey((uint)ERobotCameraKeyMap::ARVisibility, KEY_V, true, false, "Show/Hide AR Objects");
	}

	{
		m_sceneManager = new scene::SceneManager(Engine::getInstance().getDevice());
	}
	{
		m_arRoot = m_sceneManager->createSceneNode("AR_Root_Node");

		m_arRoot->setVisible(false);

		m_arManager->SetSceneManager(m_sceneManager, m_arRoot);
		core::string ip = NCAppGlobals::Instance()->GetValue("ARServer", "IP");
		uint port = core::StringConverter::toUInt(NCAppGlobals::Instance()->GetValue("ARServer", "Port"));
	
		//m_arServiceProvider->Connect(ip,port);


		//Create a default "Shared" material for generated AR contents in order to boost performance
		video::RenderMaterial* material = new video::RenderMaterial();
		video::RenderPass* pass= material->CreateTechnique("Default")->CreatePass("Default");
		pass->setRenderState(video::RS_Lighting, video::ES_DontUse);
		pass->setRenderState(video::RS_CullFace, video::ES_DontUse);
		pass->SetDiffuse(1);
		pass->SetAmbient(1);
		gMaterialResourceManager.addResource(material,"DefaultAR_Mtrl");
	}
	{
		scene::CameraNode* cam[2];
		video::EPixelFormat pf = video::EPixel_R8G8B8A8;//video::EPixel_R8G8B8A8;//
		m_carObjects.Head = m_sceneManager->createSceneNode();

		scene::LightNode* light= m_sceneManager->createLightNode("Sun");
		light->setPosition(1000);

		core::string vpName[] = { "Left", "Right" };

		for (int i = 0; i < 2; ++i)
		{

			m_carObjects.Eyes[i] = m_sceneManager->createSceneNode(vpName[i]+"_Eye");

			cam[i] = m_sceneManager->createCamera(vpName[i] + "_Cam");
			m_viewport[i] = new scene::ViewPort(vpName[i] + "_VP", cam[i], 0, 0, math::rectf(0, 0, 1, 1), 0);
			m_viewport[i]->SetClearColor(video::SColor(0,0,0, 1));

			{
				video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), pf, true);
				renderTargetTex->setBilinearFilter(false);
				renderTargetTex->setTrilinearFilter(false);
				renderTargetTex->setMipmapsFilter(false);

				video::IRenderTargetPtr rt = Engine::getInstance().getDevice()->createRenderTarget(mT(""), renderTargetTex, video::IHardwareBufferPtr::Null, video::IHardwareBufferPtr::Null, false);
				m_viewport[i]->setRenderTarget(rt);
				m_viewport[i]->setOnlyToRenderTarget(true);
				m_viewport[i]->SetAutoUpdateRTSize(true);
			}
			m_viewport[i]->AddListener(this);

			cam[i]->setZNear(0.1);
			cam[i]->setZFar(10000);

			//if (ATAppGlobal::Instance()->oculusDevice)
			cam[i]->setFovY(m_cameraRenderer->GetDisplayFov());
			cam[i]->setAutoUpdateAspect(true);
			m_carObjects.Camera[i] = cam[i];
			m_carObjects.Eyes[i]->addChild(m_carObjects.Camera[i]);

			m_carObjects.Head->addChild(m_carObjects.Eyes[i]);

			//cameras are physically shifted 9 cm from the rotation center on the Y axis
			cam[i]->setPosition(math::vector3d(0, 0.09, 0));
		}

		//Set eyes distance
		m_carObjects.Eyes[GetEyeIndex(TBee::Eye_Left)]->setPosition(math::vector3d(-0.03, 0, 0));
		m_carObjects.Eyes[GetEyeIndex(TBee::Eye_Right)]->setPosition(math::vector3d(+0.03, 0, 0));

		//Create screen display node
		m_carObjects.ProjectionPlane = m_sceneManager->createSceneNode("DisplayNode");
	}



	if (false)
	{
		ARGroup grp;
		ARPredef *predef = new ARPredef;
		predef->name = "drivable_area.obj";
		grp.objects.push_back(predef);

		ARCommandAddData cmd;
		cmd.group = &grp;
		OnARContents(&cmd);
	}
	if (false)
	{
		CreateARObject(1000, "sign2.obj", 0, 0);
	}
	if (false)
	{
		for (int i = 0; i < 1; ++i)
		{
			CreateARObject(10 + i, "drivable_area.3ds", 0, 0);
		}
	}

	if (false)
	{
		core::string files[] =
		{
			"drivable_area.obj",
			"sign.obj",
			"sign_semitrans.obj"
		};
		for (int i = 0; i < 3; ++i)
		{
			scene::SMeshPtr mesh = gMeshResourceManager.loadMesh(files[i], true);
			scene::MeshRenderableNode* node = new scene::MeshRenderableNode(mesh);
			scene::ISceneNode* sn = m_sceneManager->createSceneNode(files[i]);
			sn->AttachNode(node);
			m_arRoot->addChild(sn);
		}
	}

	if (true)
	{
		//scene::SMeshPtr mesh= gMeshResourceManager.loadMesh("van.3ds",true);
		//scene::MeshRenderableNode* node = new scene::MeshRenderableNode(mesh);
		m_carObjects.Vehicle = m_sceneManager->createSceneNode("VehicleModel");
	//	m_vehicleModel->setPosition(math::vector3d(-47197.953, 100, 61388.07));
		m_carObjects.Vehicle->addChild(m_carObjects.Head);
		m_carObjects.Vehicle->addChild(m_carObjects.ProjectionPlane);
// 		m_carObjects.Vehicle->addChild(m_carObjects.Camera[0]);
		// 		m_carObjects.Vehicle->addChild(m_carObjects.Camera[1]);
		//m_vehicleModel->AttachNode(node);
		{
			CreateARObject(1000, "", 0, 0);// math::vector3d(-47197.953, 100, 61388.07), 0);
			//CreateARObject(1000, "", math::vector3d(-47197.953, 100, 61388.07), 0);
			ARSceneGroup *g= m_arManager->GetGroupListByID(1000);
			m_vehicleRef = g->objects.begin()->second;
		}
	}
	if (false)
	{
		ARMesh* mesh = new ARMesh();
		mesh->verticies.push_back(math::vector3d(0, 0, 5));
		mesh->verticies.push_back(math::vector3d(0, 1, 5));
		mesh->verticies.push_back(math::vector3d(1, 1, 5));
		mesh->verticies.push_back(math::vector3d(1, 0, 5));
		
		mesh->normals.push_back(math::vector3d(0, 0, 1));
		mesh->normals.push_back(math::vector3d(0, 0, 1));
		mesh->normals.push_back(math::vector3d(0, 0, 1));
		mesh->normals.push_back(math::vector3d(0, 0, 1));

		mesh->colors.push_back(1);
		mesh->colors.push_back(1);
		mesh->colors.push_back(1);
		mesh->colors.push_back(1);

		mesh->meshType = EARMeshType::EQuads;

		mesh->cullface = false;

		mesh->colorType = EARColorType::EVertex;

		mesh->pos.set(0, 0, 10);
		mesh->scale = 10;

		ARGroup grp;
		grp.objects.push_back(mesh);

		ARCommandAddData cmd;
		cmd.group = &grp;
		OnARContents(&cmd);

	}

	m_commandManager->execCommand("exec initial.cmd");
	//init camera render plane
	m_cameraRenderer->Init(&m_carObjects);

	_RegisterNetworkValues();
	m_console->AddToHistory("System Inited.", video::DefaultColors::Green);
}

void RobotCameraState::_RegisterNetworkValues()
{

}
void RobotCameraState::_OnPropertyChanged(IValue* v)
{
}

bool RobotCameraState::OnEvent(Event* e, const math::rectf& rc)
{
	if (Parent::OnEvent(e, rc)  || m_guimngr->OnEvent(e,&rc))
		return true;
	bool ok = false;


	if (m_cameraRenderer->OnEvent(e))
		return true;


	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			ERobotCameraKeyMap cmdCode = (ERobotCameraKeyMap) NCAppGlobals::Instance()->keyMap.GetCommand(evt);
			switch (cmdCode)
			{
			case mray::NCam::ERobotCameraKeyMap::Unkown:
				break;
			case mray::NCam::ERobotCameraKeyMap::Connect:
				if (m_robotConnector->IsRobotConnected())
					m_robotConnector->EndUpdate();
				else
				{
					if (!m_robotConnector->IsRobotConnected())
						m_robotConnector->ConnectRobot();
					m_robotConnector->StartUpdate();
				}
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::LockX:
				m_lockAxis[0] = !m_lockAxis[0];
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::LockY:
				m_lockAxis[1] = !m_lockAxis[1];
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::LockZ:
				m_lockAxis[2] = !m_lockAxis[2];
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::Calibrate:
				m_headController->Calibrate();
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::Homing:
				m_robotConnector->SetData("Homing", "", false);
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::Console:
				m_console->SetVisible(!m_console->IsVisible());
				ok = true;
				break;
			case mray::NCam::ERobotCameraKeyMap::ARVisibility:
				m_arRoot->setVisible(!m_arRoot->isVisible(), true);
				ok = true;
				break;
			default:
				break;
			}
		}
	}
	return ok;
}
void RobotCameraState::OnEnter(TBee::IRenderingState*prev)
{
	Parent::OnEnter(prev);

	//m_robotConnector->ConnectRobot();
	m_cameraRenderer->Start();
}

void RobotCameraState::OnExit()
{
	Parent::OnExit();
	m_robotConnector->DisconnectRobot();
	m_cameraRenderer->Stop();
}

void RobotCameraState::SetTransformation( const math::vector3d& pos, const math::vector3d &angles)
{
// 		m_headNode->setOrintation(m_headRotationOffset);;// +angles);
// 		m_headNode->setPosition(m_headPosOffset);// +pos);

	// Update vehicle position and orientation
	m_vehicleRef->obj->pos += m_vMotionSpeed;
	m_vehicleRef->obj->dir += m_vRotationSpeed;
	m_vehicleRef->sceneNode->setPosition(m_vehicleRef->obj->pos );
	m_vehicleRef->sceneNode->setOrintation(m_vehicleRef->obj->dir );

	//m_carObjects.Head->setPosition(pos);

	m_vRotationSpeed = 0;
	m_vMotionSpeed = 0;

	m_cameraRenderer->SetTransformation(pos, angles);
}

float time = 0;
void RobotCameraState::Update(float dt)
{
	Parent::Update(dt);
	m_guimngr->Update(dt);
	m_robotConnector->UpdateStatus();
	m_cameraRenderer->Update(dt);


	math::quaternion q;// = m_robotConnector->GetCurrentHeadRotation();// m_robotConnector->GetHeadRotation();
	math::vector3d pos =  m_robotConnector->GetHeadPosition();


	math::vector3d rot,r ;
	//q.toEulerAngles(r);
	if (false)
	{
		//in the car
		r = m_robotConnector->GetCurrentHeadRotation();
		rot.x = r.y;
		rot.y = -r.z;
		rot.z = -r.x;
	}
	else
	{
		//in the lab
		if (m_robotConnector->IsRobotConnected())
			r = m_robotConnector->GetCurrentHeadRotation();
		else m_robotConnector->GetHeadRotation().toEulerAngles(r);
		rot.x = -r.y;
		rot.y = r.z;
		rot.z = -r.x;
	}
// 	rot.x = -rot.x;
// 	rot.z = -rot.z;
	//rot = r;
	
	if (m_lockAxis[0])
		rot.x = 0;
	if (m_lockAxis[1])
		rot.y = 0;
	if (m_lockAxis[2])
		rot.z = 0;

	SetTransformation(pos, rot);

	m_arServiceProvider->Update();

	_UpdateMovement(dt);

	m_sceneManager->update(dt);
}
void RobotCameraState::_UpdateMovement(float dt)
{

	//update car pos
	if (m_arManager->GetVehicle())
	{
		m_carObjects.Vehicle->setPosition(m_arManager->GetVehicle()->getAbsolutePosition());
		m_carObjects.Vehicle->setOrintation(m_arManager->GetVehicle()->getAbsoluteOrintation());
	}
	else
	{
		m_carObjects.Vehicle->setPosition(m_vehicleRef->sceneNode->getAbsolutePosition());
		m_carObjects.Vehicle->setOrintation(m_vehicleRef->sceneNode->getAbsoluteOrintation());
	}

	InputManager* mngr = NCAppGlobals::Instance()->App->GetInputManager();
	controllers::IKeyboardController* kb = mngr->getKeyboard();
	controllers::IMouseController* m = mngr->getMouse();

	if (kb->getKeyState(KEY_LCONTROL))
	{
		math::vector3d speed;
		speed.x = kb->getKeyState(KEY_A) - kb->getKeyState(KEY_D);
		speed.z = kb->getKeyState(KEY_W) - kb->getKeyState(KEY_S);
		speed.y = kb->getKeyState(KEY_Q) - kb->getKeyState(KEY_Z);
		speed *=  dt;

		math::vector3d pos;
		//pos = m_headNode->getPosition();

		pos = m_carObjects.Head->getOrintation()*math::vector3d::XAxis*speed.x;
		pos += m_carObjects.Head->getOrintation()*math::vector3d::YAxis*speed.y;
		pos += m_carObjects.Head->getOrintation()*math::vector3d::ZAxis*speed.z;
		//m_headNode->setPosition(pos);

		m_vMotionSpeed += pos;

		//	m_headNode->rotate(angles*100*dt, scene::TS_Local);

		// 			m_cameraOffsets.x += angles.x*dt*0.1f;
		// 			m_cameraOffsets.y += angles.y*dt*0.1f;
	}
	if (m->isPressed(controllers::EMB_Right) && false)
	{
		math::vector3d angles;
		angles.x = m->getDY();
		angles.y = -m->getDX();
		angles *= 10 * dt;
		m_vRotationSpeed += angles;

		//math::quaternion q = math::quaternion(angles.y,  m_headNode->getOrintation().getYAxis());

		//m_headNode->rotate(q, scene::TS_Parent);
		
// 		q = math::quaternion(angles.x, m_headNode->getOrintation().getXAxis());
// 		m_headNode->rotate(q, scene::TS_Parent);

		// 			m_cameraOffsets.x += angles.x*dt*0.1f;
		// 			m_cameraOffsets.y += angles.y*dt*0.1f;
	}
}


video::IRenderTarget* RobotCameraState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	
	video::IVideoDevice* device = Engine::getInstance().getDevice();
	int index = GetEyeIndex(eye);
	Parent::Render(rc, eye);
	device->set3DMode();

	m_cameraRenderer->PreRender(rc,index);


	m_viewport[index]->setAbsViewPort(rc);
	m_viewport[index]->draw();

	video::TextureUnit tex;
	device->set2DMode();
	device->setRenderTarget(m_renderTarget[index]);
	{
		tex.SetTexture(m_viewport[index]->getRenderTarget()->GetColorTexture());
		device->useTexture(0, &tex);
		math::rectf tc = math::rectf(0, 0, 1, 1);
		device->draw2DImage(rc, 1, 0, &tc);
	}

	m_cameraRenderer->PostRender(index);

	/*
	//debug render camera views
	tex.SetTexture(camTex1);
	device->useTexture(0, &tex);
	device->draw2DImage(math::rectf(0, 0, 200, 200), 1);
	tex.SetTexture(camTex);
	device->useTexture(0, &tex);
	device->draw2DImage(math::rectf(200, 0, 400, 200), 1);
	*/
	_RenderUI(rc);

	return m_renderTarget[index];
}
void RobotCameraState::onRenderDone(scene::ViewPort*vp)
{

	if (!TBee::AppData::Instance()->IsDebugging)
		return;
	
	int index = 0;
	if (vp->getName() == "Left")
		index = 0;
	else index = 1;
	
	return;

	video::IVideoDevice* device = Engine::getInstance().getDevice();
//	device->set3DMode();
	video::RenderPass pass(0);
	pass.setRenderState(video::RS_Lighting, video::ES_DontUse);
	pass.setRenderState(video::RS_ZTest, video::ES_DontUse);
	pass.SetThickness(2);
	//device->clearBuffer(video::EDB_DEPTH);
	device->useRenderPass(&pass);
	device->setTransformationState(video::TS_WORLD, m_carObjects.Head->getAbsoluteTransformation());

	math::vector3d origin;
	origin = 0;
	origin += math::vector3d::XAxis* m_cameraRenderer->GetUserOffset().x;
	origin += math::vector3d::YAxis* m_cameraRenderer->GetUserOffset().y;
	device->draw3DLine(origin, origin + math::vector3d::ZAxis * m_cameraRenderer->GetUserOffset().z, video::SColor(1, 0, 0, 1));


	int n =30;
	for (int i = 0; i < n; ++i)
	{
		math::vector3d a;
		math::vector3d b;
		a.z = m_cameraRenderer->GetUserOffset().z;
		b.z = m_cameraRenderer->GetUserOffset().z;
		float step = (i - n*0.5f)*0.1f;;

		a.y = -1 ;
		b.y = 1 ;
		b.x = a.x = step;

		float strength = pow(1 - fabs((float)i - n*0.5f) / (float)(n*0.5f),3);

		device->draw3DLine(a, b, video::SColor( strength, 0, 0, 1));

		a.x = -1 ;
		b.x = 1 ;
		b.y = a.y = step;
		a.z = m_cameraRenderer->GetUserOffset().z;
		b.z = m_cameraRenderer->GetUserOffset().z;
		device->draw3DLine(a, b, video::SColor(strength, 0, 0, 1));
	}

	device->useRenderPass(0);

}

void RobotCameraState::_RenderUI(const math::rectf& rc)
{
	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;


	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	dev->set2DMode();
	TBee::AppData* app = TBee::AppData::Instance();
	m_guimngr->DrawAll(&rc);
	m_guiRenderer->Flush();

	if (!TBee::AppData::Instance()->IsDebugging)
		return;

	m_cameraRenderer->DebugRender(m_guiRenderer);

	if (font)
	{
#define PRINT_LOG(msg)\
	font->print(r, &attr, 0, msg, m_guiRenderer); \
	r.ULPoint.y += 2*attr.fontSize  ;

		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = rc;

		{
			core::string msg = "Axis Lock: ";
			if (m_lockAxis[0])
				msg += "[X]";
			else msg += "X";
			msg += ",";
			if (m_lockAxis[1])
				msg += "[Y]";
			else msg += "Y";
			msg += ",";
			if (m_lockAxis[2])
				msg += "[Z]";
			else msg += "Z";
			attr.fontColor.Set(0, 1, 0, 1);
			PRINT_LOG(msg);
		}
		{
			core::string msg;
			if (m_robotConnector->IsRobotConnected())
				msg = "Robot Connected";
			else
				msg = "Robot Disconnected";
			PRINT_LOG(msg);
		}

		attr.fontColor.Set(1, 1, 1, 1);
		if (m_robotConnector->GetHeadController())
		{
			
			core::string msg;
			math::vector3d head;
			math::quaternion q;
			m_headController->GetHeadOrientation(q, false);
			q.toEulerAngles(head);
			
			msg = mT("Head Rotation: ") + core::StringConverter::toString((math::vector3di)head) + mT(" <--> ") + core::StringConverter::toString((math::vector3di)m_robotConnector->GetCurrentHeadRotation());
			PRINT_LOG(msg);

			head = m_robotConnector->GetHeadPosition();
			msg = mT("Head Position: ") + core::StringConverter::toString((math::vector3d)head);
			PRINT_LOG(msg);
		}
		else
		{
			core::string msg = mT("Head: Non");
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		if (m_robotConnector->GetRobotController())
		{

			math::vector2d speed;
			float rot;
			speed = m_robotConnector->GetSpeed();
			rot = m_robotConnector->GetRotation();
			core::string msg = mT("Robot Speed: ") + core::StringConverter::toString(speed);
			PRINT_LOG(msg);
			msg = mT("Robot Rotation: ") + core::StringConverter::toString(rot);
			PRINT_LOG(msg);


		}
		{
			core::string msg = mT("Capture Rate: ") + core::StringConverter::toString(m_cameraRenderer->GetVideoSource()->GetCaptureFrameRate(0)) + " / "
				+ core::StringConverter::toString(m_cameraRenderer->GetVideoSource()->GetCaptureFrameRate(1));
			PRINT_LOG(msg);
		}

		if (m_arManager->GetVehicle())
		{
			//Debug Vehicle details (Position and Direction)
			scene::ISceneNode* v = m_arManager->GetVehicle();

			core::string msg = mT("Car Position: ") + core::StringConverter::toString(v->getPosition());
			PRINT_LOG(msg);
			math::vector3d angles;
			v->getOrintation().toEulerAngles(angles);
			msg = mT("Car Heading: ") + core::StringConverter::toString(angles);
			PRINT_LOG(msg);


		}

		core::string msg = mT("Is Homing: ");
		msg += m_robotComm->IsHoming() ? "Yes" : "No";
		PRINT_LOG(msg);
		m_guiRenderer->Flush();
	}

}

void RobotCameraState::LoadFromXML(xml::XMLElement* e)
{
	Parent::LoadFromXML(e);



	xml::XMLElement* c = e->getSubElement("Calibration");
	if (c)
		m_headController->LoadFromXML(c);


	m_cameraRenderer->LoadFromXML(e);
}
xml::XMLElement* RobotCameraState::WriteToXML(xml::XMLElement* e)
{
	xml::XMLElement* elem = Parent::WriteToXML(e);

	m_headController->WriteToXML(elem);
	return elem;
}

void RobotCameraState::OnARContents(ARCommandAddData* cmd)
{
	m_arManager->AddGroup(cmd->group);
	for (int i = 0; i < cmd->group->objects.size(); ++i)
	{
	}
	/*
	if (m_arManager->GetVehicle() && m_vehicleModel->getParent() != m_arManager->GetVehicle())
	{
		m_arManager->GetVehicle()->addChild(m_vehicleModel);
		m_arManager->GetVehicle()->addChild(m_headNode);
	}*/
}
void RobotCameraState::OnVechicleData()
{

}
void RobotCameraState::OnDeletedGroup(ARCommandDeleteGroup* cmd)
{
	m_arManager->RemoveGroup(cmd->groupID);
}

void RobotCameraState::CreateARObject(uint id, const core::string& name, const math::vector3d& pos, const math::vector3d& dir,bool isVehicle)
{

	ARGroup *grp = new ARGroup();
	ARPredef *predef = new ARPredef;
	predef->name =name;
	predef->pos = pos;
	predef->dir = dir;
	predef->isVehicle = isVehicle;

	grp->objects.push_back(predef);
	grp->groupID = id;

	ARCommandAddData cmd;
	cmd.group =grp;
	OnARContents(&cmd);
}
void RobotCameraState::UpdateARObject(uint id, const math::vector3d& pos, const math::vector3d& dir)
{
	ARSceneGroup* g = m_arManager->GetGroupListByID(id);
	if (!g)
		return;
	if (!g->objects.size())
		return;
	ARSceneObject* grp = g->objects.begin()->second;
	grp->obj->pos = pos;
	grp->obj->dir = dir;
	if (!grp->obj->isVehicle)
	{
		grp->sceneNode->setPosition(pos);
		grp->sceneNode->setOrintation(dir);
	}
}
void RobotCameraState::MoveARObject(uint id, const math::vector3d& pos, const math::vector3d& dir)
{

	ARSceneGroup* g = m_arManager->GetGroupListByID(id);
	if (!g)
		return ;
	if (!g->objects.size())
		return ;

	math::vector3d p,d;
	p = g->objects.begin()->second->obj->pos;
	d = g->objects.begin()->second->obj->dir;
	p += pos;
	d += dir;
	g->objects.begin()->second->obj->pos=p;
	g->objects.begin()->second->obj->dir=d;
	g->objects.begin()->second->sceneNode->setPosition(p);
	g->objects.begin()->second->sceneNode->setOrintation(d);
}


void RobotCameraState::RemoveARObject(uint id)
{
	ARCommandDeleteGroup cmd;
	cmd.groupID = id;
	OnDeletedGroup(&cmd);
}
void RobotCameraState::SelectARObject(uint id)
{
}
void RobotCameraState::SetARAlpha(uint id, float v)
{
	if (id==-1)
		m_arManager->SetAlpha(v);
	else 
		m_arManager->SetAlpha(id, v);
}
bool RobotCameraState::QueryARObject(uint id, math::vector3d& pos, math::vector3d& dir)
{
	ARSceneGroup* g= m_arManager->GetGroupListByID(id);
	if (!g)
		return false;
	if (!g->objects.size())
		return false;

	pos = g->objects.begin()->second->obj->pos;
	dir = g->objects.begin()->second->obj->dir;

	return true;
}
void RobotCameraState::ListARObjects(std::vector<uint> &ids)
{
	const GroupMap& g=m_arManager->GetGroups();
	GroupMap::const_iterator it = g.begin();
	for (; it != g.end();++it)
	{
		ids.push_back(it->second->group->groupID);
	}

}

void RobotCameraState::ChangeARFov(float fov)
{
	m_carObjects.Camera[0]->setFovY(fov);
	m_carObjects.Camera[1]->setFovY(fov);
}



}
}
