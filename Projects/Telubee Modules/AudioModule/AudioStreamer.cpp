

#include "stdafx.h"
#include "AudioStreamer.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "IThreadManager.h"
#include "GstNetworkVideoStreamer.h"
#include "GstNetworkAudioStreamer.h"
#include "DirectSoundInputStream.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "NetworkValueController.h"
#include "StringUtil.h"
#include "AppSrcVideoSrc.h"
#include "ModuleSharedMemory.h"

#include <conio.h>

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(AudioStreamer, IServiceModule);

const std::string AudioStreamer::ModuleName("AudioStreamer");


class AudioStreamerImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:

	GCPtr<video::GstStreamBin> m_streamers;


	std::vector<uint> m_AudioPort;
	bool m_portsReceived;
	bool m_isVideoStarted;
	network::NetAddress m_remoteAddr;


	EServiceStatus m_status;

	TBeeServiceContext* m_context;
	
	struct Position
	{
		float x, y, z;
	};
	std::vector<int> m_audioInterfaceIndicies;
	std::vector<Position> m_audioSpatialPosition;
	bool m_isSpatialAudio;
	std::vector<sound::InputStreamDeviceInfo> m_audioInterfaceList;


public:
	AudioStreamerImpl()
	{
		m_status = EServiceStatus::Idle;

		m_streamers = new video::GstStreamBin();

		m_portsReceived = false;
		m_isVideoStarted = false;
		m_isSpatialAudio = false;
	}
	~AudioStreamerImpl()
	{
		Destroy();
		m_streamers = 0;
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;
		{
			sound::DirectSoundInputStream inputStream;
			inputStream.ListDevices(m_audioInterfaceList);
			{
				OS::IStreamPtr interfacesFile = gFileSystem.openFile(gFileSystem.getAppPath()+"AudioInterfaces.txt", OS::FILE_MODE::TXT_WRITE);
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
			if (m_audioInterfaceIndicies.size() == 0)
			{
				m_audioInterfaceIndicies.push_back(-1);//add the default audio interface
			}
			printf("Creating Audio Streamer\n");

			for (int i = 0; i < m_audioInterfaceIndicies.size(); ++i)
			{
				video::GstNetworkAudioStreamer::AudioInterface iface;
				video::GstNetworkAudioStreamer* streamer;
				streamer = new video::GstNetworkAudioStreamer();

				if (m_audioInterfaceIndicies[i]==-1)
					iface.deviceGUID = "";
				else
					iface.deviceGUID = m_audioInterfaceList[m_audioInterfaceIndicies[i]].deviceGUID;
				iface.channelsCount = 2;

				streamer->SetAudioInterface(iface);

				std::string interfaceID = "Audio";
				if (m_audioInterfaceIndicies.size() > 1)
					interfaceID = interfaceID + "#" + core::StringConverter::toString(i);
				m_streamers->AddStream(streamer, interfaceID);
			}
		}

		printf("Finished streams\n");

		_RegisterValues();

		context->AddListener(this);
		m_status = EServiceStatus::Inited;

		gLogManager.log("Done Initing.", ELL_INFO);
	}

	void _RegisterValues()
	{

		ValueGroup* g = new ValueGroup("Audio");
		m_context->netValueController->GetValues()->AddValueGroup(g);
		core::string value;
		IValue* tmpV;
#define GET_VALUE(X) (m_cameraController->GetCameraParameterValue(X,0))
#define ADD_CAMERA_VALUE(Type,Name)\
	value = GET_VALUE(Name); \
	tmpV = g->AddValue(new Type(Name, 0)); \
		if (value != "")\
		tmpV->parse(value); \
		tmpV->OnChanged += newClassDelegate1("", this, &AudioStreamerImpl::_OnAudioPropertyChanged);


	}
	void _OnAudioPropertyChanged(IValue* v)
	{
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		 
		StopStream();
		m_streamers->CloseAll();

		m_context->RemoveListener(this);
		m_streamers->ClearStreams(true);

		m_status = EServiceStatus::Idle;
	}


	void _startStream()
	{
		if (!m_portsReceived || m_status != EServiceStatus::Running)
			return;
		gLogManager.log("Start Streaming.", ELL_INFO);
		for (int i = 0; i < m_streamers->GetStreamsCount(); ++i)
		{
			m_streamers->GetStreamerAt(i)->CreateStream();
			if (m_context->sharedMemory->gstClockPortStreamer==0)
				m_context->sharedMemory->gstClockPortStreamer = m_streamers->GetStreamerAt(i)->GetClockPort();
		}
		m_streamers->Stream();

		gLogManager.log("Stream started.", ELL_INFO);

		m_isVideoStarted = true;
	}

	void StartStream()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

	//	Sleep(1000);
		m_status = EServiceStatus::Running;
		_startStream();
	}

	bool StopStream()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		gLogManager.log("Stopping AVStreamService.", ELL_INFO);

		m_streamers->Stop();
		Sleep(1000);
		//m_streamers->CloseAll();
		gLogManager.log("Streams stopped.", ELL_INFO);

		m_status = EServiceStatus::Stopped;
		m_isVideoStarted = false;
		return true;
	}


	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;

		if (!m_isVideoStarted)
			_startStream();
		else
		{
		}


	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + AudioStreamer::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);

		if (m_status == EServiceStatus::Running && m_portsReceived == false)
		{

			msg = "Started, but waiting for audio connection.";
			context->RenderText(msg, 0, 0,video::SColor(1,0,0,1));
		}

		msg = "Stream Settings:" ;
		context->RenderText(msg, 0, 0);

		msg = "Audio Interfaces:";
		context->RenderText(msg, 0, 0);
		for (int i = 0; i < m_audioInterfaceIndicies.size(); ++i)
		{
			msg = "\t" + core::StringConverter::toString(i) + " - " + m_audioInterfaceList[m_audioInterfaceIndicies[i]].description;
			context->RenderText(msg, 0, 0);
		}

		if (m_status != EServiceStatus::Running)
			return;
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

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

		_SendAudioSettings();
	}
	virtual void OnUserDisconnected()
	{
		m_portsReceived = false;
	}


	void _SendAudioSettings()
	{
		//reply back with audio interface specs
		xml::XMLWriter w;
		xml::XMLElement e("AudioRoot");

		e.addAttribute("StreamsCount", core::StringConverter::toString(m_audioInterfaceIndicies.size()));
		e.addAttribute("SpatialAudio", core::StringConverter::toString(m_isSpatialAudio));

		//add audio locations
		if (m_audioSpatialPosition.size() > 0)
		{
			for (int i = 0; i < m_audioSpatialPosition.size();++i)
			{
				xml::XMLElement* p = new xml::XMLElement("Pos");
				p->addAttribute("Val", core::StringConverter::toString(m_audioSpatialPosition[i].x) + "," + core::StringConverter::toString(m_audioSpatialPosition[i].y) + "," + core::StringConverter::toString(m_audioSpatialPosition[i].z));
				e.addSubElement(p);
			}
		}

		w.addElement(&e);

		core::string res = w.flush();

		int bufferLen = res.length() + sizeof(int)* 10;
		byte* buffer = new byte[bufferLen];

		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, bufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		stream.seek(0, OS::ESeek_Set);
		int reply = (int)EMessages::AudioConfig;
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
		if (msg == "AudioParameters")
		{
			_SendAudioSettings();
		}
		else if (msg == "AudioPort" && values.size() >= 1)
		{
			std::vector<uint> ports;
			for (int i = 0; i < values.size();++i)
				ports.push_back( core::StringConverter::toInt(values[i]));
			if (ports == m_AudioPort)
				return;
			m_AudioPort = ports;

			core::string clockIpAddr;
			if (m_context->sharedMemory->gstClockPortStreamer != 0)
			{
				clockIpAddr = "127.0.0.1";
			}

			for (int i = 0; i < m_streamers->GetStreamsCount() && i < m_AudioPort.size(); ++i)
			{
				m_streamers->GetStreamerAt(i)->BindPorts(m_context->remoteAddr.toString(), &m_AudioPort[i], 1, 0);
			}
			//m_streamers->GetStream("Audio")->BindPorts(m_context->remoteAddr.toString(), &m_AudioPort, 1, 0, 0);

			m_portsReceived = true;
		}
		else if (msg == "Stream" && values.size() >= 1)
		{
			bool enabled = core::StringConverter::toBool(values[0]);
			for (int i = 0; i < m_streamers->GetStreamsCount(); ++i)
				m_streamers->GetStreamerAt(i)->SetPaused(!enabled);
		}
	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{
		xml::XMLAttribute*a =elem->getAttribute("SpatialAudio");
		xml::XMLElement* e;
		m_audioSpatialPosition.clear();
		if (a)
		{
			m_isSpatialAudio = core::StringConverter::toBool(a->value);
		}
		else m_isSpatialAudio = false;
		if (m_isSpatialAudio)
		{
			e = elem->getSubElement("AudioPosition");
			while (e)
			{
				a = e->getAttribute("Pos");
				if (a)
				{
					Position p;
					std::vector<core::string> vals= core::StringUtil::Split(a->value, ",");
					if (vals.size() >= 3)
					{
						p.x = core::StringConverter::toFloat(vals[0]);
						p.y = core::StringConverter::toFloat(vals[1]);
						p.z = core::StringConverter::toFloat(vals[2]);
						m_audioSpatialPosition.push_back(p);
					}
				}
				e = e->nextSiblingElement("AudioPosition");
			}
		}
		e = elem->getSubElement("AudioInterface");
		while (e)
		{
			a = e->getAttribute("Index");
			if (a)
			{
				m_audioInterfaceIndicies.push_back(core::StringConverter::toInt(a->value));
			}
			e = e->nextSiblingElement("AudioInterface");
		}



		return true;
	}
};

AudioStreamer::AudioStreamer()
{
	m_impl = new AudioStreamerImpl();
}

AudioStreamer::~AudioStreamer()
{
	delete m_impl;
}


std::string AudioStreamer::GetServiceName()
{
	return AudioStreamer::ModuleName;
}

EServiceStatus AudioStreamer::GetServiceStatus()
{
	return m_impl->m_status;
}

void AudioStreamer::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus AudioStreamer::StartService(ServiceContext* context)
{
	m_impl->StartStream();
	return m_impl->m_status;
}

bool AudioStreamer::StopService()
{
	return m_impl->StopStream();
}

void AudioStreamer::DestroyService()
{
	m_impl->Destroy();
}


void AudioStreamer::Update(float dt)
{
	m_impl->Update();
}

void AudioStreamer::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void AudioStreamer::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool AudioStreamer::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void AudioStreamer::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

