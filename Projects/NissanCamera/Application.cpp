



#include "stdafx.h"
#include "Application.h"
#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include <FSLManager.h>


#include "IThreadManager.h"
#include "RenderWindowUtils.h"

#include "NCAppGlobals.h"
#include "TextureResourceManager.h"
#include "win32NetInterface.h"

#include "RobotInfoManager.h"

#include "XMLTree.h"

#include "GUIElementFactory.h"

#include "ParsedShaderPP.h"


#include "JoystickDefinitions.h"
#include "OptiTrackDataSource.h"

#include "RemoteCameraRenderingState.h"
#include "RobotCameraState.h"
#include "NullRenderState.h"
#include "GStreamVideoProvider.h"

#include "IFileMonitor.h"
#include "DynamicFontGenerator.h"
#include "GUIElementFactory.h"
#include "GUIConsole.h"
#include "AppData.h"
#include "FlyCameraManager.h"

//#include "PythonScriptManager.h"


namespace mray
{
namespace NCam
{

	class ApplicationOculusData
	{
	public:
		ApplicationOculusData()
		{

		}

		video::ITexturePtr rtTexture;
		video::IRenderTargetPtr renderTarget;;

		~ApplicationOculusData()
		{
		}
	};


Application::Application()
{
	new NCAppGlobals();
	NCAppGlobals::Instance()->App = this;
	m_drawUI=false;
	m_tbRenderer = 0;
	m_limitFps = true;
	m_limitFpsCount = 80;

}
Application::~Application()
{
	delete m_tbRenderer;
	m_appStateManager=0;
	m_soundManager=0;
	delete TBee::AppData::Instance();
}

void Application::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
	GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
	GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
	GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

	//load font

	GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator("Arial");
	font->SetFontName(L"Arial");
	font->SetTextureSize(1024);
	font->SetFontResolution(24);
	font->Init();
	gFontResourceManager.setDefaultFont(font);

	gLogManager.log("Resources Loaded", ELL_SUCCESS);

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
		if (e->press && e->key == KEY_F9 && e->ctrl)
		{
			AppData::Instance()->IsDebugging = !AppData::Instance()->IsDebugging;
		}
		if(e->press && e->key==KEY_F12)
		{
			math::vector3d sz;
			sz.x = GetRenderWindow()->GetSize().x;
			sz.y = GetRenderWindow()->GetSize().y;
			sz.z = 1;
			m_screenShot->createTexture(sz, video::EPixelFormat::EPixel_B8G8R8);
			video::LockedPixelBox box=m_screenShot->getSurface(0)->lock(math::box3d(0,0,0,GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::IHardwareBuffer::ELO_Discard);
			GetRenderWindow()->TakeScreenShot(box);
			m_screenShot->getSurface(0)->unlock();
			core::string fname = gFileSystem.getAppPath() + "/Screenshots/screen" + core::StringConverter::toString(gEngine.getTimer()->getSeconds()) + ".png";
			gTextureResourceManager.writeResourceToDist(m_screenShot,fname);

		}
	}

}

void Application::_initStates()
{
	gLogManager.log("Initing states",ELL_INFO);
	IRenderingState *nullState, *streamerTest, *cameraState;
	nullState = new TBee::NullRenderState();
	nullState->InitState();
	m_renderingState->AddState(nullState);

// 	streamerTest = new TBee::RemoteCameraRenderingState("CameraRemote");
// 	streamerTest->InitState();
// 	m_renderingState->AddState(streamerTest);

	cameraState = new RobotCameraState();//TBee::LocalCameraRenderingState();
	((RobotCameraState*)cameraState)->SetCameraConnection(m_camType);
	((RobotCameraState*)cameraState)->SetCameraInfo(Eye_Left, m_cameraID[Eye_Left]);
	((RobotCameraState*)cameraState)->SetCameraInfo(Eye_Right, m_cameraID[Eye_Right]);
	m_renderingState->AddState(cameraState);

	// 	ls = new TBee::LoginScreenState();
// 	ls->InitState();
// 	m_renderingState->AddState(ls, "Login");

	m_renderingState->AddTransition(nullState, cameraState, STATE_EXIT_CODE);
	//AddTransition("Streamer","Intro",STATE_EXIT_CODE);
	m_renderingState->SetInitialState(nullState);
}


void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	NCAppGlobals::Instance()->Init();

