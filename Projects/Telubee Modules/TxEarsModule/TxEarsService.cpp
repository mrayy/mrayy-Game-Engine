

#include "stdafx.h"
#include "TxEarsService.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "IThreadManager.h"
#include "GstAppNetAudioStreamer.h"
#include "LocalAudioGrabber.h"
#include "DirectSoundInputStream.h"
#include "GstNetworkAudioStreamer.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "NetworkValueController.h"
#include "StringUtil.h"
#include "ModuleSharedMemory.h"
#include "INetworkPortAssigner.h"
#include "GStreamerCore.h"
#include <conio.h>

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(TxEarsService, IServiceModule);

const std::string TxEarsService::ModuleName("TxEarsServiceModule");


class TxEarsServiceImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:

	GCPtr<video::GstStreamBin> m_streamers;


	std::vector<uint> m_AudioPort;
	bool m_portsReceived;
	bool m_isAudioStarted;
	network::NetAddress m_remoteAddr;


	EServiceStatus m_status;

	TBeeServiceContext* m_context;
	
	struct Position
	{
		float x, y, z;
	};
	struct AudioInterface
	{
		AudioInterface()
		{
			buffertime = 50;
		}
		int ID;
		int channels;
		int samplingRate;
		int buffertime;
	};
	std::vector<AudioInterface> m_audioInterfaceIndicies;
	std::vector<Position> m_audioSpatialPosition;
	bool m_isSpatialAudio;
	std::vector<sound::InputStreamDeviceInfo> m_audioInterfaceList;

	std::vector<video::LocalAudioGrabber*> m_audioGrabbers;


