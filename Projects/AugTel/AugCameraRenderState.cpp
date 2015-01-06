

#include "stdafx.h"
#include "AugCameraRenderState.h"

#include "TextureResourceManager.h"
#include "MeshFileCreatorManager.h"
#include "ShaderResourceManager.h"
#include "ATAppGlobal.h"
#include "OpenNIManager.h"
#include "OpenNIUtils.h"
#include "GUIThemeManager.h"
#include "IGUIPanelElement.h"

#include "PhysXManager.h"

#include "OptiTrackClient.h"

#include "VTBaseState.h"
#include "OptiTrackDataSource.h"

#include "HeadCameraComponent.h"
#include "CameraComponent.h"
#include "HeadMount.h"
#include "GameComponentCreator.h"
#include "DebugDrawManager.h"
#include "ICameraVideoSource.h"

#include "CRobotConnector.h"
#include "TBRobotInfo.h"
#include "RobotInfoManager.h"

#include "OpenNIHandler.h"

#include "MeshBufferData.h"
#include "IMeshBuffer.h"
#include "IHardwareStreamBuffer.h"
#include "DepthVisualizer.h"
#include "DataCommunicator.h"
#include "ParsedShaderPP.h"

#include "ATAppGlobal.h"
#include "RenderWindow.h"
#include "Application.h"

#include "NodeHeadController.h"

#include "LocalRobotCommunicator.h"
#include "RemoteRobotCommunicator.h"

#include "FontResourceManager.h"
#include "GstStreamerVideoSource.h"

#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "SceneComponent.h"

#include "CameraConfigurationManager.h"
#include "TextureRTWrap.h"

#include "LocalCameraVideoSource.h"
#include "ICameraVideoGrabber.h"
#include "RobotCommunicatorComponent.h"

#include "FadeSceneEffect.h"
#include "RefractionSceneEffect.h"
#include "MeshResourceManager.h"
#include "AugTelSceneContext.h"


#include "VirtualHandsController.h"
#include "LeapMotionHandsController.h"
#include "JoystickDefinitions.h"
#include "IInputController.h"

#include "GstNetworkAudioStreamer.h"
#include "GstNetworkVideoStreamer.h"

#include "UI3DRenderNode.h"


namespace mray
{
namespace AugTel
{


