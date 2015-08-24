
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
	namespace TBee
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
		//		OS::IThreadManager::getInstance().sleep(1);
			}
		}

	};

RobotCommunicator::RobotCommunicator()
{
	m_listener = 0;
	m_client = network::INetwork::getInstance().createUDPClient();
	m_thread = 0;
	
	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
		
}

RobotCommunicator::~RobotCommunicator()
{
	StopServer();
	delete m_client;
	delete m_dataMutex;
}
void RobotCommunicator::HandleData(network::NetAddress* addr,const core::string& name, const core::string& value)
{

	std::vector<core::string> vals;
	vals=core::StringUtil::Split(value, ",");

	OS::ScopedLock lock(m_dataMutex);

	if (name == "Connect" && vals.size() == 6)
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
		if (m_listener)
			m_listener->OnUserMessage(addr, name, value);
	}
}

void RobotCommunicator::ProcessPacket(network::NetAddress* addr,const char* buffer)
{

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buffer);
 	if (err != tinyxml2::XML_NO_ERROR)
 	{
// 		if (m_listener)
		//m_listener->OnRobotStatus(this,m_robotStatus);
 		return;
 	}
	tinyxml2::XMLElement*root = doc.RootElement();

//	m_robotStatus.connected  = core::StringConverter::toBool(root->Attribute("Connected"));
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
// 	if (m_listener)
// 		m_listener->OnRobotStatus(this, m_robotStatus);
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

	buffer[len] = 0;
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

}
}