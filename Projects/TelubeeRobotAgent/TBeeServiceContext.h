

#ifndef __TBEESERVICECONTEXT__
#define __TBEESERVICECONTEXT__

#include "ServiceContext.h"
#include "ListenerContainer.h"
#include "RobotCommunicator.h"
#include "OptionContainer.h"
#include "CMRayApplication.h"

namespace mray
{
namespace TBee
{

	//Set of events the services can listen to
	class IServiceContextListener
	{
	public:

		virtual void OnUserConnected(const UserConnectionData& data){}
	};
	
class TBeeServiceContext:public ServiceContext,public ListenerContainer<IServiceContextListener*>
{
protected:
public:
	DECLARE_FIRE_METHOD(OnUserConnected,(const UserConnectionData& data),(data))
public:
	TBeeServiceContext(){}
	virtual ~TBeeServiceContext(){}

	CMRayApplication* app;

	//application runtime options
	OptionContainer appOptions;

	//User's end address
	network::NetAddress remoteAddr;

	//Communication Channel with the user end
	network::IUDPClient* commChannel;
};

class TbeeServiceRenderContext:public ServiceRenderContext
{
protected:
	float m_xOffset, m_yOffset;
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

	void RenderText(const core::string &txt, float x, float y);
	void Reset();

	void AddXOffsetText(float x){ m_xOffset += x; }
	void AddYOffsetText(float y){ m_yOffset += y; }
};

}
}


#endif
