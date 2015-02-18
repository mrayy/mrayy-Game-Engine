#include "stdafx.h"

#include "RemoteDebugClient.h"
#include "INetwork.h"
#include "RDPacketsID.h"
#include "RDWatchPacket.h"
#include "RDWUpdatePacket.h"
#include "RDWRemovePacket.h"
#include "RDLogPacket.h"


namespace mray{
namespace debug{



class RDClientSocketListner:public network::SocketListener
{

	RemoteDebugClient *m_rd;

public:

	RDClientSocketListner(RemoteDebugClient*rd){
		m_rd=rd;
	}

	void onPeerConnected(const network::NetAddress&addr){
	}
	void onPeerDisconnect(const network::NetAddress&addr){
	}
	void onPacketArrived(network::SRecivedMessage*msg){
		m_rd->onMessageRecived(msg);
	}

	void onUnkownPacketArrived(int cmd,network::CMsgBuffer*buffer,const network::NetAddress& address,network::ESendMode mode){
		
	}

};


RemoteDebugClient::RemoteDebugClient(){
	m_listener=0;
	m_socketListener=new RDClientSocketListner(this);
}
RemoteDebugClient::~RemoteDebugClient(){
	if(m_socket)
		m_socket->close();
}

bool RemoteDebugClient::connect(const char*host,ushort port){
	if(!createSocket())
		return false;

	return m_socket->connectToHost(host,port,"");
}

bool RemoteDebugClient::connect(const network::NetAddress&addr){
	if(!createSocket())
		return false;

	return m_socket->connectToHost(addr,"");
}
bool RemoteDebugClient::isConnected(){
	if(!m_socket || !m_socket->isConnected())
		return false;
	return true;
}


bool RemoteDebugClient::createSocket(){
	if(m_socket)
		return true;

	if(!network::INetwork::isExist())
		return false;

	m_socket= network::INetwork::getInstance().createSocket(mT("RDClient"),false,true);

	m_socket->setListener(m_socketListener);

	m_socket->createSocket(0,1,30);

	return true;
}


void RemoteDebugClient::setListner(IRDMessageListener*l){
	m_listener=l;
}

int RemoteDebugClient::getWatchesCount(){
	return m_watches.size();
}

const IRemoteDebugClient::WatchList&  RemoteDebugClient::getWatches(){
	return m_watches;
}


void RemoteDebugClient::onMessageRecived(network::SRecivedMessage*msg){
	switch(msg->command){
	case ERDP_NewWatch:
		{
			RDWatchPacket packet;
			packet.readPacket(&msg->msg);
			onAddWatch(packet.m_id,packet.m_name,packet.m_val,packet.m_group);
		}break;
	case ERDP_UpdateWatch:
		{
			RDWUpdatePacket packet;
			packet.readPacket(&msg->msg);
			onUpdateWatch(packet.m_id,packet.m_val);
		}break;
	case ERDP_RemoveWatch:
		{
			RDWRemovePacket packet;
			packet.readPacket(&msg->msg);
			onRemoveWatch(packet.m_id);
		}break;
	case ERDP_LogMessage:
		{
			RDLogPacket packet;
			packet.readPacket(&msg->msg);
			onAddLog(packet.m_msg);
		}
	}
}

void RemoteDebugClient::onAddWatch(uint id,const core::string&name,
							   const core::string&val,const core::string&group)
{
	if(m_listener){
		m_listener->onAddWatch(id,name,val,group);
	}
}
void RemoteDebugClient::onUpdateWatch(uint id,const core::string&val){
	if(m_listener){
		m_listener->onUpdateWatch(id,val);
	}
}
void RemoteDebugClient::onRemoveWatch(uint id){
	if(m_listener){
		m_listener->onRemoveWatch(id);
	}
}

void RemoteDebugClient::onAddLog(const core::string&msg){
	if(m_listener){
		m_listener->onReciveLog(msg);
	}
}

}
}

