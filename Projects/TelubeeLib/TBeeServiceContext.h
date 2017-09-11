

#ifndef __TBEESERVICECONTEXT__
#define __TBEESERVICECONTEXT__

#include "ServiceContext.h"
#include "ListenerContainer.h"
#include "RobotCommunicator.h"
#include "OptionContainer.h"

 #include "IFont.h"
#include "FontAttributes.h"
#include "ViewPort.h"

namespace mray
{
	class CMRayApplication;
namespace TBee
{
	class ModuleSharedMemory;
	class NetworkValueController;

	//Set of events the services can listen to
	class IServiceContextListener
	{
	public:

		virtual void OnUserConnected(const UserConnectionData& data){}
		virtual void OnUserDisconnected(){}
		virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value){}

	};
	
	class IServiceLoader
	{
	public:
		virtual void RequestLock(){}
		virtual void RequestUnlock(){}

		virtual void RegisterCapability(const core::string& category, const core::string& name, const core::string& value){}
		virtual void RemoveCapability(const core::string& category, const core::string& name){}
		virtual void RemoveCapabilityCategory(const core::string& category){}
	};

class TBeeServiceContext:public ServiceContext,public ListenerContainer<IServiceContextListener*>
{
protected:
public:
	DECLARE_FIRE_METHOD(OnUserConnected, (const UserConnectionData& data), (data))
	DECLARE_FIRE_METHOD(OnUserDisconnected, (), ())
	DECLARE_FIRE_METHOD(OnUserMessage, (network::NetAddress* addr, const core::string& msg, const core::string& value), (addr,msg,value))
public:
	TBeeServiceContext() :serviceLoader(0), app(0), commChannel(0), sharedMemory(0), netValueController(0), portHostAddr(0)
	{}
	virtual ~TBeeServiceContext(){}

	IServiceLoader* serviceLoader;

	CMRayApplication* app;

	//application runtime options
	OptionContainer appOptions;

	//network port map
	network::NetAddress *portHostAddr;
	std::map<core::string, unsigned short> portMap;

	//User's end address
	network::NetAddress remoteAddr;

	//local Communication channel address
	network::NetAddress localAddr;

	//Communication Channel with the user end
	network::IUDPClient* commChannel;

	ModuleSharedMemory* sharedMemory;

	NetworkValueController* netValueController;

	ushort GetPortValue(const core::string& name)
	{
		std::map<core::string, unsigned short>::iterator it= portMap.find(name);
		if (it == portMap.end())
			return 0;
		return it->second;
	}

	network::NetAddress* GetTargetClientAddr()
	{
		if (portHostAddr)
			return portHostAddr;
		return &remoteAddr;

	}

};

class TbeeServiceRenderContext:public ServiceRenderContext
{
protected:
	int m_xOffset, m_yOffset;
public:
	
	GUI::IFont* font;
	GUI::FontAttributes fontAttrs;
	GUI::IGUIRenderer* guiRenderer;

	scene::ViewPort* viewPort;
	
	TbeeServiceRenderContext()
	{
		font = 0;
		viewPort = 0;
		Reset();
	}

	void RenderText(const core::string &txt, int x, int y,const video::SColor& clr=1);
	void Reset();

	void AddXOffsetText(int x){ m_xOffset += x; }
	void AddYOffsetText(int y){ m_yOffset += y; }
};

}
}


#endif
