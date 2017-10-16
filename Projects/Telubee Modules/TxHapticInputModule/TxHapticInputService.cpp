
#include "stdafx.h"
#include "TxHapticInputService.h"
#include "TBeeServiceContext.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "GstStreamBin.h"
#include "GstNetworkAudioStreamer.h"
#include "DirectSoundInputStream.h"

#include "GstCustomDataStreamer.h"
#include "GstCustomDataPlayer.h"

extern mray::core::string ModuleName;
namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(TxHapticInputService, IServiceModule)




class TxHapticInputServiceImpl :public IServiceContextListener
{
public:

	struct HapticInterface
	{
		int ID;
		int Channel;
		int SamplingRate;
	};

	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	uint m_port;
	bool m_connected;
	GCPtr<video::GstStreamBin> m_streamers;
	std::vector<HapticInterface> m_hapticInterfaceIndicies;
	std::vector<sound::InputStreamDeviceInfo> m_audioInterfaceList;

	video::GstCustomDataStreamer* m_dstreamer;
	video::GstCustomDataPlayer* m_dplayer;
	bool _streamStarted;
public:

	TxHapticInputServiceImpl()
	{
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_port = 0;
		m_connected = false;
		_streamStarted = false;
	}

	~TxHapticInputServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		m_context->serviceLoader->RegisterCapability(ModuleName, "HapticsInputSupported", "Yes");
		{
			sound::DirectSoundInputStream inputStream;
			inputStream.ListDevices(m_audioInterfaceList);
			{
				OS::IStreamPtr interfacesFile = gFileSystem.openFile(gFileSystem.getAppPath() + "HapticsInterfaces.txt", OS::FILE_MODE::TXT_WRITE);
				OS::StreamWriter w(interfacesFile);
				for (int i = 0; i < m_audioInterfaceList.size(); ++i)
				{
					char buffer[512];
					sprintf(buffer, "%d - %s : %s, %s\n", i, m_audioInterfaceList[i].name.c_str(), m_audioInterfaceList[i].description.c_str(), m_audioInterfaceList[i].deviceGUID.c_str());
					w.writeLine(buffer);
				}
				interfacesFile->close();
			}

			//Create audio streams based on the loaded interfaces
			if (m_hapticInterfaceIndicies.size() == 0)
			{
				HapticInterface iface;
				iface.ID = -1;
				iface.Channel = 0;
				iface.SamplingRate = 2000;
				m_hapticInterfaceIndicies.push_back(iface);//add the default audio interface
			}
			printf("Creating Haptic Streamer\n");
			m_streamers = new video::GstStreamBin();

			for (int i = 0; i < m_hapticInterfaceIndicies.size(); ++i)
			{
				video::GstNetworkAudioStreamer::AudioInterface iface;
				video::GstNetworkAudioStreamer* streamer;
				streamer = new video::GstNetworkAudioStreamer();

				if (m_hapticInterfaceIndicies[i].ID == -1)
					iface.deviceGUID = "";
				else
					iface.deviceGUID = m_audioInterfaceList[m_hapticInterfaceIndicies[i].ID].deviceGUID;
				iface.channelsCount = 1;
				iface.samplingRate = m_hapticInterfaceIndicies[i].SamplingRate;

				streamer->SetAudioInterface(iface);

				std::string interfaceID = "Audio";
				if (m_hapticInterfaceIndicies.size() > 1)
					interfaceID = interfaceID + "#" + core::StringConverter::toString(i);
				m_streamers->AddStream(streamer, interfaceID);
			}
		}


		{
			m_dstreamer = new video::GstCustomDataStreamer();
			m_dstreamer->SetApplicationDataType("rtp", true);
		}

		printf("Finished streams\n");

		context->AddListener(this);
		m_status = EServiceStatus::Inited;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->RemoveListener(this);


		m_status = EServiceStatus::Idle;

	}


	void _beginStreaming()
	{
		if (_streamStarted)
			return;
		_streamStarted = true;
		int port = core::StringConverter::toInt(m_context->appOptions.GetOptionValue("Port", "5111"));
		uint ports[] = { port };
		m_dstreamer->BindPorts(m_context->GetTargetClientAddr()->toString(), ports, 1, false);
		m_dstreamer->CreateStream();
		m_dstreamer->Stream();
		/*
		m_dplayer = new video::GstCustomDataPlayer();
		m_dplayer->SetApplicationDataType("rtp", true);
		m_dplayer->SetPort(0);
		m_dplayer->CreateStream();
		m_dplayer->Play();
		*/
	}

	void _endStreaming()
	{
		if (!_streamStarted)
			return;
		_streamStarted = false;
		m_dstreamer->Close();
	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		m_status = EServiceStatus::Running;
		_beginStreaming();
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;

		m_status = EServiceStatus::Stopped;
		_endStreaming();
		return true;
	}

	int _count;
	float t = 0;

	uint time;

	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;

		t += dt;


		//if(t>0.01f)
		{
			t = 0;
			std::vector<float> data;
			int length = (int)math::Randomizer::randRange(10, 100);
			for (int i = 0;i < length;++i)
			{
				data.push_back(math::Randomizer::rand01());
			}
			time++;

			m_dstreamer->AddDataFrame((uchar*)&data[0],sizeof(float)*data.size());
		}

	
		/*
		while (m_dplayer->GrabFrame())
		{
			 int length=m_dplayer->GetDataLength();

			 void* d = new uchar[length];

			 if (m_dplayer->GetData(d, length))
			 {
				 float* received = (float*)d;
				 _count = time-(int)received[0];



			 }

			 delete []d;
		}*/
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

	}
	void DebugRender(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;
		core::string msg = "Compress raio= " + core::StringConverter::toString(m_dstreamer->CompressRatio())+"%";
		context->RenderText(msg, 0, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		m_connected = true;
	}
	virtual void OnUserDisconnected()
	{
		m_connected = false;
	}
	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{
		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		std::vector<core::string> values = core::StringUtil::Split(value, ",");
		if (msg == "HapticsInPorts" && values.size() >= 1)
		{
			m_port = core::StringConverter::toInt(values[0]);
		}
	}


	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd, scene::ViewPort* vp)
	{

	}
	virtual void WindowClosed(video::RenderWindow* window)
	{
		m_status = EServiceStatus::Shutdown;
	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{

		return true;
	}
};


TxHapticInputService::TxHapticInputService()
{
	m_impl = new TxHapticInputServiceImpl();
}

TxHapticInputService::~TxHapticInputService()
{
	delete m_impl;
}


std::string TxHapticInputService::GetServiceName()
{
	return ModuleName;
}

EServiceStatus TxHapticInputService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxHapticInputService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxHapticInputService::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool TxHapticInputService::StopService()
{
	return m_impl->Stop();
}

void TxHapticInputService::DestroyService()
{
	m_impl->Destroy();
}


void TxHapticInputService::Update(float dt)
{
	m_impl->Update(dt);
}

void TxHapticInputService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxHapticInputService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxHapticInputService::LoadServiceSettings(xml::XMLElement* elem)
{
	return m_impl->LoadServiceSettings(elem);
}

void TxHapticInputService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


