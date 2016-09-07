

#include "stdafx.h"

#if !_WIN64
#define USE_POINTGREY 1
#endif

#include "AVStreamServiceModule.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "ICameraVideoGrabber.h"
#include "VideoGrabberTexture.h"
#include "IThreadManager.h"
#include "CameraProfile.h"
#include "GstNetworkVideoStreamer.h"
#include "GstNetworkAudioStreamer.h"
#include "DirectShowVideoGrabber.h"
#include "GstCustomVideoStreamer.h"
#include "GstCustomMultipleVideoStreamer.h"
#include "GstNetworkAudioPlayer.h"
#include "DirectSoundInputStream.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "GstVideoProvider.h"
#include "NetworkValueController.h"
#include "CameraConfigurationManager.h"
#include "StringUtil.h"
#include "AppSrcVideoSrc.h"
#include "CameraVideoSrc.h"
#include "CameraStreamController.h"
#include "capDevice.h"

#include <conio.h>

#if !USE_POINTGREY
#include "FlyCameraVideoGrabber.h"
#include "FlyCameraManager.h"
#endif

namespace mray
{
namespace TBee
{
#define USE_WEBCAMERA 1


//#define FOVE_PEPPER

IMPLEMENT_RTTI(AVStreamServiceModule, IServiceModule);

const std::string AVStreamServiceModule::ModuleName("AVStreamServiceModule");


class AVStreamServiceModuleImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:

	GCPtr<video::GstStreamBin> m_streamers;
	ECameraType m_cameraType;
	math::vector2di m_resolution;
	int m_fps;
	CameraConfigurationManager* m_camConfigMngr;
	TelubeeCameraConfiguration* m_camConfig;

	int m_streamsCount;
	//video::VideoGrabberTexture m_cameraTextures[2];

	bool m_supportAudio;
	xml::XMLTree m_streamingParameters;
	xml::XMLElement* m_paramsElement;

	struct CameraSettings
	{
		CameraSettings(){}
		CameraSettings(const math::vector2d& sz, int bits, int f)
		{
			size = sz;
			bitrate = bits;
			fps = f;
		}
		math::vector2di size;
		int bitrate;
		int fps;
	};
	std::vector<CameraSettings> m_cameraSettings;
	CameraSettings m_currentSettings;
	//std::vector<_CameraInfo> m_cameraIfo;

	ICameraSrcController* m_cameraController;

	std::vector<uint> m_VideoPorts;
	uint m_AudioPort;
	bool m_portsReceived;
	bool m_isVideoStarted;
	network::NetAddress m_remoteAddr;

	float m_currentGain;
	float m_lastGainUpdate;
	float m_maxGain;
	bool m_autoGain;

	int m_quality;
	CameraProfileManager* m_cameraProfileManager;
	core::string m_cameraProfile;

	EServiceStatus m_status;

	TBeeServiceContext* m_context;

	video::GstNetworkAudioPlayer* m_audioPlayer;

public:
	AVStreamServiceModuleImpl()
	{
		m_status = EServiceStatus::Idle;
		m_cameraProfileManager = new CameraProfileManager();
		m_camConfigMngr = new CameraConfigurationManager();
		m_camConfigMngr->LoadConfigurations("CameraConfigurations.xml");

		m_streamers = new video::GstStreamBin();
		LoadCameraSettings("StreamingProfiles.xml");

		m_streamsCount = 0;
		m_audioPlayer = 0;
		m_supportAudio = false;
		/*
		m_VideoPorts[0] = 7000;
		m_VideoPorts[1] = 7001;*/

		m_portsReceived = false;
		m_isVideoStarted = false;

		m_currentGain = 0;
		m_lastGainUpdate = 0;
		m_maxGain = 0.25;
		m_autoGain = false;
	}
	~AVStreamServiceModuleImpl()
	{
		Destroy();
		m_streamers = 0;
		delete m_cameraProfileManager;
		delete m_camConfigMngr;
#if USE_POINTGREY
		if (video::FlyCameraManager::isExist())
			delete video::FlyCameraManager::getInstancePtr();
#endif
	}

