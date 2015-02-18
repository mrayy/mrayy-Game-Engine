
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

		virtual bool GetHeadOrientation(math::quaternion& q)
		{
			float time = gEngine.getTimer()->getSeconds();
			math::vector3d angles;
			angles.x = 70 * sin(time*0.002f);// 0 * sin(time*0.001f);
			angles.y = 0;;// 70 * sin(time*0.002f);
			angles.z = 0;

			q= math::quaternion(angles);
			return true;
		}
		virtual bool GetHeadPosition(math::vector3d& v)
		{
			v = 0;
			return 1;
		}
	};

RobotCameraState::RobotCameraState()
	:Parent("RobotState")
{
	m_exitCode = 0;
	m_surface[0] = m_surface[1]= 0;

	m_robotConnector = new TBee::CRobotConnector();
	m_videoSource = new TBee::LocalCameraVideoSource();
	m_robotComm = new NCam::NissanRobotCommunicator();
	m_robotConnector->SetCommunicator(m_robotComm);

	//m_headController = new TBee::CalibHeadController(new TBee::OptiTrackHeadController(1));
	//m_headController = new TBee::CalibHeadController(new TBee::KeyboardHeadController());
	m_headController = new TBee::CalibHeadController(new TestController());

	m_robotConnector->SetHeadController(m_headController);
	m_hmdFov = 50;
//	SetVideoSource(m_videoSource);


	GUI::GUIBatchRenderer*r = new GUI::GUIBatchRenderer();
	r->SetDevice(Engine::getInstance().getDevice());
	m_guiRenderer = r;
	m_lockAxis[0] = false;
	m_lockAxis[1] = false;
	m_lockAxis[2] = false;
	m_camConfigDirty = true;
	m_useLensCorrection = false;
	m_cameraConfiguration = 0;

	m_cameraFPS = 85;
	m_cameraResolution.set(1280, 720);

	m_cameraOffsets.z = 1.6f;		//camera offset from user eye
	m_cameraOffsets.x = -0.6;		// camera offset from center of screen
	m_cameraOffsets.y = -0.1;		// camera offset from center of screen

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
	delete m_arServiceProvider;
	delete m_arManager;

	gLogManager.removeLogDevice(m_consoleLogDevice);
}



