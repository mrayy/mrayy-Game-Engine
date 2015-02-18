


#ifndef ___NetPacket___
#define ___NetPacket___

#include <mTypes.h>
//#include "..\mrayEngine\CompileConfig.h"
#include "CompileConfig.h"
#include "NetAddress.h"
#include "HNetCommands.h"
#include <string>

namespace mray{
namespace network{

class ISocket;
class CMsgBuffer;
enum ESendMode;

class MRAY_DLL NetPacket {

protected:
	bool sendPacketData(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate,ESendMode mode);
	
public:
	virtual commands::NTCMD getCommand()=0;

	NetPacket ();
	virtual ~NetPacket ();

	virtual void readPacket(CMsgBuffer*buffer)abstract;
	virtual void writeData(CMsgBuffer*buffer)abstract;

	virtual mray::network::ESendMode getSendMode()=0;

	bool sendPacket(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate);
	bool sendPacket(ISocket*s,CMsgBuffer*buffer,const NetAddress&toAddress,bool imediate,ESendMode mode);
};

class MRAY_DLL PeerDisconnect:public NetPacket
{
protected:
public:
	virtual void writeData(mray::network::CMsgBuffer*buffer);
	virtual void readPacket(mray::network::CMsgBuffer*buffer);

	ushort id;
	NetAddress addr;

	PeerDisconnect(ushort id,const NetAddress& addr);
	PeerDisconnect();

	virtual mray::network::commands::NTCMD getCommand();
	virtual mray::network::ESendMode getSendMode();

};

class MRAY_DLL ServerInfo:public NetPacket
{
protected:
public:
	virtual void writeData(mray::network::CMsgBuffer*buffer);
	virtual void readPacket(mray::network::CMsgBuffer*buffer);

	bool hasPassword;
	int maxClients;
	int connectedClients;
	core::string serverName;

	ServerInfo();

	virtual mray::network::commands::NTCMD getCommand();
	virtual mray::network::ESendMode getSendMode();

};
}
}


#endif




