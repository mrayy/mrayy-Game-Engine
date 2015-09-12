

#include "stdafx.h"
#include "DataCommunicator.h"

#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"
#include "CMemoryStream.h"
#include "ATAppGlobal.h"
#include "GeomDepthRect.h"
#if USE_OPENNI
#include "OpenNIHandler.h"
#endif
#include "INetwork.h"
#include "StreamReader.h"
#include "CommunicationMessages.h"
#include "TBRobotInfo.h"

namespace mray
{
namespace AugTel
{


	class DataCommunicatorThread :public OS::IThreadFunction
	{
		DataCommunicator* m_owner;
	public:
		DataCommunicatorThread(DataCommunicator* o)
		{
			m_owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (m_owner->_Process() != 0)
				{
					return;
				}
			}
		}

	};
DataCommunicator::DataCommunicator()
{
	m_client = network::INetwork::getInstance().createUDPClient();
}
DataCommunicator::~DataCommunicator()
{
	Stop();
	delete m_client;
}

void DataCommunicator::Start(int port)
{
	m_client->Open(port);
	m_thread = OS::IThreadManager::getInstance().createThread(new DataCommunicatorThread(this));
	m_thread->start(0);
}

void DataCommunicator::Stop()
{
	m_client->Close();
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	m_thread = 0;
}
void DataCommunicator::DetectRobots(int port)
{
	core::string detectMsg = "<M><Data N=\"Detect\" /></M>";
	network::NetAddress addr = network::NetAddress::AnyAddr;
	addr.port = port;
	m_client->SendTo(&addr,detectMsg.c_str(),detectMsg.length()+1);
}
int DataCommunicator::_Process()
{

	if (!m_client->IsOpen())
		return -1;
#define MAX_BUFFER 65536
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_client->RecvFrom(buffer, &len, &src,0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet
	OS::CMemoryStream stream("", (uchar*)buffer, len, false);
	OS::StreamReader rdr(&stream);
	int msg = 0;
	stream.read(&msg,sizeof(msg));

	switch (msg)
	{
	case (int)EMessages::Presence:
	{
		TBee::TBRobotInfo ifo;
		ifo.Read(&rdr);
		FIRE_LISTENR_METHOD(OnRobotInfoDetected, (ifo));

	}break;
	case (int)EMessages::DepthData:
	{
		TBee::GeomDepthRect rc;
		rc.ReadFromStream(&stream);
		FIRE_LISTENR_METHOD(OnDepthData, (rc));
		//ATAppGlobal::Instance()->depthProvider->GetNormalCalculator().AddDepthRect(&rc);
	}break;
	case (int)EMessages::DepthSize:
	{
		math::vector2di sz;
		stream.read(&sz,sizeof(sz));
		FIRE_LISTENR_METHOD(OnDepthSize, (sz));
		//ATAppGlobal::Instance()->depthProvider->CreateDepthFrame(sz.x, sz.y);
	}break;
	case (int)EMessages::IsStereo:
	{
		bool stereo;
		stream.read(&stereo, sizeof(stereo));
		FIRE_LISTENR_METHOD(OnIsStereoImages, (stereo));
		//ATAppGlobal::Instance();
	}break;
	case (int)EMessages::CalibrationDone:
	{
		FIRE_LISTENR_METHOD(OnRobotCalibrationDone, ());
		//ATAppGlobal::Instance();
	}break;
	case (int)EMessages::CameraConfig:
	{
		OS::StreamReader rdr(&stream);
		core::string profile= rdr.binReadString();
		FIRE_LISTENR_METHOD(OnCameraConfig, (profile));
		//ATAppGlobal::Instance();
	}break;
	case (int)EMessages::ReportMessage:
	{
		OS::StreamReader rdr(&stream);
		int code = rdr.binReadInt();
		core::string msg = rdr.binReadString();
		FIRE_LISTENR_METHOD(OnReportedMessage, (code,msg));
		//ATAppGlobal::Instance();
	}break;
	case (int)EMessages::BumpSensorMessage:
	{
		OS::StreamReader rdr(&stream);
		int count = rdr.binReadInt();
		if (count==0)
			break;
		bool *v = new bool[count];
		for (int i = 0; i < count; ++i)
			stream.read(&v[i], sizeof(v[i]));
		FIRE_LISTENR_METHOD(OnBumpSensor, (count,v));
		delete[] v;
		//ATAppGlobal::Instance();
	}break;
	case (int)EMessages::IRSensorMessage:
	{
		OS::StreamReader rdr(&stream);
		int count = rdr.binReadInt();
		if (count == 0)
			break;
		float *v = new float[count];
		for (int i = 0; i < count; ++i)
			v[i] = rdr.binReadFloat();
		FIRE_LISTENR_METHOD(OnIRSensor, (count, v));
		delete[] v;
	}break;
	case (int)EMessages::BatteryLevel:
	{
		OS::StreamReader rdr(&stream);
		int level = rdr.binReadInt();
		FIRE_LISTENR_METHOD(OnBatteryLevel, (level));
	}break;
	case (int)EMessages::ClockSync:
	{
		OS::StreamReader rdr(&stream);
		int c = rdr.binReadInt();
		FIRE_LISTENR_METHOD(OnClockSync, (c));
	}break;
	default:
		break;
	}

	return 0;
}

}
}


