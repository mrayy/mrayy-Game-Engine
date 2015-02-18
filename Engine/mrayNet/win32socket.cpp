
#include "stdafx.h"

#include "Win32Socket.h"
#include "Win32Network.h"
#include "SendPacketQueue.h"
#include "CMsgBuffer.h"
#include "Win32TCPSocket.h"
#include "NetPacket.h"
#include "HMessages.h"
#include <ITimer.h>
#include <TraceManager.h>
#include <IThreadManager.h>
#include <MutexLocks.h>

#include <IThread.h>
#include <Engine.h>
#include <IThreadFunction.h>
#include <IOSystem.h>

//#include <IThreadFunction.h>

namespace mray{
namespace network{



class Win32SocketProcessThread:public OS::IThreadFunction
{
	Win32Socket*m_socket;
public:
	Win32SocketProcessThread(Win32Socket*s){
		m_socket=s;
//		m_stopThread=0;
	}

//	bool m_stopThread;

protected:
	virtual void setup(){
	}
	virtual void execute(OS::IThread*caller,void*arg){
		while(caller->isActive()){
			m_socket->update();

			if(m_socket->m_sleepInterval>0){
				WSAWaitForMultipleEvents(1,&m_socket->m_reciveEvent,0,m_socket->m_sleepInterval,0);
			}
		}
	}
};



Win32Socket::Win32Socket(const core::string&name,bool isHost,bool ReliableInterface)
{
	traceFunction(eNetwork);
	setLocalName(name);

	m_isHost=isHost;

	m_closed=true;

	m_listener=0;

	//1 second timeout
	m_timeout=1000;

	m_droppedPackets=0;

	m_pingInterval=10000; // 10 seconds to ping clients

	m_sleepInterval=30;

	m_bytesRead=m_bytesSent=0;
	haveSocket=0;

	m_password[0]=0;
	m_passwordLen=0;

	m_maxPeers=5;

	m_maxRequest=10;

	m_sendTime=50;

	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();
	m_socket=net->createSocket(EPT_UDP);

	m_recMsgMutex=OS::IThreadManager::getInstance().createMutex();

	m_timeToRemovePacket=1000;	//one second to remove the packet
	m_sendQueue=new SendPacketQueue(this);
	
	if(ReliableInterface){
		m_tcpSocket=new Win32TCPSocket(this);
	}
	else 
		m_tcpSocket=0;

	m_threadFunc=new Win32SocketProcessThread(this);

	
}
Win32Socket::~Win32Socket(){
	close();

	{
		MessageList::iterator it=m_messagesGraveyard.begin();
		for (;it!=m_messagesGraveyard.end();++it)
		{
			delete *it;
		}
		it=m_recivedMessages.begin();
		for (;it!=m_recivedMessages.end();++it)
		{
			delete *it;
		}
		m_messagesGraveyard.clear();
		m_recivedMessages.clear();
	}

	if(m_tcpSocket)
		delete m_tcpSocket;
	if(m_threadFunc)
		delete m_threadFunc;


	if(m_thread)
	{
		OS::IThreadManager::getInstance().killThread(m_thread);
		delete m_thread;
	}

	
	delete m_sendQueue;
}

void Win32Socket::close(){

	traceFunction(eNetwork);

	if(!INetwork::isExist())
		return;

	m_recivedMessages.clear();

	m_closed=true;

	sendDisconnect();
	if(m_thread){
//		m_thread->m_stopThread=1;
		m_thread->terminate();
	}
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();
	if(m_tcpSocket)
		m_tcpSocket->stopSocket();
	Sleep(10+m_sleepInterval);
	if(haveSocket){
		net->closeSocket(m_socket);
		haveSocket=0;
	}

	
}

SRecivedMessage* Win32Socket::peekMessageFromGraveyard(){
	MessageList::iterator it=m_messagesGraveyard.begin();
	if(it!=m_messagesGraveyard.end()){
		SRecivedMessage* ret=*it;
		m_messagesGraveyard.erase(it);
		ret->msg.resize(0);
		ret->m_remove=0;
		ret->m_inUse=1;
		return ret;
	}
	return 0;
}
void Win32Socket::addMessageToGraveyard(SRecivedMessage* msg){
	if(!msg)return;
	m_messagesGraveyard.push_back(msg);
//	msg->drop();
}


NetID Win32Socket::getFreeID(){
	for(NetID i=0;i<m_usedID.getBitsUsed();++i){
		if(!m_usedID[i])
			return i;
	}
	return -1;
}
void Win32Socket::freeID(NetID id){
	m_usedID.set(id,0);
}
void Win32Socket::setUsedID(NetID id){
	m_usedID.set(id,1);
}
bool Win32Socket::isConnected(){
	return m_connectedPeers.size()>0;
}

void Win32Socket::setListener(SocketListener* l){
	m_listener=l;
}

void Win32Socket::createSocket(int port,int maxPeers,int sleepInterval){

	traceFunction(eNetwork);
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();

	if(m_tcpSocket){
		m_tcpSocket->startSocket(port,maxPeers,sleepInterval);
	//	m_socket=net->openUDPSocket("",0);
	}//else
		

	if(m_socket==INVALID_SOCKET){
		
		return;
	}

	net->attachUDPSocket(m_socket,"",port);
	//set local address
	m_udpAddr=net->getSockAddr(m_socket);

	haveSocket=1;
	int sockBuff=1*1024; // double throughput rate
	net->setSocketOption(m_socket,ESocketOption::ESO_SOCKOPT_RCVBUF,1);

	sockBuff=1024*16; // 
	net->setSocketOption(m_socket,ESocketOption::ESO_SOCKOPT_SNDBUF,1);

	//let the socket be nonblocking
	net->setSocketOption(m_socket,ESocketOption::ESO_NONBLOCK,1);

	//make it support broadcasting
	net->setSocketOption(m_socket,ESocketOption::ESO_SOCKOPT_BROADCAST,1);

	net->enableBroadcast(m_socket,true);

	m_reciveEvent=CreateEvent(0,0,0,0);
	WSAEventSelect(m_socket,m_reciveEvent,FD_READ);


	m_usedID.resize(maxPeers+1);
	m_usedID.clear(0);

	m_maxPeers=maxPeers;

	m_sleepInterval=sleepInterval;

	m_thread=OS::IThreadManager::getInstance().createThread(m_threadFunc);

	if(m_thread){
		m_thread->start(0);
		//wait tell thread start;
		while(m_thread->isActive()==false)
			Sleep(10);
	}

	m_closed=false;
}

bool Win32Socket::connectToHost(const char*host,int port,const char*password){
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();
	NetAddress addr;
	if(net->getHostAddress(host,addr)==false)
		return false;
	addr.port=port;
	return connectToHost(addr,password);
	
}
bool Win32Socket::connectToHost(const NetAddress&addr,const char*password){
	
	if(getPeer(addr)!=0)
		return false;		//already connected

	sendConnectRequest(addr,password);

	return true;
}
bool Win32Socket::addRequest(SRequestPeer*request){
	if(hasRequest(request->address,request->request))return false;

	request->requestTime = (int)gEngine.getTimer()->getMilliseconds();
	request->requestCount=0;
	m_requestPeers.push_back(*request);
	return true;
}

bool Win32Socket::hasRequest(const NetAddress&addr,SRequestPeer::ERequestType request){
	std::list<SRequestPeer>::iterator it=m_requestPeers.begin();
	for(;it!=m_requestPeers.end();++it){
		if((*it).address==addr && (*it).request==request)
			return true;
	}
	return false;
}

void Win32Socket::sendConnectRequest(const NetAddress&addr,const char*password){
	if(hasRequest(addr,SRequestPeer::ERT_CONNECT))
		return;

	SRequestPeer request;
	request.setPassword(password,strlen(password));
	request.address=addr;
	request.request=SRequestPeer::ERT_CONNECT;
	request.requestCount=0;
	request.requestTime = (int)gEngine.getTimer()->getMilliseconds();
	
	m_requestPeers.push_back(request);

	processReques(&request);

}

void Win32Socket::sendDisconnect(){
	m_tempBuffer.resize(0);
	m_tempBuffer.write((void*)&commands::NET_CMD_DISCONNECT,
		sizeof(commands::NET_CMD_DISCONNECT));

	sendMessageToPeers(m_tempBuffer.pointer(),m_tempBuffer.size(),1,ESM_RELIABLE);

	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		delete *it;
	}
	m_connectedPeers.clear();
}

void Win32Socket::sendInfo(const NetAddress&addr,const char*password,byte passlen){
	m_tempBuffer.resize(0);
	m_tempBuffer.write((void*)&commands::NET_CMD_PEERINFO_REPLAY,
		sizeof(commands::NET_CMD_PEERINFO_REPLAY));
	byte len=m_localName.length()+1;
	m_tempBuffer.write(&len,sizeof(len));
	m_tempBuffer.write((void*)m_localName.c_str(),len*sizeof(char));
	
	m_tempBuffer.write(&passlen,sizeof(passlen));
	m_tempBuffer.write((void*)password,passlen*sizeof(char));

	ulong ptime = gEngine.getTimer()->getMilliseconds();
	m_tempBuffer.write(&ptime,sizeof(ptime));
	
	m_tempBuffer.write(&m_udpAddr.port,sizeof(m_udpAddr.port));

	writeBytes(&m_tempBuffer,addr,1,ESM_RELIABLE);
}

void Win32Socket::sendMessageToPeers(void*data,int size,bool immediate,ESendMode mode){

	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		if((*it)->removed)continue;
		writeBytes(data,size,(*it)->address,immediate,mode);
	}
}

