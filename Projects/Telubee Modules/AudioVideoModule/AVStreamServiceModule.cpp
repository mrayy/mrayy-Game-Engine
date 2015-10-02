

#include "stdafx.h"
#include "AVStreamServiceModule.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "ICameraVideoGrabber.h"
#include "VideoGrabberTexture.h"
#include "IThreadManager.h"
#include "CameraProfile.h"
#include "GstNetworkVideoStreamer.h"
#include "GstNetworkAudioStreamer.h"
#include "FlyCameraVideoGrabber.h"
#include "DirectShowVideoGrabber.h"
#include "GstCustomVideoStreamer.h"
#include "GstCustomMultipleVideoStreamer.h"
#include "FlyCameraManager.h"
#include "DirectSoundInputStream.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "GstVideoProvider.h"
#include "NetworkValueController.h"
#include "CameraConfigurationManager.h"
#include "StringUtil.h"

namespace mray
{
namespace TBee
{
#define USE_WEBCAMERA 1
#define USE_POINTGREY 1


IMPLEMENT_RTTI(AVStreamServiceModule, IServiceModule);

const std::string AVStreamServiceModule::ModuleName("AVStreamServiceModule");


class AVStreamServiceModuleImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:


	enum class ECameraType
	{
		Webcam,
		PointGrey
	};
	GCPtr<video::GstStreamBin> m_streamers;
	ECameraType m_cameraType;
	math::vector2di m_resolution;
	int m_fps;
	CameraConfigurationManager* m_camConfigMngr;
	TelubeeCameraConfiguration* m_camConfig;


	//video::VideoGrabberTexture m_cameraTextures[2];

	bool m_streamAudio;

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
	struct _CameraInfo
	{
		CameraInfo ifo;
		int w, h, fps;
		GCPtr<video::ICameraVideoGrabber> camera;
		math::vector2di offsets;
	}m_cameraIfo[2];

	uint m_VideoPorts[2];
	uint m_AudioPort;

	int m_quality;
	CameraProfileManager* m_cameraProfileManager;
	core::string m_cameraProfile;

	EServiceStatus m_status;

	TBeeServiceContext* m_context;

public:
	AVStreamServiceModuleImpl()
	{
		m_status = EServiceStatus::Idle;
		m_cameraProfileManager = new CameraProfileManager();
		m_camConfigMngr = new CameraConfigurationManager();
		m_camConfigMngr->LoadConfigurations("CameraConfigurations.xml");

		m_streamers = new video::GstStreamBin();
		LoadCameraSettings("StreamingProfiles.xml");
	}
	~AVStreamServiceModuleImpl()
	{
		Destroy();
		m_streamers = 0;
		delete m_cameraProfileManager;
		delete m_camConfigMngr;
		if (video::FlyCameraManager::isExist())
			delete video::FlyCameraManager::getInstancePtr();
	}

	void LoadCameraSettings(const core::string &src)
	{
		xml::XMLTree tree;
		core::string path;
		gFileSystem.getCorrectFilePath(src, path);

		if (path == "" || tree.load(path) == false)
		{
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 3000, 30));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 4000, 45));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(640, 480), 5000, 50));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(960, 720), 6000, 45));
			m_cameraSettings.push_back(CameraSettings(math::vector2di(960, 720), 7000, 50));
			return;
		}

		xml::XMLElement* e = tree.getSubElement("Settings");
		e = e->getSubElement("Setting");

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
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;

		m_cameraIfo[0].w = m_cameraIfo[1].w = 1280;
		m_cameraIfo[0].h = m_cameraIfo[1].h = 720;
		m_cameraIfo[0].fps = m_cameraIfo[1].fps = 45;
		m_resolution.set(1280, 720);

