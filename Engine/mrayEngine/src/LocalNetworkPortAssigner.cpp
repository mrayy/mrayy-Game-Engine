
#include "stdafx.h"
#include "LocalNetworkPortAssigner.h"
#include "INetwork.h"
#include "IUDPClient.h"
#include "IReliableSocket.h"
#include "ILogManager.h"


namespace mray
{
namespace network
{

LocalNetworkPortAssigner::LocalNetworkPortAssigner()
{

}
LocalNetworkPortAssigner::~LocalNetworkPortAssigner()
{

}

unsigned short LocalNetworkPortAssigner::RequestPort(const core::string& name, network::EProtoType type)
{
	unsigned short port = -1;
	if (type == network::EPT_UDP)
	{
		network::IUDPClient* c= network::INetwork::getInstance().createUDPClient();
		if (c->Open(0) == network::UDP_SOCKET_ERROR)
		{
			delete c;
			return -1;
		}
		 port = c->Port();
		c->Close();
		delete c;

	}
	else {
		network::IReliableSocket* c= network::INetwork::getInstance().createTCPSocket();
		c->startSocket(0, 5, 5);
		port = c->getAddress()->port;
		c->closeConnection();
		delete c;
	}
	return port;
}
unsigned short LocalNetworkPortAssigner::AssignPort(const core::string& name, network::EProtoType type, unsigned short port)
{
	if (port == 0)
		port = RequestPort(name, type);
	 
	gLogManager.log("LocalNetworkPortAssigner: Port Assigned for [" + name + "] with type [" + (type == network::EPT_TCP ? "TCP" : "UDP") + "] : " + core::StringConverter::toString(port), ELL_INFO);
	return port;
}
bool LocalNetworkPortAssigner::UnassignPort(const core::string& name, network::EProtoType type)
{
	gLogManager.log("LocalNetworkPortAssigner: Port UnassignPort for [" + name + "] with type [" + (type == network::EPT_TCP ? "TCP" : "UDP") +"]", ELL_INFO);
	return true;
}

}
}
