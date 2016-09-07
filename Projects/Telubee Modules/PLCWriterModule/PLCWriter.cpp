

#include "stdafx.h"
#include "PLCWriter.h"
#include "TBeeServiceContext.h"
#include "IThreadManager.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "NetworkValueController.h"
#include "StringUtil.h"
#include "plc_config.h"
#include "INetwork.h"
#include <conio.h>

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(PLCWriter, IServiceModule);

const std::string PLCWriter::ModuleName("PLCWriter");

#define PC_COMM_PORT 8010

class PLCWriterImpl :public IServiceContextListener
{
public:



	EServiceStatus m_status;

	TBeeServiceContext* m_context;
	

	MCClient *mc;

	mc_buff mcWriteBuf;
	mc_buff mcReadBuf;

	int m_plcPort;
	core::string m_plcIP;

	network::IUDPClient* netClientReceiver;
	GCPtr<OS::IMutex> dataMutex;
	bool isDone = false;

	int m_dataReceived;

	HANDLE hThreadRecv;
	static DWORD WINAPI timerThreadRecv(PLCWriterImpl *robot, LPVOID pdata)
	{
		robot->_ProcessReceive();
		return 0;
	}

	void _ProcessReceive()
	{
		const byte TORSO_DATA = 5;
		char buffer[sizeof(mc_buff)+100];
		uint len;
		network::NetAddress src;
		OS::StreamReader rdr;
		while (!isDone){
			len = sizeof(buffer);
			if (netClientReceiver->RecvFrom(buffer, &len, &src, 0) == network::UDP_SOCKET_ERROR_NONE && len > 0)
			{
				OS::CMemoryStream stream("", (byte*)buffer, len, false);
				stream.seek(0, OS::ESeek_Set);
				rdr.setStream(&stream);
				uchar v = rdr.readByte();
				if (v == TORSO_DATA && len>sizeof(mcWriteBuf.torso)){
					//read torso data
					rdr.read(&mcWriteBuf.torso, sizeof(mcWriteBuf.torso));
					//update torso plc
					mc->batch_write("W", SELECT_TORSO, 0xA0, &mcWriteBuf, 0x20); 
					Sleep(1);
				}
				++m_dataReceived;
				//regardless of the message, reply with the entire data buffer
				mc->batch_read("W", SELECT_INTERLOCK, 0x0360, &mcReadBuf, 0x06);
				Sleep(1);
				//reply with the data buffer
				netClientReceiver->SendTo(&src, (const char*)&mcReadBuf, sizeof(mcReadBuf));
			}
		}
	}

public:
	PLCWriterImpl()
	{
		m_status = EServiceStatus::Idle;
		m_plcPort= PLC_PORT_TCP_TORSO;
		m_plcIP = MELSEC_PLC;
		m_dataReceived = 0;
		 netClientReceiver = 0;

		dataMutex = OS::IThreadManager::getInstance().createMutex();
	}
	~PLCWriterImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;

		_RegisterValues();

		context->AddListener(this);
		m_status = EServiceStatus::Inited;

		memset(&mcWriteBuf, 0, sizeof(mcWriteBuf)); // initializing test write data
		memset(&mcReadBuf, 0, sizeof(mcReadBuf)); // initializing test read data
		
		gLogManager.log("Connecting to PLC.", ELL_INFO);

		// create connection to Melsec PC via MC Protocol 
		mc = new MCClient(m_plcPort, (char*)m_plcIP.c_str());

		netClientReceiver = network::INetwork::getInstance().createUDPClient();
		netClientReceiver->Open(PC_COMM_PORT);

		isDone = false;
		hThreadRecv = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadRecv, this, NULL, NULL);

		gLogManager.log("Done Initing.", ELL_INFO);
	}

	void _RegisterValues()
	{

	}

	void StartService()
	{
	}

	bool StopService()
	{
		return true;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		 

		isDone = true;
		Sleep(100);
		if (netClientReceiver)
		{
			netClientReceiver->Close();
			netClientReceiver = 0;
		}
		TerminateThread(hThreadRecv, 0);
		m_status = EServiceStatus::Idle;
	}




	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;
	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + PLCWriter::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);
		context->RenderText(msg, 0, 0);

		msg = "TORSO Data Received:" + core::StringConverter::toString(m_dataReceived);
		context->RenderText(msg, 0, 0);

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

	}
	virtual void OnUserDisconnected()
	{
	}


	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{

		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		std::vector<core::string> values = core::StringUtil::Split(value, ",");

	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{
		xml::XMLElement* e;
		xml::XMLAttribute*a;
		
		a=elem->getAttribute("PLCPort");
		if (a)
			m_plcPort = core::StringConverter::toInt(a->value);

		a = elem->getAttribute("PLCIP");
		if (a)
			m_plcIP= a->value;

		return true;
	}
};

PLCWriter::PLCWriter()
{
	m_impl = new PLCWriterImpl();
}

PLCWriter::~PLCWriter()
{
	delete m_impl;
}


std::string PLCWriter::GetServiceName()
{
	return PLCWriter::ModuleName;
}

EServiceStatus PLCWriter::GetServiceStatus()
{
	return m_impl->m_status;
}

void PLCWriter::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus PLCWriter::StartService(ServiceContext* context)
{
	m_impl->StartService();
	return m_impl->m_status;
}

bool PLCWriter::StopService()
{
	return m_impl->StopService();
}

void PLCWriter::DestroyService()
{
	m_impl->Destroy();
}


void PLCWriter::Update(float dt)
{
	m_impl->Update();
}

void PLCWriter::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void PLCWriter::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool PLCWriter::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void PLCWriter::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

