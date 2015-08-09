

#include "stdafx.h"
#include "TRApplication.h"

#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include "win32NetInterface.h"

#include <ViewPort.h>

#include "IThreadManager.h"
#include <windows.h>
#include "RobotCommunicator.h"
#include "CombineVideoGrabber.h"

#include "CMemoryStream.h"
#include "DynamicFontGenerator.h"

#include "TextureRTWrap.h"

#include "AVStreamServiceModule.h"
#include "AVPlayerServiceModule.h"
#include "HandsWindowServiceModule.h"
#include "RobotControlServiceModule.h"

#include "Console.h"

#include <conio.h>

#undef StartService

#define SEND_ROBOT_SENSORS 0
#define COMMUNICATION_PORT 6000

namespace mray
{



TRApplication::TRApplication()
{
	m_startVideo = 0;
#if USE_OPENNI
	m_openNi = 0;
#endif
	m_serviceContext.commChannel= 0;

	this->m_limitFps = true;
	this->m_limitFpsCount = 60;
	m_robotInited = false;


	m_debugging = false;
	m_enablePlayers = false;
	m_enableStream = true;

	m_robotCommunicator = 0;
	m_isDone = false;
}

TRApplication::~TRApplication()
{
	onClose();
}

void TRApplication::onClose()
{
	m_isDone = true;
	//wait until threads are done 
	Sleep(1000);
#if USE_OPENNI
	if (m_openNi)
		m_openNi->Close();
	delete m_openNi;
#endif

	for (int i = 0; i < m_services.size(); ++i)
	{
		m_services[i]->DestroyService();
		delete m_services[i];
	}
	m_services.clear();


	delete m_serviceContext.commChannel;
}


void TRApplication::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	(gLogManager.StartLog(ELL_INFO) << "Initing Resources").flush();;

	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
	gImageSetResourceManager.loadImageSet(mT("Icons\\icons.imageset"));
	GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
	GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
	GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

	//load font
	GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator("Arial24");
	font->SetFontName(L"Arial");
	font->SetTextureSize(1024);
	font->SetFontResolution(24);
	font->Init();

	//GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
	//gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
	gFontResourceManager.setDefaultFont(font);

	gLogManager.log("Resources Loaded", ELL_SUCCESS);
}


void TRApplication::onEvent(Event* e)
{
//#define JOYSTICK_SelectButton 8
	CMRayApplication::onEvent(e);
	if (e->getType() == ET_Joystick)
	{
		JoystickEvent* evt = (JoystickEvent*)e;
		if (evt->event == JET_BUTTON_PRESSED)
		{
			if (evt->button == 6 && m_controller==EController::Logicool || evt->button== 6 && m_controller==EController::XBox)
			{
			//	m_robotCommunicator->SetLocalControl(!m_robotCommunicator->IsLocalControl());
			}
		}
	}
	else if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press && evt->key == KEY_S)
		{
#if 0
			bool started = core::StringConverter::toBool(m_robotCommunicator->GetRobotController()->ExecCommand(IRobotController::CMD_IsStarted, ""));
			if (started)
				m_robotCommunicator->GetRobotController()->ExecCommand(IRobotController::CMD_Stop,"");
			else
				m_robotCommunicator->GetRobotController()->ExecCommand(IRobotController::CMD_Start,"");
#endif
		}
		if (evt->press && evt->key == KEY_F9)
		{
			m_debugging = !m_debugging;
		}
	}
}

void TRApplication::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	m_serviceContext.app = this;
	m_serviceContext.appOptions = extraOptions;
	{
		m_debugData.debug = extraOptions.GetOptionByName("Debugging")->getValue() == "Yes";

#if USE_OPENNI
		m_depthSend = extraOptions.GetOptionByName("DepthStream")->getValue() == "Yes";
#endif
#if USE_PLAYERS
		m_enablePlayers = extraOptions.GetOptionByName("EnablePlayers")->getValue() == "Yes";
#endif

		m_enableStream = extraOptions.GetOptionByName("EnableStreams")->getValue() == "Yes";
		
		m_controller = extraOptions.GetOptionByName("Controller")->getValue() == "XBox" ? EController::XBox : EController::Logicool;
// 		if (quality == "Ultra Low")m_quality = EStreamingQuality::UltraLow;
// 		if (quality == "Low")m_quality = EStreamingQuality::Low;
// 		if (quality == "Medium")m_quality = EStreamingQuality::Medium;
// 		if (quality == "High")m_quality = EStreamingQuality::High;
// 		if (quality == "Ultra High")m_quality = EStreamingQuality::UltraHigh;

	}
	_InitResources();

	m_limitFps = true;
	network::createWin32Network();


	printf("Initializing RobotCommunicator\n");
	m_robotCommunicator = new RobotCommunicator();
	m_robotCommunicator->StartServer(COMMUNICATION_PORT);
	m_robotCommunicator->SetListener(this);
