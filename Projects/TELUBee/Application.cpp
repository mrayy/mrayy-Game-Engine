



#include "stdafx.h"
#include "Application.h"
#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include <TheoraManager.h>
#include <FSLManager.h>


#include "IThreadManager.h"
#include "RenderWindowUtils.h"

#include "TBAppGlobals.h"
#include "TextureResourceManager.h"
#include "win32NetInterface.h"
#include "mySqlManager.h"

#include "IndicatorComponent.h"
#include "GameComponentCreator.h"
#include "RobotInfoManager.h"

#include "XMLTree.h"

#include "GUIElementFactory.h"
#include "GUIDateElement.h"
#include "GUIBlinkingText.h"
#include "GUILoadingElement.h"


#include "OculusManager.h"
#include "OculusDevice.h"
#include "OculusCameraComponent.h"
#include "ParsedShaderPP.h"

#include "OculusDetectState.h"

#include "JoystickDefinitions.h"
#include "OptiTrackDataSource.h"


#include "DynamicFontGenerator.h"

#include "LocalCameraRenderingState.h"
#include "RemoteCameraRenderingState.h"
#include "FlyingTelubeeRenderState.h"
#include "IntroRenderingState.h"
#include "LoginScreenState.h"
#include "InMapRenderState.h"
#include "NullRenderState.h"
#include "ConnectingToRobotScreen.h"
#include "GStreamVideoProvider.h"
#include "GSTVideoState.h"

#include "IFileMonitor.h"


namespace mray
{
namespace TBee
{

	class ApplicationOculusData
	{
	public:
		ApplicationOculusData()
		{
			TBAppGlobals::Instance()->oculusDevice = 0;

		}
		GCPtr<video::OculusManager> oculusManager;
		GCPtr<video::ParsedShaderPP> oculusRenderer[2];
		game::OculusCameraComponent* oculusComponents[2];

		video::ITexturePtr rtTexture;
		video::IRenderTargetPtr renderTarget;;

		~ApplicationOculusData()
		{
			oculusRenderer[0]=0;
			oculusRenderer[1]=0;
			TBAppGlobals::Instance()->oculusDevice = 0;
			oculusManager=0;
		}
	};


Application::Application()
{
	new TBAppGlobals();
	TBAppGlobals::Instance()->App = this;
	m_drawUI=false;
	m_tbRenderer = 0;
}
Application::~Application()
{
	delete m_tbRenderer;
	m_appStateManager=0;
	delete TBAppGlobals::Instance()->sqlManager;
	m_videoManager=0;
	m_soundManager=0;
	delete TBAppGlobals::Instance()->robotInfoManager;
}

void Application::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
	GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
	GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
	GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

	//load font
	GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator();
	font->SetFontName(L"Arial");
	font->SetTextureSize(1024);
	font->SetFontResolution(24);
	font->Init();

	//GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
	//gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
	gFontResourceManager.setDefaultFont(font);


	gLogManager.log("Resources Loaded",ELL_SUCCESS);
}
void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);

	if(event->getType()==ET_Mouse)
	{
		MouseEvent* e=(MouseEvent* )event;
	}
	if (m_appStateManager)
	{
		for (int i = 0; i < m_tbRenderer->GetViewportCount(); ++i)
		{
			if (m_appStateManager->OnEvent(event,m_tbRenderer->GetViewport(i)->getAbsRenderingViewPort()))
				break;
		}
	}


	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent* e=(KeyboardEvent*)event;
		if(e->press && e->key==KEY_F12)
		{
			m_drawUI=!m_drawUI;
		}
		if (e->press && e->key == KEY_F9)
		{
			AppData::Instance()->IsDebugging = !AppData::Instance()->IsDebugging;
		}
		/*	if(e->press && e->key==KEY_F12)
		{

			video::LockedPixelBox box=m_screenShot->getSurface(0)->lock(math::box3d(0,0,0,GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::IHardwareBuffer::ELO_Discard);
			GetRenderWindow()->TakeScreenShot(box);
			m_screenShot->getSurface(0)->unlock();
			gTextureResourceManager.writeResourceToDist(m_screenShot,"screenShot.tga");

		}*/
	}

}

