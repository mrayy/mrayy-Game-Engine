
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   16:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RemoteDebugger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RemoteDebugger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RemoteDebugger___
#define ___RemoteDebugger___

#include "IRemoteDebugger.h"
#include "ISocket.h"
#include "IRDWatchInfo.h"


namespace mray{
namespace debug{



class MRAY_DLL RemoteDebugger:public IRemoteDebugger
{
private:
protected:

	struct SWatchInfo:public IRDWatchInfo
	{
	public:
		SWatchInfo(){
			watch=0;
			updateInterval=0;
			timer=0;
		}
		CPropertieSet* object;
		IProperty*watch;
		uint id;
		core::string group;
		float updateInterval;

		float timer;


		virtual uint getID(){
			return id;
		}
		virtual const core::string& getName(){
			return watch->getName();
		}
		virtual core::string getVal(){
			return watch->toString(object);
		}
		virtual const core::string& getGroup(){
			return group;
		}
	};

	GCPtr<network::SocketListener> m_listener;

	uint m_lastID;

	typedef std::list<SWatchInfo> WatchList;
	WatchList m_watches;

	GCPtr<network::ISocket> m_socket;

	network::CMsgBuffer m_msgBuffer;


	bool m_enabled;
	ushort m_port;

	bool createSocket();


	void sendWatch(const SWatchInfo&w,const network::NetAddress&addr);

	void removeWatch(const SWatchInfo&w);
	void sendWatch(const SWatchInfo&w);
	void updateWatch(const SWatchInfo&w);

	SWatchInfo* getWatch(uint id);

public:
	RemoteDebugger();
	virtual~RemoteDebugger();

	virtual void setPort(ushort p);
	virtual ushort getPort();

	virtual bool startLog();
	virtual void stopLog();
	virtual void pauseLog();
	virtual void resumeLog();

	virtual bool isLogEnabled();

	virtual void addLogMsg(const core::string&msg);

	virtual void addWatch(CPropertieSet* object,IProperty*prop,const core::string&group,float updateInterval);
	virtual void removeWatch(CPropertieSet* object,IProperty*prop);

	virtual void clear();

	virtual void update(float dt);


	void onClientConnected(const network::NetAddress&addr);
	void onClientDisconnected(const network::NetAddress&addr);

	void onMessageRecived(network::SRecivedMessage*message);
};

}
}


#endif //___RemoteDebugger___