void Win32Socket::setReadTimeout(int timeMS){
	m_timeout=timeMS;
}
int Win32Socket::getTimeout(){
	return m_timeout;
}
int Win32Socket::getLastMsg(){
	return m_lastMsg;
}


void Win32Socket::setPassword(const char*pass,int len){
	if(len>64)
		len=64;
	if(len<0)
	{
		m_password[0]=0;
		m_passwordLen=0;
		return;
	}
	mraySystem::memCopy(m_password,pass,len);
	m_passwordLen=len;

}
const char* Win32Socket::getPassword(){
	return m_password;
}


void Win32Socket::addToBannd(NetAddress&addr){
	if(isBanned(addr))return;
	m_bannedAddress.push_back(addr);
}
bool Win32Socket::isBanned(const NetAddress&addr){
	std::list<NetAddress>::iterator it=m_bannedAddress.begin();
	for (;it!=m_bannedAddress.end();++it)
	{
		if(addr.address==(*it).address)
			return true;
	}
	return false;
}

const NetAddress*Win32Socket::getUDPAddress(){
	return &m_udpAddr;
}
const NetAddress*Win32Socket::getTCPAddress(){
	if(m_tcpSocket)
		return m_tcpSocket->getAddress();
	else return 0;
}

int Win32Socket::bytesRead(){
	return m_bytesRead;
}