	NCAppGlobals::Instance()->Load("NCSettings.conf");
	{
		core::string v=extraOptions.GetOptionValue("Debugging");
		if(v=="Yes")
			NCAppGlobals::Instance()->IsDebugging = true;
		else
			NCAppGlobals::Instance()->IsDebugging=false;

		v = extraOptions.GetOptionValue("CameraType");
		if (v == "DirectShow")
			m_camType = TBee::ECam_DirectShow;
		else 
			m_camType = TBee::ECam_PointGray;

		if (m_camType == TBee::ECam_DirectShow)
		{
			// -1 for the None index
			m_cameraID[Eye_Left] = extraOptions.GetOptionByName("DS_Camera_Left")->getValueIndex()-1;
			m_cameraID[Eye_Right] = extraOptions.GetOptionByName("DS_Camera_Right")->getValueIndex() - 1;
		}else 
		{
			//point grey cameras have unique serial number
			int count = video::FlyCameraManager::getInstance().GetCamerasCount();
			int c1 = core::StringConverter::toInt(extraOptions.GetOptionByName("PT_Camera_Left")->getValue());
			int c2 = core::StringConverter::toInt(extraOptions.GetOptionByName("PT_Camera_Right")->getValue());
			for (int i = 0; i < count; ++i)
			{
				uint sp;
				video::FlyCameraManager::getInstance().GetCameraSerialNumber(i, sp);
				if (sp == c1)
				{
					m_cameraID[Eye_Left] = i;
				}
				if (sp == c2)
				{
					m_cameraID[Eye_Right] = i;
				}

			}
		}
		AppData::Instance()->headController = EHeadControllerType::OptiTrack;
		AppData::Instance()->robotController = ERobotControllerType::None;

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
	}
	_InitResources();

	NCAppGlobals::Instance()->Load("NissanConfig.cfg");

	LoadSettingsXML("NissanSettings.xml");


	network::createWin32Network();

	using namespace GUI;

	{
		REGISTER_GUIElement_FACTORY(GUIConsole);
	}
	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());
	printf("Starting up\n");

	NCAppGlobals::Instance()->inputMngr = m_inputManager;

	//m_soundManager=new sound::FSLManager();


	m_screenShot=getDevice()->createEmptyTexture2D(true);
	m_screenShot->setMipmapsFilter(false);
	m_screenShot->createTexture(math::vector3d(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::EPixel_B8G8R8A8);

	printf("Creating Render Manager\n");
	m_tbRenderer = new TBeeRenderer();
	m_tbRenderer->AddListener(this);
	m_tbRenderer->Init(GetRenderWindow(0));

	printf("Creating App Manager\n");
	m_appStateManager = new TBee::ApplicationStateManager();


	printf("Creating Render State Manager\n");
	m_renderingState = new TBee::RenderingStateManager();
	m_renderingState->SetSleepTime(0);// 1.0 / 150.0f);
	printf("Initing States\n");
	_initStates();
	m_appStateManager->AddState(m_renderingState,"Rendering");
	m_appStateManager->SetInitialState("Rendering");
	LoadSettingsXML("States.xml");
	m_renderingState->InitStates();
	_createViewports();

	//Set head controller to optiTrack
	NCAppGlobals::Instance()->headController = EHeadControllerType::OptiTrack;
	if (NCAppGlobals::Instance()->headController == EHeadControllerType::OptiTrack)
		NCAppGlobals::Instance()->optiDataSource->ConnectLocal();

	_RegisterNetworkValues();
	NCAppGlobals::Instance()->netValueController->StartReceiver(6001);
	NCAppGlobals::Instance()->LoadNetValues();

	gLogManager.log("Starting Application", ELL_INFO);


// 	script::PythonScriptManager* scriptManager = new script::PythonScriptManager();
// 	scriptManager->ExecuteFile(gFileSystem.openFile("testPython.py"));

}

