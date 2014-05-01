



#include "stdafx.h"
#include "Application.h"
#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include <TheoraManager.h>
#include <SFModSoundmanager.h>


#include "IThreadManager.h"
#include "RenderWindowUtils.h"

#include "ATAppGlobal.h"
#include "TextureResourceManager.h"
#include "win32NetInterface.h"
#include "mySqlManager.h"

#include "GameComponentCreator.h"
#include "RobotInfoManager.h"

#include "XMLTree.h"

#include "GUIElementFactory.h"


#include "OculusManager.h"
#include "OculusDevice.h"
#include "OculusCameraComponent.h"
#include "ParsedShaderPP.h"
#include "NullRenderState.h"
#include "ConnectingState.h"
#include "AugCameraRenderState.h"
#include "LoginScreenState.h"
#include "LocalCameraRenderingState.h"
#include "RemoteCameraRenderingState.h"
#include "GeomDepthState.h"

#include "JoystickDefinitions.h"
#include "LocalCameraVideoSource.h"
#include "GstStereoNetVideoSource.h"
#include "DataCommunicator.h"

#include "TBRobotInfo.h"
#include "DynamicFontGenerator.h"

#include "VTLib.h"

#include "ATGameComponents.h"
#include "GUIRegisterer.h"


#include "AtlasPacker.h"

#include "IDirOS.h"

namespace mray
{
namespace AugTel
{
	class MyRT
	{
	public:
		video::ITexturePtr tex;
		video::IRenderTargetPtr rt;

		MyRT(){}
		
		void Init(int w,int h)
		{
			tex = Engine::getInstance().getDevice()->createEmptyTexture2D(true);
			tex->setMipmapsFilter(false);
			rt = Engine::getInstance().getDevice()->createRenderTarget("", tex, 0, 0, 0);
			tex->createTexture(math::vector3di(w, h, 1), video::EPixel_R8G8B8A8);
		}
		void Set()
		{
			Engine::getInstance().getDevice()->setRenderTarget(rt);
		}

		void Unset()
		{
			Engine::getInstance().getDevice()->setRenderTarget(0);
		}

		void Draw(const math::rectf& rc)
		{
			video::TextureUnit tu;
			tu.SetTexture(tex);
			Engine::getInstance().getDevice()->useTexture(0, &tu);
			Engine::getInstance().getDevice()->draw2DImage(rc,1);
		}
	}g_rt1,g_rt2;


Application::Application()
{
	new ATAppGlobal();
	ATAppGlobal::Instance()->App = this;
	m_drawUI = false;

	this->m_limitFps = true;
}
Application::~Application()
{
	VT::ReleaseVTLib();

	delete m_tbRenderer;
	m_appStateManager = 0;
	delete ATAppGlobal::Instance()->sqlManager;
	m_videoManager = 0;
	m_soundManager = 0;
	delete ATAppGlobal::Instance()->robotInfoManager;
	delete ATAppGlobal::Instance();
}

void Application::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
	GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
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

	gLogManager.log("Resources Loaded", ELL_SUCCESS);
}
void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);
	if (!m_tbRenderer)
		return;

	for (int i = 0; i < m_tbRenderer->GetViewportCount(); ++i)
	{
		if (m_appStateManager->OnEvent(event, m_tbRenderer->GetViewport(i)->getAbsRenderingViewPort()))
			break;
	}


	if (event->getType() == ET_Keyboard)
	{
		KeyboardEvent* e = (KeyboardEvent*)event;
// 		if (e->press && e->key == KEY_F12)
// 		{
// 			m_drawUI = !m_drawUI;
// 		}
		if (e->press && e->key == KEY_F9)
		{
			AppData::Instance()->IsDebugging = !AppData::Instance()->IsDebugging;
		}
			if(e->press && e->key==KEY_F12)
		{

			video::LockedPixelBox box=m_screenShot->getSurface(0)->lock(math::box3d(0,0,0,GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::IHardwareBuffer::ELO_Discard);
			GetRenderWindow()->TakeScreenShot(box);
			m_screenShot->getSurface(0)->unlock();
			core::string name;
			time_t rawtime;
			struct tm timeinfo;
			char buffer[80];

			time(&rawtime);
			localtime_s(&timeinfo,&rawtime);

			strftime(buffer, 80, "%I_%M_%S%p_", &timeinfo);
			name = "screenShot";
			name += buffer;
			name += ".png";
			gTextureResourceManager.writeResourceToDist(m_screenShot,name);

		}
	}

}

