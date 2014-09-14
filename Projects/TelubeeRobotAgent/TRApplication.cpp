

#include "stdafx.h"
#include "TRApplication.h"

#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include "win32NetInterface.h"

#include <ViewPort.h>

#include "DirectShowVideoGrabber.h"
#include "IThreadManager.h"
#include <windows.h>
#include "RobotCommunicator.h"
#include "CombineVideoGrabber.h"

#include "DirectSoundInputStream.h"
#include "CMemoryStream.h"
#include "DynamicFontGenerator.h"

#define COMMUNICATION_PORT 6000
#define VIDEO_PORT 5000
#define AUDIO_PORT 5002

namespace mray
{
	class GstVideoGrabberImpl :public GstVideoGrabber
	{
		OS::IMutex* m_mutex;
		video::IVideoGrabber* m_grabber;
	public:
		GstVideoGrabberImpl(video::IVideoGrabber* v)
		{
			m_grabber = v;
			m_mutex = OS::IThreadManager::getInstance().createMutex();
		}
		~GstVideoGrabberImpl()
		{
			delete m_mutex;
		}
		virtual void Lock()
		{
			m_mutex->lock();
		}
		virtual void Unlock()
		{
			m_mutex->unlock();
		}
		virtual video::IVideoGrabber* GetGrabber()
		{
			return m_grabber;
		}
	};

	class AppRobotCommunicatorListener :public IRobotCommunicatorListener
	{
		TRApplication* m_app;
	public:
		AppRobotCommunicatorListener(TRApplication* app)
		{
			m_app = app;
		}
		virtual void OnUserConnected(RobotCommunicator* sender, const network::NetAddress& address, int videoPort, int audioPort)
		{
			m_app->OnUserConnected(address, videoPort, audioPort);
		}
		virtual void OnRobotStatus(RobotCommunicator* sender, const RobotStatus& status)
		{
			m_app->OnRobotStatus(sender, status);
		}
		void OnUserDisconnected(RobotCommunicator* sender, const network::NetAddress& address)
		{
			m_app->OnUserDisconnected(sender, address);

		}
		virtual void OnCollisionData(RobotCommunicator* sender, float left, float right)
		{
		}
	};
	class AppRobotMessageSink :public IMessageSink
	{
		TRApplication* m_app;
	public:
		AppRobotMessageSink(TRApplication* app)
		{
			m_app = app;
		}
		virtual void OnMessage(network::NetAddress* addr,const core::string& msg, const core::string& value)
		{
			m_app->OnMessage(addr,msg, value);
		}

	};


TRApplication::TRApplication()
{
	m_robotCommunicator = 0;
	m_startVideo = 0;
	m_videoProvider = 0;
	m_videoGrabber = 0;
	m_communicatorListener = new AppRobotCommunicatorListener(this);
	m_msgSink = new AppRobotMessageSink(this);
	m_openNi = 0;
	m_commChannel= 0;

	this->m_limitFps = true;
	this->m_limitFpsCount = 30;
	m_robotInited = false;

	m_cameraProfileManager = new CameraProfileManager();
}

TRApplication::~TRApplication()
{
	if (m_videoProvider)
		m_videoProvider->Stop();
	delete m_videoProvider;
	delete m_videoGrabber;
	delete m_robotCommunicator;
	delete m_communicatorListener;
	delete m_msgSink;
	delete m_openNi;
	delete m_commChannel;
	delete m_cameraProfileManager;
}



void TRApplication::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
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
			if (evt->button == 8 && m_controller==EController::Logicool || evt->button== 6 && m_controller==EController::XBox)
			{
				m_robotCommunicator->SetLocalControl(!m_robotCommunicator->IsLocalControl());
			}
		}
	}
}


