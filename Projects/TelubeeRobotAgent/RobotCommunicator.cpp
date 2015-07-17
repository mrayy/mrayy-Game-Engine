
#include "stdafx.h"
#include "RobotCommunicator.h"
#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"

#include "tinyxml2.h"
#include "StringUtil.h"
#include "IRobotController.h"
#include "IDllManager.h"
#include "ILogManager.h"
#include "MutexLocks.h"


namespace mray
{


	class RobotCommunicatorThread :public OS::IThreadFunction
	{
		RobotCommunicator* m_owner;
	public:
		RobotCommunicatorThread(RobotCommunicator* o)
		{
			m_owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (m_owner->_Process()!=0)
				{
					return;
				}
			}
		}

	};

	typedef void(*dllFunctionPtr)();
	typedef IRobotController*(*dllLoadRobotFunctionPtr)();

RobotCommunicator::RobotCommunicator()
{
	m_listener = 0;
	m_client = network::INetwork::getInstance().createUDPClient();
	m_thread = 0;
	
	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
	m_localControl = 0;
	m_robotLib = OS::IDllManager::getInstance().getLibrary("Robot.dll");
	m_msgSink = 0;
	if (!m_robotLib)
	{
		gLogManager.log("Failed to load Robot.dll!! Please make sure Robot.dll is placed next to application.", ELL_WARNING);
		m_robotController = 0;
		return;
	}
	dllFunctionPtr libInitPtr;
	dllLoadRobotFunctionPtr robotLoadPtr;
	libInitPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotInit");
	robotLoadPtr = (dllLoadRobotFunctionPtr)m_robotLib->getSymbolName("DLL_GetRobotController");

	libInitPtr();


	m_robotController = robotLoadPtr();

		
}

RobotCommunicator::~RobotCommunicator()
{
	dllFunctionPtr libDestroyPtr;
	if (m_robotLib)
	{
		libDestroyPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotDestroy");
	}
	StopServer();
	delete m_client;
	//delete m_robotController;
	libDestroyPtr();
	delete m_dataMutex;
}
void RobotCommunicator::Initialize()
{
	if (m_robotController)
	{
		m_robotController->InitializeRobot(this);
	}
	if (m_listener)
		m_listener->OnCalibrationDone(this);
}
void RobotCommunicator::GetRobotStatus(RobotStatus& st)const
{
	OS::ScopedLock l(m_dataMutex);
	memcpy(&st, &m_robotStatus, sizeof(m_robotStatus));
}

void RobotCommunicator::HandleData(network::NetAddress* addr,const core::string& name, const core::string& value)
{

	std::vector<core::string> vals;
	vals=core::StringUtil::Split(value, ",");

	OS::ScopedLock lock(m_dataMutex);

	if (name == "Speed" && vals.size() == 2)
	{
		m_robotStatus.speed[0] = atof(vals[0].c_str());
		m_robotStatus.speed[1] = atof(vals[1].c_str());
		//limit the speed
		m_robotStatus.speed[0] = -math::clamp<float>(m_robotStatus.speed[0], -1, 1);
		m_robotStatus.speed[1] = math::clamp<float>(m_robotStatus.speed[1], -1, 1);
	}
	else if (name == "HeadRotation" && vals.size() == 4)
	{
		m_robotStatus.headRotation[0] = atof(vals[0].c_str());
		m_robotStatus.headRotation[1] = atof(vals[1].c_str());
		m_robotStatus.headRotation[2] = atof(vals[2].c_str());
		m_robotStatus.headRotation[3] = atof(vals[3].c_str());

		//do head limits
// 		m_robotStatus.tilt = math::clamp(m_robotStatus.tilt, -50.0f, 50.0f);
// 		m_robotStatus.yaw = math::clamp(m_robotStatus.yaw, -70.0f, 70.0f);
// 		m_robotStatus.roll = math::clamp(m_robotStatus.roll, -40.0f, 40.0f);
	}
	else if (name == "HeadPosition" && vals.size() == 3)
	{
		m_robotStatus.headPos[0] = atof(vals[0].c_str());
		m_robotStatus.headPos[1] = atof(vals[1].c_str());
		m_robotStatus.headPos[2] = atof(vals[2].c_str());

	}
	else if (name == "Rotation" && vals.size() == 1)
	{
		m_robotStatus.rotation = atof(vals[0].c_str());
		m_robotStatus.rotation = math::clamp<float>(m_robotStatus.rotation, -1, 1);
	}
	else if (name == "Connect" && vals.size() == 6)
	{
		int videoPort = atoi(vals[1].c_str());
		int audioPort = atoi(vals[2].c_str());
		int handsPort = atoi(vals[3].c_str());
		int clockPort = atoi(vals[4].c_str());
		bool rtcp = core::StringConverter::toBool(vals[5].c_str());
		network::NetAddress addr = network::NetAddress(vals[0], videoPort);
		//if (addr.address != m_userStatus.address.address || addr.port!=m_userStatus.address.port)
		{
			m_userStatus.address = addr;
			if (m_listener)
			{
				UserConnectionData data;
				data.address = m_userStatus.address;
				data.videoPort = videoPort;
				data.audioPort = audioPort;
				data.handsPort = handsPort;
				data.clockPort = clockPort;
				data.rtcp = rtcp;
				m_listener->OnUserConnected(this, data);
			}
		}
	}
	else if (name == "Disconnect" && vals.size() == 2)
	{
		network::NetAddress addr = network::NetAddress(vals[0], atoi(vals[1].c_str()));
		if (addr.address == m_userStatus.address.address)
		{
			if (m_listener)
			{
				m_listener->OnUserDisconnected(this, m_userStatus.address);
			}
		}
	}
	else
	{
		if (m_msgSink)
			m_msgSink->OnMessage(addr,name, value);
	}
}

