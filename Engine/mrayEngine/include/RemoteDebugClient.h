
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RemoteDebugClient.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RemoteDebugClient
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RemoteDebugClient___
#define ___RemoteDebugClient___

#include "IRemoteDebugClient.h"
#include "ISocket.h"

namespace mray{
namespace debug{

class MRAY_DLL RemoteDebugClient:public IRemoteDebugClient
{
private:
protected:

	class SWatchInfo:public IRDWatchInfo
	{
	private:
	protected:
	public:

		uint id;
		core::string name;
		core::string val;
		core::string group;

		virtual uint getID(){return id;}
		virtual const core::string& getName(){return name;}
		virtual core::string getVal(){return val;}
		virtual const core::string& getGroup(){return group;}

	};

	WatchList m_watches;

	IRDMessageListener* m_listener;


	GCPtr<network::SocketListener> m_socketListener;
	GCPtr<network::ISocket> m_socket;

	bool createSocket();

public:
	RemoteDebugClient();
	virtual~RemoteDebugClient();

	virtual bool connect(const char*host,ushort port);
	virtual bool connect(const network::NetAddress&addr);

	virtual bool isConnected();

	virtual void setListner(IRDMessageListener*l);

	virtual int getWatchesCount();

	virtual const WatchList&  getWatches();

	void onMessageRecived(network::SRecivedMessage*message);

	void onAddWatch(uint id,const core::string&name,const core::string&val,const core::string&group);
	void onUpdateWatch(uint id,const core::string&val);
	void onRemoveWatch(uint id);

	void onAddLog(const core::string&msg);
};

}
}

#endif //___RemoteDebugClient___