void TRApplication::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	{
		m_ip = extraOptions.GetOptionValue("TargetIP");
		m_cameraIfo[0].ifo.index = extraOptions.GetOptionByName("Camera0")->getValueIndex();
		m_cameraIfo[1].ifo.index = extraOptions.GetOptionByName("Camera1")->getValueIndex();

		m_cameraIfo[0].w = m_cameraIfo[1].w = 1280;
		m_cameraIfo[0].h = m_cameraIfo[1].h = 720;
		m_cameraIfo[0].fps = m_cameraIfo[1].fps = 25;

		m_debugData.debug = extraOptions.GetOptionByName("Debugging")->getValue() == "Yes";
		m_depthSend = extraOptions.GetOptionByName("DepthStream")->getValue() == "Yes";

		m_controller = extraOptions.GetOptionByName("Controller")->getValue() == "XBox"? EController::XBox : EController::Logicool;

		m_cameraProfile = extraOptions.GetOptionValue("CameraProfile");

		core::string quality = extraOptions.GetOptionByName("Quality")->getValue();
		if (quality == "Ultra Low")m_quality = EStreamingQuality::UltraLow;
		if (quality == "Low")m_quality = EStreamingQuality::Low;
		if (quality == "Medium")m_quality = EStreamingQuality::Medium;
		if (quality == "High")m_quality = EStreamingQuality::High;
		if (quality == "Ultra High")m_quality = EStreamingQuality::UltraHigh;

		m_resolution.set(1280, 720);
		core::string res = extraOptions.GetOptionByName("StreamResolution")->getValue();
		if (res == "HD")
			m_resolution.set(1280, 720);
		else if (res == "FullHD")
			m_resolution.set(1920, 1080);

		m_streamAudio = extraOptions.GetOptionByName("Audio")->getValue() == "Yes";
		m_isLocal = extraOptions.GetOptionByName("Network")->getValue() == "Local";
		m_videoPort = core::StringConverter::toInt(extraOptions.GetOptionByName("VideoPort")->getValue() );
	}
	_InitResources();

	m_limitFps = true;
	network::createWin32Network();

	{
		std::vector<sound::InputStreamDeviceInfo> lst;
		sound::DirectSoundInputStream inputStream;
		inputStream.ListDevices(lst);
		for (int i = 0; i < lst.size(); ++i)
		{
			printf("%d - %s : %s\n", lst[i].ID, lst[i].name.c_str(), lst[i].description.c_str());
		}
	}

	if (m_depthSend)
	{

		m_openNIMngr = new OpenNIManager();
		m_openNIMngr->Init(0, 0);
		m_openNi = new TBee::OpenNIHandler;
		m_openNi->Init();
		
		m_openNi->Start(320,240);
	}
	m_guiRender = new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	if (m_debugData.debug)
	{


		m_viewPort = GetRenderWindow()->CreateViewport("VP", 0, 0, math::rectf(0, 0, 1, 1), 0);
		m_viewPort->AddListener(this);
	}else
		this->GetRenderWindow(0)->Hide();

	{
		core::string camParams[] = {
			video::ICameraVideoGrabber::Param_Brightness,
			video::ICameraVideoGrabber::Param_Contrast,
			video::ICameraVideoGrabber::Param_Hue,
			video::ICameraVideoGrabber::Param_Saturation,
			video::ICameraVideoGrabber::Param_Sharpness,
			video::ICameraVideoGrabber::Param_ColorEnable,
			video::ICameraVideoGrabber::Param_WhiteBalance,
			video::ICameraVideoGrabber::Param_Gain,
			video::ICameraVideoGrabber::Param_BacklightCompensation,
			video::ICameraVideoGrabber::Param_Exposure,
			video::ICameraVideoGrabber::Param_Iris,
			video::ICameraVideoGrabber::Param_Focus
		};
		int count = sizeof(camParams) / sizeof(core::string);
		//init cameras
		CameraProfile* prof = m_cameraProfileManager->GetProfile(m_cameraProfile);
		{
			for (int i = 0; i < 2; ++i)

			{
				video::DirectShowVideoGrabber * grabber = new video::DirectShowVideoGrabber();
				m_cameras[i] = grabber;

				if (m_cameraIfo[i].ifo.index >= 0)
				{
					grabber->InitDevice(m_cameraIfo[i].ifo.index, m_cameraIfo[i].w, m_cameraIfo[i].h, m_cameraIfo[i].fps);//1280, 720
					m_cameraIfo[i].ifo.guidPath = grabber->GetDevicePath(m_cameraIfo[i].ifo.index);
					grabber->SetParameter(video::ICameraVideoGrabber::Param_Focus, "0");
				}

					
				if (prof && false)
				{
					for (int j = 0; j < count; ++j)
					{
						core::string v;
						if (prof->GetValue(camParams[j], v))
							grabber->SetParameter(camParams[j], v);
					}
				}
				//	if (!m_debugData.debug)
				{
					m_cameras[i]->Stop();
				}
				m_cameraTextures[i].Set(m_cameras[i], getDevice()->createEmptyTexture2D(true));

			}
		}
	}

	m_combinedCameras = new CombineVideoGrabber();
	/*if (m_quality==EStreamingQuality::UltraLow)
		m_combinedCameras->SetFrameSize(640, 360);
	else if (m_quality == EStreamingQuality::Low)
		m_combinedCameras->SetFrameSize(768, 432);
	else if (m_quality == EStreamingQuality::Medium)
		m_combinedCameras->SetFrameSize(896, 504);
	else if (m_quality == EStreamingQuality::High)
		m_combinedCameras->SetFrameSize(1024, 576);
	else if (m_quality == EStreamingQuality::UltraHigh)*/
	m_combinedCameras->SetFrameSize(m_resolution.x, m_resolution.y);
	((CombineVideoGrabber*)m_combinedCameras.pointer())->SetGrabbers(m_cameras[0], m_cameras[1]);
	m_cameraTextures[2].Set(m_combinedCameras, getDevice()->createEmptyTexture2D(true));
	m_videoGrabber = new GstVideoGrabberImpl(m_combinedCameras);

	m_videoProvider = new GstVideoProvider();
	m_videoProvider->SetCameras(m_cameraIfo[0].ifo, m_cameraIfo[1].ifo);
	m_videoProvider->SetTargetResolution(m_resolution);
	m_videoProvider->SetNetworkType(m_isLocal);
	m_videoProvider->EnableAudio(m_streamAudio);
	m_videoProvider->SetDataSource(m_videoGrabber);
	m_videoProvider->StreamDataTo(network::NetAddress(m_ip, m_videoPort), m_videoPort, AUDIO_PORT);
