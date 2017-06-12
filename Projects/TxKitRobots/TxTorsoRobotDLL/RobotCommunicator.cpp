
#include "stdafx.h"
#include "RobotCommunicator.h"
#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"

#include "tinyxml2.h"
#include "StringUtil.h"

#include "RobotSerialPort.h"

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

RobotCommunicator::RobotCommunicator()
{
	m_listener = 0;
	m_client = network::INetwork::getInstance().createUDPClient();
	m_thread = 0;
	

	m_controller = new RobotSerialPort();
		
}

RobotCommunicator::~RobotCommunicator()
{
	StopServer();
	delete m_client;
	delete m_controller;
}

void RobotCommunicator::HandleData(const core::string& name, const core::string& value)
{

	std::vector<core::string> vals;
	vals=core::StringUtil::Split(value, ",");

	if (name == "Speed" && vals.size() == 2)
	{
		m_robotStatus.speedX = atof(vals[0].c_str());
		m_robotStatus.speedY = atof(vals[1].c_str());
	}
	else if (name == "Head" && vals.size() == 3)
	{
		m_robotStatus.tilt = atof(vals[0].c_str());
		m_robotStatus.yaw = atof(vals[1].c_str());
		m_robotStatus.roll = atof(vals[2].c_str());
	}
	else if (name == "Rotation" && vals.size() == 1)
	{
		m_robotStatus.rotation = atof(vals[0].c_str());
	}
	else if (name == "User" && vals.size() == 2)
	{
		network::NetAddress addr = network::NetAddress(vals[0], atoi(vals[1].c_str()));
		if (addr.address != m_userStatus.address.address || addr.port!=m_userStatus.address.port)
		{
			m_userStatus.address = addr;
			if (m_listener)
			{
				m_listener->OnUserConnected(this, m_userStatus.address);
			}
		}
	}
}

void RobotCommunicator::ProcessPacket(const char* buffer)
{

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buffer);
	if (err != tinyxml2::XML_NO_ERROR)
	{
		//send update once
		m_controller->UpdateRobotStatus(&m_robotStatus);
		return;
	}

	tinyxml2::XMLElement*root = doc.RootElement();
	m_robotStatus.connected = root->BoolAttribute("Connected");
	if (!m_robotStatus.connected)
		return;
	tinyxml2::XMLElement* node = root->FirstChildElement("Data");
	while (node)
	{
		std::string name = node->Attribute("N");//name of the value
		std::string value = node->Attribute("V");//name of the value
		HandleData(name, value);
		node = node->NextSiblingElement("Data");
	}

	if (m_listener)
		m_listener->OnRobotStatus(this, m_robotStatus);
	

	m_controller->UpdateRobotStatus(&m_robotStatus);
}

int RobotCommunicator::_Process()
{
	if (!m_client->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_client->RecvFrom(buffer, &len, &src) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	ProcessPacket(buffer);
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

}