void RobotCameraState::SetCameraInfo(TBee::ETargetEye eye, int id)
{
	((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraID(GetEyeIndex(eye), id);
}


void RobotCameraState::GenerateSurface(bool plane, float hfov, float vfov, int segments,  float cameraScreenDistance)
{
	float radius = cameraScreenDistance;
	m_surfaceParams.plane = plane;
	m_surfaceParams.hfov = hfov;
	m_surfaceParams.vfov = vfov;
	m_surfaceParams.segments = segments;
	m_surfaceParams.radius = radius;
	m_surfaceParams.scale[0] = m_surfaceParams.scale[1] = 1;
	int vertCount = (segments + 1)*(segments + 1);
	int indCount = 6 * segments*segments;

	for (int i = 0; i < 2; ++i)
	{
		if (m_surface[i])
		{
			m_screenNode[i]->RemoveNode(m_surface[i]);
			m_surface[i] = 0;
		}
	}

	if (plane)
	{
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{
			m_screenNode[i] = m_sceneManager->createSceneNode("ScreenNode");
			scene::MeshRenderableNode* rnode = new scene::MeshRenderableNode(new scene::SMesh());
			m_surface[i] = rnode;

			scene::MeshBufferData* bdata = rnode->getMesh()->addNewBuffer();
			scene::IMeshBuffer* buffer = bdata->getMeshBuffer();

			video::IHardwareStreamBuffer* pos = buffer->createStream(0, video::EMST_Position, video::ESDT_Point3f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareStreamBuffer* tc = buffer->createStream(0, video::EMST_Texcoord, video::ESDT_Point2f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareIndexBuffer* idx = buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, 6, video::IHardwareBuffer::EUT_WriteOnly, false, false);

			math::vector3d posPtr[4] =
			{
				math::vector3d(-1, -1, 0),
				math::vector3d(+1, -1, 0),
				math::vector3d(+1, +1, 0),
				math::vector3d(-1, +1, 0)
			};
			math::vector2d tcPtr[4] = {
				math::vector2d(1, 0),
				math::vector2d(0, 0),
				math::vector2d(0, 1),
				math::vector2d(1, 1),
			};

			math::matrix3x3 rotMat;


			if (m_cameraConfiguration->cameraRotation[i] != TBee::TelubeeCameraConfiguration::None)
			{
				if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CW)
					rotMat.setAngle(90);
				else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CCW)
					rotMat.setAngle(-90);
				else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::Flipped)
					rotMat.setAngle(180);
				//     		math::Swap(tc.ULPoint.x, tc.ULPoint.y);
				//     		math::Swap(tc.BRPoint.x, tc.BRPoint.y);

			}
			/*
			if (i == 0)
			rotMat.setAngle(90);
			else rotMat.setAngle(-90);
			*/
			for (int j = 0; j < 4; ++j)
				tcPtr[j] = (rotMat*(tcPtr[j] * 2 - 1))*0.5 - 0.5f;
			ushort idxPtr[6] = { 0, 1, 2, 0, 2, 3 };

			pos->writeData(0, 4 * sizeof(math::vector3d), posPtr, true);
			tc->writeData(0, 4 * sizeof(math::vector2d), tcPtr, true);
			idx->writeData(0, 6 * sizeof(ushort), idxPtr, true);

			video::RenderMaterialPtr mtrl = new video::RenderMaterial();
			m_screenMtrl[i] = mtrl->CreateTechnique("Default")->CreatePass("ScreenPass");
			bdata->setMaterial(mtrl);

			rnode->SetHasCustomRenderGroup(true);
			rnode->SetTargetRenderGroup(scene::RGH_Solid - 5);

			m_screenMtrl[i]->setRenderState(video::RS_Lighting, video::ES_DontUse);
			m_screenMtrl[i]->setRenderState(video::RS_ZWrite, video::ES_DontUse);
			m_screenMtrl[i]->setRenderState(video::RS_ZTest, video::ES_DontUse);
			m_screenMtrl[i]->setRenderState(video::RS_CullFace, video::ES_DontUse);

			m_screenNode[i]->AttachNode(rnode);
			m_camera[i]->addChild(m_screenNode[i]);
			//m_headNode->addChild(m_screenNode[i]);
			m_screenNode[i]->setPosition(math::vector3d(0, 0, cameraScreenDistance));
			m_screenNode[i]->setVisible(false);
			m_screenNode[i]->setScale(1);
			m_screenNode[i]->setCullingType(scene::SCT_NONE);
		}

	}
	else
	{
		float w = 2 * math::PI32*radius*hfov / 360.0f;
		float h = 2 * math::PI32*radius*vfov / 360.0f;
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{
			m_screenNode[i] = m_sceneManager->createSceneNode("ScreenNode");
			scene::MeshRenderableNode* rnode = new scene::MeshRenderableNode(new scene::SMesh());

			scene::MeshBufferData* bdata = rnode->getMesh()->addNewBuffer();
			scene::IMeshBuffer* buffer = bdata->getMeshBuffer();

			m_surface[i] = rnode;

			video::IHardwareStreamBuffer* pos = buffer->createStream(0, video::EMST_Position, video::ESDT_Point3f, vertCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareStreamBuffer* tc = buffer->createStream(0, video::EMST_Texcoord, video::ESDT_Point2f, vertCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareIndexBuffer* idx = buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, indCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);

			math::vector3d* posPtr = (math::vector3d*) pos->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			math::vector2d* uvPtr = (math::vector2d*)tc->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			ushort* indPtr = (ushort*)idx->lock(0, indCount, video::IHardwareBuffer::ELO_NoOverwrite);


			math::matrix3x3 rotMat;


			if (m_cameraConfiguration->cameraRotation[i] != TBee::TelubeeCameraConfiguration::None)
			{
				if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CW)
					rotMat.setAngle(90);
				else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CCW)
					rotMat.setAngle(-90);
				else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::Flipped)
					rotMat.setAngle(180);
				//     		math::Swap(tc.ULPoint.x, tc.ULPoint.y);
				//     		math::Swap(tc.BRPoint.x, tc.BRPoint.y);

			}
			for (int x = 0; x <= segments; ++x)
			{
				float u = ((float)x) / (float)segments;;
				float a1 = hfov*(x - segments / 2.0f) / (float)segments;
				for (int y = 0; y <= segments; ++y)
				{
					float v = ((float)y) / (float)segments;;
					float a2 = vfov*(y - segments / 2.0f) / (float)segments ;
					float xp = radius*math::sind(a1) *math::sind(a2);
					float yp = radius* radius*math::cosd(a2); //((float)y - segments / 2.0f) / (float)segments;;;//
					float zp = radius*math::cosd(a1) *math::sind(a2);



					(*posPtr++).set(xp, yp, zp);
					(*uvPtr).set(u, v);
					*uvPtr = (rotMat*(*uvPtr * 2 - 1))*0.5 - 0.5f;

					uvPtr++;

				}
			}

			for (int x = 0; x < segments; ++x)
			{
				for (int y = 0; y < segments; ++y)
				{
					*indPtr++ = y*(segments + 1) + x;
					*indPtr++ = y*(segments + 1) + x + 1;
					*indPtr++ = (y + 1)*(segments + 1) + (x + 1);

					*indPtr++ = y*(segments + 1) + x;
					*indPtr++ = (y + 1)*(segments + 1) + (x + 1);
					*indPtr++ = (y + 1)*(segments + 1) + x;
				}
			}

			pos->unlock();
			tc->unlock();
			idx->unlock();


			video::RenderMaterialPtr mtrl = new video::RenderMaterial();
			m_screenMtrl[i] = mtrl->CreateTechnique("Default")->CreatePass("Default");
			bdata->setMaterial(mtrl);

			m_screenMtrl[i]->setRenderState(video::RS_Lighting, video::ES_DontUse);
			//m_screenMtrl[i]->setRenderState(video::RS_Points, video::ES_Use);
			m_screenMtrl[i]->setRenderState(video::RS_CullFace, video::ES_DontUse);

			m_screenNode[i]->AttachNode(rnode);
			m_headNode->addChild(m_screenNode[i]);
			//m_screenNode[i]->setPosition(math::vector3d(0, 0, cameraScreenDistance));
			m_screenNode[i]->setVisible(false);
		}
	}
	m_screenNode[GetEyeIndex(TBee::Eye_Left)]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));
	m_screenNode[GetEyeIndex(TBee::Eye_Right)]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));

}

