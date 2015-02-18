
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   16:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRemoteDebugger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRemoteDebugger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRemoteDebugger___
#define ___IRemoteDebugger___

#include "mTypes.h"
#include "mstring.h"
#include "IProperty.h"

#include "ListenerContainer.h"

#include "NetAddress.h"

namespace mray{
namespace debug{

	class IRemoteDebuggerListener
	{
	public:
		virtual void OnDebugClientConnected(const network::NetAddress&addr)=0;
		virtual void OnDebugClientDisconnected(const network::NetAddress&addr)=0;
	};

class IRemoteDebugger:public ListenerContainer<IRemoteDebuggerListener*>
{
private:
protected:
	DECLARE_FIRE_METHOD(OnDebugClientConnected,(const network::NetAddress&addr),(addr));
	DECLARE_FIRE_METHOD(OnDebugClientDisconnected,(const network::NetAddress&addr),(addr));
public:

	virtual~IRemoteDebugger(){}

	virtual void setPort(ushort p)=0;
	virtual ushort getPort()=0;

	virtual bool startLog()=0;
	virtual void stopLog()=0;

	virtual void pauseLog()=0;
	virtual void resumeLog()=0;

	virtual bool isLogEnabled()=0;

	virtual void addLogMsg(const core::string&msg)=0;

	virtual void addWatch(CPropertieSet* object,IProperty*prop,const core::string&group,float updateInterval)=0;
	virtual void removeWatch(CPropertieSet* object, IProperty*prop)=0;

	virtual void clear()=0;

	virtual void update(float dt)=0;
};

}
}


#endif //___IRemoteDebugger___