void Application::_initStates()
{
	IRenderingState *nullState, *streamerTest, *flyingTbeeState, *introState, *oc, *ls, *maps, *ctr,  *videoState;
	TBee::LocalCameraRenderingState *cameraState;
	nullState = new TBee::NullRenderState();
	nullState->InitState();
	m_renderingState->AddState(nullState);

	streamerTest = new TBee::RemoteCameraRenderingState("CameraRemote");
	streamerTest->InitState();
	m_renderingState->AddState(streamerTest);

	flyingTbeeState = new TBee::FlyingTelubeeRenderState();
	flyingTbeeState->InitState();
	m_renderingState->AddState(flyingTbeeState);

	cameraState = new TBee::LocalCameraRenderingState("CameraLocal");
	cameraState->InitState();
	cameraState->SetCameraInfo(ETargetEye::Eye_Right, m_cam1);
	cameraState->SetCameraInfo(ETargetEye::Eye_Left, m_cam2);
	m_renderingState->AddState(cameraState);

	videoState = new TBee::GSTVideoState();
	videoState->InitState();
	m_renderingState->AddState(videoState);

	introState = new TBee::IntroRenderingState();
	introState->InitState();
	m_renderingState->AddState(introState);

	 oc = TBee::OculusDetectState::Instance();
	oc->InitState();
	m_renderingState->AddState(oc);

	ls = new TBee::LoginScreenState();
	ls->InitState();
	m_renderingState->AddState(ls);

	maps = new TBee::InMapRenderState();
	//maps->InitState();
	m_renderingState->AddState(maps);

	ctr = new TBee::ConnectingToRobotScreen();
	ctr->InitState();
	m_renderingState->AddState(ctr);


	m_renderingState->AddTransition(nullState, cameraState, STATE_EXIT_CODE);
	//AddTransition("Streamer","Intro",STATE_EXIT_CODE);
	if (TBAppGlobals::Instance()->headController==EHeadControllerType::Oculus)
	{
		m_renderingState->AddTransition(introState, oc, STATE_EXIT_CODE);
		m_renderingState->AddTransition(oc, ls, STATE_EXIT_CODE);
	}
	else
		m_renderingState->AddTransition(introState, ls, STATE_EXIT_CODE);
	m_renderingState->AddTransition(ls, maps, ToMap_CODE);
	//m_renderingState->AddTransition(ls, "CameraRemote", ToRemoteCamera_CODE);//Camera
	//m_renderingState->AddTransition(ls, "FlyingTbee", ToFlyingTelubee_CODE);//Camera
	m_renderingState->AddTransition(ls, cameraState, ToLocalCamera_CODE);//Camera
	//m_renderingState->AddTransition(ls, "Video", ToVideo_CODE);//Video
	//m_renderingState->AddTransition("CameraRemote", ls, STATE_EXIT_CODE);
	//m_renderingState->AddTransition("CameraLocal", ls, STATE_EXIT_CODE);
	//m_renderingState->AddTransition("Video", ls, STATE_EXIT_CODE);
	//m_renderingState->AddTransition("Map", ls, BackToTile_Code);
	//m_renderingState->AddTransition("Map", "Connecting", ConnectToRobot_Code);
	//m_renderingState->AddTransition("Connecting", "Map", BackToMap_Code);
	m_renderingState->SetInitialState(nullState);
}

class FileMonitorListener : public OS::IFileMonitorListener
{
	virtual void OnMonitorFiles(OS::IFileMonitor* sender, OS::IMonitorFileInformation* f, OS::EMonitorAction action)
	{
		printf("%s\n", f->GetPath().c_str());
	}

}gFileMonitorL;

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	OS::IFileMonitor* monitor= OS::IOSystem::getInstance().CreateFileMonitor();
	monitor->AddListener(&gFileMonitorL);
	monitor->SetDirectory(gFileSystem.getCurrPath());
	monitor->StartMonitoring();

	TBAppGlobals::Instance()->Load("TBSettings.conf");
	{
		core::string v=extraOptions.GetOptionValue("Debugging");
		if(v=="Yes")
			TBAppGlobals::Instance()->IsDebugging = true;
		else
			TBAppGlobals::Instance()->IsDebugging=false;
	
		TBAppGlobals::Instance()->DVIPort=extraOptions.GetOptionValue("DVIPort");

		TBAppGlobals::Instance()->m_controller = extraOptions.GetOptionByName("Controller")->getValue() == "XBox" ? EController::XBox : EController::Logicool;

		if (TBAppGlobals::Instance()->m_controller == EController::XBox)
		{
			JOYSTICK_SelectButton = 6;
			JOYSTICK_StartButton = 7;

			JOYSTICK_Axis0 = 2;
			JOYSTICK_Axis1 = 3;
			JOYSTICK_Axis2 = 0;
			JOYSTICK_Axis3 = 1;
		}
		v = extraOptions.GetOptionValue("Head");
		if (v == "Keyboard")
			AppData::Instance()->headController = EHeadControllerType::Keyboard;
		else if (v == "Oculus")
			AppData::Instance()->headController = EHeadControllerType::Oculus;
		else if (v == "OptiTrack")
			AppData::Instance()->headController = EHeadControllerType::OptiTrack;

		v = extraOptions.GetOptionValue("Robot");
		if (v == "Keyboard")
			AppData::Instance()->robotController = ERobotControllerType::Keyboard;
		else if (v == "Joystick")
			AppData::Instance()->robotController = ERobotControllerType::Joystick;
		else if (v == "Wiiboard")
			AppData::Instance()->robotController = ERobotControllerType::Wiiboard;

		v = extraOptions.GetOptionValue("Stereoscopic");
		if (v == "None")
			AppData::Instance()->stereoMode=ERenderStereoMode::None;
		else if (v == "Side-by-side")
			AppData::Instance()->stereoMode = ERenderStereoMode::SideBySide;
		else if (v == "Up-bottom")
			AppData::Instance()->stereoMode = ERenderStereoMode::TopDown;
		else if (v == "StereoTV")
			AppData::Instance()->stereoMode = ERenderStereoMode::StereoTV;
		else if (v == "Oculus")
			AppData::Instance()->stereoMode = ERenderStereoMode::Oculus;



		m_cam1 = extraOptions.GetOptionByName("Camera0")->getValueIndex();
		m_cam2 = extraOptions.GetOptionByName("Camera1")->getValueIndex();
	}
	_InitResources();

	LoadSettingsXML("TBeeSettings.xml");

	this->m_limitFps = false;

	network::createWin32Network();



	REGISTER_COMPONENT_FACTORY(IndicatorComponent);
	using namespace GUI;
	REGISTER_GUIElement_FACTORY(GUIDateElement);
	REGISTER_GUIElement_FACTORY(GUILoadingElement);
	REGISTER_GUIElement_FACTORY(GUIBlinkingText);

	m_wiimoteManager = new controllers::WiimoteManager;
	m_wiimoteManager->ConnectWithAllMotes();

	TBAppGlobals::Instance()->sqlManager=new db::mySqlManager();

	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	TBAppGlobals::Instance()->inputMngr = m_inputManager;
