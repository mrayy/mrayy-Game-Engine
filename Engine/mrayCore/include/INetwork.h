
/********************************************************************
	created:	2009/05/24
	created:	24:5:2009   23:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\INetwork.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	INetwork
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___INetwork___
#define ___INetwork___

#include "ISingleton.h"
#include "CompileConfig.h"
#include "NetAddress.h"

namespace mray{
namespace network{

	class  ISocket;
	class  IWebRequest;
	class  IUDPClient;
	class  IReliableSocket;

class  MRAY_CORE_DLL INetwork:public ISingleton<INetwork>
{
public:
	virtual~INetwork(){}
	virtual int getSyncNumber()=0;

	virtual IWebRequest* CreateWebRequest()=0;

	virtual ISocket* createSocket(const core::string&name,bool isHost,bool Reliable)=0;

	virtual IUDPClient* createUDPClient()=0;
	virtual IReliableSocket* createTCPSocket()=0;

	virtual bool getHostAddress(const char*name,NetAddress &addr)=0;

	virtual bool getHostIP(const NetAddress &addr,char*name,int maxLen)=0;
	virtual bool getHostName(const NetAddress &addr,char*name,int maxLen)=0;

	//get all ip addresses for this pc
	virtual int getLocalIPs(char ipLst[8][16])=0;

	virtual ulong getLocalAddress()=0;


	virtual int sendto(ISocket*s,const char*buff,int buff_size,
		const NetAddress * toAddress,int flags)=0;
};

}
}


#endif //___INetwork___
