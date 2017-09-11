
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   23:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\IReliableSocket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	IReliableSocket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IReliableSocket___
#define ___IReliableSocket___

#include "NetDataTypes.h"
#include "ListenerContainer.h"

namespace mray{
namespace network{

	class  IReliableSocket;

class  IReliableSocketListener
{
public: 
	virtual void OnPeerConnected(IReliableSocket*s, const NetAddress& peer) {}
	virtual void OnPeerDisconnected(IReliableSocket*s, const NetAddress& peer) {}
	virtual void OnSocketStarted(IReliableSocket*s) {}
	virtual void OnSocketStopped(IReliableSocket*s) {}
};

class  IReliableSocket :public ListenerContainer<IReliableSocketListener*>
{
private:
protected:
	DECLARE_FIRE_METHOD(OnPeerConnected, (IReliableSocket*s, const NetAddress& peer), (s, peer))
	DECLARE_FIRE_METHOD(OnPeerDisconnected, (IReliableSocket*s, const NetAddress& peer), (s, peer))
	DECLARE_FIRE_METHOD(OnSocketStarted, (IReliableSocket*s), (s))
	DECLARE_FIRE_METHOD(OnSocketStopped, (IReliableSocket*s), (s))
public:
	IReliableSocket(){}
	virtual~IReliableSocket(){}

	//if manual socket, then the receiving will be handled by the user
	virtual void SetManualSocket(bool isManual) = 0;

	virtual bool startSocket(int port,int maxIncomingConnection,int sleepInterval)=0;
	virtual void stopSocket()=0;

	virtual void closeConnection()=0;
	virtual bool isConnected() = 0;

	virtual const NetAddress *connect(const core::string&host,ushort port)=0;
	virtual const NetAddress *connect(const NetAddress&addr)=0;

	virtual uint GetPeerSocket(const NetAddress&addr) = 0;

	virtual bool peerConnected(const NetAddress&addr)=0;

	virtual void GetConnectedPeers(std::list<NetAddress>& peers)=0;

	virtual const NetAddress*getAddress()=0;

	virtual void send(void*data,uint size,bool imediate,const NetAddress&addr)=0;

	virtual int receive(const NetAddress&peer,void* data, uint size, int flags) = 0;

	virtual SPacket*popMessage()=0;

	virtual void update()=0;
};

MakeSharedPtrType(IReliableSocket)

}
}


#endif //___IReliableSocket___