void RobotCameraState::RescaleMesh(int index,const math::vector3d &scaleFactor)
{
	m_surfaceParams.scale[index] = scaleFactor;
	if (m_surfaceParams.plane)
	{
		m_screenNode[index]->setScale(scaleFactor);

	}
	else
	{
		float radius = m_surfaceParams.radius;
		float hfov = m_surfaceParams.hfov*m_surfaceParams.scale[0].x;
		float vfov = m_surfaceParams.vfov*m_surfaceParams.scale[1].y;
		int segments = m_surfaceParams.segments;
		int vertCount = (segments + 1)*(segments + 1);
		int indCount = 6 * segments*segments;

		float w = 2 * math::PI32*radius*hfov / 360.0f;
		float h = 2 * math::PI32*radius*vfov / 360.0f;
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{


			scene::IMeshBuffer* buffer = m_surface[i]->getMesh()->getBuffer(0) ;

			video::IHardwareStreamBuffer* pos = buffer->getStream(0, video::EMST_Position);

			math::vector3d* posPtr = (math::vector3d*) pos->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			for (int x = 0; x <= segments; ++x)
			{
				float a1 = hfov*(x - (segments + 1) / 2.0f) / (float)(segments + 1);
				for (int y = 0; y <= segments; ++y)
				{
					float a2 = vfov*(y - (segments + 1) / 2.0f) / (float)(segments + 1) + 90;
					float xp = radius*math::sind(a1)*math::sind(a2);
					float yp = radius*math::cosd(a2);
					float zp = radius*math::cosd(a1)*math::sind(a2);

					(*posPtr++).set(xp, yp, zp);

				}
			}
			pos->unlock();
		}
	}
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
		m_sceneManager = new scene::SceneManager(Engine::getInstance().getDevice());
	}
	{
		((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraResolution(m_cameraResolution, m_cameraFPS);
		m_videoSource->Init();
	}

	m_lensCorrectionPP = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_lensCorrectionPP->LoadXML(gFileSystem.openFile("LensCorrection.peff"));
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
		m_headNode = m_sceneManager->createSceneNode();

		scene::LightNode* light= m_sceneManager->createLightNode("Sun");
		light->setPosition(1000);

		core::string vpName[] = { "Left", "Right" };

		for (int i = 0; i < 2; ++i)
		{
			cam[i] = m_sceneManager->createCamera();
			m_viewport[i] = new scene::ViewPort("", cam[i], 0, 0, math::rectf(0, 0, 1, 1), 0);
			m_viewport[i]->SetClearColor(video::SColor(0,0,0, 1));

			video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), pf, true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);

			video::IRenderTargetPtr rt = Engine::getInstance().getDevice()->createRenderTarget(mT(""), renderTargetTex, video::IHardwareBufferPtr::Null, video::IHardwareBufferPtr::Null, false);
			m_viewport[i]->setRenderTarget(rt);
			m_viewport[i]->setOnlyToRenderTarget(true);
			m_viewport[i]->SetAutoUpdateRTSize(true);
			m_viewport[i]->AddListener(this);

			cam[i]->setZNear(0.1);
			cam[i]->setZFar(100000);

			//if (ATAppGlobal::Instance()->oculusDevice)
			cam[i]->setFovY(m_hmdFov);
			cam[i]->setAutoUpdateAspect(true);
			m_camera[i] = cam[i];
			m_headNode->addChild(cam[i]);

		}

		GenerateSurface(true, 100, 100, 20, m_cameraOffsets.z);

		m_camera[GetEyeIndex(TBee::Eye_Left )]->setPosition(math::vector3d(-0.03, 0, 0));
		m_camera[GetEyeIndex(TBee::Eye_Right)]->setPosition(math::vector3d(+0.03, 0, 0));
	}

	m_console->AddToHistory("System Inited.", video::DefaultColors::Green);


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
		m_vehicleModel = m_sceneManager->createSceneNode("VehicleModel");
	//	m_vehicleModel->setPosition(math::vector3d(-47197.953, 100, 61388.07));
		m_vehicleModel->addChild(m_headNode);
		//m_vehicleModel->AttachNode(node);
		{
			CreateARObject(1000, "", math::vector3d(-47197.953, 100, 61388.07), 0);
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
}


