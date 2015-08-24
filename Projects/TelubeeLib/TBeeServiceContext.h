

#ifndef __TBEESERVICECONTEXT__
#define __TBEESERVICECONTEXT__

#include "ServiceContext.h"
#include "ListenerContainer.h"
#include "RobotCommunicator.h"
#include "OptionContainer.h"

namespace mray
{
	class CMRayApplication;
namespace TBee
{

	//Set of events the services can listen to
	class IServiceContextListener
	{
	public:

		virtual void OnUserConnected(const UserConnectionData& data){}
		virtual void OnUserDisconnected(){}
		virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value){}
	};
	
class TBeeServiceContext:public ServiceContext,public ListenerContainer<IServiceContextListener*>
{
protected:
public:
	DECLARE_FIRE_METHOD(OnUserConnected, (const UserConnectionData& data), (data))
	DECLARE_FIRE_METHOD(OnUserDisconnected, (), ())
	DECLARE_FIRE_METHOD(OnUserMessage, (network::NetAddress* addr, const core::string& msg, const core::string& value), (addr,msg,value))
public:
	TBeeServiceContext(){}
	virtual ~TBeeServiceContext(){}

	CMRayApplication* app;

	//application runtime options
	OptionContainer appOptions;

	//User's end address
	network::NetAddress remoteAddr;

	//local Communication channel address
	network::NetAddress localAddr;

	//Communication Channel with the user end
	network::IUDPClient* commChannel;
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
