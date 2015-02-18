
#include "stdafx.h"
#include "LocalRobotCommunicator.h"
#include "shmem.h"
#include "multicast.h"
#include "IUDPClient.h"
#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "IThreadManager.h"
#include "multicast.h"
#include "TBAppGlobals.h"
#include "TBRobotInfo.h"

namespace mray
{
namespace TBee
{
#define USE_UDPCLIENT
#define USE_OPTI
	class LocalRobotCommunicatorData;
	class LocalRobotCommunicatorThread:public OS::IThreadFunction
	{
		LocalRobotCommunicatorData* m_owner;
	public:
		LocalRobotCommunicatorThread(LocalRobotCommunicatorData* o)
		{
			m_owner=o;
		}
		virtual void execute(OS::IThread*caller,void*arg);
	};
	class LocalRobotCommunicatorData
	{
	public:
		 struct shmem_data data;
		 LocalRobotCommunicator* owner;
#ifdef USE_UDPCLIENT
		network::IUDPClient* client;
#else
		 UDPSender*	client;
#endif
		network::NetAddress addr;
		OS::IThread* thread;
		bool connected;
		LocalRobotCommunicatorData(LocalRobotCommunicator* o)
		{
			owner=o;
			connected=false;
#ifdef USE_UDPCLIENT
			client=network::INetwork::getInstance().createUDPClient();
			client->Open();
#else
			client=new UDPSender();
			client->startSending();
#endif
			memset(&data, 0, sizeof(shmem_data_t));
		}
		~LocalRobotCommunicatorData()
		{
			if(client)
			{
#ifdef USE_UDPCLIENT
				client->Close();
#else
				client->stopSending();
#endif
				delete client;
				client=0;
			}
		}
		int getmyip(){
			char ac[80];
			if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
				printf("Error when getting local host name \n");
				return 1;
			}

			struct hostent *phe = gethostbyname(ac);
			if (phe == 0) {
				printf("Bad host lookup.\n");
				return 1;
			}

			for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
				struct in_addr addr;
				memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
				data.user.ipaddress = addr;	
			}

			return 0;
		}

	};


	void LocalRobotCommunicatorThread::execute(OS::IThread*caller,void*arg)
	{
		while(caller->isActive())
		{
			if(m_owner->connected)
			{
				m_owner->owner->Update(0);
			}
			if(m_owner->data.robot.status.connected && m_owner->connected)
			{
				OS::IThreadManager::getInstance().sleep(2);
			}else
			{
				OS::IThreadManager::getInstance().sleep(100);
			}
		}
	}
LocalRobotCommunicator::LocalRobotCommunicator()
{
	m_data=new LocalRobotCommunicatorData(this);
	m_data->data.user.status.online=true;
	m_data->getmyip();
//	Connect("127.0.0.1",3000);

	m_data->thread=OS::IThreadManager::getInstance().createThread(new LocalRobotCommunicatorThread(m_data));
	m_data->thread->start(0);
}
LocalRobotCommunicator::~LocalRobotCommunicator()
{
	OS::IThreadManager::getInstance().killThread(m_data->thread);
	delete m_data;
}

bool LocalRobotCommunicator::Connect(const core::string& ip,int port)
{
	if(m_data->connected)
		Disconnect();
	bool res=false;
	m_data->addr=network::NetAddress(ip,port);
#ifdef USE_UDPCLIENT
	if(m_data->client->Connect(m_data->addr))
#else
	if(m_data->client->Connect(port,ip.c_str()))
#endif
	{

#ifdef USE_OPTI
		{
			core::string ipaddr=inet_ntoa(m_data->data.user.ipaddress);
		//	TBAppGlobals::Instance()->optiObj->Connect(ipaddr,ipaddr);
		}
#endif
		m_data->connected=true;
		res=true;
	}

	return res;
}
void LocalRobotCommunicator::Disconnect()
{
	if(!m_data->connected)
		return;
#ifdef USE_OPTI
	//TBAppGlobals::Instance()->optiObj->Disconnect();
#endif
#ifdef USE_UDPCLIENT
	m_data->client->Disconnect();
#else
	m_data->client->stopSending();
#endif
	m_data->connected=false;
}
 bool LocalRobotCommunicator::IsConnected()
{
	return m_data->connected ;
}