	AugCameraRenderState::AugCameraRenderState(TBee::ICameraVideoSource* src , TBee::IRobotCommunicator* comm, const core::string& name)
		:Parent(name)
{

// 	m_eyes[0].flip90 = 0;
// 	m_eyes[1].flip90 = 0;
// 
// 	m_eyes[0].cw = 0;
// 	m_eyes[1].cw = 1;

	m_status = EConnectingRobot;
//	m_camVideoSrc = src;

	SetVideoSource(src);

	m_openNiHandler = new TBee::OpenNIHandler();
	m_openNiHandler->SetScale(0.5);
	m_depthTime = 0;

	m_depthVisualizer = new DepthVisualizer();
	m_robotConnector->SetCommunicator(comm);

	m_viewDepth = false;

	m_enableHands = false;
	m_enableMic = false;
	m_enableVideo = false;
	m_showScene = false;

	m_showDebug = false;
	m_vtState = new VTBaseState();

	m_loadScreen = new LoadingScreen();

	m_context = new AugTelSceneContext();
	m_context->videoSource = src;
	

	m_streamer = new video::GstStreamBin();


}
AugCameraRenderState::~AugCameraRenderState()
{
	m_streamer->ClearStreams(true);
	delete m_loadScreen;
	delete m_vtState;
	m_gameManager = 0;
	m_sceneManager = 0;
	m_phManager = 0;
	delete m_openNiHandler;
	delete m_depthVisualizer;
	delete m_context;

	{
		for (int i = 0; i < m_hands.size(); ++i)
		{
			delete m_hands[i];
		}

	}
}


bool AugCameraRenderState::OnEvent(Event* e, const math::rectf& rc)
{
// 	if (Parent::OnEvent(e, rc))
// 		return true;
	bool ok = false;

	if (m_guiManager)
	{
		if (m_guiManager->OnEvent(e, &rc))
			return true;
	}
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
	//		printf("Key Pressed : %d, %d\n", evt->key,(int)evt->Char);
			if (evt->key == KEY_F11)
			{
				gTextureResourceManager.writeResourceToDist(m_renderTarget[0]->GetColorTexture(), "depth.tga");
			}
			else
			if ( evt->key == KEY_R)
			{
				if (evt->ctrl)
				{
					math::vector3d pos = m_context->headNode->getAbsolutePosition();
					math::vector3d ang;
					m_context->headNode->getAbsoluteOrintation().toEulerAngles(ang);
					printf("%2.3f,%2.3f,%2.3f   %2.3f,%2.3f,%2.3f\n", pos.x, pos.y, pos.z,ang.x,ang.y,ang.z);
				}else
					m_context->headNode->SetDisabled(!m_context->headNode->IsDisabled());
				ok = true;
			}
			else if ( evt->key == KEY_F10)
			{
				core::string name = "screenShots/ScreenShot_";
				name += core::StringConverter::toString((int)gEngine.getTimer()->getSeconds());
				name += ".png";
				gTextureResourceManager.writeResourceToDist(m_rtTexture[0], name);

				ok = true;
			}
			else if (evt->key == KEY_D && evt->ctrl)//show/hide depth
			{
				m_viewDepth = !m_viewDepth;
				ok = true;
			}
			else if (evt->key == KEY_F && evt->ctrl)//show/hide depth
			{
				m_showDebug = !m_showDebug;
				ok = true;
			}
			
			else if (evt->key==82 || evt->key==KEY_SPACE)
			{
				 if (m_status == EWaitStart)
					_ChangeState(EConnectingRobot);
				 else 
					 _ChangeState(EWaitStart);
				ok = true;
			}
			else if (evt->key == KEY_ESCAPE)// && evt->lshift)
			{
				m_exitCode = STATE_EXIT_CODE;
				ok = true;
			}
			else
			if (evt->key == KEY_C || evt->key==78)
			{
				m_vtState->CalibratePosition();

				m_robotConnector->GetHeadController()->Recalibrate();
				m_robotConnector->GetRobotController()->Recalibrate();
				ok = true;
			}
			else if (evt->key == KEY_F12 || evt->key == 71/*&& evt->ctrl*/)
			{
				m_takeScreenShot = true;

				ok = true;

			}
			else if (false && evt->key >= KEY_1 && evt->key < KEY_1 + m_hands.size())
			{
				int idx = evt->key - KEY_1;
				for (int i = 0; i < m_hands.size(); ++i)
				{
					m_hands[i]->SetEnabled(i == idx);
				}
				ok = true;
			}
			else if (evt->key == KEY_V && evt->ctrl) //enable video
			{
				_EnableVideo(!_IsVideoEnabled());
				ok = true;
			}
			else if (evt->key == KEY_H && evt->ctrl || evt->key == 72)//Show/Hide arms
			{
				_EnableHands(!_IsHandsEnabled());
				ok = true;
			}
			else if (evt->key == KEY_M && evt->ctrl) //enable video
			{
				_EnableMic(!_IsMicEnabled());
				ok = true;
			}
			else if (evt->key == KEY_N && evt->ctrl )//show/hide navigation  //
			{
				m_screenLayout->NavElem->SetVisible(!m_screenLayout->NavElem->IsVisible());
				ok = true;
			}else if (evt->key == KEY_N /*|| evt->key == 78*/)
			{
				if (!m_screenLayout->ScenarioElem->Next())
					m_screenLayout->ScenarioElem->Reset();
				ok = true;
			}
			
		}
	}
	if (e->getType() == ET_Joystick)
	{
		JoystickEvent* evt = (JoystickEvent*)e;
		if (evt->event == JET_BUTTON_PRESSED)
		{
			if (evt->button == JOYSTICK_SelectButton)
			{
				m_exitCode = STATE_EXIT_CODE;
				m_robotConnector->EndUpdate();
				ok = true;
			}
			else if (evt->button == JOYSTICK_StartButton)
			{
				if (m_status == EWaitStart)
					_ChangeState(EConnectingRobot);
				else
					_ChangeState(EWaitStart);
				/*
				if (m_robotConnector->IsRobotConnected())
					m_robotConnector->EndUpdate();
				else
					m_robotConnector->StartUpdate();*/
				ok = true;
			}
		}
	}
	return ok;
}