int Win32Socket::bytesSent(){
	return m_bytesSent;
}


void Win32Socket::resetBytesRead(){
	m_bytesRead=0;
}

void Win32Socket::resetBytesSent(){
	m_bytesSent=0;
}


void Win32Socket::setLocalName(const core::string&name){
	m_localName=name;
	if(m_localName.length()>255){
		m_localName=m_localName.substr(0,255);
	}
}
const core::string& Win32Socket::getLocalName(){
	return m_localName;
}



void Win32Socket::setSocket(SOCKET s){
	m_socket=s;
	if(m_socket!=INVALID_SOCKET)
		haveSocket=true;
	else haveSocket=false;
}

bool Win32Socket::isHasSocket(){
	return haveSocket;
}


SOCKET Win32Socket::getSocket(){
	return m_socket;
}

CMsgBuffer*Win32Socket::initAndGetBuffer(){
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();
	int syn=net->getSyncNumber();

	msgBuffer.reset();
	//write Synchronize number
	msgBuffer.write(&syn,sizeof(syn));

	return &msgBuffer;
}

CMsgBuffer*Win32Socket::getBuffer() {
	return &msgBuffer;
}
int Win32Socket::readBytes(void*data,int maxLen,NetAddress * fromAddress)
{
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();

	int wres=net->wait(m_socket,m_timeout,(int)ESW_RECEIVE);
	if(wres==(int)ESW_NONE){
		m_lastMsg=messages::HMSG_TIMEOUT;
		return 0;
	}

	int rec=net->inner_receivefrom(m_socket,data,maxLen,fromAddress);
	if(rec>0)
		m_bytesRead+=rec;
	return rec;
}
int Win32Socket::readBytes(CMsgBuffer*buffer,NetAddress * fromAddress){
	
	Win32Network*net=(Win32Network*)INetwork::getInstancePtr();

	int wres=net->wait(m_socket,m_timeout,(int)ESW_RECEIVE);
	if(wres==(int)ESW_NONE){
		m_lastMsg=messages::HMSG_TIMEOUT;
		return 0;
	}

	int rec=net->inner_receivefrom(m_socket,buffer,fromAddress);
	if(rec>0)
		m_bytesRead+=rec;
/*
	uint vsize=0;
	int rec=0;

	rec=net->receive(this,(char*)&vsize,sizeof(vsize));
	if(rec<0){
	return -1;
	}
	if(rec==0){
		return 0;
	}

	buffer->resize(vsize);
	rec=readBytes(buffer->pointer(),vsize);
	*/
	return rec;
}

int Win32Socket::writeBytes(const void*data,uint size,const NetAddress & toAddress,bool immediate,ESendMode mode){

	ConnectedPeer*peer=getPeer(toAddress);
	if(peer){
		NetAddress addr;
		if(mode==ESM_RELIABLE || mode==ESM_RELIABLE_SEQ)addr=peer->relAddress;
		else addr=peer->address;

		peer->m_sendQueue->addPacket(data,size,addr,immediate,mode);
	}else
		m_sendQueue->addPacket(data,size,toAddress,immediate,mode);
	
	m_bytesSent+=size;
/*
	int target=size;
	char*p=(char*)(data);


	Win32Network*net=(Win32Network*)INetworkgetInstance();

//	int sync=net->getSyncNumber();

	int res=net->send(m_socket,p,size,toAddress);

	if(res<0){
		return -1;
	}
	if(res==0){
		return 0;
	}
/*
	while(1){
		rec=net->send(this,p,target);
		if(rec<0){
			return -1;
		}
		if(rec==0){
			return 0;
		}
		target-=rec;
		if(target<=0)break;
		p+=rec;
	}*/
/*	m_bytesSent+=res;

	return size;*/
	return 1;
}

