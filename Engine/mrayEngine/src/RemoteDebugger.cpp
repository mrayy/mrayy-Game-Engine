#include "stdafx.h"


#include "RemoteDebugger.h"
#include "INetwork.h"

#include "RDWatchPacket.h"
#include "RDWUpdatePacket.h"
#include "RDWRemovePacket.h"
#include "RDLogPacket.h"

#include "ConnectedPeer.h"


namespace mray{
namespace debug{


class RDSocketListner:public network::SocketListener
{

	RemoteDebugger *m_rd;

public:

	RDSocketListner(RemoteDebugger*rd){
		m_rd=rd;
	}

	void onPeerConnected(const network::NetAddress&addr){
		m_rd->onClientConnected(addr);
	}
	void onPeerDisconnect(const network::NetAddress&addr){
		m_rd->onClientDisconnected(addr);
	}
	void onPacketArrived(network::SRecivedMessage*msg){
		m_rd->onMessageRecived(msg);
	}

	void onUnkownPacketArrived(int cmd,network::CMsgBuffer*buffer,const network::NetAddress& address,network::ESendMode mode){
	}

};


RemoteDebugger::RemoteDebugger(){
	m_port=RDDefaultPort;
	m_enabled=false;

	m_lastID=0;

	m_listener=new RDSocketListner(this);

	createSocket();
}
RemoteDebugger::~RemoteDebugger(){
	stopLog();

	m_watches.clear();
}


bool RemoteDebugger::createSocket(){
	if(m_socket)
		return true;

	if(!network::INetwork::isExist())
		return false;

	m_socket= network::INetwork::getInstance().createSocket(mT("RD"),true,true);

	m_socket->setListener(m_listener);

	return true;
}

void RemoteDebugger::setPort(ushort p){
	m_port=p;
}
ushort RemoteDebugger::getPort(){
	return m_port;
}

bool RemoteDebugger::startLog(){
	if(!m_socket && !createSocket())
		return false;
	if(m_socket->isConnected()){
		m_socket->close();
	}

	m_socket->createSocket(m_port,5,30);
	m_enabled=true;

	return true;
}
void RemoteDebugger::stopLog(){
	if(!m_socket)
		return;
	m_socket->close();
	m_enabled=false;
}
void RemoteDebugger::pauseLog(){
	m_enabled=false;
}

void RemoteDebugger::resumeLog(){
	m_enabled=true;
}

bool RemoteDebugger::isLogEnabled(){
	return m_enabled;
}


void RemoteDebugger::sendWatch(const SWatchInfo&w,const network::NetAddress&addr){
	RDWatchPacket packet;
	packet.m_name=w.watch->getName();
	packet.m_val=w.watch->toString(w.object);
	packet.m_group=w.group;
	packet.m_id=w.id;

	packet.sendPacket(m_socket,&m_msgBuffer,addr,true);

}

void RemoteDebugger::sendWatch(const SWatchInfo&w){
	RDWatchPacket packet;
	packet.m_name=w.watch->getName();
	packet.m_val=w.watch->toString(w.object);
	packet.m_group=w.group;
	packet.m_id=w.id;

	network::ISocket::PeersList::const_iterator p=m_socket->getPeers().begin();
	network::ISocket::PeersList::const_iterator end=m_socket->getPeers().end();

	for(;p!=end;++p)
	{
		packet.sendPacket(m_socket,&m_msgBuffer,(*p)->address,true);
	}

}

void RemoteDebugger::updateWatch(const SWatchInfo&w){
	RDWUpdatePacket packet;
	packet.m_val=w.watch->toString(w.object);
	packet.m_id=w.id;

	network::ISocket::PeersList::const_iterator p=m_socket->getPeers().begin();
	network::ISocket::PeersList::const_iterator end=m_socket->getPeers().end();

	for(;p!=end;++p)
	{
		packet.sendPacket(m_socket,&m_msgBuffer,(*p)->address,true);
	}

}


void RemoteDebugger::removeWatch(const SWatchInfo&w){
	RDWRemovePacket packet;
	packet.m_id=w.id;

	network::ISocket::PeersList::const_iterator p=m_socket->getPeers().begin();
	network::ISocket::PeersList::const_iterator end=m_socket->getPeers().end();

	for(;p!=end;++p)
	{
		packet.sendPacket(m_socket,&m_msgBuffer,(*p)->address,true);
	}

}

void RemoteDebugger::addLogMsg(const core::string&msg){
	RDLogPacket packet;
	packet.m_msg=msg;

	network::ISocket::PeersList::const_iterator p=m_socket->getPeers().begin();
	network::ISocket::PeersList::const_iterator end=m_socket->getPeers().end();

	for(;p!=end;++p)
	{
		packet.sendPacket(m_socket,&m_msgBuffer,(*p)->address,true);
	}

}

void RemoteDebugger::addWatch(CPropertieSet* object,IProperty*prop,const core::string&group,float updateInterval){
	SWatchInfo info;
	info.watch=prop;
	info.group=group;
	info.updateInterval=updateInterval;
	info.id=m_lastID++;
	info.object=object;

	m_watches.push_back(info);
}
void RemoteDebugger::removeWatch(CPropertieSet* object,IProperty*prop){
	WatchList::iterator p=m_watches.begin();
	WatchList::iterator end=m_watches.end();

	for(;p!=end;++p)
	{
		if((*p).watch==prop && (*p).object==object){
			m_watches.erase(p);
			return;
		}
	}
}

void RemoteDebugger::clear(){
	m_watches.clear();
}



void RemoteDebugger::update(float dt){
	if(!m_enabled || !m_socket)
		return;
	WatchList::iterator p=m_watches.begin();
	WatchList::iterator end=m_watches.end();

	for(;p!=end;++p)
	{
		(*p).timer+=dt;
		if((*p).timer>=(*p).updateInterval){
			(*p).timer=0;
			updateWatch(*p);
		}
	}
}


void RemoteDebugger::onClientConnected(const network::NetAddress&addr)
{
	FIRE_LISTENR_METHOD(OnDebugClientConnected,(addr));
	WatchList::iterator p=m_watches.begin();
	WatchList::iterator end=m_watches.end();

	for(;p!=end;++p)
	{
		sendWatch(*p,addr);
	}
}

void RemoteDebugger::onClientDisconnected(const network::NetAddress&addr)
{
	FIRE_LISTENR_METHOD(OnDebugClientDisconnected,(addr));
}

void RemoteDebugger::onMessageRecived(network::SRecivedMessage*msg){
	switch(msg->command){
	case ERDP_UpdateWatch:
		{
			RDWUpdatePacket packet;
			packet.readPacket(&msg->msg);
			SWatchInfo*w=getWatch(packet.m_id);
			if(w){
				w->watch->parse(w->object,packet.m_val);
			}
		}break;
	}
}


RemoteDebugger::SWatchInfo* RemoteDebugger::getWatch(uint id){

	WatchList::iterator p=m_watches.begin();
	WatchList::iterator end=m_watches.end();

	for(;p!=end;++p)
	{
		if((*p).id==id){
			return &(*p);
		}
	}
	return 0;
}

}
}

