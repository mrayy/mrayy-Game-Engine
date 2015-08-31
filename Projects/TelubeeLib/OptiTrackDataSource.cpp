

#include "stdafx.h"
#include "OptiTrackDataSource.h"
#include "OptiTrackClient.h"

#include <winsock2.h>		// for struct in_addr


namespace mray
{
namespace TBee
{

	class OptiTrackDataSourceListener:public animation::IOptiTrackClientListener
	{
		OptiTrackDataSource* m_owner;
	public:
		OptiTrackDataSourceListener(OptiTrackDataSource* o)
		{
			m_owner=o;
		}
		virtual void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body)
		{
			m_owner->DataReceived(&body);
		}

	};

OptiTrackDataSource::OptiTrackDataSource()
{
	m_listener=new OptiTrackDataSourceListener(this);
	m_tracker=new animation::OptiTrackClient();
	m_tracker->AddListener(m_listener);
}
OptiTrackDataSource::~OptiTrackDataSource()
{
	Disconnect();
	delete m_tracker;
	delete m_listener;
}

void OptiTrackDataSource::Connect(const core::string& serverIP,const core::string& localIP)
{
	if (IsConnected())
		return;
	core::string ip = localIP;
	if (ip == "")
	{

		in_addr ipaddress;

		char ac[80];
		if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
			printf("Error when getting local host name \n");
			return;
		}

		struct hostent *phe = gethostbyname(ac);
		if (phe == 0) {
			printf("Bad host lookup.\n");
			return;
		}

		for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
			struct in_addr addr;
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			ipaddress = addr;
		}
		ip = inet_ntoa(ipaddress);
		printf("OptiTrack local IP address: %s\n", ip.c_str());

	}
	printf("Trying to connect to optiTrack server: %s\n", serverIP.c_str());

	//TODO: Make multicast interface as a parameter!!
	if (!m_tracker->Connect(animation::Opti_Unicast, serverIP, ip, "239.255.42.99"))
		printf("Failed to connect to optiTrack!\n");
	else 
		printf("Successfully connected to optiTrack!\n");
}
void OptiTrackDataSource::Disconnect()
{
	m_tracker->Disconnect();
}
bool OptiTrackDataSource::IsConnected()
{
	return m_tracker->IsConnected();
}

void OptiTrackDataSource::DataReceived(const animation::OptiTrackRigidBody* body)
{
	BodyInfo b;
	b.id = body->GetID();
	b.name = body->GetName();
	b.position = body->GetPosition();
	b.orintation = body->GetOrintation();
	m_bodies[body->GetID()] = b;
}

math::vector3d* OptiTrackDataSource::GetPositionByID(int id)
{

	OptiMap::iterator it = m_bodies.find(id);
	if (it == m_bodies.end())
		return 0;
	return &it->second.position;
}
math::quaternion* OptiTrackDataSource::GetOrientationByID(int id)
{
	OptiMap::iterator it = m_bodies.find(id);
	if (it == m_bodies.end())
		return 0;
	return &it->second.orintation;
}
void OptiTrackDataSource::ConnectLocal()
{
	if (IsConnected())
		return;
	in_addr ipaddress;

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("Error when getting local host name \n");
		return;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		printf("Bad host lookup.\n");
		return;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ipaddress = addr;
	}
	core::string ipaddr = inet_ntoa(ipaddress);

	Connect(ipaddr, ipaddr);
}
}
}