#if USE_POINTGREY && USE_WEBCAMERA
		m_cameraType = context->appOptions.GetOptionByName("CameraConnection")->getValue() == "DirectShow" ? ECameraType::Webcam : ECameraType::PointGrey;
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
				gLogManager.log("Couldn't find camera configurations! : " + m_cameraProfile, ELL_WARNING);
		}
		m_quality = core::StringConverter::toInt(context->appOptions.GetOptionByName("Quality")->getValue());
		/*core::string res = context->appOptions.GetOptionByName("StreamResolution")->getValue();
		
		if (res == "0-VGA")
			m_resolution.set(640, 480);
		else if (res == "1-HD")
			m_resolution.set(1280, 720);
		else if (res == "2-FullHD")
			m_resolution.set(1920, 1080);*/
		m_streamAudio = context->appOptions.GetOptionByName("Audio")->getValue() == "Yes";


		if (m_cameraType == ECameraType::Webcam)
		{
			// -1 for the None index
			m_cameraIfo[0].ifo.index = core::StringConverter::toInt(context->appOptions.GetOptionByName("DS_Camera_Left")->getValue());
			m_cameraIfo[1].ifo.index = core::StringConverter::toInt(context->appOptions.GetOptionByName("DS_Camera_Right")->getValue());
		}
		else
		{
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
					m_cameraIfo[0].ifo.index = i;
				}
				if (sp == c2)
				{
					m_cameraIfo[1].ifo.index = i;
				}

			}
		}



		{
			std::vector<sound::InputStreamDeviceInfo> lst;
			sound::DirectSoundInputStream inputStream;
			inputStream.ListDevices(lst);
			for (int i = 0; i < lst.size(); ++i)
			{
				printf("%d - %s : %s\n", lst[i].ID, lst[i].name.c_str(), lst[i].description.c_str());
			}
		}

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

		for (int i = 0; i < 2; ++i)
		{
			m_cameraIfo[i].fps = fps;
			m_cameraIfo[i].w = capRes.x;
			m_cameraIfo[i].h = capRes.y;
		}
#if USE_WEBCAMERA
		if (m_cameraType == ECameraType::Webcam)
		{
			video::GstNetworkVideoStreamer* vstreamer;

			for (int i = 0; i < 2; ++i)
			{
				if (m_cameraIfo[i].ifo.index >= 0)
				{
					m_cameraIfo[i].camera = new video::DirectShowVideoGrabber();
					m_cameraIfo[i].camera->InitDevice(m_cameraIfo[i].ifo.index, m_cameraIfo[i].w, m_cameraIfo[i].h, m_cameraIfo[i].fps);//1280, 720
					m_cameraIfo[i].ifo.guidPath = m_cameraIfo[i].camera->GetDeviceName(m_cameraIfo[i].ifo.index);

				}
			}

			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Focus, "0");
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Exposure, context->appOptions.GetOptionByName("Exposure")->getValue());
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gain, context->appOptions.GetOptionByName("Gain")->getValue());

			// Now close cameras
			for (int i = 0; i < 2; ++i)
			{
 				if (m_cameraIfo[i].camera)
 					m_cameraIfo[i].camera->Stop();
			}

			printf("Creating Video Streamer\n");
			if (true)
			{

				video::GstCustomMultipleVideoStreamer* hs = new video::GstCustomMultipleVideoStreamer();
				hs->AddListener(this);

				std::vector<video::IVideoGrabber*> grabbers;
				grabbers.push_back(m_cameraIfo[0].camera);
				grabbers.push_back(m_cameraIfo[1].camera);
				hs->SetVideoGrabber(grabbers);//
				hs->SetResolution(m_resolution.x, m_resolution.y, fps, true);
				hs->SetBitRate(m_currentSettings.bitrate);
				m_streamers->AddStream(hs, "Video");
			}else if (false)
			{
				vstreamer = new video::GstNetworkVideoStreamer();
				vstreamer->AddListener(this);

				vstreamer->SetCameraResolution(m_cameraIfo[0].w, m_cameraIfo[0].h, fps);
				vstreamer->SetFrameResolution(m_resolution.x, m_resolution.y);
				vstreamer->SetCameras(m_cameraIfo[0].ifo.index, m_cameraIfo[1].ifo.index);
				vstreamer->SetBitRate(m_currentSettings.bitrate);


				m_streamers->AddStream(vstreamer, "Video");

			}
			else if (false)
			{
				// 				m_cameraIfo[0].camera->Start();
				// 				m_cameraIfo[1].camera->Start();


				video::GstCustomVideoStreamer* hs = new video::GstCustomVideoStreamer();

				hs->AddListener(this);
				hs->SetVideoGrabber(m_cameraIfo[0].camera, m_cameraIfo[1].camera);//
				hs->SetBitRate(m_currentSettings.bitrate);
				hs->SetResolution(m_resolution.x, m_resolution.y, fps);
				m_streamers->AddStream(hs, "Video");
			}

		}
#endif
#if USE_POINTGREY && USE_WEBCAMERA
		else