int Win32Socket::writeBytes(CMsgBuffer*buffer,const NetAddress & toAddress,bool immediate,ESendMode mode){
	return writeBytes(buffer->pointer(),buffer->size(),toAddress,immediate,mode);
}


ConnectedPeer* Win32Socket::addNewPeer(const NetAddress&addr,EPeerConnectState state,bool local){
	
	//check for max peers
	if(m_connectedPeers.size()>=m_maxPeers)
		return 0;

	int id=0;
	if(!local){
		id=getFreeID();
		if(id==-1)return 0;
	}

	ConnectedPeer* peer=new ConnectedPeer(this);
	if(!local){
		peer->id=id;
		setUsedID(id);
	}
	peer->address=addr;
	peer->state=state;
	peer->lastPingTime=peer->lastMsgTime=peer->lastPongTime=peer->connectionTime=(int)gEngine.getTimer()->getMilliseconds();
	peer->nextPingTime=peer->lastPingTime+m_pingInterval;
	m_connectedPeers.push_back(peer);
	//PeersList::iterator it=m_connectedPeers.end();--it;

	return peer;
	
}
ConnectedPeer*Win32Socket::getPeer(const NetAddress&addr){
	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		if((*it)->address==addr || (*it)->relAddress==addr ){
		/*	if((*it)->removed)
				return 0;*/
			return (*it);
		}
	}
	return 0;
}

int Win32Socket::getPeersCount(){
	return m_connectedPeers.size();
}


const ISocket::PeersList& Win32Socket::getPeers(){
	return m_connectedPeers;
}

NetID Win32Socket::getPeerID(const NetAddress&addr){
	ConnectedPeer*peer=getPeer(addr);
	if(!peer)return -1;
	return peer->id;
}
NetID Win32Socket::getPeerIDByName(const core::string&name){
	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		if((*it)->m_socket->getLocalName().equals_ignore_case(name)){
		/*	if((*it)->removed)
				return 0;*/
			return (*it)->id;
		}
	}
	return -1;
}
ConnectedPeer* Win32Socket::getPeer(NetID ID){
	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		if((*it)->id==ID){
		/*	if((*it)->removed)
				return 0;*/
			return (*it);
		}
	}
	return 0;
}
const NetAddress* Win32Socket::getPeerAddr(NetID ID){
	ConnectedPeer*peer=getPeer(ID);
	if(!peer)return 0;
	return &peer->address;
}

SRequestPeer *Win32Socket::getRequest(SRequestPeer::ERequestType type,const NetAddress&addr){
	std::list<SRequestPeer>::iterator it=m_requestPeers.begin();
	for (;it!=m_requestPeers.end();++it)
	{
		if((*it).address==addr && (*it).request==type){
			return &(*it);
		}
	}
	return 0;
}
bool Win32Socket::removePeer(const NetAddress&addr){
	PeersList::iterator it=m_connectedPeers.begin();
	for (;it!=m_connectedPeers.end();++it)
	{
		if((*it)->address==addr){
			freeID((*it)->id);
			//m_connectedPeers.remove(it);
			(*it)->removed=true;
			return 1;
		}
	}
	if(m_listener)
		m_listener->onPeerDisconnect(addr);
	return 0;
}

bool Win32Socket::packetArrived(SRecivedMessage*msg){
//	msg->setInuse(false);
	if(m_listener){
		m_listener->onPacketArrived(msg);
	}
	return false;
}


void Win32Socket::kickPeer(const NetAddress&addr,ushort id){
	m_tempBuffer.resize(0);
	static PeerDisconnect pd;
	pd.addr=addr;
	pd.id=id;
	/*
	m_tempBuffer.write((void*)&commands::NET_CMD_PEER_DISCONNECT,sizeof(commands::NET_CMD_PEER_DISCONNECT));
	m_tempBuffer.write((void*)addr,sizeof(Address));
	m_tempBuffer.write((void*)&id,sizeof(id));*/
	pd.writeData(&m_tempBuffer);

	sendMessageToPeers(m_tempBuffer.pointer(),m_tempBuffer.size(),1,ESM_RELIABLE);
}

