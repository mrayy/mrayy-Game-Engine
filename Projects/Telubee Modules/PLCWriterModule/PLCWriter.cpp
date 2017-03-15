

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
#include "GNSSSharedMemory.h"
#include "ModuleSharedMemory.h"
#include "shmem.h"
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
	
	mc_gnss* m_gnssShMem;
	shmem m_gnssSharedMemory;

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
	HANDLE hThreadPLC;

	static DWORD WINAPI timerThreadRecv(PLCWriterImpl *robot, LPVOID pdata)
	{
		robot->_ProcessReceive();
		return 0;
	}
	static DWORD WINAPI timerThreadPLC(PLCWriterImpl *robot, LPVOID pdata)
	{
		robot->_ProcessPLC();
		return 0;
	}

	void _ProcessPLC()
	{
		while (!isDone){
			//update GNSS to PLC
			if (m_gnssShMem)
			{
				// mcWriteBuf is internal copy of shmem for PLC , m_gnssShMem is the actual shared memory from GNSS service
				memcpy(&mcWriteBuf.gnss, m_gnssShMem, sizeof(mc_gnss));
				mc->batch_write("W", SELECT_GNSS, PLC_GNSS_OFFSET, &mcWriteBuf, PLC_GNSS_SIZE);
			}

			//update TORSO to PLC
			mcWriteBuf.torso.userConnected = m_context->sharedMemory->UserConnected;
			mc->batch_write("W", SELECT_TORSO, PLC_TORSO_OFFSET, &mcWriteBuf, PLC_TORSO_SIZE);
			Sleep(10);
			mc->batch_read("W", SELECT_INTERLOCK, PLC_INTERLOCK_OFFSET, &mcReadBuf, PLC_INTERLOCK_SIZE);
			Sleep(10);
		}
	}
	void _ProcessReceive()
	{
		const byte TORSO_DATA = 5;
		char buffer[sizeof(mc_buff)+100];
		uint len;
		network::NetAddress src;
		OS::StreamReader rdr;
		m_dataReceived = 1;
		while (!isDone){
			len = sizeof(buffer);
			network::UDPClientError e= netClientReceiver->RecvFrom(buffer, &len, &src, 0);
			if (e == network::UDP_SOCKET_ERROR_NONE && len > 0)
			{
				++m_dataReceived;
				OS::CMemoryStream stream("", (byte*)buffer, len, false);
				stream.seek(0, OS::ESeek_Set);
				rdr.setStream(&stream);
				uchar v = rdr.readByte();
				if (v == TORSO_DATA && len>sizeof(mcWriteBuf.torso)){
					//read torso data
					rdr.read(&mcWriteBuf.torso, sizeof(mcWriteBuf.torso));
					Sleep(1);
				}
				//reply with the data buffer
				netClientReceiver->SendTo(&src, (const char*)&mcReadBuf, sizeof(mcReadBuf));
			}
			else {
				gLogManager.log("Failed to receive data!", ELL_WARNING);
			}
		}
		m_dataReceived = -1;
	}

public:
	PLCWriterImpl()
	{
		m_status = EServiceStatus::Idle;
		//m_plcPort= PLC_PORT_TCP_TORSO;
		//m_plcIP = MELSEC_PLC;
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

		Sleep(1000);//wait until GNSS services inited
		//start shared memory
		m_gnssSharedMemory.SetDataSize(sizeof(mc_gnss));
		m_gnssSharedMemory.SetName("SH_Tx_GNSS");
		m_gnssSharedMemory.openRead();

		m_gnssShMem = m_gnssSharedMemory.GetData<mc_gnss>();
		if (!m_gnssShMem)
		{
			gLogManager.log("Failed open GNSS shared memory! Make sure GNSS service is running!", ELL_WARNING);
		}


		memset(&mcWriteBuf, 0, sizeof(mcWriteBuf)); // initializing test write data
		memset(&mcReadBuf, 0, sizeof(mcReadBuf)); // initializing test read data
		
		gLogManager.log("Connecting to PLC.", ELL_INFO);

		// create connection to Melsec PC via MC Protocol 
		mc = new MCClient(m_plcPort, (char*)m_plcIP.c_str());

		netClientReceiver = network::INetwork::getInstance().createUDPClient();
		netClientReceiver->Open(PC_COMM_PORT);

		isDone = false;
		hThreadRecv = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadRecv, this, NULL, NULL);
		hThreadPLC = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadPLC, this, NULL, NULL);

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
		TerminateThread(hThreadPLC, 0);
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

		msg = "PLC Status:" + core::string(mc->IsConnected()?"Connected":"Disconnected");
		context->RenderText(msg, 0, 0);

		if (m_gnssShMem)
		{
			msg = "Lat, Lng:" + core::StringConverter::toString(m_gnssShMem->latitude) + "," + core::StringConverter::toString(m_gnssShMem->longitude) ;
			context->RenderText(msg, 0, 0);
		}

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