bool RobotCameraState::OnEvent(Event* e, const math::rectf& rc)
{
	if (Parent::OnEvent(e, rc)  || m_guimngr->OnEvent(e,&rc))
		return true;
	bool ok = false;



	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			if (evt->key == KEY_SPACE)
			{
				if (m_robotConnector->IsRobotConnected())
					m_robotConnector->EndUpdate();
				else
				{
					if (!m_robotConnector->IsRobotConnected())
						m_robotConnector->ConnectRobot();
					m_robotConnector->StartUpdate();
				}
				ok = true;
			}
			else if (evt->key == KEY_X)
			{
				m_lockAxis[0] = !m_lockAxis[0];
				ok = true;
			}
			else if (evt->key == KEY_Y)
			{
				m_lockAxis[1] = !m_lockAxis[1];
				ok = true;
			}
			else if (evt->key == KEY_Z)
			{
				m_lockAxis[2] = !m_lockAxis[2];
				ok = true;
			}
			else if (evt->key == KEY_C)
			{
				m_headController->Calibrate();
				ok = true;
			}
			else if (evt->key == KEY_H)
			{
				m_robotConnector->SetData("Homing", "", false);
				ok = true;
			}
			else if (evt->key == KEY_P)
			{
			//	GenerateSurface(!m_surfaceParams.plane, m_surfaceParams.hfov, m_surfaceParams.vfov, m_surfaceParams.segments, m_surfaceParams.radius);
				ok = true;
			}
			else if (evt->key == KEY_TAB)
			{
				m_console->SetVisible(!m_console->IsVisible());
				ok = true;
			}
			else if (evt->key == KEY_V && evt->ctrl)
			{
				m_arRoot->setVisible(!m_arRoot->isVisible(),true);
				ok = true;
			}
		}
	}
	return ok;
}
void RobotCameraState::OnEnter(TBee::IRenderingState*prev)
{
	Parent::OnEnter(prev);
	m_videoSource->Open();
	((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Focus, "0.0");

	//m_robotConnector->ConnectRobot();

}

void RobotCameraState::OnExit()
{
	Parent::OnExit();
	m_videoSource->Close();
	m_robotConnector->DisconnectRobot();
}

void RobotCameraState::SetTransformation( const math::vector3d& pos, const math::vector3d &angles)
{
	math::vector3d p;
	if (true)
	{
		math::quaternion q(0, 0, angles.z);
		p.z = m_cameraOffsets.z;
		p.x = m_cameraOffsets.x + math::sind(angles.y)*m_cameraOffsets.z;
		p.y = m_cameraOffsets.y + math::sind(-angles.x)*m_cameraOffsets.z;
	//	m_headNode->setPosition(pos);
		m_screenNode[0]->setPosition(p);
		m_screenNode[1]->setPosition(p);
 		m_screenNode[0]->setOrintation(q);
 		m_screenNode[1]->setOrintation(q);
	}
#if 0
	else
	{
		p.z = m_cameraOffsets.z;
		p.x = m_cameraOffsets.x + math::sind(angles.y)*m_cameraOffsets.z;
		p.y = m_cameraOffsets.y + math::sind(-angles.x)*m_cameraOffsets.z;
	
		p = m_camera[0]->getAbsolutePosition() + m_camera[0]->getAbsoluteOrintation()*p;

		math::vector3d proj1 = m_camera[0]->WorldToScreen(p);

		SetContentsPosition(proj1.x, proj1.y);
		SetContentsRotation(-angles.z);
	}
#endif
	{
// 		m_headNode->setOrintation(m_headRotationOffset);;// +angles);
// 		m_headNode->setPosition(m_headPosOffset);// +pos);
		m_vehicleRef->obj->pos += m_headPosOffset;
		m_vehicleRef->obj->dir += m_headRotationOffset;
		m_vehicleRef->sceneNode->setPosition(m_vehicleRef->obj->pos );
		m_vehicleRef->sceneNode->setOrintation(m_vehicleRef->obj->dir );

		m_headNode->setPosition(pos);

		m_headRotationOffset = 0;	
		m_headPosOffset = 0;
	}
}

float time = 0;
void RobotCameraState::Update(float dt)
{
	Parent::Update(dt);
	m_videoSource->Blit();
	m_guimngr->Update(dt);
	m_robotConnector->UpdateStatus();

	

	math::quaternion q;// = m_robotConnector->GetCurrentHeadRotation();// m_robotConnector->GetHeadRotation();
	math::vector3d pos =  m_robotConnector->GetHeadPosition();


	math::vector3d rot,r ;
	//q.toEulerAngles(r);
	r = m_robotConnector->GetCurrentHeadRotation();
	
	rot.x = r.y;
	rot.z = r.x;
	rot.y = r.z;
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
		m_vehicleModel->setPosition(m_arManager->GetVehicle()->getAbsolutePosition());
		m_vehicleModel->setOrintation(m_arManager->GetVehicle()->getAbsoluteOrintation());
	}
	else
	{
		m_vehicleModel->setPosition(m_vehicleRef->sceneNode->getAbsolutePosition());
		m_vehicleModel->setOrintation(m_vehicleRef->sceneNode->getAbsoluteOrintation());
	}

	InputManager* mngr = NCAppGlobals::Instance()->App->GetInputManager();
	controllers::IKeyboardController* kb = mngr->getKeyboard();
	controllers::IMouseController* m = mngr->getMouse();

	if (kb->getKeyState(KEY_LCONTROL))
	{
		math::vector3d angles;
		angles.x = kb->getKeyState(KEY_LEFT) - kb->getKeyState(KEY_RIGHT);
		angles.y = kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN);

		//	m_headNode->rotate(angles*100*dt, scene::TS_Local);

		m_cameraOffsets.x += angles.x*dt*0.1f;
		m_cameraOffsets.y += angles.y*dt*0.1f;
	}
	if (kb->getKeyState(KEY_LCONTROL))
	{
		math::vector3d speed;
		speed.x = kb->getKeyState(KEY_A) - kb->getKeyState(KEY_D);
		speed.z = kb->getKeyState(KEY_W) - kb->getKeyState(KEY_S);
		speed.y = kb->getKeyState(KEY_Q) - kb->getKeyState(KEY_Z);
		speed *=  dt;

		math::vector3d pos;
		//pos = m_headNode->getPosition();

		pos = m_headNode->getOrintation()*math::vector3d::XAxis*speed.x;
		pos += m_headNode->getOrintation()*math::vector3d::YAxis*speed.y;
		pos += m_headNode->getOrintation()*math::vector3d::ZAxis*speed.z;
		//m_headNode->setPosition(pos);

		m_headPosOffset += pos;

		//	m_headNode->rotate(angles*100*dt, scene::TS_Local);

		// 			m_cameraOffsets.x += angles.x*dt*0.1f;
		// 			m_cameraOffsets.y += angles.y*dt*0.1f;
	}
	if (m->isPressed(controllers::EMB_Right))
	{
		math::vector3d angles;
		angles.x = m->getDY();
		angles.y = -m->getDX();
		angles *= 10 * dt;
		m_headRotationOffset += angles;

		//math::quaternion q = math::quaternion(angles.y,  m_headNode->getOrintation().getYAxis());

		//m_headNode->rotate(q, scene::TS_Parent);
		
// 		q = math::quaternion(angles.x, m_headNode->getOrintation().getXAxis());
// 		m_headNode->rotate(q, scene::TS_Parent);

		// 			m_cameraOffsets.x += angles.x*dt*0.1f;
		// 			m_cameraOffsets.y += angles.y*dt*0.1f;
	}
}