bool Win32Socket::parsePacket(const NetAddress&addr,CMsgBuffer*msg){
	traceFunction(eNetwork);

	if(m_closed)
		return false;

	static char tempString[255];
	//don't accept packets from banned address
	if(isBanned(addr)){
		//replay with banned message
		commands::NTCMD replay=commands::NET_CMD_BANNED;
		writeBytes(&replay,sizeof(replay),addr,1,ESM_RELIABLE);
		
		return false;
	}
	ConnectedPeer *peer=getPeer(addr);

	m_tempBuffer.resize(0);

	ulong currTime = gEngine.getTimer()->getMilliseconds();


	//first strip out control data from message 
	bool notDrop=0;
	if(peer){
		notDrop=peer->m_sendQueue->toPacket(msg,m_arrivedPacket);
		peer->lastMsgTime=currTime;
	}
	else{ 
		m_sendQueue->toPacket(msg,m_arrivedPacket);
		notDrop=1;
	}
	m_arrivedPacket.data.reset();
	commands::NTCMD cmd;
	m_arrivedPacket.data.read(&cmd,sizeof(cmd));



	//only accept connect and ping and some requests from unknown users
	if(!peer && cmd!=commands::NET_CMD_CONNECT_REQUEST && cmd!=commands::NET_CMD_PING &&
		cmd!=commands::NET_CMD_PONG && cmd!=commands::NET_CMD_PEERINFO_REPLAY
		&& cmd!=commands::NET_CMD_CAN_CONNECT_REQ && cmd!=commands::NET_CMD_CAN_CONNECT_ANS
		&& cmd!=commands::NET_CMD_CAN_CONNECT_FAILED && cmd!=commands::NET_CMD_SERVER_FULL_INFO_REQ
		 && cmd!=commands::NET_CMD_SERVER_FULL_INFO_REPLAY)
	{
		bool tcpP=0;
 		if(m_tcpSocket && m_tcpSocket->peerConnected(addr))
			tcpP=1;
		if( !tcpP && !getRequest(SRequestPeer::ERT_CONNECT,addr)
			&& !getRequest(SRequestPeer::ERT_CAN_CONNECT,addr)){
				
				return false;
		}
	}
/*
	if(m_arrivedPacket.isReliable()){
		m_tempBuffer.resize(0);
		m_tempBuffer.write((void*)&commands::NET_CMD_REPLAY,sizeof(commands::NET_CMD_REPLAY));
		m_tempBuffer.write((void*)&m_arrivedPacket.packetID,sizeof(m_arrivedPacket.packetID));;
		writeBytes(&m_tempBuffer,addr,1,ESM_NOT_RELIABLE);
	}*/
	if(peer){
		peer->lastMsgTime=currTime;
	}
	if(!notDrop){
		//drop the packet
		m_droppedPackets++;
		
		return false;
	}
	bool storePacket=true;


	switch(cmd){
	case commands::NET_CMD_CONNECT_REQUEST:
		{
		if(!peer){
			commands::NTCMD replay;
			if(m_connectedPeers.size()<m_maxPeers){
			//	replay=commands::NET_CMD_CONNECT_ACCEPT;
				replay=commands::NET_CMD_PEERINFO;
				m_tempBuffer.resize(0);
				m_tempBuffer.write(&replay,sizeof(replay));
				bool passReq=0;
				if(m_passwordLen>0)
					passReq=1;
				m_tempBuffer.write(&passReq,sizeof(passReq));

			}else{
				//max peers reached
				replay=commands::NET_CMD_MAXCLIENTSREACH;
				m_tempBuffer.resize(0);
				m_tempBuffer.write(&replay,sizeof(replay));
			}
			writeBytes(&m_tempBuffer,addr,1,ESM_RELIABLE);

		}else
		{

		}
		}
		break;
	case commands::NET_CMD_SERVER_FULL_INFO_REQ:
		{
			ServerInfo info;
			info.serverName=m_localName;
			info.hasPassword=m_passwordLen>0;
			info.connectedClients=m_connectedPeers.size();
			info.maxClients=m_maxPeers;
			info.sendPacket(this,&m_tempBuffer,addr,1,ESendMode::ESM_NOT_RELIABLE);
		}break;
	case commands::NET_CMD_CAN_CONNECT_REQ:
		{
			commands::NTCMD replay;
			if(m_connectedPeers.size()<m_maxPeers){
				
				byte len=0;
				m_arrivedPacket.data.read(&len,sizeof(len));
				m_arrivedPacket.data.read(tempString,len);

				if(m_passwordLen>0 && len!=m_passwordLen){
					replay=commands::NET_CMD_INVALID_PASSWORD;
				}else{
					m_arrivedPacket.data.read(tempString,len);
					if(memcmp(m_password,tempString,m_passwordLen)!=0){
						replay=commands::NET_CMD_INVALID_PASSWORD;
					}else
						replay=commands::NET_CMD_CAN_CONNECT_ANS;
				}
			}else
				replay=commands::NET_CMD_MAXCLIENTSREACH;


			CMsgBuffer buffer;
			buffer.write((void*)&replay,sizeof(replay));
			writeBytes(&buffer,addr,1,ESM_NOT_RELIABLE);
		}break;
	case commands::NET_CMD_CAN_CONNECT_ANS:
		{
			removeRequest(SRequestPeer::ERT_CAN_CONNECT,addr);
		}break;
	case commands::NET_CMD_CONNECT_ACCEPT:
		{
			NetAddress remoteAddr;
			NetID id;
			remoteAddr.address=addr.address;
			m_arrivedPacket.data.read(&id,sizeof(id));
			m_arrivedPacket.data.read(&remoteAddr.port,sizeof(remoteAddr.port));
			ConnectedPeer*cpeer= addNewPeer(remoteAddr,EPeerConnectState::EPCS_CONNECTED,false);
			if(!cpeer)break;
			long ptime;
			m_arrivedPacket.data.read(&ptime,sizeof(ptime));
			cpeer->id=id;
			cpeer->relAddress = addr;
			cpeer->peerStartTime = ptime;
			CMsgBuffer buffer;
			buffer.write((void*)&commands::NET_CMD_CONNECTED,sizeof(commands::NET_CMD_CONNECTED));
			writeBytes(&buffer,addr,1,ESM_RELIABLE);
		}
	case commands::NET_CMD_INVALID_PASSWORD:
	case commands::NET_CMD_MAXCLIENTSREACH:
		removeRequest(SRequestPeer::ERT_CONNECT,addr);
		removeRequest(SRequestPeer::ERT_CAN_CONNECT,addr);
		break;
	case commands::NET_CMD_PING:
		{
		CMsgBuffer buffer;
		ConnectedPeer*peer=getPeer(addr);
		if(peer){
		//	peer->lastPingTime=currTime;
		}
		buffer.write((void*)&commands::NET_CMD_PONG,sizeof(commands::NET_CMD_PONG));
		buffer.write(&currTime,sizeof(currTime));
		writeBytes(&buffer,addr,1,ESM_NOT_RELIABLE);
		}
		break;
	case commands::NET_CMD_PONG:
		{
			if(peer){
				uint ptime;
				m_arrivedPacket.data.read(&ptime,sizeof(ptime));
				peer->pingTime=(ptime-peer->peerStartTime+peer->localStartTime)-peer->lastPingTime;
				peer->lastPongTime=currTime;
			}
		}break;
	case commands::NET_CMD_PEERINFO:
		{
			SRequestPeer*req= getRequest(SRequestPeer::ERT_CONNECT,addr);
			if(!req)
				break;
			req->requestTime+=m_timeout;
			bool passwordRequired=0;
			m_arrivedPacket.data.read(&passwordRequired,sizeof(passwordRequired));
			sendInfo(addr,passwordRequired ? req->password : "",passwordRequired ? req->passLen:0);

		}break;
	case commands::NET_CMD_PEERINFO_REPLAY:
		{
			static char tmpPass[255];
			byte len=0;
			commands::NTCMD replay;
			m_arrivedPacket.data.read(&len,sizeof(len));//length of peer name
			m_arrivedPacket.data.read(tempString,len);

			m_arrivedPacket.data.read(&len,sizeof(len));//length of peer password

			uint peerID=0;
			bool isReliable=false;

			bool connected=false;
			if(m_passwordLen>0 && len!=m_passwordLen){
				replay=commands::NET_CMD_INVALID_PASSWORD;
			}else{
				m_arrivedPacket.data.read(tmpPass,len);
				if(m_connectedPeers.size()>=m_maxPeers){
					replay=commands::NET_CMD_MAXCLIENTSREACH;
				}else
				if(memcmp(m_password,tmpPass,m_passwordLen)!=0){
					replay=commands::NET_CMD_INVALID_PASSWORD;
				}else{
					ulong ptime;
					m_arrivedPacket.data.read(&ptime,sizeof(ptime));
					NetAddress remoteAddr;
					remoteAddr.address=addr.address;
					m_arrivedPacket.data.read(&remoteAddr.port,sizeof(remoteAddr.port));
					ConnectedPeer*cpeer=addNewPeer(remoteAddr,EPeerConnectState::EPCS_REQ_CONNECT,false);
					connected=true;
					if(!cpeer){
						replay=commands::NET_CMD_MAXCLIENTSREACH;
					}else{
						replay=commands::NET_CMD_CONNECT_ACCEPT;
						cpeer->relAddress=addr;
						cpeer->clientName=tempString;
						peerID=cpeer->id;
						cpeer->peerStartTime=ptime;
						cpeer->localStartTime=currTime;
						isReliable=1;
					}
				}
			}
			m_tempBuffer.resize(0);
			m_tempBuffer.write(&replay,sizeof(replay));
			if(replay==commands::NET_CMD_CONNECT_ACCEPT){
				m_tempBuffer.write(&peerID,sizeof(peerID));
				//write udp port to connect with
				m_tempBuffer.write(&m_udpAddr.port,sizeof(m_udpAddr.port));
				m_tempBuffer.write(&currTime,sizeof(currTime));
				
			}
			writeBytes(&m_tempBuffer,addr,1,isReliable ? ESM_RELIABLE:ESM_NOT_RELIABLE);

			if(connected){
				if(m_listener)
					m_listener->onPeerConnected(addr);
			}
		}break;
		/*
	case commands::NET_CMD_REPLAY:
		{
		ulong seq;
		m_arrivedPacket.data.read(&seq,sizeof(seq));
		bool res=0;
		if(peer)
			res=peer->m_sendQueue->gotReplay(seq);
		
		if(!res)
			res=m_sendQueue->gotReplay(seq);

		if(res){
			m_tempBuffer.resize(0);
			m_tempBuffer.write((void*)&commands::NET_CMD_REMOVE_SEQ,sizeof(commands::NET_CMD_REMOVE_SEQ));
			m_tempBuffer.write((void*)&m_arrivedPacket.packetID,sizeof(m_arrivedPacket.packetID));
			writeBytes(&m_tempBuffer,addr,1,ESM_NOT_RELIABLE);
		}
		storePacket=false;
		}
		break;*/
	case commands::NET_CMD_REMOVE_SEQ:
		{
		ulong seq;
		m_arrivedPacket.data.read(&seq,sizeof(seq));
		bool res=0;
		if(peer)
			res=peer->m_sendQueue->gotReplay(seq);
		
		if(!res)
			res=m_sendQueue->gotReplay(seq);
		}
		break;
	case commands::NET_CMD_DISCONNECT:
		{
			ushort id=getPeerID(addr);
			removePeer(addr);
			if(m_isHost){
				kickPeer(addr,id);
			}
		}
		break;
	case commands::NET_CMD_PEER_DISCONNECT:{
		PeerDisconnect pd;
		pd.readPacket(&m_arrivedPacket.data);
	//	m_arrivedPacket.data.read(&paddr,sizeof(paddr));
		removePeer(pd.addr);
		}break;

	case commands::NET_CMD_CONNECTED:
	case commands::NET_CMD_REPLAY:
		break;
	default:
		if(m_listener){
			m_listener->onUnkownPacketArrived(cmd,&m_arrivedPacket.data,m_arrivedPacket.address,m_arrivedPacket.mode);
		}
	};
	
	 

	if(storePacket)
	{
		//add the message to the offline list
		SRecivedMessage*rmsg=peekMessageFromGraveyard();
		if(!rmsg)
			rmsg=new SRecivedMessage();
		rmsg->addr=addr;
		rmsg->command=cmd;
		rmsg->msg.resize(msg->getSize()-sizeof(cmd));
		mraySystem::memCopy(rmsg->msg.pointer(),m_arrivedPacket.data.pointer()+sizeof(cmd),m_arrivedPacket.data.getSize()-sizeof(cmd));
		rmsg->timeToRemove=currTime+m_timeToRemovePacket;

//		rmsg->m_inUse=true;
		if(!packetArrived(rmsg)){
			OS::ScopedLock locker(m_recMsgMutex);
			m_recivedMessages.push_back(rmsg);
			
		}
	}
	
	
	return true;
}

