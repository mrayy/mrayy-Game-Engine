

#include "stdafx.h"
#include "IXMLNetMessageHandler.h"

#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"

#include "tinyxml2.h"
#include "StringUtil.h"
#include "ILogManager.h"
#include "MutexLocks.h"

namespace mray
{
namespace TBee
{



	class IXMLNetMessageHandlerThread :public OS::IThreadFunction
	{
		IXMLNetMessageHandler* m_owner;
	public:
		IXMLNetMessageHandlerThread(IXMLNetMessageHandler* o)
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
				//		OS::IThreadManager::getInstance().sleep(1);
			}
		}

	};


IXMLNetMessageHandler::IXMLNetMessageHandler()
{

	m_client = network::INetwork::getInstance().createUDPClient();
	m_thread = 0;
	m_port = 0;

	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
}

IXMLNetMessageHandler::~IXMLNetMessageHandler()
{
	StopHandler();
	delete m_client;
	delete m_dataMutex;
}


void IXMLNetMessageHandler::StartHandler(int port)
{
	StopHandler();
	m_client->Open(port);
	m_port = m_client->Port();
	m_thread = OS::IThreadManager::getInstance().createThread(new IXMLNetMessageHandlerThread(this));
	m_thread->start(0);
	printf("Communication Channel started - Port : %d\n", port);
}

void IXMLNetMessageHandler::StopHandler()
{
	m_client->Close();
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	m_thread = 0;
	printf("Communication Channel closed\n");
}


int IXMLNetMessageHandler::_Process()
{
	if (!m_client->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_client->RecvFrom(buffer, &len, &src, 0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	buffer[len] = 0;
	ProcessPacket(&src, buffer);
	return 0;
}


void IXMLNetMessageHandler::ProcessPacket(network::NetAddress* addr, const char* buffer)
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

	_OnDataArrived(addr,buffer);
	tinyxml2::XMLElement* node = root->FirstChildElement("Data");
	while (node)
	{
		std::string name = node->Attribute("N");//name of the value
		std::string value = node->Attribute("V");//name of the value
		_HandleData(addr, name, value);
		node = node->NextSiblingElement("Data");
	}
}



}
}
