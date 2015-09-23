

#include "stdafx.h"
#include "NetworkValueController.h"
#include "INetwork.h"
#include "IThreadManager.h"
#include "tinyxml2.h"
#include "IXMLParser.h"
#include "XMLTree.h"

namespace mray
{
namespace TBee
{


	class NetworkValueControllerThread :public OS::IThreadFunction
	{
		NetworkValueController* m_owner;
	public:
		NetworkValueControllerThread(NetworkValueController* o)
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

NetworkValueController::NetworkValueController() :m_values("Values")
{
	m_client = 0;
	m_thread = 0;
	m_isReceiver = false;

	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
}

NetworkValueController::~NetworkValueController()
{
	Stop();
	delete m_dataMutex;

}


ValueGroup* NetworkValueController::GetValues()
{
	return &m_values;
}


void NetworkValueController::StartReceiver(int port)
{
	Stop();	
	m_isReceiver = true;
	m_client = network::INetwork::getInstance().createUDPClient();
	m_client->Open(port);
	m_thread = OS::IThreadManager::getInstance().createThread(new NetworkValueControllerThread(this));
	m_thread->start(0);
	printf("NetworkValueController started - Port : %d\n", m_client->Port());
}
int NetworkValueController::GetPort()
{
	if (m_client == 0)
		return 0;
	return m_client->Port();
}
void NetworkValueController::StartSender(const network::NetAddress& target)
{
	Stop();
	m_isReceiver = false;
	m_remote = target;
	m_client = network::INetwork::getInstance().createUDPClient();
	m_client->Open();
}

void NetworkValueController::Stop()
{
	if (m_client)
	{
		m_client->Close();
	}
	if (m_thread){
		OS::IThreadManager::getInstance().killThread(m_thread);
		delete m_thread;
		m_thread = 0;
	}
	if (m_client)
	{
		delete m_client;
		m_client = 0;
	}
	printf("NetworkValueController closed\n");
}

void NetworkValueController::SendData()
{
	if (/*m_isReceiver ||*/ !m_client->IsOpen())
		return;
	xml::XMLWriter w;
	xml::XMLElement root("RobotData");
	//m_dataMutex->lock();
	xml::XMLElement* e=m_values.exportXMLSettings(&root);
	//m_dataMutex->unlock();
	w.addElement(e);
	core::string outputValues = w.flush();
	m_client->SendTo(&m_remote, outputValues.c_str(), outputValues.length() + 1);
}

bool NetworkValueController::_Process()
{

	if (!m_client->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	uint len = MAX_BUFFER;
	if (m_client->RecvFrom(buffer, &len, &m_remote, 0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	buffer[len] = 0;
	if(false)
		printf("%s\n", buffer);

	core::string msg;
	char*ptr = buffer;
	int msgIdx = -1;
	for (int i = 0; i < len; ++i)
	{
		if (buffer[i] == '#')
		{
			msgIdx = i;
			break;
		}
	}
	if (msgIdx != -1)
	{
		buffer[msgIdx] = 0;
		msg = buffer;
		ptr = &buffer[msgIdx + 1];
	}
	if (msg == "Data")
	{
		xml::XMLTree t;

		if (!xml::IXMLParser::getInstance().parserXML(ptr, &t))
			return 0;

		xml::XMLElement* root = t.getSubElement("ValueGroup");
		if (!root)
			return 0;

		m_values.loadXMLSettings(root);
	}
	else if (msg == "RequestScheme")
	{
		xml::XMLWriter w;
		xml::XMLElement e("");
		w.addElement(m_values.exportXMLSettings(&e));
		core::string ret = w.flush();
		m_client->SendTo(&m_remote, ret.c_str(), ret.length());
	}
	return 0;
}

}
}