void LocalRobotCommunicator::SetUserID(const core::string& userID)
{
	m_data->data.user.user_id=userID.c_str();
}

void LocalRobotCommunicator::ConnectUser(bool c)
{
	m_data->data.user.status.connected=c;
	_SendUpdate();
}
void LocalRobotCommunicator::ConnectRobot(bool c)
{
	m_data->data.robot.status.connected=c;
	_SendUpdate();
}

void LocalRobotCommunicator::Update(float dt)
{
	//return ;
	if(!m_data->connected)
		return;
	
#if TELUBEE_VER==1
	m_data->data.user.head.x=TBAppGlobals::Instance()->optiObj->GetPos().x;
	m_data->data.user.head.y=TBAppGlobals::Instance()->optiObj->GetPos().y;
	m_data->data.user.head.z=TBAppGlobals::Instance()->optiObj->GetPos().z;

	m_data->data.user.head.pitch=-TBAppGlobals::Instance()->optiObj->GetRotation().x;
	m_data->data.user.head.yaw=TBAppGlobals::Instance()->optiObj->GetRotation().y;
	m_data->data.user.head.roll=-TBAppGlobals::Instance()->optiObj->GetRotation().z;

#define ToStr(x) core::StringConverter::toString(x)
	core::string str=core::string(m_data->data.user.user_id.c_str())
		+ ", "+ToStr(m_data->data.user.ipaddress.S_un.S_addr)
		+ ", "+ToStr(m_data->data.user.location.latitude)
		+ ", "+ToStr(m_data->data.user.location.longitude)
		+ ", "+ToStr(m_data->data.user.head.x)
		+ ", "+ToStr(m_data->data.user.head.y)
		+ ", "+ToStr(m_data->data.user.head.z)
		+ ", "+ToStr(m_data->data.user.head.roll)
		+ ", "+ToStr(m_data->data.user.head.pitch)
		+ ", "+ToStr(m_data->data.user.head.yaw)
		+ ", "+ToStr((int)m_data->data.user.status.connected)
		+ ", "+ToStr((int)m_data->data.user.status.online)
		+ ", "+ToStr((int)m_data->data.user.status.home)
		+ ", "+ToStr((int)m_data->data.user.status.initial)
		+ ", "+ToStr((int)m_data->data.user.status.reset)
		+ ", "+ToStr(m_data->data.robot.status.connected);


	printf("%.1f %.1f %.1f\n",math::toDeg(m_data->data.user.head.pitch),math::toDeg(m_data->data.user.head.yaw),math::toDeg(m_data->data.user.head.roll));

#ifdef USE_UDPCLIENT
	m_data->client->SendTo(&m_data->addr,str.c_str(),str.length()+1);
#else
	m_data->client->setBuffer(str.c_str());
#endif

#endif
}

void LocalRobotCommunicator::_SendUpdate()
{
	core::string ipaddr;
	if(TBee::TBAppGlobals::Instance()->selectedRobot)
		ipaddr=TBee::TBAppGlobals::Instance()->selectedRobot->IP;//inet_ntoa(m_data->data.user.ipaddress);
	else
		return;
	char buffer[512];
	sprintf_s (buffer, "%s %d %d",512, ipaddr.c_str(), m_data->data.user.status.connected, m_data->data.robot.status.connected); 

#ifdef USE_UDPCLIENT
	m_data->client->SendTo(&m_data->addr,buffer,strlen(buffer));
#else
	m_data->client->setBuffer(buffer);
#endif


}
void LocalRobotCommunicator::LoadFromXml(xml::XMLElement* e)
{
}

}
}
