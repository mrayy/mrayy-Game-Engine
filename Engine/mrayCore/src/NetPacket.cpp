#include "stdafx.h"

#include "NetPacket.h"
#include "ISocket.h"
#include "NetAddress.h"
#include "CMsgBuffer.h"


namespace mray{
namespace network{
NetPacket::NetPacket(){
/*	exclusive=false;
	toId=0;*/
}
void NetPacket::writeData(CMsgBuffer*buffer){
	buffer->resize(0);
	commands::NTCMD command=getCommand();
	buffer->write(&command,sizeof(command));
}
bool NetPacket::sendPacketData(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate,ESendMode mode){
	if(s->writeBytes(buffer,toAddress,imediate,mode)<=0)return false;
	return true;
}


NetPacket::~NetPacket(){
}

void NetPacket::readPacket(CMsgBuffer*buffer){
	buffer->reset();
	//command been read by socket
}

bool NetPacket::sendPacket(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate){
	return sendPacket(s,buffer,toAddress,imediate,getSendMode());
}

bool NetPacket::sendPacket(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate,ESendMode mode){
	writeData(buffer);
	if(!sendPacketData(s,buffer,toAddress,imediate,mode))return false;

	return true;
}

//////////////////////////
PeerDisconnect::PeerDisconnect(ushort id,const NetAddress& addr){
	this->id=id;
	this->addr=addr;
}

PeerDisconnect::PeerDisconnect(){
}

mray::network::commands::NTCMD PeerDisconnect::getCommand(){
	return commands::NET_CMD_PEER_DISCONNECT;
}
void PeerDisconnect::writeData(mray::network::CMsgBuffer*buffer){
	NetPacket::writeData(buffer);
	buffer->write(&id,sizeof(id));
	buffer->write(&addr,sizeof(addr));
}
void PeerDisconnect::readPacket(mray::network::CMsgBuffer*buffer){
	NetPacket::readPacket(buffer);
	buffer->read(&id,sizeof(id));
	buffer->read(&addr,sizeof(addr));
}
mray::network::ESendMode PeerDisconnect::getSendMode(){
	return mray::network::ESM_RELIABLE_SEQ;
}




ServerInfo::ServerInfo(){
	serverName="";
	hasPassword=0;
	maxClients=connectedClients=0;
}

void ServerInfo::writeData(mray::network::CMsgBuffer*buffer){
	NetPacket::writeData(buffer);
	int len=(serverName.length()+1)*sizeof(mchar);
	buffer->write(&len,sizeof(len));
	buffer->write((void*)serverName.c_str(),len);
	buffer->write(&hasPassword,sizeof(hasPassword));
	buffer->write(&maxClients,sizeof(maxClients));
	buffer->write(&connectedClients,sizeof(connectedClients));
}
void ServerInfo::readPacket(mray::network::CMsgBuffer*buffer){
	NetPacket::readPacket(buffer);
	int len;
	buffer->read(&len,sizeof(len));
	char Buffer[256];
	buffer->read(Buffer,len);
	serverName=Buffer;
	buffer->read(&hasPassword,sizeof(hasPassword));
	buffer->read(&maxClients,sizeof(maxClients));
	buffer->read(&connectedClients,sizeof(connectedClients));
}


commands::NTCMD ServerInfo::getCommand(){
	return commands::NET_CMD_SERVER_FULL_INFO_REPLAY;
}

mray::network::ESendMode ServerInfo::getSendMode(){
	return ESM_NOT_RELIABLE;
}


}
}

