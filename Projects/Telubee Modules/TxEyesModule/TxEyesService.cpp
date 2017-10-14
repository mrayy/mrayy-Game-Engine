

#include "stdafx.h"

#if !_WIN64
#define USE_POINTGREY 1
#endif

#include "TxEyesService.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "ICameraVideoGrabber.h"
#include "VideoGrabberTexture.h"
#include "IThreadManager.h"
#include "CameraProfile.h"
#include "GstNetworkVideoStreamer.h"
#include "DirectShowVideoGrabber.h"
#include "GstCustomVideoStreamer.h"
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
#include "ModuleSharedMemory.h"
#include "INetworkPortAssigner.h"
#include "Benchmarks.h"

#include <conio.h>
#include <XMLTextNode.h>

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

IMPLEMENT_RTTI(TxEyesService, IServiceModule);

const std::string TxEyesService::ModuleName("TxEyesServiceModule");


class TxEyesServiceImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:

	GCPtr<video::GstStreamBin> m_streamers;
	ECameraType m_cameraType;
	bool m_optimizedOVRVision;
	bool m_enableEyegaze;
	math::vector2di m_eyegazeSize;
	int m_eyegazeFoV;
	int m_eyegazeLevels;
	math::vector2di m_resolution;
	int m_fps;
	CameraConfigurationManager* m_camConfigMngr;
	TelubeeCameraConfiguration* m_camConfig;

	int m_streamsCount;
	//video::VideoGrabberTexture m_cameraTextures[2];

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


	std::vector<_CameraInfo> m_cameraIfo;

	ICameraSrcController* m_cameraController;
	video::ICustomVideoSrc* m_cameraSource;

	std::vector<uint> m_VideoPorts;
	bool m_portsReceived;
	bool m_isVideoStarted;
	network::NetAddress m_remoteAddr;


	int m_quality;
	CameraProfileManager* m_cameraProfileManager;
	core::string m_cameraProfile;

	EServiceStatus m_status;

	TBeeServiceContext* m_context;

	Benchmarks m_benchmarks;
	FILE* benchmarkFile;
	ulong m_benchLastTime;