#if USE_OPENNI
	if (m_depthSend)
	{

		m_openNIMngr = new OpenNIManager();
		m_openNIMngr->Init(0, 0);
		m_openNi = new TBee::OpenNIHandler;
		m_openNi->Init();
		
		m_openNi->Start(320,240);
	}
#endif
	m_guiRender = new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	if (m_debugData.debug)
	{


		m_viewPort = GetRenderWindow()->CreateViewport("VP", 0, 0, math::rectf(0, 0, 1, 1), 0);
		m_viewPort->AddListener(this);
	}else
		this->GetRenderWindow(0)->Hide();

	m_services.push_back(new TBee::RobotControlServiceModule());
	if (m_enableStream)
	{
		//Setup AVStreamer

		m_services.push_back(new TBee::AVStreamServiceModule());
	}

#if USE_HANDS
	m_services.push_back(new TBee::HandsWindowServiceModule());
#endif

#if USE_PLAYERS
	if (m_enablePlayers)
	{
		m_services.push_back(new TBee::HandsWindowServiceModule());
	}
#endif


	Console::setColor(0, 1, 0);
	printf("Initializing Services:\n");
	for (int i = 0; i < m_services.size(); ++i)
	{
		Console::setColor(1, 0, 0);
		printf("\t>Service:%s\n", m_services[i]->GetServiceName().c_str());
		Console::setColor(1, 1, 1);
		m_services[i]->InitService(&m_serviceContext);
		if (m_services[i]->GetServiceStatus() == TBee::EServiceStatus::Inited)
		{
			Console::setColor(0, 1, 0);

			printf("\t>>Service %s Initialized\n", m_services[i]->GetServiceName().c_str());
		}
		else
		{
			Console::setColor(1, 0, 0);

			printf("\t>>Service %s Failed\n", m_services[i]->GetServiceName().c_str());
		}

	}
	Console::setColor(1, 1, 1);



	m_serviceContext.commChannel = network::INetwork::getInstance().createUDPClient();
	m_serviceContext.commChannel->Open();

	m_isStarted = false;

	if (false)
	{
		printf("Press [space] to ignore remote connection. (5 seconds timeout)\n");
		float t0 = gEngine.getTimer()->getSeconds();
		float t1;
		do
		{
			t1 = gEngine.getTimer()->getSeconds();
			if (kbhit())
			{
				uchar c = getch();
				if (c == ' '){
					m_debugData.userConnected = true;
					m_debugData.userAddress.setIP("127.0.0.1");
#if 0
					if (m_enableStream && m_streamers)
					{
						m_streamers->GetStream("Video")->BindPorts("127.0.0.1", 7000, 0, 0);
					}
#endif 
					m_startVideo = true;
					printf("Force starting robot connection ignoring remote side.\n");
					break;
				}
			}
		} while (t1 - t0 < 5000);
	}
	if (!m_startVideo)
	{
		printf("Start listening to incoming connections.\n");
	}

	return;

}

void TRApplication::draw(scene::ViewPort* vp)
{
}

void TRApplication::WindowPostRender(video::RenderWindow* wnd)
{
}

void TRApplication::update(float dt)
{
	CMRayApplication::update(dt);


	if (m_startVideo || m_debugData.debug)
	{
		//m_cameraTextures[2].Blit();
 		//m_videoGrabber->Lock();
	//	m_videoGrabber->GetGrabber()->GrabFrame();
		
	/*	if (m_debugData.debug)
			m_cameraTextures[2].Blit();
		else
			m_cameraTextures[2].GetGrabber()->GrabFrame();
 		*/
		//m_videoGrabber->Unlock();


		if (m_startVideo && !m_isStarted )
		{
			//User got connected

			for (int i = 0; i < m_services.size(); ++i)
			{
				m_services[i]->StartService(&m_serviceContext);
			}
			m_isStarted = true;

			//m_openNi->Start(320,240);
		}

		if (m_isStarted)
		{
		}
	}
	if (!m_startVideo && m_isStarted)
	{
#if USE_OPENNI
		m_openNi->Close();
#endif
		for (int i = 0; i < m_services.size(); ++i)
		{
			m_services[i]->StopService();
		}
		m_isStarted = false;
	}


	for (int i = 0; i < m_services.size(); ++i)
	{
		m_services[i]->Update(dt);
	}

#if USE_OPENNI
	if (m_depthSend)
		m_openNi->Update(dt);
#endif
	Sleep(30);
}

void TRApplication::onDone()
{
	CMRayApplication::onDone();
}