void AugCameraRenderState::_CreatePhysicsSystem()
{
	core::string maxIter = gAppData.GetValue("Physics", "MaxIterations");
	core::string maxTimestepDiv = gAppData.GetValue("Physics", "MaxTimeStepDiv");
	core::string simulationStepDiv = gAppData.GetValue("Physics", "SimulationStepDiv");

	float simStep;

	if (simulationStepDiv == "")
		simStep = 1.0f / 60.0f;
	else simStep = 1.0f / core::StringConverter::toFloat(simulationStepDiv);

	physics::PhysicsSystemDesc desc;
	desc.gravity.set(0, -9.8 * 1, 0);
	if (maxIter == "")
		desc.maxIter = 32;
	else desc.maxIter = core::StringConverter::toInt(maxIter);

	if (maxTimestepDiv == "")
		desc.maxTimestep = 1.0f / (16 * 60.0f);
	else desc.maxTimestep = 1.0f / core::StringConverter::toFloat(maxTimestepDiv);
	desc.useFixedTimeStep = true;
	m_phManager = new physics::PhysXManager(&desc);

	//if(VTAppGlobals::IsDebugging)
	m_phManager->ConnectToRemoteDebugger();

}


void AugCameraRenderState::_createHands()
{
}

void AugCameraRenderState::_EnableHands(bool e)
{
	m_enableHands = e;
	m_interfaceUI->EnableHand(m_enableHands);
	m_hands[0]->SetEnabled(m_enableHands);
}
void AugCameraRenderState::_EnableVideo(bool e)
{
	m_enableVideo = e;
	m_interfaceUI->EnableVideo(m_enableVideo);
	if (m_enableVideo)
		m_streamer->GetStream("Video")->SetPaused(false);
	else
		m_streamer->GetStream("Video")->SetPaused(true);

}
void AugCameraRenderState::_EnableMic(bool e)
{
	m_enableMic = e;
	m_interfaceUI->EnableMic(m_enableMic);
	if (m_enableMic)
		m_streamer->GetStream("Audio")->SetPaused(false);
	else
		m_streamer->GetStream("Audio")->SetPaused(true);

}
void AugCameraRenderState::InitState()
{
	Parent::InitState();

	{
		_CreatePhysicsSystem();
		m_gameManager = new game::GameEntityManager();
		m_sceneManager = new scene::SceneManager(Engine::getInstance().getDevice());
		m_guiManager = new GUI::GUIManager(Engine::getInstance().getDevice());
		m_debugRenderer = new scene::DebugDrawManager(Engine::getInstance().getDevice());

		m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
		m_guiroot = (GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""), m_guiManager);
		m_guiroot->SetDocking(GUI::EED_Fill);
		m_guiManager->SetRootElement(m_guiroot);

		m_gameManager->SetPhysicsManager(m_phManager);
		m_gameManager->SetSceneManager(m_sceneManager);

		m_showScene = true;

		gAppData.guiManager = m_guiManager;

		m_context->entManager = m_gameManager;
	}

	{
		GUI::GUIOverlay* screenOverlay = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIAugTelScreenLayout.gui");
		m_screenLayout = new GUI::GUIAugTelScreen(m_guiManager);
		screenOverlay->CreateElements(m_guiManager, m_guiroot, 0, m_screenLayout);
	}

	{
		m_effects = new SceneEffectManager();
		{
			FadeSceneEffect* fadeEffect = new FadeSceneEffect();
			fadeEffect->SetTargetColor(video::SColor(1, 1, 1, 1));
			fadeEffect->SetTime(3);
			m_effects->AddEffect("Start", fadeEffect, true, true);
		}
		if (0)
		{
			RefractionSceneEffect* effect = new RefractionSceneEffect();
			effect->SetAmount(0.1f);
			m_effects->AddEffect("Refraction", effect, true, true);
		}


		m_effects->Init();
	}
	{
		gShaderResourceManager.parseShaderXML(gFileSystem.openFile("AT_Shaders.shd"));
		gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile("AT_materials.mtrl"));
	}
	{
		m_interfaceUI = new GUI::GUIInterfaceScreenImpl(gAppData.App->GetPreviewGUIManager());
	}
	{
		m_loadScreen->Init();
	}
	{

		HeadMount* hm = new HeadMount(m_sceneManager, 1);
		scene::CameraNode* cam[2];
		video::EPixelFormat pf = video::EPixel_Float16_RGBA;//video::EPixel_R8G8B8A8;//


		for (int i = 0; i < 2; ++i)
		{

			cam[i] = m_sceneManager->createCamera();
			m_viewport[i] = new scene::ViewPort("", cam[i], 0, 0, math::rectf(0, 0, 1, 1), 0);
			m_viewport[i]->SetClearColor(video::SColor(0, 0, 0, 0));

			video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), pf, true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);

			video::IRenderTargetPtr rt = Engine::getInstance().getDevice()->createRenderTarget(mT(""), renderTargetTex, video::IHardwareBufferPtr::Null, video::IHardwareBufferPtr::Null, false);
			m_viewport[i]->setRenderTarget(rt);
			m_viewport[i]->setOnlyToRenderTarget(true);
			m_viewport[i]->SetAutoUpdateRTSize(true);
			m_viewport[i]->enablePostProcessing(true);
			m_viewport[i]->AddListener(this);

			if (false)
			{
				renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(1, pf, true);
				renderTargetTex->setBilinearFilter(false);
				renderTargetTex->setTrilinearFilter(false);
				renderTargetTex->setMipmapsFilter(false);
				rt->attachRenderTarget(renderTargetTex, 1);
				if (false)
				{
					renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(1, pf, true);
					renderTargetTex->setBilinearFilter(false);
					renderTargetTex->setTrilinearFilter(false);
					renderTargetTex->setMipmapsFilter(false);
					rt->attachRenderTarget(renderTargetTex, 2);
				}

				video::ParsedShaderPP* pp = new video::ParsedShaderPP(Engine::getInstance().getDevice());
				pp->LoadXML(gFileSystem.openFile("AugTel.peff"));
				m_viewport[i]->setPostProcessing(pp);
			}

			hm->addChild(cam[i]);
			cam[i]->setZNear(0.1);
			cam[i]->setZFar(1000);

			//if (gAppData.oculusDevice)
			cam[i]->setFovY(m_hmdFov);// m_cameraConfiguration->fov);
			cam[i]->setPosition(math::vector3d(0.03 - 0.06*i, 0.07, 0));
			m_camera[i] = cam[i];

			m_context->cameras[i] = cam[i];
		}

		hm->SetOculus(gAppData.oculusDevice);
		m_sceneManager->addSceneNode(hm);
		m_context->headNode = hm;
		m_context->headNode->SetCamera(m_camera[0], m_camera[1]);


		if (AppData::Instance()->headController == TBee::EHeadControllerType::SceneNode)
		{
			TBee::NodeHeadController* c = dynamic_cast<TBee::NodeHeadController*>(m_robotConnector->GetHeadController());
			c->SetNode(hm);
			c->SetInitialOrientation(hm->getAbsoluteOrintation());
		}
		//load arrow
		{
			m_gameManager->loadFromFile("FrontArrow.xml", 0);
		}
		if (true)
		{
			m_lightSrc = m_sceneManager->createLightNode();
			m_lightSrc->setPosition(math::vector3d(0, 50, 0));
			m_lightSrc->setCastShadows(false);
			/*
			video::ITexturePtr t = gEngine.getDevice()->createEmptyTexture2D(false);
			t->setMipmapsFilter(false);
			t->createTexture(math::vector3di(2048, 2048, 1), video::EPixel_Float16_R);
			m_lightSrc->setShadowMap(gEngine.getDevice()->createRenderTarget("", t, 0, 0, 0));
			*/
		}

		if (false)
		{
			scene::SMeshPtr mesh= gMeshResourceManager.loadMesh("car.3ds",true);
			if (mesh)
			{
				scene::MeshRenderableNode* n = new scene::MeshRenderableNode(mesh);
				scene::ISceneNode* node = m_sceneManager->createSceneNode();
				node->AttachNode(n);
			}
		}

		{
			//Setup 3D UI

			scene::UI3DRenderNode* n = new scene::UI3DRenderNode();
			n->SetUIManager(m_guiManager);
			n->SetUISize(math::vector2d(1024, 1024));
			scene::ISceneNode* node = m_sceneManager->createSceneNode();
			node->AttachNode(n);
			node->setScale(math::vector3d(1.5, 1.5, 2));

			node->setPosition(math::vector3d(0, 0, 1.5));
			m_context->headNode->addChild(node);
		}
	}
	{
		for (int i = 0; i < m_hands.size();++i)
		{
			m_hands[i]->Init(m_context);
		}
	}
	{
		if (m_videoSource->IsLocal())
		{
			m_openNiHandler->Init();
		}
		else
		{
			m_openNiHandler->CreateDepthFrame(320, 240);
		}
		m_openNiHandler->GetNormalCalculator().SetCalculateNormals(true);
		math::vector2di sz = m_openNiHandler->GetSize();

		m_depthVisualizer->Init();
		m_depthVisualizer->SetViewNormals(true);
		m_depthVisualizer->SetDepthFrame(m_openNiHandler->GetNormalCalculator().GetDepthFrame());

	}

	//if (false)
	{
		//init streamers
		video::GstNetworkAudioStreamer* as = new video::GstNetworkAudioStreamer();
		m_streamer->AddStream(as, "Audio");
		video::GstNetworkVideoStreamer* vs = new video::GstNetworkVideoStreamer();
		vs->SetCameras(0, 0);
		vs->SetResolution(640, 480);
		vs->SetBitRate(500);
		m_streamer->AddStream(vs, "Video");
	}
}

