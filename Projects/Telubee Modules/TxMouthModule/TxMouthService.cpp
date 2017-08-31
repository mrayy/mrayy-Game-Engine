

#include "stdafx.h"
#include "TxMouthService.h"
#include "TBeeServiceContext.h"
#include "GstStreamBin.h"
#include "IThreadManager.h"
#include "GstNetworkAudioPlayer.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "NetworkValueController.h"
#include "StringUtil.h"
#include "AppSrcVideoSrc.h"
#include "ModuleSharedMemory.h"
#include "INetworkPortAssigner.h"
#include <conio.h>

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(TxMouthService, IServiceModule);

const std::string TxMouthService::ModuleName("TxMouthServiceModule");


class TxMouthServiceImpl :public video::IGStreamerStreamerListener, public IServiceContextListener
{
public:


	uint m_AudioPort;
	bool m_isAudioStarted;
	network::NetAddress m_remoteAddr;


	EServiceStatus m_status;

	TBeeServiceContext* m_context;

	video::GstNetworkAudioPlayer* m_audioPlayer;


public:
	TxMouthServiceImpl()
	{
		m_status = EServiceStatus::Idle;

		m_audioPlayer = 0;
		m_isAudioStarted = false;
	}
	~TxMouthServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;
		m_audioPlayer = new video::GstNetworkAudioPlayer();
		m_AudioPort = gNetworkPortAssigner.AssignPort("AudioPlayer", network::EPT_UDP, m_context->GetPortValue("AudioPlayer"));
			

		printf("Finished player\n");

		_RegisterValues();

		context->AddListener(this);
		m_status = EServiceStatus::Inited;
		m_context->serviceLoader->RegisterCapability(TxMouthService::ModuleName, "MouthSupported", "Yes");

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
		tmpV->OnChanged += newClassDelegate1("", this, &TxMouthServiceImpl::_OnAudioPropertyChanged);


	}
	void _OnAudioPropertyChanged(IValue* v)
	{
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->serviceLoader->RemoveCapabilityCategory(TxMouthService::ModuleName);

		StopStream();

		m_context->RemoveListener(this);

		if (m_audioPlayer)
		{
			m_audioPlayer->Close();
			delete m_audioPlayer;
		}

		m_status = EServiceStatus::Idle;
	}


	void _startStream()
	{
		if (m_status != EServiceStatus::Running)
			return;
		gLogManager.log("Creating Streaming.", ELL_INFO);

		if (m_audioPlayer)
		{
			m_audioPlayer->Close();

			core::string clockIpAddr;
			if (m_context->sharedMemory->gstClockPortPlayer != 0)
			{
				clockIpAddr = "127.0.0.1";
			}
			m_audioPlayer->SetIPAddress(m_context->GetTargetClientAddr()->toString(), m_AudioPort, 0);
			m_audioPlayer->SetClockAddr(clockIpAddr, m_context->sharedMemory->gstClockPortPlayer);
			m_audioPlayer->CreateStream();
			m_audioPlayer->Play();
			m_AudioPort = m_audioPlayer->GetPort(0);
			if (m_context->sharedMemory->gstClockPortPlayer == 0)//only if first time
				m_context->sharedMemory->gstClockPortPlayer = m_audioPlayer->GetClockPort();

			gLogManager.log("Starting audio player at port: " + core::StringConverter::toString(m_audioPlayer->GetPort(0)), ELL_INFO);
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

		if (m_audioPlayer)
		{
			//	gLogManager.log("Stopping Audio.", ELL_INFO);
			m_audioPlayer->Close();
			//	gLogManager.log("Audio Stopped.", ELL_INFO);
		}

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

		core::string msg = "[" + TxMouthService::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);


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
	}


	void _SendAudioSettings()
	{
		if (!m_audioPlayer)
			return;

		//reply with camera settings
		xml::XMLWriter w;

		xml::XMLElement e("AudioRoot");
		e.addAttribute("AudioPlayerPort", core::StringConverter::toString(m_AudioPort));
		w.addElement(&e);

		core::string res = w.flush();

		int bufferLen = res.length() + sizeof(int)* 10;
		byte* buffer = new byte[bufferLen];

		OS::CMemoryStream stream("", buffer, bufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		stream.seek(0, OS::ESeek_Set);
		int reply = (int)EMessages::AudioPlayerConfig;
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
	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{

		return true;
	}
};

TxMouthService::TxMouthService()
{
	m_impl = new TxMouthServiceImpl();
}

TxMouthService::~TxMouthService()
{
	delete m_impl;
}


std::string TxMouthService::GetServiceName()
{
	return TxMouthService::ModuleName;
}

EServiceStatus TxMouthService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxMouthService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxMouthService::StartService(ServiceContext* context)
{
	m_impl->StartStream();
	return m_impl->m_status;
}

bool TxMouthService::StopService()
{
	return m_impl->StopStream();
}

void TxMouthService::DestroyService()
{
	m_impl->Destroy();
}


void TxMouthService::Update(float dt)
{
	m_impl->Update();
}

void TxMouthService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxMouthService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxMouthService::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void TxMouthService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

