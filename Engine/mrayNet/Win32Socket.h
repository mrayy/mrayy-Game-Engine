

#ifndef ___Win32Socket___
#define ___Win32Socket___


#include "CompileConfig.h"

#include <ISocket.h>
#include <GCPtr.h>
#include <mTypes.h>
#include <BitVector.h>
#include <HNetCommands.h>
#include <NetAddress.h>


#include <NetDataTypes.h>
#include <ConnectedPeer.h>
#include <string>

	
#include <winsock2.h>


namespace mray{
	namespace OS{
		class IThreadFunction;
	}
namespace network{

	class IReliableSocket;
	class SendPacketQueue;

class MRAY_NET_DLL Win32Socket:public ISocket
{
public:
protected:
	friend class Win32SocketProcessThread;
	friend class SendPacketQueue;
	friend class Win32TCPSocket;

	typedef std::list<SRecivedMessage*> MessageList;

	bool m_logToFile;


	IReliableSocket* m_tcpSocket;

	PeersList m_connectedPeers;
	std::list<NetAddress> m_bannedAddress;
	std::list<SRequestPeer> m_requestPeers;
	MessageList m_recivedMessages;
	MessageList m_messagesGraveyard;

	bool m_closed;


	mray::core::BitVector m_usedID;

	SendPacketQueue* m_sendQueue;
	SendPacketQueue::SPacketItem m_arrivedPacket;
	CMsgBuffer		m_tempBuffer;

	mray::GCPtr<OS::IMutex> m_recMsgMutex;

	NetAddress m_udpAddr;

	int m_bytesRead;
	int m_bytesSent;
		
	int m_droppedPackets;

	bool haveSocket;

	int m_timeout;

	SOCKET m_socket;

	CMsgBuffer msgBuffer;

	int m_lastMsg;

	core::string m_localName;

	char m_password[64];
	int m_passwordLen;

	//time in ms for main thread loop
	int m_sleepInterval;

	WSAEVENT m_reciveEvent;

	int m_pingInterval; // time to ping clients

	int m_sendTime;

	int m_maxPeers;

	int m_maxRequest;

	int m_timeToRemovePacket;

	OS::IThread*m_thread;
	OS::IThreadFunction *m_threadFunc;

	bool m_isHost;

	SocketListener* m_listener;


	/////////////////////////////////////////////////
	
	NetID getFreeID();
	void freeID(NetID id);
	void setUsedID(NetID id);

	void update();

	virtual bool parsePacket(const NetAddress&addr,CMsgBuffer*msg);
	//TODO : overwrite this method to handle your packets
	virtual bool packetArrived(SRecivedMessage*msg);


	ConnectedPeer* addNewPeer(const NetAddress&addr,EPeerConnectState state,bool local);
	ConnectedPeer* getPeer(NetID ID);
	ConnectedPeer* getPeer(const NetAddress&addr);
	bool			removePeer(const NetAddress&addr);

	void processRequestList();
	bool processReques(SRequestPeer*request);
	void removeRequest(SRequestPeer::ERequestType type,const NetAddress&addr);
	void processRecivedMessages();
	void processClients();

	SRequestPeer *getRequest(SRequestPeer::ERequestType type,const NetAddress&addr);
	//////////////////////////////////////////////////////////////////////////
	bool hasRequest(const NetAddress&addr,SRequestPeer::ERequestType);
	void sendConnectRequest(const NetAddress&addr,const char*password);
	void sendDisconnect();

	void sendInfo(const NetAddress&addr,const char*password,byte passlen);

	void sendMessageToPeers(void*data,int size,bool immediate,ESendMode mode);

	////////////////////////
	
	SRecivedMessage* peekMessageFromGraveyard();
	void addMessageToGraveyard(SRecivedMessage* msg);

public:

	void addBytesSent(size_t cnt){
		m_bytesSent+=cnt;
	}

public:



	Win32Socket(const core::string&name,bool isHost,bool ReliableInterface);
	virtual~Win32Socket();

	void createSocket(int port,int maxPeers,int sleepInterval);


	virtual void setListener(SocketListener* l);

	bool connectToHost(const char*host,int port,const char*password);
	bool connectToHost(const NetAddress&addr,const char*password);

	bool addRequest(SRequestPeer*request);

	bool isConnected();
	
	void kickPeer(const NetAddress&addr,ushort id);

	void setSocket(SOCKET s);
	SOCKET getSocket();

	void setPassword(const char*pass,int len);
	const char* getPassword();


	bool isHasSocket();

	void setReadTimeout(int timeMS);
	int getTimeout();

	void setLocalName(const core::string&name);
	const core::string& getLocalName();

	int bytesRead();
	int bytesSent();

	void resetBytesSent();
	void resetBytesRead();

	void addToBannd(NetAddress&addr);
	bool isBanned(const NetAddress&addr);

	const NetAddress*getUDPAddress();
	const NetAddress*getTCPAddress();

	void close();

	int getLastMsg();

	CMsgBuffer*initAndGetBuffer();
	CMsgBuffer*getBuffer();

	int readBytes(void*data,int maxLen,NetAddress * fromAddress);
	int readBytes(CMsgBuffer*buffer,NetAddress * fromAddress);
	int writeBytes(const void*data,uint size,const NetAddress & toAddress,bool immediate,ESendMode mode);
	int writeBytes(CMsgBuffer*buffer,const NetAddress & toAddress,bool immediate,ESendMode mode);

	int getPeersCount();


	virtual const PeersList& getPeers();

	NetID getPeerID(const NetAddress&addr);
	NetID getPeerIDByName(const core::string&name);
	const NetAddress* getPeerAddr(NetID ID);

	void sendPing(const NetAddress&addr);
	void sendPacketToPeer(NetID ID,CMsgBuffer*buffer,bool immediate,ESendMode mode);

	SRecivedMessage*peekMessage();

	void removeMessage(SRecivedMessage*msg);


	virtual IReliableSocket*  getReliableSocket(){
		return m_tcpSocket;
	}
};

}
}


#endif