void AugCameraRenderState::OnEnter(IRenderingState*prev)
{
	gAppData.optiDataSource->Connect(m_optiProvider);

	if (m_videoSource->IsLocal())
	{
		m_openNiHandler->Start(320, 240);
	}

	gAppData.headObject = m_context->headNode;
	gAppData.depthProvider = m_openNiHandler;
	gAppData.cameraProvider = m_videoSource;
	gAppData.dataCommunicator->AddListener(this);
	gAppData.dataCommunicator->Start(gAppData.TargetCommunicationPort);
	gAppData.robotConnector = m_robotConnector;

	TBee::TBRobotInfo* ifo = gAppData.selectedRobot;
	
	{
		TBee::GstStreamerVideoSource* src = dynamic_cast<GstStreamerVideoSource*>(m_videoSource);
		if (src && ifo)
		{
			src->SetIP(ifo->IP, gAppData.TargetVideoPort, gAppData.TargetAudioPort, gAppData.RtcpStream);
		}
		m_videoSource->Open();
	}
	
	if (ifo)
		m_robotConnector->ConnectRobotIP(ifo->IP, gAppData.TargetVideoPort, gAppData.TargetAudioPort, gAppData.TargetCommunicationPort, gAppData.RtcpStream);

	m_robotConnector->SetData("depthSize", "", false);
	//m_robotConnector->EndUpdate();

	m_screenLayout->OnDisconnected();

	m_effects->SetAcive("Start", true);
	m_effects->Begin();

	m_status = ENone;
	m_loadScreen->Start();

	gAppData.App->GetPreviewGUIManager()->GetRootElement()->AddElement(m_interfaceUI);
	{

		for (int i = 0; i < m_hands.size(); ++i)
		{
			m_hands[i]->Start(m_context);
		}
	}
	if (true)
	{
		m_streamer->GetStream("Audio")->BindPorts(ifo->IP, gAppData.TargetAudioPort, gAppData.RtcpStream);
		m_streamer->GetStream("Audio")->CreateStream();

		m_streamer->GetStream("Video")->BindPorts(ifo->IP, gAppData.TargetVideoPort, gAppData.RtcpStream);
		m_streamer->GetStream("Video")->CreateStream();

		m_streamer->Stream();
	}
//	if (false)
	{
		//enable hands,video and mic
		_EnableHands(true);
		_EnableMic(true);
		_EnableVideo(true);
	}

	Parent::OnEnter(prev);
}

