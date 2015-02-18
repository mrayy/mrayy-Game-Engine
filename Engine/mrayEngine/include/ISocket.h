
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   23:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\ISocket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	ISocket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISocket___
#define ___ISocket___

#include <NetDataTypes.h>
#include "SocketListener.h"


namespace mray{
namespace network{

	class IReliableSocket;
	class ConnectedPeer;


class ISocket
{
private:
protected:
public:

	typedef std::list<ConnectedPeer*>  PeersList;

	ISocket(){}
	virtual~ISocket(){}

	virtual void addBytesSent(size_t cnt)=0;

	virtual void setListener(SocketListener* l)=0;

	virtual void createSocket(int port,int maxPeers,int sleepInterval)=0;


	virtual bool connectToHost(const char*host,int port,const char*password)=0;
	virtual bool connectToHost(const NetAddress&addr,const char*password)=0;

	virtual bool isConnected()=0;

	virtual void kickPeer(const NetAddress&addr,ushort id)=0;


	virtual void setPassword(const char*pass,int len)=0;
	virtual const char* getPassword()=0;


	virtual void setReadTimeout(int timeMS)=0;
	virtual int getTimeout()=0;

	virtual void setLocalName(const core::string&name)=0;
	virtual const core::string& getLocalName()=0;

	virtual int bytesRead()=0;
	virtual int bytesSent()=0;

	virtual void resetBytesSent()=0;
	virtual void resetBytesRead()=0;

	virtual void addToBannd(NetAddress&addr)=0;
	virtual bool isBanned(const NetAddress&addr)=0;

	virtual const NetAddress*getUDPAddress()=0;
	virtual const NetAddress*getTCPAddress()=0;

	virtual void close()=0;

	virtual int getLastMsg()=0;

	virtual int readBytes(void*data,int maxLen,NetAddress * fromAddress)=0;
	virtual int readBytes(CMsgBuffer*buffer,NetAddress * fromAddress)=0;
	virtual int writeBytes(const void*data,uint size,const NetAddress& toAddress,bool immediate,ESendMode mode)=0;
	virtual int writeBytes(CMsgBuffer*buffer,const NetAddress & toAddress,bool immediate,ESendMode mode)=0;

	virtual int getPeersCount()=0;


	virtual const PeersList& getPeers()=0;

	virtual NetID getPeerID(const NetAddress&addr)=0;
	virtual NetID getPeerIDByName(const core::string&name)=0;
	virtual const NetAddress* getPeerAddr(NetID ID)=0;

	virtual void sendPing(const NetAddress&addr)=0;
	virtual void sendPacketToPeer(NetID ID,CMsgBuffer*buffer,bool immediate,ESendMode mode)=0;

	virtual SRecivedMessage*peekMessage()=0;

	virtual void removeMessage(SRecivedMessage*msg)=0;


	virtual IReliableSocket* getReliableSocket()=0;

};

MakeSharedPtrType(ISocket)

}
}


#endif //___ISocket___