//	m_videoProvider->Start();

	m_robotCommunicator = new RobotCommunicator();
	m_robotCommunicator->StartServer(COMMUNICATION_PORT);
	m_robotCommunicator->SetListener(m_communicatorListener);
	m_robotCommunicator->SetMessageSink(m_msgSink);


	m_commChannel = network::INetwork::getInstance().createUDPClient();
	m_commChannel->Open();


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

	if (m_debugData.userConnected && !m_robotInited)
	{
		m_robotCommunicator->Initialize();
		m_robotInited = true;
	}

	if (m_startVideo || m_debugData.debug)
	{
		//m_cameraTextures[2].Blit();
 		m_videoGrabber->Lock();
	//	m_videoGrabber->GetGrabber()->GrabFrame();
		
	/*	if (m_debugData.debug)
			m_cameraTextures[2].Blit();
		else
			m_cameraTextures[2].GetGrabber()->GrabFrame();
 		*/
		m_videoGrabber->Unlock();


		if (m_startVideo && !m_videoProvider->IsConnected())
		{
			/*
			if (!m_debugData.debug )
			{
				for (int i = 0; i < 2; ++i)
				{
					video::DirectShowVideoGrabber * grabber = (video::DirectShowVideoGrabber *)m_cameras[i].pointer();
					if (m_cameraIfo[i].id >= 0)
						grabber->InitDevice(m_cameraIfo[i].id, m_cameraIfo[i].w, m_cameraIfo[i].h, m_cameraIfo[i].fps);//1280, 720
				}
			}*/
			m_videoProvider->Start(m_quality);
			//m_openNi->Start(320,240);
		}
		if (!m_startVideo && m_videoProvider->IsConnected())
		{
			//m_openNi->Close();
			m_videoProvider->Stop();
			if (!m_debugData.debug)
			{
				m_cameras[0]->Stop();
				m_cameras[1]->Stop();
			}
		}
	}

	if (m_robotCommunicator->IsLocalControl())
	{
		math::vector2d speed;
		float rotation;

		controllers::IJoysticController* joystick = m_inputManager->getJoystick(0);
		if (joystick)
		{
			RobotStatus st;
			controllers::JoysticAxis x = joystick->getAxisState(0);
			controllers::JoysticAxis y = joystick->getAxisState(1);
			controllers::JoysticAxis r = joystick->getAxisState(3);

			st.speed[0] = x.abs;
			st.speed[1] = y.abs;
			st.rotation= r.abs;

			st.connected = true;
			st.headRotation[0] = 1;
			st.headRotation[1] = 0;
			st.headRotation[2] = 0;
			st.headRotation[3] = 0;
			m_robotCommunicator->SetRobotData(st);
		}
	}

	if (m_depthSend)
		m_openNi->Update(dt);
	Sleep(30);
}