void AugCameraRenderState::OnExit()
{
	Parent::OnExit();

	gAppData.optiDataSource->Disconnect();
	m_videoSource->Close();

	m_openNiHandler->Close();

	gAppData.dataCommunicator->Stop();
	gAppData.dataCommunicator->RemoveListener(this);
	gAppData.App->GetPreviewGUIManager()->GetRootElement()->RemoveElement(m_interfaceUI);

	gAppData.cameraProvider = 0;
	gAppData.robotConnector = 0;

	m_loadScreen->End();

	m_streamer->CloseAll();
	{

		for (int i = 0; i < m_hands.size(); ++i)
		{
			m_hands[i]->End(m_context);
		}

	}
}
void AugCameraRenderState::onRenderBegin(scene::ViewPort*vp)
{
	ETargetEye eye;
	if (vp == m_viewport[GetEyeIndex(Eye_Left)])
	{
		eye = Eye_Left;
	}
	else
	{
		eye = Eye_Right;
	}
	/*
	Engine::getInstance().getDevice()->set2DMode();
	video::TextureUnit tu;
	tu.SetTexture(gTextureResourceManager.loadTexture2D("00353_full.jpg"));
	Engine::getInstance().getDevice()->useTexture(0, &tu);
	Engine::getInstance().getDevice()->draw2DImage(vp->getAbsRenderingViewPort(), 1);
	*/

	Engine::getInstance().getDevice()->set3DMode();

}
void AugCameraRenderState::onRenderDone(scene::ViewPort*vp)
{
	if (gAppData.IsDebugging)
	{
		gEngine.getDevice()->set3DMode();
		gEngine.getDevice()->unuseShader();
		m_debugRenderer->EndDraw();
	}
}