// 	m_guiManager=new GUI::GUIManager(getDevice());
// 	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
	gLogManager.log("Managers", ELL_INFO);

	m_soundManager=new sound::FSLManager();
	m_videoManager=new video::TheoraManager();
	m_videoManager->setSoundManager(m_soundManager);

	m_screenShot=getDevice()->createEmptyTexture2D(true);
	m_screenShot->setMipmapsFilter(false);
	m_screenShot->createTexture(math::vector3d(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::EPixel_B8G8R8A8);

	m_tbRenderer = new TBeeRenderer();
	m_tbRenderer->AddListener(this);
	m_tbRenderer->Init(GetRenderWindow(0));

	TBAppGlobals::Instance()->robotInfoManager=new RobotInfoManager();
	TBAppGlobals::Instance()->robotInfoManager->LoadRobots("RobotsMap.xml");

	m_appStateManager=new TBee::ApplicationStateManager();

	gLogManager.log("States", ELL_INFO);

//	TBee::ApplicationMenuState* menuState=new TBee::ApplicationMenuState();
	m_renderingState=new TBee::RenderingStateManager();
	_initStates();
	//m_appStateManager->AddState(menuState,"Menu");
	m_appStateManager->AddState(m_renderingState,"Rendering");
	m_appStateManager->SetInitialState("Rendering");
	//m_appStateManager->AddTransition("Menu","Rendering",STATE_EXIT_CODE);
	
	_createViewports();
	LoadSettingsXML("States.xml");

	if (TBAppGlobals::Instance()->headController == EHeadControllerType::OptiTrack)
		AppData::Instance()->optiDataSource->ConnectLocal();

	gLogManager.log("Starting Application", ELL_INFO);

}
void Application::_createViewports()
{

	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
	
}
void Application::RenderUI(const math::rectf& rc)
{
	if(TBAppGlobals::Instance()->IsDebugging )
	{


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
			msg += core::StringConverter::toString((int)gEngine.getFPS()->getFPS());
			font->print(math::rectf(rc.getWidth() - 250, rc.getHeight() - 50, 10, 10), &attr, 0, msg, m_guiRender);
			yoffset+=attr.fontSize;

		}

		m_guiRender->Flush();
		getDevice()->useShader(0);
	}
}

void Application::draw(scene::ViewPort* vp)
{


}
void Application::_RenderVP(int i)
{
	
}

void Application::OnRendererDraw(TBeeRenderer* r, const math::rectf& vp, video::IRenderTarget* rt, ETargetEye eye)
{

	m_appStateManager->Draw(vp, rt, eye);
	RenderUI(vp);

}

void Application::onRenderDone(scene::ViewPort*vp)
{
}
void Application::WindowPostRender(video::RenderWindow* wnd)
{
	getDevice()->set2DMode();
	getDevice()->setViewport(m_mainVP);
	m_tbRenderer->Render(m_mainVP);
}
void Application::update(float dt)
{
	CMRayApplication::update(dt);

	if(m_videoManager)
		m_videoManager->Update(dt);
	if(m_soundManager)
		m_soundManager->runSounds(dt);

	m_appStateManager->Update(dt);

	m_tbRenderer->Update(dt);
	//OS::IThreadManager::getInstance().sleep(1000/30);

}
void Application::onDone()
{
	//TBAppGlobals::Instance()->Save("VTSettings.conf");

}

void Application::LoadSettingsXML(const core::string &path)
{
	xml::XMLTree tree;
	if(!tree.load(path))
		return;
	xml::XMLElement* e;
	e=tree.getSubElement("States");
	if(e)
	{
		xml::XMLElement* se=e->getSubElement("Rendering");
		if(se)
		{
			m_renderingState->LoadSettingsXML(se);
		}
	}
	e=tree.getSubElement("Application");
	if(e)
	{
		//TBAppGlobals::Instance()->usingOculus=e->getValueBool("UsingOculus");
	}
}

}
}