void Application::_RegisterNetworkValues()
{
// 	ValueGroup* g = new ValueGroup("App");
// 	NCAppGlobals::Instance()->netValueController->GetValues()->AddValueGroup(g);
// 
//	g->AddValue(new Vector2dfValue("Resolution", GetRenderWindow(0)->GetSize() / 100.0f))->OnChanged += newClassDelegate1("", this, &Application::_OnPropertyChanged);
}

void Application::_OnPropertyChanged(IValue* v)
{
	printf("Value[%s] = %s\n", v->getName().c_str(), v->toString().c_str());
}

void Application::_createViewports()
{

	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
	
}
void Application::RenderUI(const math::rectf& rc)
{
	if(NCAppGlobals::Instance()->IsDebugging )
	{


		GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
		if(font){
			m_guiRender->Prepare();

			float yoffset = rc.getHeight() - 200;

#define PRINT_LOG(msg)\
	font->print(math::rectf(rc.getWidth() - 350, yoffset, 10, 10), &attr, 0, msg, m_guiRender); \
	yoffset +=  attr.fontSize;

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
			msg+=core::StringConverter::toString((int)gEngine.getFPS()->getFPS());
			PRINT_LOG(msg);
			 msg = mT("Draw calls= ");
			 msg += core::StringConverter::toString(gEngine.getDevice()->getBatchDrawnCount());
			 PRINT_LOG(msg);
			msg = mT("Primitives= ");
			msg += core::StringConverter::toString(gEngine.getDevice()->getPrimitiveDrawnCount());
			PRINT_LOG(msg);

			attr.fontSize = 18;
			attr.hasShadow = false;
			attr.fontColor.Set(1, 1, 1, 1);
			yoffset = 200;


			const controllers::InputKeyMap::CommandList& keys= NCAppGlobals::Instance()->keyMap.GetCommands();
			for (int i = 0; i < keys.size(); ++i)
			{
				PRINT_LOG(((controllers::InputKeyMap::CommandInfo&) keys[i]).ToString(true));

			}

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
	if(m_soundManager)
		m_soundManager->runSounds(dt);

	m_appStateManager->Update(dt);

	m_tbRenderer->Update(dt);
	//OS::IThreadManager::getInstance().sleep(1000/30);

}
void Application::onDone()
{
	//NCAppGlobals::Instance()->Save("VTSettings.conf");
	//WriteSettingsXML();
}

void Application::WriteSettingsXML()
{
	xml::XMLWriter w;
	xml::XMLElement* root = new xml::XMLElement("States");

	m_renderingState->WriteSettingsXML(root);
	w.addElement(root);

 	core::string xmlData=w.flush();

	OS::IStreamPtr stream= gFileSystem.createTextFileWriter(m_settingsPath);
	OS::StreamWriter wtr(stream);
	wtr.writeString(xmlData);
	stream->close();
	delete root;
}

void Application::LoadSettingsXML(const core::string &path)
{
	xml::XMLTree tree;

	OS::IFileSystem::getInstance().getCorrectFilePath(path, m_settingsPath);
	if (!tree.load(m_settingsPath))
		return;
	xml::XMLElement* e;
	e=tree.getSubElement("States");
	if(e)
	{
		m_renderingState->LoadSettingsXML(e);
	}
	e=tree.getSubElement("Application");
	if(e)
	{
		//NCAppGlobals::Instance()->usingOculus=e->getValueBool("UsingOculus");
	}
}

}
}