class TextureRenderTarget :public video::IRenderArea
{
protected:
	video::ITexturePtr m_tex;
public:
	TextureRenderTarget(video::ITexturePtr tex){ m_tex = tex; }
	virtual~TextureRenderTarget()
	{
	}
	virtual const video::ITexturePtr& GetColorTexture(int i = 0) { return m_tex; }
	virtual int GetColorTextureCount() { return 1; }
	virtual void Resize(int x, int y) {}
	virtual math::vector2di GetSize()
	{
		return math::vector2di(m_tex->getSize().x, m_tex->getSize().y);
	}

};


video::IRenderTarget* RobotCameraState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	
	video::IVideoDevice* device = Engine::getInstance().getDevice();
	int index = GetEyeIndex(eye);
	Parent::Render(rc, eye);

#if 1
	video::ITexture* camTex = m_videoSource->GetEyeTexture(index);;//gTextureResourceManager.loadTexture2D("Checker.png");// //

	video::ShaderSemanticTable::getInstance().setRenderPass(0);

	math::vector3d scale;
	scale.x = (float)camTex->getSize().x / (float)camTex->getSize().y;
	scale.y = 1;
	scale.z = 1;
	if (m_useLensCorrection )
	{
		math::vector2d size(rc.getSize());
		video::ParsedShaderPP::MappedParams* texRect = m_lensCorrectionPP->GetParam("texRect");

		if (texRect)
		{
			math::rectf r = m_videoSource->GetEyeTexCoords(index);
			texRect->SetValue(math::vector4d(r.ULPoint.x, r.ULPoint.y, r.BRPoint.x, r.BRPoint.y));
		}
		if (m_camConfigDirty)
		{
			video::ParsedShaderPP::MappedParams* OpticalCenter = m_lensCorrectionPP->GetParam("OpticalCenter");
			video::ParsedShaderPP::MappedParams* FocalCoeff = m_lensCorrectionPP->GetParam("FCoff");
			video::ParsedShaderPP::MappedParams* KPCoeff = m_lensCorrectionPP->GetParam("KCoff");

			if (OpticalCenter)
				OpticalCenter->SetValue(m_cameraConfiguration->OpticalCenter);
			if (FocalCoeff)
				FocalCoeff->SetValue(m_cameraConfiguration->FocalCoeff);
			if (KPCoeff)
				KPCoeff->SetValue(m_cameraConfiguration->KPCoeff);
		}

		m_lensCorrectionPP->Setup(math::rectf(0, size));
		m_lensCorrectionPP->render(&TextureRenderTarget(camTex));
		camTex = m_lensCorrectionPP->getOutput()->GetColorTexture();
	}
	if (scale != m_surfaceParams.scale[index])
	{
		RescaleMesh(index,scale);
	}
	device->set3DMode();