#endif
#if USE_POINTGREY
		{
			if (m_cameraIfo[0].ifo.index != -1)
			{
				printf("Initializing Pointgrey Camera\n");
				video::FlyCameraVideoGrabber* c = 0;
				m_cameraIfo[0].camera = (c = new video::FlyCameraVideoGrabber());
				c->SetCroppingOffset(m_cameraIfo[0].offsets.x, m_cameraIfo[0].offsets.y);
				m_cameraIfo[0].camera->InitDevice(m_cameraIfo[0].ifo.index, m_cameraIfo[0].w, m_cameraIfo[0].h, fps);
				m_cameraIfo[0].camera->SetImageFormat(video::EPixel_R8G8B8);
				//	m_cameraIfo[0].camera->Start();
			}

			if (m_cameraIfo[1].ifo.index != -1
				&& m_cameraIfo[0].ifo.index != m_cameraIfo[1].ifo.index)
			{
				printf("Initializing Pointgrey Camera\n");
				video::FlyCameraVideoGrabber* c = 0;
				m_cameraIfo[1].camera = (c = new video::FlyCameraVideoGrabber());
				c->SetCroppingOffset(m_cameraIfo[1].offsets.x, m_cameraIfo[1].offsets.y);
				m_cameraIfo[1].camera->InitDevice(m_cameraIfo[1].ifo.index, m_cameraIfo[1].w, m_cameraIfo[1].h, fps);
				m_cameraIfo[1].camera->SetImageFormat(video::EPixel_R8G8B8);
				//	m_cameraIfo[1].camera->Start();
			}

			//Bug: Open and close the cameras once, needed to make the camera run next time
			for (int i = 0; i < 2; ++i)
			{
				if (m_cameraIfo[i].camera)
				{
					m_cameraIfo[i].camera->Start();
					m_cameraIfo[i].camera->Stop();
				}
			}

			printf("Creating Video Streamer\n");
			video::GstCustomMultipleVideoStreamer* hs = new video::GstCustomMultipleVideoStreamer();
			hs->AddListener(this);

			std::vector<video::IVideoGrabber*> grabbers;
			grabbers.push_back(m_cameraIfo[0].camera);
			grabbers.push_back(m_cameraIfo[1].camera);

			printf("Setting Streaming Settings: %dx%d@%d - %d kbps\n", m_resolution.x, m_resolution.y, fps, m_currentSettings.bitrate);
			hs->SetVideoGrabber(grabbers);//
			hs->SetResolution(m_resolution.x, m_resolution.y, fps, true);
			hs->SetBitRate(m_currentSettings.bitrate);
			m_streamers->AddStream(hs, "Video");
		}