void TRApplication::onDone()
{
	CMRayApplication::onDone();
}


void TRApplication::onRenderDone(scene::ViewPort*vp)
{
	getDevice()->set2DMode();
/*	video::TextureUnit tex;
	tex.SetTexture(m_cameraTextures[2].GetTexture());
	getDevice()->useTexture(0, &tex);
	math::rectf texCoords(0,1,1,0);
	getDevice()->draw2DImage(vp->getAbsRenderingViewPort(), 1,0,&texCoords);
	*/
	GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();
	if (font){
		m_guiRender->Prepare();

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

#define LOG_OUT(msg,x,y)\
	font->print(math::rectf((x), (y) + yoffset, 10, 10), &attr, 0, msg, m_guiRender);\
	yoffset += attr.fontSize;

		{
			attr.fontSize = 18;
			yoffset = 100;
			core::string msg;
			msg = core::string("User Status: ") + (m_debugData.userConnected ? "Connected":"Disconnected");
			LOG_OUT(msg, 50, 50);
			if (m_debugData.userConnected)
			{
				msg = "Address: " + m_debugData.userAddress.toString();
				LOG_OUT(msg, 100, 50);
			}
			{
				msg = core::string("Robot Status: ") + (m_debugData.robotData.connected ? "Connected" : "Disconnected");
				LOG_OUT(msg, 50, 100);
				msg = core::string("Controlling: ") + (m_robotCommunicator->IsLocalControl() ? "Local" : "Remote");
				LOG_OUT(msg, 50, 100);
				msg = core::string("Sensors : ") + core::StringConverter::toString(math::vector2d(m_debugData.collision));
				LOG_OUT(msg, 50, 100);
				if (m_debugData.robotData.connected || m_robotCommunicator->IsLocalControl())
				{
					msg = core::string("Speed: ") + core::StringConverter::toString(math::vector2d(m_debugData.robotData.speed[0], m_debugData.robotData.speed[1]));
					LOG_OUT(msg, 100, 100);

					msg = core::string("Rotation: ") + core::StringConverter::toString(m_debugData.robotData.rotation);
					LOG_OUT(msg, 100, 100);

					math::vector3d angles;
					math::quaternion q(m_debugData.robotData.headRotation[0], m_debugData.robotData.headRotation[1],
						m_debugData.robotData.headRotation[2], m_debugData.robotData.headRotation[3]);
					q.toEulerAngles(angles);
					msg = core::string("Head Rotation: ") + core::StringConverter::toString(angles);
					LOG_OUT(msg, 100, 100);

					msg = core::string("Head Position: ") + core::StringConverter::toString(math::vector3d(m_debugData.robotData.headPos[0], m_debugData.robotData.headPos[1], m_debugData.robotData.headPos[2]));
					LOG_OUT(msg, 100, 100);

				}

			}
		}


	}

	m_guiRender->Flush();
	getDevice()->useShader(0);
}
void TRApplication::OnUserConnected(const network::NetAddress& address, int videoPort, int audioPort)
{
	if (m_remoteAddr.address != address.address)
		printf("User Connected : %s\n", address.toString().c_str());
	m_remoteAddr.address = address.address;
	m_videoProvider->StreamDataTo(address,videoPort,audioPort);
	m_debugData.userAddress = address;
	m_debugData.userConnected = true;

	m_startVideo = true;

	const int BufferLen = 128;
	uchar buffer[BufferLen];
	//tell the client if we are sending stereo or single video images
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	{
		int reply = (int)EMessages::IsStereo;
		int len = stream.write(&reply, sizeof(reply));
		bool stereo = m_videoProvider->IsStereoCameras();
		len += stream.write(&stereo, sizeof(stereo));
		m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);
	}


	{
		stream.seek(0,OS::ESeek_Set);
		int reply = (int)EMessages::CameraConfig;
		int len = stream.write(&reply, sizeof(reply));
		len+= wrtr.binWriteString(m_cameraProfile);
		m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);
	}


}
void TRApplication::OnRobotStatus(RobotCommunicator* sender, const RobotStatus& status)
{
	m_debugData.robotData = status;
}
void TRApplication::OnCollisionData(RobotCommunicator* sender, float left, float right)
{
	m_debugData.collision.x = left;
	m_debugData.collision.y = right;
}
void TRApplication::OnUserDisconnected(RobotCommunicator* sender, const network::NetAddress& address)
{
	m_debugData.userConnected = false;
	m_startVideo = false;
	printf("User Disconnected : %s\n", address.toString().c_str());
}
void TRApplication::OnCalibrationDone(RobotCommunicator* sender)
{

	const int BufferLen = 64;
	uchar buffer[BufferLen];
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	int reply = (int)EMessages::DepthSize;
	int len = stream.write(&reply, sizeof(reply));
	m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);
}

