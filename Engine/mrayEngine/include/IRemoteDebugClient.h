
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRemoteDebugClient.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRemoteDebugClient
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRemoteDebugClient___
#define ___IRemoteDebugClient___

#include "mTypes.h"
#include "mstring.h"

#include "IRDWatchInfo.h"
#include "GCPtr.h"
#include "NetAddress.h"

namespace mray{
namespace debug{

class IRDMessageListener{
public:

	virtual void onAddWatch(uint id,const core::string&name,
		const core::string&val,const core::string&group)=0;

	virtual void onUpdateWatch(uint id,const core::string&val)=0;

	virtual void onRemoveWatch(uint id)=0;

	virtual void onReciveLog(const core::string&msg)=0;
};

class MRAY_DLL IRemoteDebugClient
{
private:
protected:
public:

	typedef std::list<GCPtr<IRDWatchInfo>> WatchList;

	virtual~IRemoteDebugClient(){}

	virtual bool connect(const char*host,ushort port)=0;
	virtual bool connect(const network::NetAddress&addr)=0;

	virtual bool isConnected()=0;

	virtual void setListner(IRDMessageListener*l)=0;

	virtual int getWatchesCount()=0;

	virtual const WatchList& getWatches()=0;
};

}
}


#endif //___IRemoteDebugClient___