void AugCameraRenderState::_RenderUI(const math::rectf& rc, math::vector2d& pos)
{
	Parent::_RenderUI(rc,pos);

	if (!AppData::Instance()->IsDebugging)
		return;
	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	AppData* app = AppData::Instance();


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
		r.ULPoint = pos;

		core::string msg = mT("Depth Center:") + core::StringConverter::toString(m_openNiHandler->GetCenter());
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		msg = mT("Depth Center:") + core::StringConverter::toString(m_openNiHandler->GetScale());
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		

		if (m_robotConnector->GetHeadController())
		{
			math::vector3d head;
			math::quaternion q, q2(m_robotConnector->GetHeadRotation());
			q = q2;
			q.x = q2.z;
			q.y = q2.x;
			q.z = q2.y;
			q.toEulerAngles(head);
			/*
			math::matrix4x4 m;
			qtomatrix(m, q);

			char buff[512];
			sprintf(buff, "%0.2f, %0.2f, %0.2f, %0.2f",)


			core::string msg = mT(" ") + core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			*/

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
		for (int i = 0; i < m_irSensor.size(); ++i)
		{
			std::stringstream ss;
			ss << mT("Sensor[") << i << "]: " << m_irSensor[i];
			msg = ss.str();
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		m_guiRenderer->Flush();
	}
}

#define DRAW_GRID false
void AugCameraRenderState::_RenderStarted(const math::rectf& rc, ETargetEye eye)
{
	video::IVideoDevice* device = Engine::getInstance().getDevice();
	int index = GetEyeIndex(eye);

	if (m_depthTime > 0.15)
	{
		m_depthTime = 0;
		//request depth data
		m_robotConnector->SetData("depth#1", core::StringConverter::toString(math::recti(0, 0, 320, 240)), false);
	}


	{

		for (int i = 0; i < m_hands.size(); ++i)
		{
			m_hands[i]->RenderStart(rc, eye);
		}

	}
	m_depthVisualizer->Update();

	//m_camVideoSrc->Blit();

	if (m_showScene)
	{
		m_viewport[index]->setAbsViewPort(rc);
		m_viewport[index]->draw();
	}
	if (gAppData.IsDebugging || m_showDebug)
	{
		m_gameManager->DebugRender(m_debugRenderer);
		if (DRAW_GRID)
		{
			int n = 20;
			for (int i = 0; i < n; ++i)
			{
				m_debugRenderer->AddLine(math::vector3d(-10, 0, 0.5f*(i - n / 2)), math::vector3d(10, 0, 0.5f*(i - n / 2)), video::DefaultColors::Gray);
				m_debugRenderer->AddLine(math::vector3d(0.5f*(i - n / 2), 0, -10), math::vector3d(0.5f*(i - n / 2), 0, 10), video::DefaultColors::Gray);
			}
		}
	}



	math::rectf vprect = rc;
	video::TextureUnit tex;
	//Parent::Render(vp->getAbsRenderingViewPort(), eye);
	Parent::Render(rc, eye);

	if (/*m_openNiHandler->IsStarted() &&*/ m_viewDepth)
	{
		tex.SetTexture(m_depthVisualizer->GetNormalsTexture());
		device->useTexture(0, &tex);
		math::rectf r(m_openNiHandler->GetCenter() - m_openNiHandler->GetScale()*0.5, m_openNiHandler->GetCenter() + m_openNiHandler->GetScale()*0.5);
		r.ULPoint *= rc.getSize();
		r.BRPoint *= rc.getSize();

		device->draw2DImage(r, video::SColor(1, 1, 1, 0.5));
		//	m_openNiHandler->Render(rc, 0.5f);
	}
	if (AppData::Instance()->IsDebugging && false)
		m_gameManager->GUIRender(Parent::m_guiRenderer, vprect);

	if (m_showScene)
	{
		tex.SetTexture(m_viewport[index]->getRenderTarget()->GetColorTexture());
		device->useTexture(0, &tex);
		math::rectf tc = math::rectf(0, 0, 1, 1);
		device->draw2DImage(vprect, 1, 0, &tc);
	}
	if (gAppData.IsDebugging)
	{
		for (int i = 0; i < m_hands.size(); ++i)
		{
			m_hands[i]->DebugRender(m_debugRenderer, rc, eye);
		}

		m_debugRenderer->StartDraw(m_viewport[index]->getCamera());
	}

	//math::rectf vp(0, m_renderTarget[index]->GetSize());
	//m_guiManager->DrawAll(&vp);


	/*video::ITexture* ret= m_effects->Render(m_renderTarget[index]->GetColorTexture(), rc);
	device->setRenderTarget(m_renderTarget[index],false);

	tex.SetTexture(ret);
	device->useTexture(0, &tex);
	device->draw2DImage(vprect, 1);
	*/
}
video::IRenderTarget* AugCameraRenderState::Render(const math::rectf& rc, ETargetEye eye)
{
	video::IVideoDevice* device = Engine::getInstance().getDevice();
	int index = GetEyeIndex(eye);
	Parent::Render(rc, eye);
	switch (m_status)
	{
	case mray::AugTel::AugCameraRenderState::ENone:
		break;
	case mray::AugTel::AugCameraRenderState::EWaitStart:
	{
		math::rectf vp(0, m_renderTarget[index]->GetSize());
		m_guiManager->DrawAll(&vp);
	}break;
	case mray::AugTel::AugCameraRenderState::EConnectingRobot:
			m_loadScreen->Draw(rc);
		break;
	case mray::AugTel::AugCameraRenderState::EStarted:
		_RenderStarted(rc, eye);
		break;
	default:
		break;
	}
	if (m_takeScreenShot)
	{
		ulong currTime = gEngine.getTimer()->getMilliseconds();

		core::string name = gFileSystem.getAppPath() + "screenShots/Final_";
		name += core::StringConverter::toString(currTime);
		name += ".png";
		gTextureResourceManager.writeResourceToDist(m_renderTarget[0]->GetColorTexture(0), name);
		m_takeScreenShot = false;
	}
	return m_renderTarget[index];


}

void AugCameraRenderState::_UpdateStarted(float dt)
{
	m_sceneManager->update(dt);
	m_gameManager->Update(dt);
	//m_guiManager->Update(dt);
	m_phManager->update(dt);
	m_debugRenderer->Update(dt);

	m_effects->Update(dt);

	m_openNiHandler->Update(dt);


	{

		for (int i = 0; i < m_hands.size(); ++i)
		{
			m_hands[i]->Update(dt);
		}

	}
	controllers::IKeyboardController* kb = gAppData.inputMngr->getKeyboard();

	if (kb->getKeyState(KEY_LSHIFT))
	{
		math::vector2d s = m_openNiHandler->GetScale();
		s.x += (kb->getKeyState(KEY_K) - kb->getKeyState(KEY_J))*dt*0.5f;
		s.y += (kb->getKeyState(KEY_I) - kb->getKeyState(KEY_M))*dt*0.5f;

		math::vector2d c = m_openNiHandler->GetCenter();
		c.x += (kb->getKeyState(KEY_RIGHT) - kb->getKeyState(KEY_LEFT))*dt*0.5f;
		c.y -= (kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN))*dt*0.5f;
		m_openNiHandler->SetScale(s);
		m_openNiHandler->SetCenter(c);
	}


	m_depthTime += dt;

	float f = m_focus;
	m_focus += (gAppData.inputMngr->getKeyboard()->getKeyState(KEY_ADD) - gAppData.inputMngr->getKeyboard()->getKeyState(KEY_MINUS))*0.5*dt;
	m_focus = math::clamp(m_focus, 0.0f, 1.0f);
	if (m_videoSource->IsLocal() && f != m_focus)
	{
		TBee::LocalCameraVideoSource* src = dynamic_cast<TBee::LocalCameraVideoSource*>(m_videoSource);
		//	src->GetCamera(0)->SetParameter(video::ICameraVideoGrabber::Param_Focus, core::StringConverter::toString(m_focus));
	}

	{
		const math::vector2d &s= m_robotConnector->GetSpeed();
		float rotation = m_robotConnector->GetRotation();
		math::vector3d h;
		m_robotConnector->GetHeadRotation().toEulerAngles(h);
		m_screenLayout->NavElem->SetSpeed(s.x,s.y,rotation,h.y,h.x);
	}
}