	void LoadCameraSettings(const core::string &src)
	{
		core::string path;
		gFileSystem.getCorrectFilePath(src, path);

		if (path == "" || m_streamingParameters.load(path) == false)
		{
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 3000, 30));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 4000, 45));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 5000, 50));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(960, 720), 6000, 45));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(960, 720), 7000, 50));
			return;
		}

		xml::XMLElement* e;
		m_paramsElement = m_streamingParameters.getSubElement("Settings");
		e = m_paramsElement->getSubElement("Setting");

		while (e)
		{
			CameraSettings s;

			s.size = core::StringConverter::toVector2d(e->getValueString("Resolution"));
			s.fps = e->getValueInt("FPS");
			s.bitrate = e->getValueInt("Bitrate");
			m_cameraSettings.push_back(s);
			e = e->nextSiblingElement("Setting");
		}
	}
	/*
	void SetCameraParameterValue(const core::string& name, const core::string& value)
	{

		for (int i = 0; i < 2; ++i)
		{
			if (m_cameraIfo[i].camera)
			{
				m_cameraIfo[i].camera->SetParameter(name, value);

				printf("Camera [%d] %s value is set to: %s\n", i, name.c_str(), m_cameraIfo[i].camera->GetParameter(name).c_str());
			}
		}
	}
	core::string GetCameraParameterValue(const core::string& name, int i)
	{

		if (m_cameraIfo[i].camera)
		{
			return m_cameraIfo[i].camera->GetParameter(name);
		}
		return core::string::Empty;
	}*/

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;
		m_resolution.set(1280, 720);

#if USE_POINTGREY && USE_WEBCAMERA
		core::string camType= context->appOptions.GetOptionByName("CameraConnection")->getValue();
		if (camType == "DirectShow")
			m_cameraType=ECameraType::Webcam ;
		if (camType == "Ovrvision")
			m_cameraType = ECameraType::Ovrvision;
		if (camType == "OvrvisionRaw")
			m_cameraType = ECameraType::OvrvisionCompressed;
		if (camType == "PointGrey")
			m_cameraType=ECameraType::PointGrey ;
#else 
#if USE_POINTGREY
		m_cameraType = ECameraType::PointGrey;
#else
		m_cameraType = ECameraType::Webcam;
#endif

#endif
		m_cameraProfile = context->appOptions.GetOptionValue("CameraProfile");

		{

			m_camConfig = m_camConfigMngr->GetCameraConfiguration(m_cameraProfile);
			if (!m_camConfig)
				gLogManager.log("Couldn't find camera configurations! : " + m_cameraProfile, ELL_ERROR);

			gLogManager.log("Capture Type:" + core::StringConverter::toString(m_camConfig->captureType), ELL_INFO);

			//m_camConfig->captureType = TBee::TelubeeCameraConfiguration::Capture

			//Create Camera Controller
			if (m_camConfig->captureType == TBee::TelubeeCameraConfiguration::CaptureRaw)
			{
				gLogManager.log("Creating Raw Capture Camera", ELL_INFO);
				m_cameraController = new EncodedCameraStreamController(m_camConfig->captureType);
				//m_cameraController = new CameraGrabberController();
			}else 
			{
				gLogManager.log("Creating Encoded Capture Camera", ELL_INFO);
				m_cameraController = new EncodedCameraStreamController(m_camConfig->captureType);
			}
		}
		m_quality = core::StringConverter::toInt(context->appOptions.GetOptionByName("Quality")->getValue());
		if (context->appOptions.GetOptionByName("AutoGain"))
			m_autoGain = core::StringConverter::toBool(context->appOptions.GetOptionByName("AutoGain")->getValue());
		else m_autoGain = false;
		/*core::string res = context->appOptions.GetOptionByName("StreamResolution")->getValue();
		
		if (res == "0-VGA")
			m_resolution.set(640, 480);
		else if (res == "1-HD")
			m_resolution.set(1280, 720);
		else if (res == "2-FullHD")
			m_resolution.set(1920, 1080);*/
		m_supportAudio = context->appOptions.GetOptionByName("Audio")->getValue() == "Yes";

		std::vector<_CameraInfo> m_cameraIfo;