#endif

		//Create Audio Stream
		if (m_streamAudio)
		{
			printf("Creating Audio Streamer\n");
			video::GstNetworkAudioStreamer* streamer;
			streamer = new video::GstNetworkAudioStreamer();
			m_streamers->AddStream(streamer, "Audio");
		}
		printf("Finished streams\n");


		m_streamers->GetStream("Video")->CreateStream();
		if (m_streamAudio)
			m_streamers->GetStream("Audio")->CreateStream();

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
		CameraProfile* prof = m_cameraProfileManager->GetProfile(m_cameraProfile);
		if (false)
		{
			for (int i = 0; i < 2; ++i)
			{

				if (prof && false)
				{
					for (int j = 0; j < count; ++j)
					{
						core::string v;
						if (prof->GetValue(camParams[j], v))
							m_cameraIfo[i].camera->SetParameter(camParams[j], v);
					}
				}
				//	if (!m_debugData.debug)
				if (m_cameraType == ECameraType::Webcam)
				{
					//	m_cameraIfo[i].camera->Stop();
				}
				//m_cameraTextures[i].Set(m_cameraIfo[i].camera, gEngine.getDevice()->createEmptyTexture2D(true));

			}
		}
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
#define GET_VALUE(X) (GetCameraParameterValue(X,0))
#define ADD_CAMERA_VALUE(Type,Name)\
	value = GET_VALUE(Name); \
	tmpV = g->AddValue(new Type(Name, 0)); \
		if (value != "")\
		tmpV->parse(value); \
		tmpV->OnChanged += newClassDelegate1("", this, &AVStreamServiceModuleImpl::_OnCameraPropertyChanged);

		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Exposure);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Gain);
		ADD_CAMERA_VALUE(Vector2dfValue, video::ICameraVideoGrabber::Param_WhiteBalance);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Gamma);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Brightness);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Saturation);
		ADD_CAMERA_VALUE(FloatValue, video::ICameraVideoGrabber::Param_Sharpness);

	}
	void _OnCameraPropertyChanged(IValue* v)
	{
		SetCameraParameterValue(v->getName(), v->toString());
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
		if (m_cameraIfo[0].camera)
		{
			m_cameraIfo[0].camera->Stop();
			m_cameraIfo[0].camera = 0;
		}
		if (m_cameraIfo[1].camera)
		{
			m_cameraIfo[1].camera->Stop();
			m_cameraIfo[1].camera = 0;
		}

		m_status = EServiceStatus::Idle;
	}


	void StartStream()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		printf("Starting Stream at :%dx%d@%d\n", m_resolution.x, m_resolution.y, m_fps);
		//  Begin the video stream

		//if (m_cameraType == ECameraType::PointGrey)
		{
			//Pointgrey cameras need to be started manually
			printf("Starting Cameras.\n");
			if (m_cameraIfo[0].camera)
				m_cameraIfo[0].camera->Start();

			if (m_cameraIfo[1].camera)
				m_cameraIfo[1].camera->Start();
		}
		m_streamers->Stream();
	//	Sleep(1000);
		m_status = EServiceStatus::Running;
	}

	bool StopStream()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		printf("Stopping AVStreamService.\n");

		m_streamers->Stop();
		Sleep(1000);

		printf("Stopping Cameras.\n");
		if (m_cameraIfo[0].camera)
			m_cameraIfo[0].camera->Stop();
		if (m_cameraIfo[1].camera)
			m_cameraIfo[1].camera->Stop();

		m_status = EServiceStatus::Stopped;
		return true;
	}


	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;

		//check camera condition if connected or not
	//	if (m_cameraType == ECameraType::PointGrey)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (m_cameraIfo[i].camera && m_cameraIfo[i].camera->IsConnected() == false)
				{
					//	printf("Camera %d has stopped, restarting it\n", i);
					m_cameraIfo[i].camera->Start();

					//give the camera sometime to start
					OS::IThreadManager::getInstance().sleep(30);
				}
			}
		}
	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + AVStreamServiceModule::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);


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
		for (int i = 0; i < 2;++i)
		if (m_cameraIfo[i].camera)
		{

			msg = "Capture FPS [" + core::StringConverter::toString(i) + "]: " + core::StringConverter::toString(m_cameraIfo[i].camera->GetCaptureFrameRate());
			context->RenderText(msg, 0, 0);
		}
		//if (m_cameraType == ECameraType::PointGrey)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (m_cameraIfo[i].camera && m_cameraIfo[i].camera->IsConnected() == false)
				{
					msg = "Camera: " + core::StringConverter::toString(i) + " is not open!";
					context->RenderText(msg, 100, 0, video::SColor(1, 0, 0, 1));
				}
			}
		}
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

	void _SendCameraSettings()
	{
		if (!m_camConfig)
			return;

		//reply with camera settings
		xml::XMLWriter w;
		xml::XMLElement e("root");

		xml::XMLElement* ret = m_camConfig->ExportToXML(&e);

		w.addElement(ret);
		core::string res = w.flush();

		int bufferLen = res.length() + sizeof(int)* 10;
		byte* buffer = new byte[bufferLen];

		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, bufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		stream.seek(0, OS::ESeek_Set);
		int reply = (int)EMessages::CameraConfig;
		int len = stream.write(&reply, sizeof(reply));
		len += wrtr.binWriteString(res);
		m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);

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
		else if (msg == "VideoPorts" && values.size()>=2)
		{
			int ports[2];

			ports[0] = core::StringConverter::toInt(values[0]);
			ports[1] = core::StringConverter::toInt(values[1]);
			if (m_VideoPorts[0] == ports[0] && m_VideoPorts[1] == ports[1])
				return;
			m_VideoPorts[0] = ports[0];
			m_VideoPorts[1] = ports[1];
			if (m_streamers)
			{
				printf("Starting video stream to: %s:%d,%d\n", m_context->remoteAddr.toString().c_str(), m_VideoPorts[0], m_VideoPorts[1]);

				m_streamers->GetStream("Video")->BindPorts(m_context->remoteAddr.toString(), m_VideoPorts, 2, 0, 0);


				int reply = (int)EMessages::IsStereo;
				int len = stream.write(&reply, sizeof(reply));
				bool stereo = m_cameraIfo[0].ifo.index != m_cameraIfo[1].ifo.index &&
					m_cameraIfo[0].ifo.index != -1 && m_cameraIfo[1].ifo.index != -1;// ((video::GstNetworkVideoStreamer*)m_streamers->GetStream("Video"))->IsStereo();
				len += stream.write(&stereo, sizeof(stereo));
				m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);

				_SendCameraSettings();
			}
		}
		else if (msg == "AudioPort" && values.size() >= 1)
		{
			int port;
			port= core::StringConverter::toInt(values[0]);
			if (port == m_AudioPort)return;
			m_AudioPort - port;
			if (m_streamAudio)
				m_streamers->GetStream("Audio")->BindPorts(m_context->remoteAddr.toString(), &m_AudioPort, 1, 0, 0);
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