void TRApplication::OnReportMessage(RobotCommunicator* sender, int code, const core::string& msg)
{
	const int BufferLen = 512;
	uchar buffer[BufferLen];
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	int reply = (int)EMessages::ReportMessage;
	int len = stream.write(&reply, sizeof(reply));
	len += wrtr.binWriteInt(code);
	len += wrtr.binWriteString(msg);
	m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);

}

void TRApplication::OnMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
{
	const int BufferLen = 65537;
	uchar buffer[BufferLen];
	int i= msg.find('#');
	core::string m;
	if (i != -1)
		m = msg.substr(0, i);
	else m = msg;
	if (m.equals_ignore_case("commPort"))
	{
		m_remoteAddr.port = core::StringConverter::toInt(value);
	}
	else
	if (m.equals_ignore_case("depthSize") && m_depthSend)
	{
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		int reply = (int)EMessages::DepthSize;
		int len = stream.write(&reply, sizeof(reply));
		math::vector2di sz= m_openNi->GetSize();
		len += stream.write(&sz,sizeof(sz));
		m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);
	}
	else
	if (m.equals_ignore_case("depth") && m_depthSend)
	{
		math::rectf rc = core::StringConverter::toRect(value);
		OS::CMemoryStream stream("",buffer,BufferLen,false,OS::BIN_WRITE);
		TBee::DepthFrame* f= m_openNi->GetNormalCalculator().GetDepthFrame();
		m_depthRect.SetFrame(f, rc);
		int reply = (int)EMessages::DepthData;
		int len = stream.write(&reply, sizeof(reply));
		len+=m_depthRect.WriteToStream(&stream);
		m_commChannel->SendTo(&m_remoteAddr, (char*)buffer, len);
	}
}
CameraProfileManager* TRApplication::LoadCameraProfiles(const core::string& path)
{
	m_cameraProfileManager->LoadFromXML(path);
	return m_cameraProfileManager;
}

}