#ifdef USE_WEBCAMERA
		if (m_cameraType == ECameraType::Ovrvision)
		{

			m_camConfig->streamType = TelubeeCameraConfiguration::StreamCoded;
			// -1 for the None index
			_CameraInfo ifo;
			ifo.ifo.index = 0;
			m_cameraIfo.push_back(ifo);
			ifo.ifo.index = 1;
			m_cameraIfo.push_back(ifo);
			m_streamsCount = 2;
		}
		else
		if (m_cameraType == ECameraType::OvrvisionCompressed)
		{

			m_camConfig->streamType = TelubeeCameraConfiguration::StreamOvrvision;
			// -1 for the None index
			_CameraInfo ifo;
			ifo.ifo.index = 0;
			m_cameraIfo.push_back(ifo);
			m_streamsCount = 1;
		}
		else
		if (m_cameraType == ECameraType::Webcam)
		{
			// -1 for the None index
			_CameraInfo ifo;
			ifo.ifo.index = core::StringConverter::toInt(context->appOptions.GetOptionByName("DS_Camera_Left")->getValue());
			if (ifo.ifo.index>= 0)
			{
				m_cameraIfo.push_back(ifo);
				++m_streamsCount;
			}
			ifo.ifo.index = core::StringConverter::toInt(context->appOptions.GetOptionByName("DS_Camera_Right")->getValue());
			if (ifo.ifo.index >= 0)
			{
				m_cameraIfo.push_back(ifo);
				++m_streamsCount;
			}
		}
#endif
#ifdef USE_POINTGREY 
		if (m_cameraType == ECameraType::PointGrey)
		{
			_CameraInfo ifo;

			//create flycapture manager
			new video::FlyCameraManager();

			//point grey cameras have unique serial number
			int count = video::FlyCameraManager::getInstance().GetCamerasCount();
			int c1 = core::StringConverter::toInt(context->appOptions.GetOptionByName("PT_Camera_Left")->getValue());
			int c2 = core::StringConverter::toInt(context->appOptions.GetOptionByName("PT_Camera_Right")->getValue());
			for (int i = 0; i < count; ++i)
			{
				uint sp;
				video::FlyCameraManager::getInstance().GetCameraSerialNumber(i, sp);
				if (sp == c1)
				{
					ifo.ifo.index = i;
					m_cameraIfo.push_back(ifo);
					++m_streamsCount;
				}
				if (sp == c2)
				{
					ifo.ifo.index = i;
					m_cameraIfo.push_back(ifo);
					++m_streamsCount;
				}

			}
		}
#endif

		m_VideoPorts.resize(m_streamsCount);
/*
		{
			std::vector<sound::InputStreamDeviceInfo> lst;
			sound::DirectSoundInputStream inputStream;
			inputStream.ListDevices(lst);
			for (int i = 0; i < lst.size(); ++i)
			{
				printf("%d - %s : %s, %s\n", lst[i].ID, lst[i].name.c_str(), lst[i].description.c_str(),lst[i].deviceGUID.c_str());
			}
		}
*/

		{
			//load camera cropping offsets

			OS::IStreamPtr s = gFileSystem.openFile(gFileSystem.getAppPath() + "CameraOffsets.txt", OS::TXT_READ);
			if (!s.isNull())
			{
				OS::StreamReader rdr(s);
				core::StringConverter::parse(rdr.readLine(), m_cameraIfo[0].offsets);
				core::StringConverter::parse(rdr.readLine(), m_cameraIfo[1].offsets);
				s->close();
			}
		}
		printf("Initializing Cameras\n");
		m_quality = math::Min<int>((int)m_quality, m_cameraSettings.size());
		m_currentSettings = m_cameraSettings[m_quality];
		math::vector2d capRes = m_resolution = m_currentSettings.size;
		int fps = m_fps = m_currentSettings.fps;

		for (int i = 0; i < m_cameraIfo.size(); ++i)
		{
			m_cameraIfo[i].fps = fps;
			m_cameraIfo[i].w = capRes.x;
			m_cameraIfo[i].h = capRes.y;
		}
#if USE_WEBCAMERA
		if (m_cameraType == ECameraType::Webcam || m_cameraType == ECameraType::Ovrvision || m_cameraType == ECameraType::OvrvisionCompressed)
		{
			//video::GstNetworkVideoStreamer* vstreamer;
			//Init Cameras
			m_cameraController->SetCameras(m_cameraIfo,m_cameraType);

			m_currentGain =  core::StringConverter::toFloat(context->appOptions.GetOptionByName("Gain")->getValue());
			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Focus, "0");
			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Exposure, context->appOptions.GetOptionByName("Exposure")->getValue());
			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gain, core::StringConverter::toString(m_currentGain));// context->appOptions.GetOptionByName("Gain")->getValue());

			// Now close cameras
			m_cameraController->Stop();

			printf("Creating Video Streamer\n");

			video::GstCustomMultipleVideoStreamer* hs = new video::GstCustomMultipleVideoStreamer();
			hs->AddListener(this);

			
			video::ICustomVideoSrc* src = m_cameraController->CreateVideoSrc();
			src->SetResolution(m_resolution.x, m_resolution.y, fps, true);
			src->SetBitRate(m_currentSettings.bitrate);

			src->LoadParameters(m_paramsElement->getSubElement("Encoder"));
			m_camConfig->encoderType = src->GetEncoderType();
			hs->SetVideoSrc(src);

			m_streamers->AddStream(hs, "Video");

		}