void RobotCommunicator::SetRobotData(const RobotStatus &st)
{
	if (m_localControl)
		_RobotStatus(st);
}

void RobotCommunicator::_RobotStatus(const RobotStatus& st)
{
	if (!m_robotController)
		return;
		if ((st.connected || m_localControl) && !m_robotController->IsConnected())
		m_robotController->ConnectRobot();
	
	if ((!st.connected && !m_localControl) && m_robotController->IsConnected())
		m_robotController->DisconnectRobot();

	m_robotController->UpdateRobotStatus(st);
	if (m_listener)
		m_listener->OnRobotStatus(this, st);
}
void RobotCommunicator::ProcessPacket(network::NetAddress* addr,const char* buffer)
{

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buffer);
	if (err != tinyxml2::XML_NO_ERROR)
	{
		//send update once
		if (!m_localControl)
			_RobotStatus(m_robotStatus);
		return;
	}
	tinyxml2::XMLElement*root = doc.RootElement();

	m_robotStatus.connected  = root->BoolAttribute("Connected");;
// 	if (!c && !m_robotStatus.connected)
// 		return;
// 	if (!m_robotStatus.connected)
// 		return;
	tinyxml2::XMLElement* node = root->FirstChildElement("Data");
	while (node)
	{
		std::string name = node->Attribute("N");//name of the value
		std::string value = node->Attribute("V");//name of the value
		HandleData(addr,name, value);
		node = node->NextSiblingElement("Data");
	}
	if (!m_localControl)
		_RobotStatus(m_robotStatus);
}

int RobotCommunicator::_Process()
{
	if (!m_client->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_client->RecvFrom(buffer, &len, &src,0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	ProcessPacket(&src,buffer);
	return 0;
}


void RobotCommunicator::StartServer(int port)
{
	StopServer();
	m_client->Open(port);
	m_thread = OS::IThreadManager::getInstance().createThread(new RobotCommunicatorThread(this)); 
	m_thread->start(0);
	printf("Communication Channel started - Port : %d\n", port);
}

void RobotCommunicator::StopServer()
{
	m_client->Close();
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	m_thread = 0;
	printf("Communication Channel closed\n");

}

void RobotCommunicator::OnCollisionData(float left, float right)
{
	if (m_listener)
	{
		m_listener->OnCollisionData(this,left, right);
	}
}

void RobotCommunicator::OnReportMessage(int code, const std::string& msg)
{
	if (m_listener)
	{
		m_listener->OnReportMessage(this, code, msg);
	}

}

}