#endif
	m_screenNode[index]->setVisible(true);
	//m_screenNode[index]->setScale(scale);
	m_screenMtrl[index]->setTexture(camTex, 0);
	m_viewport[index]->setAbsViewPort(rc);
	m_viewport[index]->draw();

	video::TextureUnit tex;

	device->setRenderTarget(m_renderTarget[index]);
	tex.SetTexture(m_viewport[index]->getRenderTarget()->GetColorTexture());
	device->useTexture(0, &tex);
	math::rectf tc = math::rectf(0, 0, 1, 1);
	device->draw2DImage(rc, 1, 0, &tc);
	m_screenNode[index]->setVisible(false);

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
	video::IVideoDevice* device = Engine::getInstance().getDevice();
//	device->set3DMode();
	video::RenderPass pass(0);
	pass.setRenderState(video::RS_Lighting, video::ES_DontUse);
	pass.setRenderState(video::RS_ZTest, video::ES_DontUse);
	pass.SetThickness(2);
	device->useRenderPass(&pass);
	device->setTransformationState(video::TS_WORLD, m_headNode->getAbsoluteTransformation());

	math::vector3d origin;
	origin = 0;
	origin += math::vector3d::XAxis* m_cameraOffsets.x;
	origin += math::vector3d::YAxis* m_cameraOffsets.y;
	device->draw3DLine(origin, origin + math::vector3d::ZAxis * m_cameraOffsets.z, video::SColor(1, 0, 0, 1));


	int n =30;
	for (int i = 0; i < n; ++i)
	{
		math::vector3d a;
		math::vector3d b;
		a.z = m_cameraOffsets.z;
		b.z = m_cameraOffsets.z;
		float step = (i - n*0.5f)*0.1f;;

		a.y = -1;
		b.y = 1;
		b.x = a.x = step;

		float strength = pow(1 - fabs((float)i - n*0.5f) / (float)(n*0.5f),3);

		device->draw3DLine(a, b, video::SColor( strength, 0, 0, 1));

		a.x = -1;
		b.x = 1;
		b.y = a.y = step;
		a.z = m_cameraOffsets.z;
		b.z = m_cameraOffsets.z;
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
	if (font)
	{
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
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		{

			core::string msg = mT("Camera Offset: ") + core::StringConverter::toString(math::vector2d(m_cameraOffsets.x, m_cameraOffsets.y));
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		attr.fontColor.Set(1, 1, 1, 1);
		if (m_robotConnector->GetHeadController())
		{
			math::vector3d head;
			 m_robotConnector->GetHeadRotation().toEulerAngles(head);
			core::string msg = mT("Head Rotation: ") + core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;

			head = m_robotConnector->GetHeadPosition();
			msg = mT("Head Position: ") + core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
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
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			msg = mT("Robot Rotation: ") + core::StringConverter::toString(rot);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;


		}

		if (m_arManager->GetVehicle())
		{

			scene::ISceneNode* v = m_arManager->GetVehicle();

			core::string msg = mT("Car Position: ") + core::StringConverter::toString(v->getPosition());
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			math::vector3d angles;
			v->getOrintation().toEulerAngles(angles);
			msg = mT("Car Heading: ") + core::StringConverter::toString(angles);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;


		}

		core::string msg = mT("Is Homing: ");
		msg += m_robotComm->IsHoming() ? "Yes" : "No";
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		m_guiRenderer->Flush();
	}

}

void RobotCameraState::LoadFromXML(xml::XMLElement* e)
{
	Parent::LoadFromXML(e);



	xml::XMLElement* c = e->getSubElement("Calibration");
	if (c)
		m_headController->LoadFromXML(c);


	xml::XMLAttribute* attr;


	attr = e->getAttribute("Size");
	if (attr)
	{
		m_cameraResolution = core::StringConverter::toVector2d(attr->value);
	}
	attr = e->getAttribute("FPS");
	if (attr)
	{
		m_cameraFPS = core::StringConverter::toInt(attr->value);
	}

	core::string camConfigName = e->getValueString("CameraConfiguration");

	m_useLensCorrection = e->getValueBool("UseLensCorrection");
	math::vector2d proj = core::StringConverter::toVector2d(e->getValueString("CameraProjection"));
	m_cameraOffsets.x = proj.x;
	m_cameraOffsets.y = proj.y;


	m_cameraConfiguration = NCAppGlobals::Instance()->camConfig->GetCameraConfiguration(camConfigName);


	m_camConfigDirty = true;
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

void RobotCameraState::CreateARObject(uint id, const core::string& name, const math::vector3d& pos, const math::vector3d& dir)
{

	ARGroup *grp = new ARGroup();
	ARPredef *predef = new ARPredef;
	predef->name =name;
	predef->pos = pos;
	predef->dir = dir;

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

	g->objects.begin()->second->obj->pos = pos;
	g->objects.begin()->second->obj->dir = dir;
	g->objects.begin()->second->sceneNode->setPosition(pos);
	g->objects.begin()->second->sceneNode->setOrintation(dir);
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
	m_camera[0]->setFovY(fov);
	m_camera[1]->setFovY(fov);
}



}
}
