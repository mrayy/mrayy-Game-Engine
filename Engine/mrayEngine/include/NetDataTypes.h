

#ifndef ___NetDataTypes_H___
#define ___NetDataTypes_H___

#include <GCPtr.h>

#include "CMsgBuffer.h"
#include "NetAddress.h"
#include "HNetCommands.h"


namespace mray{
namespace network{

typedef  ulong NetID;
typedef long long TIME_Stamp;


#define MAX_DGRAM_SIZE 1500

enum ESocketOption
{
	ESO_NONBLOCK  ,
	ESO_SOCKOPT_BROADCAST ,
	ESO_SOCKOPT_RCVBUF    ,
	ESO_SOCKOPT_SNDBUF    
} ;

enum ESocketWait
{
	ESW_NONE=(1<<0),
	ESW_SEND=(1<<1),
	ESW_RECEIVE=(1<<2)
};
enum EProtoType{
	EPT_TCP,
	EPT_UDP
};


enum ESendMode{
	ESM_NOT_RELIABLE,
	ESM_NOT_RELIABLE_SEQ,
	ESM_RELIABLE,
	ESM_RELIABLE_SEQ,
	ESM_COUNT
};

enum EPeerConnectState{
	EPCS_NONE,
	EPCS_REQ_CONNECT,
	EPCS_CONNECTED
};

struct SPacket{
	CMsgBuffer data;
	NetAddress remoteAddress;
};


struct MRAY_DLL SRequestPeer{
	SRequestPeer();
	enum ERequestType{ERT_CONNECT,ERT_CAN_CONNECT};

	void setPassword(const char*pass,int len);
	
	char password[255];
	byte passLen;

	NetAddress address;
	int requestTime;
	ERequestType request; 
	int requestCount;

};


class MRAY_DLL SRecivedMessage{
public:
	bool m_remove;
	int m_inUse;
public:
/*	void setInuse(bool set){
		m_inUse=set;
	}*/
	SRecivedMessage();
	virtual~SRecivedMessage();
	void use();
	void release();
	commands::NTCMD command;
	NetAddress addr;
	CMsgBuffer msg;
	int timeToRemove;
};
}
}
	


#endif