void AugCameraRenderState::_ChangeState(EStatus st)
{
	m_status = st;
	/*
	if (m_robotConnector->IsRobotConnected())
	{
		m_screenLayout->OnDisconnected();
		m_robotConnector->EndUpdate();
	}
	else
	{
		m_vtState->CalibratePosition();

		m_robotConnector->GetHeadController()->Recalibrate();
		m_robotConnector->StartUpdate();
		m_screenLayout->OnConnected();
	}*/
	switch (m_status)
	{
	case mray::AugTel::AugCameraRenderState::ENone:
		break;
	case mray::AugTel::AugCameraRenderState::EWaitStart:
		m_screenLayout->OnDisconnected();
		m_robotConnector->EndUpdate();
		break;
	case mray::AugTel::AugCameraRenderState::EConnectingRobot:
		m_vtState->CalibratePosition();

		m_robotConnector->GetHeadController()->Recalibrate();
		m_robotConnector->UpdateStatus();
		Sleep(200);
		m_robotConnector->StartUpdate();
		m_screenLayout->OnConnected();

		m_loadScreen->Start();
		break;
	case mray::AugTel::AugCameraRenderState::EStarted:
		break;
	default:
		break;
	}
}

void AugCameraRenderState::Update(float dt)
{
	Parent::Update(dt);
	m_vtState->Update(dt);

	switch (m_status)
	{
	case mray::AugTel::AugCameraRenderState::ENone:
		_ChangeState(EWaitStart);
		break;
	case mray::AugTel::AugCameraRenderState::EWaitStart:
		m_guiManager->Update(dt);
		break;
	case mray::AugTel::AugCameraRenderState::EConnectingRobot:
		m_loadScreen->Update(dt);
		if (m_loadScreen->IsDone())
		{
			m_status = EStarted;
		}
		break;
	case mray::AugTel::AugCameraRenderState::EStarted:
		_UpdateStarted(dt);
		break;
	default:
		break;
	}


}
void AugCameraRenderState::LoadFromXML(xml::XMLElement* e)
{
	IEyesRenderingBaseState::LoadFromXML(e);
	m_videoSource->LoadFromXML(e);
	{
		xml::XMLElement* he = e->getSubElement("Hands");
		int i = 0;
		while (he)
		{
			IHandsController* h = 0;
			xml::XMLAttribute* attr = he->getAttribute("Type");
			if (attr->value == "Virtual")
			{

				h = new VirtualHandsController();
				h->LoadFromXML(he);
			}
			else if (attr->value == "Leap")
			{

				h = new LeapMotionHandsController();
				h->LoadFromXML(he);
			}
			else continue;;

			m_hands.push_back(h);
			m_handsMap[he->getValueString("Name")]=m_hands.size()-1;

			if (i > 0)
				h->SetEnabled(false);
			++i;
			he = he->nextSiblingElement("Hands");
		}
	}

	m_optiProvider = e->getValueString("OptiProvider");

	m_depthParams = core::StringConverter::toVector4d(e->getValueString("DepthParams"));
	m_openNiHandler->SetCenter(math::vector2d(m_depthParams.x, m_depthParams.y));
	m_openNiHandler->SetScale(math::vector2d(m_depthParams.z, m_depthParams.w));
}