void Application::_initStates()
{
	IRenderingState *nullState, *intro, *login, *camera, *depth, *streamerTest;
	nullState = new NullRenderState();
	nullState->InitState();
	m_renderingState->AddState(nullState, "Null");

	intro = new ConnectingState();
	m_renderingState->AddState(intro, "Intro");
	 

	login = new LoginScreenState();
	m_renderingState->AddState(login, "Login");

	TBee::TBRobotInfo* ifo = AppData::Instance()->robotInfoManager->GetRobotInfo(0);
	core::string ip = "127.0.0.1";
	if (ifo)
		ip = ifo->IP;

// 	streamerTest = new AugCameraRenderState(new TBee::GstStereoNetVideoSource(ip));
// 	m_renderingState->AddState(streamerTest, "CameraRemote");

 	camera = new AugCameraRenderState(new TBee::LocalCameraVideoSource(m_cam1, m_cam2));
 	m_renderingState->AddState(camera, "AugCam");

	depth = new GeomDepthState();
	m_renderingState->AddState(depth, "Depth");

	m_renderingState->AddTransition("Null", "Login", STATE_EXIT_CODE);
	//m_renderingState->AddTransition("Login", "CameraRemote", ToRemoteCamera_CODE);//Camera
	m_renderingState->AddTransition("Login", "AugCam", ToLocalCamera_CODE);
	//m_renderingState->AddTransition("Intro", "AugCam", STATE_EXIT_CODE);
	m_renderingState->AddTransition("Login", "Depth", ToDepthView_CODE);
	m_renderingState->AddTransition("CameraRemote", "Login", STATE_EXIT_CODE);
	m_renderingState->AddTransition("AugCam", "Login", STATE_EXIT_CODE);
	m_renderingState->AddTransition("Depth", "Login", STATE_EXIT_CODE);
	m_renderingState->SetInitialState("Null");

}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	ATAppGlobal::Instance()->Load("TBSettings.conf");
	{
		core::string v = extraOptions.GetOptionValue("Debugging");
		if (v == "Yes")
			ATAppGlobal::Instance()->IsDebugging = true;
		else
			ATAppGlobal::Instance()->IsDebugging = false;

		ATAppGlobal::Instance()->DVIPort = extraOptions.GetOptionValue("DVIPort");

		v = extraOptions.GetOptionValue("Stereoscopic");
		if (v == "None")
			AppData::Instance()->stereoMode = ERenderStereoMode::None;
		else if (v == "Side-by-side")
			AppData::Instance()->stereoMode = ERenderStereoMode::SideBySide;
		else if (v == "Up-bottom")
			AppData::Instance()->stereoMode = ERenderStereoMode::TopDown;
		else if (v == "StereoTV")
			AppData::Instance()->stereoMode = ERenderStereoMode::StereoTV;
		else if (v == "Oculus")
			AppData::Instance()->stereoMode = ERenderStereoMode::Oculus;
		//ATAppGlobal::Instance()->usingOculus= extraOptions.GetOptionValue("Oculus")=="Yes";

		v = extraOptions.GetOptionValue("Robot");
		if (v == "Keyboard")
			AppData::Instance()->robotController = ERobotControllerType::Keyboard;
		else if (v == "Joystick")
			AppData::Instance()->robotController = ERobotControllerType::Joystick;
		else if (v == "Wiiboard")
			AppData::Instance()->robotController = ERobotControllerType::Wiiboard;


		m_cam1 = extraOptions.GetOptionByName("Camera0")->getValueIndex();
		m_cam2 = extraOptions.GetOptionByName("Camera1")->getValueIndex();
	}
	_InitResources();



	GUI::GUIRegisterer::RegisterElements();

	LoadSettingsXML("TBeeSettings.xml");

	VT::RefVTLib();

	network::createWin32Network();

	m_wiiManager = new controllers::WiimoteManager();
	int cnt=m_wiiManager->ConnectWithAllMotes().size();
	printf("Wiimote found: %d\n", cnt);

	ATGameComponents::RegisterComponents();

	ATAppGlobal::Instance()->sqlManager = new db::mySqlManager();

	ATAppGlobal::Instance()->dataCommunicator = new DataCommunicator();

	m_guiRenderer = new GUI::GUIBatchRenderer();
	m_guiRenderer->SetDevice(getDevice());

	ATAppGlobal::Instance()->inputMngr = m_inputManager;
	gLogManager.log("Managers", ELL_INFO);

	m_soundManager = new sound::SFModSoundManager();
	m_videoManager = new video::TheoraManager();
	m_videoManager->setSoundManager(m_soundManager);
	m_openNIMngr = new OpenNIManager();
	m_openNIMngr->Init(0, 0);

	ATAppGlobal::Instance()->soundManager = m_soundManager;

	m_screenShot = getDevice()->createEmptyTexture2D(true);
	m_screenShot->setMipmapsFilter(false);
	m_screenShot->createTexture(math::vector3d(GetRenderWindow()->GetSize().x, GetRenderWindow()->GetSize().y, 1), video::EPixel_R8G8B8A8);

	gLogManager.log("Robots", ELL_INFO);

	ATAppGlobal::Instance()->robotInfoManager = new TBee::RobotInfoManager();
	ATAppGlobal::Instance()->robotInfoManager->LoadRobots("RobotsMap.xml");

	m_appStateManager = new TBee::ApplicationStateManager();

	gLogManager.log("States", ELL_INFO);

	m_renderingState = new TBee::RenderingStateManager();
	m_appStateManager->AddState(m_renderingState, "Rendering");
	m_appStateManager->SetInitialState("Rendering");
	
	m_tbRenderer = new TBeeRenderer();
	m_tbRenderer->AddListener(this);
	m_tbRenderer->Init(GetRenderWindow(0));

	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);

	_initStates();
	LoadSettingsXML("States.xml");

	if (false)
	{
		video::AtlasPacker packer;
		std::vector < video::ITexture*> textures;
		OS::IDirOS* dir= OS::IOSystem::getInstance().createDirSystem();
		dir->changeDir("E:\\Development\\mrayEngine\\Bin\\Debug\\Cache\\");
		std::vector<OS::SFileData> files= dir->getFiles();
		for (int i = 0; i < files.size() && i<50; i++)
		{
			video::ITexture* tex=gTextureResourceManager.loadTexture2D(files[i].name);
			tex->setMipmapsFilter(false);
			tex->load(false);
			textures.push_back(dynamic_cast<video::ITexture*>(tex));
		}
		packer.SetMaxSize(4096);
		packer.PackTextures(&textures[0], 5);
		packer.AddTexture(textures[0]);
	}
	m_renderingState->InitStates();

	gLogManager.log("Starting Application", ELL_INFO);

	{
		m_previewWnd = 0;
		if (extraOptions.GetOptionValue("Preview") == "Yes")
		{
			OptionContainer opt;
			opt["title"].value = "Preview";
			opt["VSync"].value = "false";
			opt["Monitor"].value = "1";
			m_previewWnd = getDevice()->CreateRenderWindow("Preview", math::vector2di(1280,720), true, opt, 0);
			m_previewWnd->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
			AddRenderWindow(m_previewWnd);
		}

	}

}