void Win32Socket::processRecivedMessages(){

	if(!m_thread->isActive())return;
	{
		OS::ScopedLock locker(m_recMsgMutex);
		int currTime = (int)gEngine.getTimer()->getMilliseconds();
		MessageList::iterator it=m_recivedMessages.begin();
		MessageList::iterator it2;
		bool remove=0;
		for (;it!=m_recivedMessages.end();)
		{
			remove=0;
			if(((*it)->timeToRemove<=currTime || (*it)->m_remove)){// && !(*it)->m_inUse ){
				addMessageToGraveyard(*it);
				remove=1;
			}

			if(remove){
				it2=it;
				++it2;
				m_recivedMessages.erase(it);
				it=it2;
			}else
				++it;
		}
	}


}
bool Win32Socket::processReques(SRequestPeer*request){
	switch(request->request){
	case SRequestPeer::ERT_CONNECT:
		{
			if(m_tcpSocket)
				if(!m_tcpSocket->connect(request->address))
					return true;
			commands::NTCMD msg=commands::NET_CMD_CONNECT_REQUEST;
			writeBytes(&msg,sizeof(msg),request->address,1,ESM_RELIABLE);
			return true;
		}
	case SRequestPeer::ERT_CAN_CONNECT:
		{
			CMsgBuffer buffer;
			buffer.resize(0);
			ESendMode mode;
			if(m_tcpSocket)mode=ESM_RELIABLE;
			else mode=ESM_NOT_RELIABLE;
			commands::NTCMD msg=commands::NET_CMD_CAN_CONNECT_REQ;
			buffer.write(&msg,sizeof(msg));
			buffer.write(&request->passLen,sizeof(request->passLen));
			buffer.write(&request->password,request->passLen);

			writeBytes(&buffer,request->address,1,ESM_RELIABLE);
			return true;
		}
	}
	return false;
}
void Win32Socket::processRequestList(){
	if(!m_thread->isActive())return;

	std::list<SRequestPeer>::iterator it=m_requestPeers.begin();
	std::list<SRequestPeer>::iterator it2;
	bool remove=0;
	for (;it!=m_requestPeers.end();)
	{
		remove=0;
		int currTime = (int)gEngine.getTimer()->getMilliseconds();
		if((*it).requestCount<m_maxRequest){
			if((*it).requestTime<currTime ){
				if(processReques(&(*it))){
					(*it).requestTime=currTime+m_timeout;	//next request time
					(*it).requestCount++;
				}else
					remove =1;
			}
		}else {
			remove=1;
		}
		if(remove){

			switch((*it).request){
				case SRequestPeer::ERT_CONNECT:
					{
					//maximum request reached
					//add an offline message indicates that the request failed
					GCPtr<SRecivedMessage>msg=peekMessageFromGraveyard();
					if(!msg)
						msg=new SRecivedMessage();
					msg->command=commands::NET_CMD_CONNECT_FAILED;
					msg->addr=(*it).address;
					msg->timeToRemove=currTime+m_timeToRemovePacket;
					m_recivedMessages.push_back(msg);
					break;
					}
				case SRequestPeer::ERT_CAN_CONNECT:
					{
					//maximum request reached
					//add an offline message indicates that the request failed
					GCPtr<SRecivedMessage>msg=peekMessageFromGraveyard();
					if(!msg)
						msg=new SRecivedMessage();
					msg->command=commands::NET_CMD_CAN_CONNECT_FAILED;
					msg->addr=(*it).address;
					msg->timeToRemove=currTime+m_timeToRemovePacket;
					m_recivedMessages.push_back(msg);
					break;

					}
			}

			it2=it;++it2;
			m_requestPeers.erase(it);
			if(it2==m_requestPeers.end()){
				return;
			}
			it=it2;
		}else
			++it;
	}

}
void Win32Socket::processClients(){
	if(!m_thread->isActive())return;


	PeersList::iterator it=m_connectedPeers.begin();
	PeersList::iterator it2;
	long currTime = gEngine.getTimer()->getMilliseconds();
	bool remove=0;
	for (;it!=m_connectedPeers.end();)
	{
		remove=0;
		if(!(*it) || (*it)->removed)
			remove=1;
		else{
		//	uint tmp=max((*it)->lastPongTime,(*it)->lastMsgTime)-(*it)->peerStartTime+(*it)->localStartTime;
		//	uint tmp=(*it)->lastMsgTime;
			if(currTime-(*it)->lastMsgTime>3*m_pingInterval){
				//send disconnect message
				kickPeer((*it)->address,(*it)->id);
				remove=1;
			}else{
				if((*it)->nextPingTime<=currTime){
					sendPing((*it)->address);
					(*it)->lastPingTime=currTime;
					(*it)->nextPingTime=currTime+m_pingInterval;
				}
				(*it)->m_sendQueue->update(m_sendTime);
			}
		}
		if(!remove)
			++it;
		else{
			it2=it;++it2;
			delete *it;
			m_connectedPeers.erase(it);
			it=it2;
		}
	}
}
void Win32Socket::removeRequest(SRequestPeer::ERequestType type,const NetAddress&addr){

	std::list<SRequestPeer>::iterator it=m_requestPeers.begin();
	for (;it!=m_requestPeers.end();++it){
		if((*it).address==addr && (*it).request==type){
			m_requestPeers.erase(it);
			return;
		}
	}
	return ;
}
void Win32Socket::update(){
	//traceFunction(eNetwork);

	//try to check for any arrived data
	int res=-1;
	NetAddress addr;
	do{
		res=readBytes(&msgBuffer,&addr);
		if(res>0){
			parsePacket(addr,&msgBuffer);
		}
	}while(res>0);

	processRequestList();
	processRecivedMessages();
	processClients();
	m_sendQueue->update(m_sendTime);

	

}