public:
	TxEyesServiceImpl()
	{
		m_status = EServiceStatus::Idle;
		m_cameraProfileManager = new CameraProfileManager();
		m_camConfigMngr = new CameraConfigurationManager();
		printf("Loading camera configurations\n");
		m_camConfigMngr->LoadConfigurations("CameraConfigurations.xml");

		m_streamers = new video::GstStreamBin();
		printf("Loading streaming profiles\n");
		LoadCameraSettings("StreamingProfiles.xml");

		m_streamsCount = 0;
		/*
		m_VideoPorts[0] = 7000;
		m_VideoPorts[1] = 7001;*/

		m_portsReceived = false;
		m_isVideoStarted = false;

		m_optimizedOVRVision = false;

		benchmarkFile = fopen("TxEyesServiceImpl.benchmark", "w");
		fprintf(benchmarkFile, "FPS\tBytes\tEncoding\tCPU\tMem\n");
		m_benchLastTime = gEngine.getTimer()->getMilliseconds();
	}
	~TxEyesServiceImpl()
	{
		fclose(benchmarkFile);
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

		printf("Initing..\n");
//#if USE_POINTGREY && USE_WEBCAMERA
		core::string camType = core::StringUtil::ToLower(context->appOptions.GetOptionValue("CameraConnection", "directshow"));
		if (camType == "directshow")
			m_cameraType=ECameraType::Webcam ;
		if (camType == "ovrvision")
			m_cameraType = ECameraType::Ovrvision;
		if (camType == "ovrvisionraw")
			m_cameraType = ECameraType::OvrvisionCompressed;
		if (camType == "pointgrey")
			m_cameraType=ECameraType::PointGrey ;

		m_optimizedOVRVision = core::StringConverter::toBool(context->appOptions.GetOptionValue("OptimizedOVR","false"));
		m_enableEyegaze = core::StringConverter::toBool(context->appOptions.GetOptionValue("Eyegaze", "false"));
//		m_eyegazeSize = core::StringConverter::toVector2d(context->appOptions.GetOptionValue("EyegazeSize"));
		m_eyegazeLevels = core::StringConverter::toInt(context->appOptions.GetOptionValue("EyegazeLevels", "0"));
		m_eyegazeFoV = core::StringConverter::toInt(context->appOptions.GetOptionValue("EyegazeFOV", "15"));
		if (m_eyegazeFoV == 0)
			m_eyegazeFoV = 10;
/*#else 
#if USE_POINTGREY
		m_cameraType = ECameraType::PointGrey;
#else
		m_cameraType = ECameraType::Webcam;
#endif

#endif*/
		m_cameraProfile = context->appOptions.GetOptionValue("CameraProfile","None");

		m_quality = core::StringConverter::toInt(context->appOptions.GetOptionValue("Quality", "0"));
		printf("Parameters loaded..\n");
		{

			m_camConfig = m_camConfigMngr->GetCameraConfiguration(m_cameraProfile);
			if (!m_camConfig)
				gLogManager.log("Couldn't find camera configurations! : " + m_cameraProfile, ELL_ERROR);
			printf("Camera Configuration loaded..\n");

			gLogManager.log("Capture Type:" + core::StringConverter::toString(m_camConfig->captureType), ELL_INFO);

			//m_camConfig->captureType = TBee::TelubeeCameraConfiguration::Capture

			//Create Camera Controller
			if (m_camConfig->captureType == TBee::TelubeeCameraConfiguration::CaptureRaw)
			{
				gLogManager.log("Creating Raw Capture Camera", ELL_INFO);
 				if ( false && (m_cameraType == ECameraType::Ovrvision || m_cameraType == ECameraType::OvrvisionCompressed))
 					m_cameraController = new CameraGrabberController();
 				else
				{
				
					m_cameraController = new EncodedCameraStreamController(m_camConfig->captureType, m_cameraType);
					((EncodedCameraStreamController*)m_cameraController)->optimizeOVR = m_optimizedOVRVision;
					((EncodedCameraStreamController*)m_cameraController)->EnableEyegaze(m_enableEyegaze);
				}
			}else 
			{
				gLogManager.log("Creating Encoded Capture Camera", ELL_INFO);
				m_cameraController = new EncodedCameraStreamController(m_camConfig->captureType, m_cameraType);
			}
			printf("Camera controller created..\n");
		}
		/*core::string res = context->appOptions.GetOptionByName("StreamResolution")->getValue();
		
		if (res == "0-VGA")
			m_resolution.set(640, 480);
		else if (res == "1-HD")
			m_resolution.set(1280, 720);
		else if (res == "2-FullHD")
			m_resolution.set(1920, 1080);*/

#ifdef USE_WEBCAMERA
		m_streamsCount = m_cameraIfo.size();
		if (m_cameraType == ECameraType::Ovrvision)
		{

			m_camConfig->streamType = TelubeeCameraConfiguration::StreamCoded;
			
		}
		else
		if (m_cameraType == ECameraType::OvrvisionCompressed)
		{

			if (m_enableEyegaze)
				m_camConfig->streamType = TelubeeCameraConfiguration::StreamFoveatedOvrvision;
			else
				m_camConfig->streamType = TelubeeCameraConfiguration::StreamOvrvision;
			// -1 for the None index
			m_streamsCount = 1;

			m_cameraIfo.clear();
			_CameraInfo ifo;
			ifo.ifo.index = 0;
			m_cameraIfo.push_back(ifo);
		}
		else
		if (m_cameraType == ECameraType::Webcam)
		{
			if (m_enableEyegaze)
				m_camConfig->streamType = TelubeeCameraConfiguration::StreamEyegazeRaw;
			else
				m_camConfig->streamType = TelubeeCameraConfiguration::StreamRaw;

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

			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Focus, "0");
			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Exposure, context->appOptions.GetOptionByName("Exposure")->getValue());
			m_cameraController->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gain, "1");// context->appOptions.GetOptionByName("Gain")->getValue());

			// Now close cameras
			m_cameraController->Stop();

			gLogManager.log("Creating Video Streamer",ELL_INFO);

			video::GstNetworkVideoStreamer* hs = new video::GstNetworkVideoStreamer();
			hs->AddListener(this);


			
			video::ICustomVideoSrc* src = m_cameraController->CreateVideoSrc();
			m_cameraSource = src;
			if (m_enableEyegaze)
			{
				video::EyegazeCameraVideoSrc* cs = (video::EyegazeCameraVideoSrc*)src;

				m_eyegazeSize.x = m_eyegazeSize.y = m_resolution.x*(((float)m_eyegazeFoV) / m_camConfig->fov);

				cs->SetEyegazeCrop(m_eyegazeSize.x, m_eyegazeSize.y);
				cs->SetEyegazeLevels(m_eyegazeLevels);
			}

			m_cameraController->SetResolution(m_resolution.x, m_resolution.y);

			src->SetResolution(m_resolution.x, m_resolution.y, fps, true);
			src->SetBitRate(m_currentSettings.bitrate);

			src->LoadParameters(m_paramsElement->getSubElement("Encoder"));
			m_camConfig->encoderType = src->GetEncoderType();
			m_camConfig->separateStreams = src->IsSeparateStreams();
			m_camConfig->CameraStreams = src->GetVideoSrcCount();
			hs->SetVideoSrc(src);

			m_streamers->AddStream(hs, "Video");

			m_streamsCount = src->GetStreamsCount();

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

		
		m_VideoPorts.resize(m_streamsCount);

		//Create Audio Stream
		/*
		if (m_supportAudio)
		{
			printf("Creating Audio Streamer\n");
			video::GstNetworkAudioStreamer* streamer;
			streamer = new video::GstNetworkAudioStreamer();
			m_streamers->AddStream(streamer, "Audio");
		}*/

		printf("Finished streams\n");


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
		m_context->serviceLoader->RegisterCapability(TxEyesService::ModuleName, "EyesSupported", "Yes");

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
		tmpV->OnChanged += newClassDelegate1("", this, &TxEyesServiceImpl::_OnCameraPropertyChanged);

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
		m_context->serviceLoader->RemoveCapabilityCategory(TxEyesService::ModuleName);

		StopStream();

		m_context->RemoveListener(this);
		m_streamers->ClearStreams(true);
		m_cameraController->Stop();
		delete m_cameraController;
		m_cameraController = 0;
		

		m_status = EServiceStatus::Idle;
	}


	void _startVideoStream()
	{
		if ( (!m_portsReceived && m_context->portHostAddr==0 ) || m_status != EServiceStatus::Running)
			return;
		printf("Starting Stream at :%dx%d@%d\n", m_resolution.x, m_resolution.y, m_fps);
		//  Begin the video stream
		core::string clockIpAddr;
		if (m_context->sharedMemory->gstClockPortStreamer != 0)
		{
			clockIpAddr = "127.0.0.1";
		}
		for (int i = 0; i < m_VideoPorts.size(); ++i)
		{
			core::string pname = "Video" + core::StringConverter::toString(i);
			m_VideoPorts[i]=gNetworkPortAssigner.AssignPort(pname, network::EPT_UDP, m_context->GetPortValue(pname));
		}
		//if (m_cameraType == ECameraType::PointGrey)
		{
			//Pointgrey cameras need to be started manually
			printf("Starting Cameras.\n");
			m_cameraController->Start();
			//Sleep(100);
		}

		m_cameraSource->SetResolution(m_resolution.x, m_resolution.y, m_fps, true);
		m_streamers->GetStream("Video")->SetClockAddr(clockIpAddr, m_context->sharedMemory->gstClockPortStreamer);
		m_streamers->GetStream("Video")->BindPorts(m_context->GetTargetClientAddr()->toString(),&m_VideoPorts[0],m_VideoPorts.size(),false);
		m_streamers->GetStream("Video")->CreateStream();
		if (m_context->sharedMemory->gstClockPortStreamer == 0)//only if first time
			m_context->sharedMemory->gstClockPortStreamer = m_streamers->GetStream("Video")->GetClockPort();

		printf("Start Streaming.\n");
		m_streamers->Stream();
		//Sleep(500);

		m_cameraController->OnStreamStarted();

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
		_startVideoStream();
	}

	bool StopStream()
	{
		if (m_status != EServiceStatus::Running)
			return false;
//		gLogManager.log("Stopping AVStreamService.",ELL_INFO);

		m_streamers->Stop();
		m_streamers->CloseAll();
	//	Sleep(1000);

	//	gLogManager.log("Stopping Cameras.", ELL_INFO);
		m_cameraController->Stop();
	//	gLogManager.log("Camera Stopped.", ELL_INFO);

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

		ulong t=gEngine.getTimer()->getMilliseconds();
		if (t-m_benchLastTime>1000)
		{
			m_benchLastTime = t;
			int c = 0;
			for (int i = 0; i < m_cameraSource->GetStreamsCount(); ++i)
			{
				c += m_cameraSource->GetCurrentFPS(i);
			}
			c /= (float)m_cameraSource->GetStreamsCount();
			fprintf(benchmarkFile, "%d\t%d\t%f\t%d\t%d\n", c, m_streamers->GetStream("Video")->GetAverageBytesSent(), m_cameraSource->GetEncodingTimePS(),
				(int)m_benchmarks.CPUProcessUsage->getCurrentValue(), (int)m_benchmarks.PhysicalMemoryUsage->getCurrentValue());

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

		core::string msg = "[" + TxEyesService::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);

		if (m_status == EServiceStatus::Running && (!m_portsReceived && m_context->portHostAddr == 0))
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

		msg = "Stream Status:";
		context->RenderText(msg, 0, 0);
		for (int i = 0; i < m_cameraSource->GetVideoSrcCount(); ++i)
		{
			msg = "   FPS ["+core::StringConverter::toString(i)+"]:" + core::StringConverter::toString(m_cameraSource->GetCurrentFPS(i));
			context->RenderText(msg, 0, 0);
		}
		msg = "   Average Sent Bytes:" + core::StringConverter::toString(m_streamers->GetStream("Video")->GetAverageBytesSent()/1024)+"KB";
		context->RenderText(msg, 0, 0);

		msg = "   Encoding Time:" + core::StringConverter::toString(m_cameraSource->GetEncodingTimePS());
		context->RenderText(msg, 0, 0);

		if (m_enableEyegaze)
		{
			msg = "Foveated Rendering Settings:" ;
			context->RenderText(msg, 0, 0);
			msg = "   FoV:" + core::StringConverter::toString(m_eyegazeFoV);
			context->RenderText(msg, 0, 0);
			msg = "   Size:" + core::StringConverter::toString(m_eyegazeSize);
			context->RenderText(msg, 0, 0);
			msg = "   Levels:" + core::StringConverter::toString(m_eyegazeLevels);
			context->RenderText(msg, 0, 0);
		}

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
		// 		if (m_audioPlayer)
// 		{
// 			ret->addAttribute("AudioPlayerPort", core::StringConverter::toString(m_AudioPort));
// 			gLogManager.log("AudioPlayerPort: " + core::StringConverter::toString(m_AudioPort), ELL_INFO);
// 		}
		ret->addAttribute("FrameSize", core::StringConverter::toString(m_cameraSource->GetFrameSize(0)));

		if (m_cameraType == ECameraType::Ovrvision || m_cameraType == ECameraType::OvrvisionCompressed)
		{
			//add ovrvision setting string
			if ((EncodedCameraStreamController*)m_cameraController)
			{
				EncodedCameraStreamController* c = (EncodedCameraStreamController*)m_cameraController;

				ret->addAttribute("OptimizeOVR", c->optimizeOVR?"true":"false");
				{
					core::string settings = c->GetCameraParameterValue("settings", 0);
					xml::XMLTextNode* node = new xml::XMLTextNode(settings);
					//gLogManager.log("camera settings:" + settings, ELL_INFO);
					ret->addSubElement(node);
				}
			}else if ((CameraGrabberController*)m_cameraController)
			{
				CameraGrabberController* c = (CameraGrabberController*)m_cameraController;
				if (c->cameras.size() > 0 && c->cameras[0])
				{
					core::string settings= c->cameras[0]->GetParameter("settings");
					xml::XMLTextNode* node = new xml::XMLTextNode(settings);
				//	gLogManager.log("camera settings:" + settings, ELL_INFO);
					ret->addSubElement(node);
				}
			}
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

				if (!m_context->portHostAddr)
				{
					m_context->portMap["Video" + core::StringConverter::toString(i)] = ports[i];
				}
			}
			if (ok)
				return;
			m_VideoPorts = ports;
			m_remoteAddr = m_context->remoteAddr;
			if (m_streamers)
			{
				gLogManager.log("Starting video stream to: " +m_context->remoteAddr.toString(),ELL_INFO);
				//m_streamers->GetStream("Video")->BindPorts(m_context->remoteAddr.toString(), &m_VideoPorts[0], m_VideoPorts.size(),  true);
				

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
		else if (msg == "Gaze" && values.size() >= 2)
		{
			std::vector<math::vector2df> gaze;
			for (int i = 0; i < values.size(); i+=2)
			{
				math::vector2d p;
				p.x = math::clamp(core::StringConverter::toFloat(values[i]),0.0f,1.0f);
				p.y = math::clamp(core::StringConverter::toFloat(values[i + 1]), 0.0f, 1.0f);
				gaze.push_back(p);
			}
			video::EyegazeCameraVideoSrc* src= dynamic_cast<video::EyegazeCameraVideoSrc*>(m_cameraSource);
			if (src)
			{
				src->SetEyegazePos(gaze);
			}
		}
	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{
		
		xml::XMLElement*e=elem->getSubElement("Cameras");
		if (e != 0)
		{
			_CameraInfo ifo;
			e = e->getSubElement("Camera");
			while (e)
			{
				ifo.ifo.index = e->getValueInt("Index");
				m_cameraIfo.push_back(ifo);
				e = e->nextSiblingElement("Camera");
			}
		}
		return true;
	}
};

TxEyesService::TxEyesService()
{
	m_impl = new TxEyesServiceImpl();
}

TxEyesService::~TxEyesService()
{
	delete m_impl;
}


std::string TxEyesService::GetServiceName()
{
	return TxEyesService::ModuleName;
}

EServiceStatus TxEyesService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxEyesService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxEyesService::StartService(ServiceContext* context)
{
	m_impl->StartStream();
	return m_impl->m_status;
}

bool TxEyesService::StopService()
{
	return m_impl->StopStream();
}

void TxEyesService::DestroyService()
{
	m_impl->Destroy();
}


void TxEyesService::Update(float dt)
{
	m_impl->Update();
}

void TxEyesService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxEyesService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxEyesService::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void TxEyesService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