void Application::RenderUI(const math::rectf& rc)
{
	if ((ATAppGlobal::Instance()->IsDebugging || m_drawUI) )
	{
		GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();
		getDevice()->set2DMode();
		if (font){
			m_guiRenderer->Prepare();

			float yoffset = 50;


			GUI::FontAttributes attr;
			attr.fontColor.Set(0.05, 1, 0.5, 1);
			attr.fontAligment = GUI::EFA_MiddleLeft;
			attr.fontSize = 24;
			attr.hasShadow = true;
			attr.shadowColor.Set(0, 0, 0, 1);
			attr.shadowOffset = math::vector2d(2);
			attr.spacing = 2;
			attr.wrap = 0;
			attr.RightToLeft = 0;
			core::string msg = mT("FPS= ");
			msg += core::StringConverter::toString((int)core::CFPS::getInstance().getFPS());
			font->print(math::rectf(rc.getWidth() - 250, rc.getHeight() - 150, 10, 10), &attr, 0, msg, m_guiRenderer);
			yoffset += attr.fontSize;

		}

		m_guiRenderer->Flush();
		getDevice()->useShader(0);
	}
}

void Application::draw(scene::ViewPort* vp)
{


}
void Application::OnRendererDraw(TBeeRenderer* r, const math::rectf& rc, video::IRenderTarget* rt, ETargetEye eye)
{

	m_appStateManager->Draw(rc, rt, eye);
	RenderUI(rc);
}

void Application::onRenderDone(scene::ViewPort*vp)
{

}
void Application::WindowPostRender(video::RenderWindow* wnd)
{
	getDevice()->set2DMode();

	if (wnd == m_previewWnd)
	{
		getDevice()->set2DMode();
		math::rectf rc(0, wnd->GetSize());
		video::TextureUnit tex;
		
		tex.SetTexture(m_tbRenderer->GetEyeImage(0)->getColorTexture());
		getDevice()->useTexture(0, &tex);

		math::rectf trc;
		getDevice()->draw2DImage(rc, 1);
		return;
	}

	getDevice()->setViewport(m_mainVP);
	m_tbRenderer->Render(m_mainVP);
}
void Application::update(float dt)
{
	CMRayApplication::update(dt);

	if (m_videoManager)
		m_videoManager->Update(dt);
	if (m_soundManager)
		m_soundManager->runSounds(dt);

	m_appStateManager->Update(dt);
	m_tbRenderer->Update(dt);

	m_wiiManager->PollEvents();

	//OS::IThreadManager::getInstance().sleep(1000/30);

}
void Application::onDone()
{
	//ATAppGlobal::Instance()->Save("VTSettings.conf");

}

void Application::LoadSettingsXML(const core::string &path)
{
	xml::XMLTree tree;
	if (!tree.load(path))
		return;
	xml::XMLElement* e;
	e = tree.getSubElement("States");
	if (e)
	{
		xml::XMLElement* se = e->getSubElement("Rendering");
		if (se)
		{
			m_renderingState->LoadSettingsXML(se);
		}
		se = e->getSubElement("Camera");
	}
	e = tree.getSubElement("Application");
	if (e)
	{
	}
}

}
}