public:
	TxEarsServiceImpl()
	{
		m_status = EServiceStatus::Idle;

		m_streamers = new video::GstStreamBin();

		m_portsReceived = false;
		m_isAudioStarted = false;
		m_isSpatialAudio = false;
	}
	~TxEarsServiceImpl()
	{
		Destroy();
		m_streamers = 0;
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;
		video::GStreamerCore::Ref();
		m_context->serviceLoader->RegisterCapability(TxEarsService::ModuleName, "EarsSupported", "Yes");
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
				AudioInterface iface;
				iface.ID = -1;
				iface.channels = 2;
				iface.samplingRate = 44100;
				m_audioInterfaceIndicies.push_back(iface);//add the default audio interface
			}
			printf("Creating Audio Streamer\n");

			for (int i = 0; i < m_audioInterfaceIndicies.size(); ++i)
			{
				video::GstNetworkAudioStreamer::AudioInterface iface;
				video::GstNetworkAudioStreamer* streamer;
				streamer = new video::GstNetworkAudioStreamer();

				if (m_audioInterfaceIndicies[i].ID==-1)
					iface.deviceGUID = "";
				else
					iface.deviceGUID = m_audioInterfaceList[m_audioInterfaceIndicies[i].ID].deviceGUID;
				iface.channelsCount = m_audioInterfaceIndicies[i].channels;
				iface.samplingRate = m_audioInterfaceIndicies[i].samplingRate;
				iface.buffertime = m_audioInterfaceIndicies[i].buffertime;

				streamer->SetAudioInterface(iface);
				/*
				video::GstAppNetAudioStreamer* streamer;
				video::LocalAudioGrabber* g;
				g = new video::LocalAudioGrabber();
				streamer = new video::GstAppNetAudioStreamer();
				g->Init(m_audioInterfaceList[m_audioInterfaceIndicies[i].ID].deviceGUID, m_audioInterfaceIndicies[i].channels, m_audioInterfaceIndicies[i].samplingRate);
				streamer->SetAudioGrabber(g);

				m_audioGrabbers.push_back(g);*/


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
		tmpV->OnChanged += newClassDelegate1("", this, &TxEarsServiceImpl::_OnAudioPropertyChanged);


	}
	void _OnAudioPropertyChanged(IValue* v)
	{
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->serviceLoader->RemoveCapabilityCategory(TxEarsService::ModuleName);
		 
		StopStream();
		m_streamers->CloseAll();

		m_context->RemoveListener(this);
		m_streamers->ClearStreams(true);

		for (int i = 0; i < m_audioGrabbers.size();++i)
		{
			delete m_audioGrabbers[i];
		}
		m_audioGrabbers.clear();

		m_status = EServiceStatus::Idle;
	}


	void _startStream()
	{
		if (!m_portsReceived || m_status != EServiceStatus::Running)
			return;
		gLogManager.log("Creating Streaming.", ELL_INFO);
	/*	for (int i = 0; i < m_AudioPort.size(); ++i)
		{
			core::string pname = "Audio" + core::StringConverter::toString(i);
			m_AudioPort[i] = gNetworkPortAssigner.AssignPort(pname, network::EPT_UDP, m_context->GetPortValue(pname));
		}*/
		core::string clockIpAddr;
		if (m_context->sharedMemory->gstClockPortStreamer != 0)
		{
			clockIpAddr = "127.0.0.1";
		}
		for (int i = 0; i < m_streamers->GetStreamsCount() && i < m_AudioPort.size(); ++i)
		{
			m_streamers->GetStreamerAt(i)->SetClockAddr(clockIpAddr, m_context->sharedMemory->gstClockPortStreamer);
			m_streamers->GetStreamerAt(i)->BindPorts(m_context->GetTargetClientAddr()->toString(), &m_AudioPort[i], 1, 0);
			m_streamers->GetStreamerAt(i)->CreateStream();
			if (m_context->sharedMemory->gstClockPortStreamer == 0)
				m_context->sharedMemory->gstClockPortStreamer = m_streamers->GetStreamerAt(i)->GetClockPort();
		}
		gLogManager.log("Start Streaming.", ELL_INFO);
		m_streamers->Stream();

		for (int i = 0; i < m_audioGrabbers.size(); ++i)
		{
			m_audioGrabbers[i]->Start();
		}
		gLogManager.log("Stream started.", ELL_INFO);

		m_isAudioStarted = true;
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

		for (int i = 0; i < m_audioGrabbers.size(); ++i)
		{
			m_audioGrabbers[i]->Pause();
		}
		m_streamers->Stop();
	//	Sleep(1000);
		//m_streamers->CloseAll();
		gLogManager.log("Streams stopped.", ELL_INFO);

		m_status = EServiceStatus::Stopped;
		m_isAudioStarted = false;
		return true;
	}


	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;

		if (!m_isAudioStarted)
			_startStream();
		else
		{
		}


	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + TxEarsService::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);

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
			msg = "\t" + core::StringConverter::toString(i) + " - " + m_audioInterfaceList[m_audioInterfaceIndicies[i].ID].description + " - Channels=" + core::StringConverter::toString(m_audioInterfaceIndicies[i].channels)
				+ "@" + core::StringConverter::toString(m_audioInterfaceIndicies[i].samplingRate);
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
		gLogManager.log("Sending Audio Config",ELL_INFO);
		m_context->commChannel->SendTo(m_context->GetTargetClientAddr(), (char*)buffer, len);
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
		if (msg == "Parameters")
		{
			_SendAudioSettings();
		}
		else if (msg == "Port" && values.size() >= 1)
		{
			std::vector<uint> ports;
			ports.resize(values.size());
			m_AudioPort.resize(values.size());
			bool ok = (m_remoteAddr == m_context->remoteAddr);
			for (int i = 0; i < values.size(); ++i){
				ports[i] = core::StringConverter::toInt(values[i]);
				ok &= (m_AudioPort[i] == ports[i]);
				if (!m_context->portHostAddr)
				{
					m_context->portMap["Audio" + core::StringConverter::toString(i)] = ports[i];
				}
			}
			if (m_portsReceived && ok)
				return;
			m_AudioPort = ports;
			m_remoteAddr = m_context->remoteAddr;

			if(m_isAudioStarted)
				m_streamers->GetStream("Audio")->BindPorts(m_context->remoteAddr.toString(), &m_AudioPort[0], 1, false);

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
			AudioInterface iface;
			iface.ID = 0;
			iface.samplingRate = 44100;
			iface.channels = 2;
			iface.buffertime = 100;
			a = e->getAttribute("Index");
			if (a)
				iface.ID = core::StringConverter::toInt(a->value);
			a = e->getAttribute("Channels");
			if (a)
				iface.channels = core::StringConverter::toInt(a->value);
			a = e->getAttribute("SamplingRate");
			if (a)
				iface.samplingRate = core::StringConverter::toInt(a->value);
			a = e->getAttribute("BufferTime");
			if (a)
				iface.buffertime = core::StringConverter::toInt(a->value);
			m_audioInterfaceIndicies.push_back(iface);
			e = e->nextSiblingElement("AudioInterface");
		}



		return true;
	}
};

TxEarsService::TxEarsService()
{
	m_impl = new TxEarsServiceImpl();
}

TxEarsService::~TxEarsService()
{
	delete m_impl;
}


std::string TxEarsService::GetServiceName()
{
	return TxEarsService::ModuleName;
}

EServiceStatus TxEarsService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxEarsService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxEarsService::StartService(ServiceContext* context)
{
	m_impl->StartStream();
	return m_impl->m_status;
}

bool TxEarsService::StopService()
{
	return m_impl->StopStream();
}

void TxEarsService::DestroyService()
{
	m_impl->Destroy();
}


void TxEarsService::Update(float dt)
{
	m_impl->Update();
}

void TxEarsService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxEarsService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxEarsService::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void TxEarsService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