void AugCameraRenderState::OnDepthData(const TBee::GeomDepthRect& dpRect)
{
	m_openNiHandler->GetNormalCalculator().AddDepthRect(&dpRect);
}
void AugCameraRenderState::OnDepthSize(const math::vector2di &sz)
{
	ATAppGlobal::Instance()->depthProvider->CreateDepthFrame(sz.x, sz.y);
}
void AugCameraRenderState::OnIsStereoImages(bool isStereo)
{
	m_videoSource->SetIsStereo(isStereo);
}

void AugCameraRenderState::OnCameraConfig(const core::string& cameraProfile)
{
	TelubeeCameraConfiguration* config = AppData::Instance()->camConfig->GetCameraConfiguration(cameraProfile);
	if (!config)
		return;
	m_cameraConfiguration = config;
	m_camConfigDirty = true;
}
void AugCameraRenderState::OnRobotCalibrationDone()
{
	printf("Robot was calibrated!\n");

}


void AugCameraRenderState::OnReportedMessage(int code, const core::string& msg)
{
	printf("Message arrived [%d]:%s\n", code, msg.c_str());
}

void AugCameraRenderState::OnBumpSensor(int count, bool* v)
{
	m_bumpSensor.resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_bumpSensor[i] = v[i];
	}

	if (m_bumpSensor.size())
		m_screenLayout->CollisionElem->SetBumper(m_bumpSensor.size(), v);
}
void AugCameraRenderState::OnIRSensor(int count, float* v)
{
	m_irSensor.resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_irSensor[i] = v[i];
	}

	if (m_irSensor.size())
		m_screenLayout->CollisionElem->SetSensors(m_irSensor.size(), &m_irSensor[0]);
}
}
}