void TRApplication::onRenderDone(scene::ViewPort*vp)
{
	if (vp == m_viewPort)
	{
		getDevice()->set2DMode();
		video::TextureUnit tex;
		math::vector2d txsz;

		/*	*/

		GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();

		m_renderContext.font = font;
		m_renderContext.viewPort = vp;
		m_renderContext.guiRenderer = m_guiRender;
		m_guiRender->Prepare();
		m_renderContext.Reset();


		for (int i = 0; i < m_services.size(); ++i)
		{
			m_renderContext.RenderText("[" + m_services[i]->GetServiceName() + "]:" + TBee::IServiceModule::ServiceStatusToString(m_services[i]->GetServiceStatus()), 50, 0,video::SColor(0,1,0,1));
			m_services[i]->Render(&m_renderContext);
			m_renderContext.RenderText("-----------", 50, 0);
		}

		if (font && m_debugging){

			m_renderContext.RenderText("Debug Data", 50, 0);
			m_renderContext.RenderText("-----------", 50, 0);
			{
				core::string msg;
				msg = core::string("User Status: ") + (m_debugData.userConnected ? "Connected" : "Disconnected");
				m_renderContext.RenderText(msg, 50, 0);
				if (m_debugData.userConnected)
				{
					msg = "Address: " + m_debugData.userAddress.toString();
					m_renderContext.RenderText(msg, 100, 0);
				}
			}
			for (int i = 0; i < m_services.size(); ++i)
			{
				m_renderContext.RenderText("[" + m_services[i]->GetServiceName() + "]", 50, 0, video::SColor(0, 1, 0, 1));
				m_services[i]->DebugRender(&m_renderContext);
				m_renderContext.RenderText("-----------", 50, 0);
			}

		}

		m_guiRender->Flush();
		getDevice()->useShader(0);
	}
}
void TRApplication::OnUserConnected(RobotCommunicator* sender,const UserConnectionData& data)
{
	if (m_isDone)
		return;
	if (m_serviceContext.remoteAddr.address != data.address.address)
		printf("User Connected : %s\n", data.address.toString().c_str());
	m_serviceContext.remoteAddr.address = data.address.address;
	//m_videoProvider->StreamDataTo(address,videoPort,audioPort);
	int ip[4];
	data.address.getIP(ip);
	core::string ipaddr = core::StringConverter::toString(ip[0]) + "."+
		core::StringConverter::toString(ip[1]) + "." +
		core::StringConverter::toString(ip[2]) + "." +
		core::StringConverter::toString(ip[3]);


	m_debugData.userAddress = data.address;
	m_debugData.userConnected = true;


	m_serviceContext.__FIRE_OnUserConnected(data);

	m_startVideo = true;



}
void TRApplication::OnRobotStatus(RobotCommunicator* sender, const RobotStatus& status)
{
}
void TRApplication::OnUserDisconnected(RobotCommunicator* sender, const network::NetAddress& address)
{
	m_debugData.userConnected = false;
	m_startVideo = false;
	printf("User Disconnected : %s\n", address.toString().c_str());
	m_serviceContext.__FIRE_OnUserDisconnected();
}
void TRApplication::OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
{
	const int BufferLen = 65537;
	uchar buffer[BufferLen];
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	int i= msg.find('#');
	core::string m;
	if (i != -1)
		m = msg.substr(0, i);
	else m = msg;
	if (m.equals_ignore_case("commPort"))
	{
		m_serviceContext.remoteAddr.port = core::StringConverter::toInt(value);
	}
	m_serviceContext.__FIRE_OnUserMessage(addr,msg,value);
#if USE_OPENNI
	else
	if (m.equals_ignore_case("depthSize") && m_depthSend)
	{
		int reply = (int)EMessages::DepthSize;
		int len = stream.write(&reply, sizeof(reply));
		math::vector2di sz= m_openNi->GetSize();
		len += stream.write(&sz,sizeof(sz));
		m_serviceContext.commChannel->SendTo(&m_serviceContext.remoteAddr, (char*)buffer, len);
	}
	else
	if (m.equals_ignore_case("depth") && m_depthSend)
	{
		math::rectf rc = core::StringConverter::toRect(value);
		TBee::DepthFrame* f= m_openNi->GetNormalCalculator().GetDepthFrame();
		m_depthRect.SetFrame(f, rc);
		int reply = (int)EMessages::DepthData;
		int len = stream.write(&reply, sizeof(reply));
		len+=m_depthRect.WriteToStream(&stream);
		m_serviceContext.commChannel->SendTo(&m_serviceContext.remoteAddr, (char*)buffer, len);
	}
#endif
}

void TRApplication::OnStreamerReady(video::IGStreamerStreamer* s)
{

	printf("Stream is Ready\n");
	const int BufferLen = 128;
	uchar buffer[BufferLen];
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	//Send clock message
#if 0
	{
		stream.seek(0, OS::ESeek_Set);
		int reply = (int)EMessages::ClockSync;
		int len = stream.write(&reply, sizeof(reply));

		ulong baseClock = s->GetClockBase();

		len += wrtr.binWriteInt(baseClock);
		m_serviceContext.commChannel->SendTo(&m_serviceContext.remoteAddr, (char*)buffer, len);
	}
#endif

}
void TRApplication::OnStreamerStarted(video::IGStreamerStreamer* s)
{
	printf("Stream has Started\n");
}
void TRApplication::OnStreamerStopped(video::IGStreamerStreamer* s)
{
	printf("Stream has Stopped\n");
}


}