#endif
#if USE_POINTGREY
		if (m_cameraType == ECameraType::PointGrey)
		{
			//Bug: Open and close the cameras once, needed to make the camera run next time
			m_cameraController->SetCameras(m_cameraIfo, m_cameraType);
			m_cameraController->Start();
			m_cameraController->Stop();

			printf("Creating Video Streamer\n");
			video::GstCustomMultipleVideoStreamer* hs = new video::GstCustomMultipleVideoStreamer();
			hs->AddListener(this);


			printf("Setting Streaming Settings: %dx%d@%d - %d kbps\n", m_resolution.x, m_resolution.y, fps, m_currentSettings.bitrate);
			video::ICustomVideoSrc* src = m_cameraController->CreateVideoSrc();
			src->SetResolution(m_resolution.x, m_resolution.y, fps, true);
			src->SetBitRate(m_currentSettings.bitrate);
			hs->SetVideoSrc(src);

			m_streamers->AddStream(hs, "Video");
		}
#endif

		//Create Audio Stream
		/*
		if (m_supportAudio)
		{
			printf("Creating Audio Streamer\n");
			video::GstNetworkAudioStreamer* streamer;
			streamer = new video::GstNetworkAudioStreamer();
			m_streamers->AddStream(streamer, "Audio");
		}*/

		if (m_supportAudio)
		{
			m_audioPlayer = new video::GstNetworkAudioPlayer();
		}
		else m_audioPlayer = 0;
		printf("Finished streams\n");


		m_streamers->GetStream("Video")->CreateStream();
		/*if (m_streamAudio)
			m_streamers->GetStream("Audio")->CreateStream();*/

		//setup cameras settings
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
		_RegisterValues();

		context->AddListener(this);
		m_status = EServiceStatus::Inited;

		printf("Done Initing.\n");
	}

	void _RegisterValues()
	{

		ValueGroup* g = new ValueGroup("Camera");
		m_context->netValueController->GetValues()->AddValueGroup(g);
		core::string value;
		IValue* tmpV;
#define GET_VALUE(X) (m_cameraController->GetCameraParameterValue(X,0))
#define ADD_CAMERA_VALUE(Type,Name)\
	value = GET_VALUE(Name); \
	tmpV = g->AddValue(new Type(Name, 0)); \
		if (value != "")\
		tmpV->parse(value); \
		tmpV->OnChanged += newClassDelegate1("", this, &AVStreamServiceModuleImpl::_OnCameraPropertyChanged);

		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Exposure);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Gain);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_WhiteBalance);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Gamma);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Brightness);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Saturation);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Sharpness);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Contrast);

	}
	void _OnCameraPropertyChanged(IValue* v)
	{
		m_cameraController->SetCameraParameterValue(v->getName(), v->toString());
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		 
// 		m_cameraTextures[0].Set(0, 0);
// 		m_cameraTextures[1].Set(0, 0);

		StopStream();
		m_streamers->CloseAll();

		m_context->RemoveListener(this);
		m_streamers->ClearStreams(true);
		m_cameraController->Stop();
		delete m_cameraController;
		m_cameraController = 0;

		if (m_audioPlayer)
		{
			m_audioPlayer->Close();
			delete m_audioPlayer;
		}

		m_status = EServiceStatus::Idle;
	}


	void _startVideoStream()
	{
		if (!m_portsReceived || m_status != EServiceStatus::Running)
			return;
		printf("Starting Stream at :%dx%d@%d\n", m_resolution.x, m_resolution.y, m_fps);
		//  Begin the video stream

		//if (m_cameraType == ECameraType::PointGrey)
		{
			//Pointgrey cameras need to be started manually
			printf("Starting Cameras.\n");
			m_cameraController->Start();
			Sleep(100);
		}
		printf("Start Streaming.\n");
		m_streamers->Stream();
		Sleep(500);

		m_lastGainUpdate = gEngine.getTimer()->getSeconds() + 2000;
		printf("Stream started.\n");


		m_isVideoStarted = true;
	}

	void StartStream()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

	//	Sleep(1000);
		m_status = EServiceStatus::Running;