void Win32Socket::sendPing(const NetAddress&addr){
	static CMsgBuffer buffer;
	buffer.reset();
	buffer.write((void*)&commands::NET_CMD_PING,sizeof(commands::NET_CMD_PING));
	uint timeStamp = gEngine.getTimer()->getMilliseconds();
	buffer.write(&timeStamp,sizeof(timeStamp));

	writeBytes(&buffer,addr,1,ESM_NOT_RELIABLE);

	
}
void Win32Socket::sendPacketToPeer(NetID ID,CMsgBuffer*buffer,bool immediate,ESendMode mode){
	const NetAddress*addr=getPeerAddr(ID);
	if(!addr)return;
	writeBytes(buffer,*addr,immediate,mode);
}

SRecivedMessage*Win32Socket::peekMessage(){

	SRecivedMessage*msg=0;
	{
		OS::ScopedLock locker(m_recMsgMutex);

		MessageList::iterator it=m_recivedMessages.begin();
		if(it!=m_recivedMessages.end()){
			msg=(*it);
			m_recivedMessages.erase(it);
		}
	}
	return msg;

}

void Win32Socket::removeMessage(SRecivedMessage*msg)
{
	OS::ScopedLock locker(m_recMsgMutex);
	msg->release();
	MessageList::iterator it=m_recivedMessages.begin();
	for(;it!=m_recivedMessages.end();++it){
		if((*it)==msg){
			m_recivedMessages.erase(it);
			if(msg->m_inUse==0){
				addMessageToGraveyard(msg);
			}
			break;
		}
	}
}

}
}