#ifdef FOVE_PEPPER
		m_VideoPorts[0] = 7000;
		m_VideoPorts[1] = 7001;
		m_portsReceived = true;
		m_streamers->GetStream("Video")->BindPorts(m_context->remoteAddr.toString(), m_VideoPorts, 2, 0, 0);
#endif
		if (m_audioPlayer)
		{
			m_audioPlayer->Close();
			m_audioPlayer->SetIPAddress(m_context->remoteAddr.toString(), 91234, 0, 0);
			m_audioPlayer->CreateStream();
			m_audioPlayer->Play();

			gLogManager.log("Starting audio player at port: " + core::StringConverter::toString(m_audioPlayer->GetPort(0)), ELL_INFO);
		}
		_startVideoStream();
	}

	bool StopStream()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		gLogManager.log("Stopping AVStreamService.",ELL_INFO);

		m_streamers->Stop();
		Sleep(1000);

		gLogManager.log("Stopping Cameras.", ELL_INFO);
		m_cameraController->Stop();
		gLogManager.log("Camera Stopped.", ELL_INFO);

		if (m_audioPlayer)
		{
			gLogManager.log("Stopping Audio.", ELL_INFO);
			m_audioPlayer->Close();
			gLogManager.log("Audio Stopped.", ELL_INFO);
		}

		m_status = EServiceStatus::Stopped;
		m_isVideoStarted = false;
		return true;
	}


	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;

		if (!m_isVideoStarted)
			_startVideoStream();
		else
		{/*
			if (m_autoGain )
			{
				if (m_cameraIfo[0].camera && m_cameraIfo[0].camera->IsConnected())
				{
					float t = gEngine.getTimer()->getSeconds();
					if (m_cameraIfo[0].camera->GetCaptureFrameRate() < m_currentSettings.fps-4 )
					{
						if (m_lastGainUpdate + 2000 < t)
						{
							if (m_currentGain < 0)
								m_currentGain = 0;
							else
								m_currentGain += 0.02f;
							m_cameraController->SetCameraParameterValue("Gain", core::StringConverter::toString(m_currentGain));
							m_lastGainUpdate = t;
						}
					}
					else if (m_cameraIfo[0].camera->GetCaptureFrameRate() >= m_currentSettings.fps)
					{
						if (m_lastGainUpdate + 5000< t)
						{
							m_currentGain -= 0.01f;
							if (m_currentGain < 0)
								m_currentGain = -1;
							m_cameraController->SetCameraParameterValue("Gain", core::StringConverter::toString(m_currentGain));
							m_lastGainUpdate = t;
						}
					}
				}
			}*/
		}

		m_cameraController->Update();

		if (kbhit())
		{
			if (getch() == 'v')
			{
				m_streamers->GetStream("Video")->SetPaused(!m_streamers->GetStream("Video")->IsPaused());
			}
		}

	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + AVStreamServiceModule::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);

		if (m_status == EServiceStatus::Running && m_portsReceived == false)
		{

			msg = "Started, but waiting for video connection.";
			context->RenderText(msg, 0, 0,video::SColor(1,0,0,1));
		}

		msg = "Stream Settings:" ;
		context->RenderText(msg, 0, 0);
		msg = "   Resolution:" + core::StringConverter::toString(m_currentSettings.size);
		context->RenderText(msg, 0, 0);
		msg = "   Framerate:" + core::StringConverter::toString(m_currentSettings.fps);
		context->RenderText(msg, 0, 0);
		msg = "   Bitrate:" + core::StringConverter::toString(m_currentSettings.bitrate);
		context->RenderText(msg, 0, 0);

		if (m_status != EServiceStatus::Running)
			return;
		m_cameraController->DebugRender(context);
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

	}


	CameraProfileManager* LoadCameraProfiles(const core::string& path)
	{
		m_cameraProfileManager->LoadFromXML(path);
		return m_cameraProfileManager;
	}

	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		_SendCameraSettings();
		/*
		m_portsReceived = true;
		uint videoPorts[] = { 5000, 5001 };
		m_streamers->GetStream("Video")->BindPorts("192.168.100.105", videoPorts, 2, 0, 0);
		*/
		/*
		if (false)
		{
			stream.seek(0, OS::ESeek_Set);
			int reply = (int)EMessages::ClockSync;
			int len = stream.write(&reply, sizeof(reply));

			ulong baseClock = m_streamers->GetStream("Video")->GetClockBase();

			len += wrtr.binWriteInt(baseClock);
			m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
		}*/
	}
	virtual void OnUserDisconnected()
	{
		m_portsReceived = false;
		for (int i = 0; i < m_VideoPorts.size(); ++i)
			m_VideoPorts[i] = 0;
	}


	void _SendCameraSettings()
	{
		if (!m_camConfig)
			return;

		//reply with camera settings
		xml::XMLWriter w;
		xml::XMLElement e("root");

		xml::XMLElement* ret = m_camConfig->ExportToXML(&e);
		ret->addAttribute("StreamsCount", core::StringConverter::toString(m_streamsCount));
		if (m_audioPlayer)
		{
			ret->addAttribute("AudioPlayerPort", core::StringConverter::toString(m_audioPlayer->GetPort(0)));
			gLogManager.log("AudioPlayerPort: " + core::StringConverter::toString(m_audioPlayer->GetPort(0)), ELL_INFO);
		}

		w.addElement(ret);

		core::string res = w.flush();

		int bufferLen = res.length() + sizeof(int)* 10;
		byte* buffer = new byte[bufferLen];

		OS::CMemoryStream stream("", buffer, bufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		stream.seek(0, OS::ESeek_Set);
		int reply = (int)EMessages::CameraConfig;
		int len = stream.write(&reply, sizeof(reply));
		len += wrtr.binWriteString(res);
		m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
		
		delete[]buffer;

	}
	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{

		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		std::vector<core::string> values = core::StringUtil::Split(value, ",");
		if (msg == "CameraParameters")
		{
			_SendCameraSettings();
		}
		else if (msg == "VideoPorts" && values.size()>=m_streamsCount)
		{
			std::vector<uint> ports;
			ports.resize(m_streamsCount);

			m_portsReceived = true;
			bool ok = (m_remoteAddr == m_context->remoteAddr);
			for (int i = 0; i < m_streamsCount; ++i)
			{
				ports[i] = core::StringConverter::toInt(values[i]);
				ok &= (m_VideoPorts[i] == ports[i]);
			}
			if (ok)
				return;
			m_VideoPorts = ports;
			m_remoteAddr = m_context->remoteAddr;
			if (m_streamers)
			{
				gLogManager.log("Starting video stream to: " +m_context->remoteAddr.toString(),ELL_INFO);
				m_streamers->GetStream("Video")->BindPorts(m_context->remoteAddr.toString(), &m_VideoPorts[0], m_VideoPorts.size(), 0, 0);


				int reply = (int)EMessages::IsStereo;
				int len = stream.write(&reply, sizeof(reply));
				bool stereo = m_cameraController->IsStereo();// ((video::GstNetworkVideoStreamer*)m_streamers->GetStream("Video"))->IsStereo();
				len += stream.write(&stereo, sizeof(stereo));
				m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);

				_SendCameraSettings();
			}
		}/*
		else if (msg == "AudioPort" && values.size() >= 1)
		{
			int port;
			port= core::StringConverter::toInt(values[0]);
			if (port == m_AudioPort)return;
			m_AudioPort = port;
			if (m_streamAudio)
				m_streamers->GetStream("Audio")->BindPorts(m_context->remoteAddr.toString(), &m_AudioPort, 1, 0, 0);
		}*/
		else if (msg == "Stream" && values.size() >= 1)
		{
			bool enabled=core::StringConverter::toBool(values[0]);
			/*if (m_streamAudio)
				m_streamers->GetStream("Audio")->SetPaused(!enabled);*/
			m_streamers->GetStream("Video")->SetPaused(!enabled);
		}
	}

};

AVStreamServiceModule::AVStreamServiceModule()
{
	m_impl = new AVStreamServiceModuleImpl();
}

AVStreamServiceModule::~AVStreamServiceModule()
{
	delete m_impl;
}


std::string AVStreamServiceModule::GetServiceName()
{
	return AVStreamServiceModule::ModuleName;
}

EServiceStatus AVStreamServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void AVStreamServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus AVStreamServiceModule::StartService(ServiceContext* context)
{
	m_impl->StartStream();
	return m_impl->m_status;
}

bool AVStreamServiceModule::StopService()
{
	return m_impl->StopStream();
}

void AVStreamServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void AVStreamServiceModule::Update(float dt)
{
	m_impl->Update();
}

void AVStreamServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void AVStreamServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool AVStreamServiceModule::LoadServiceSettings(xml::XMLElement* e)
{
	return true;
}

void AVStreamServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

