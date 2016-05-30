#ifndef __RemoteController__
#define __RemoteController__

#include <string>
#include "IRobotController.h"
#include "ServiceContext.h"

namespace mray
{
	namespace TBee
	{

class RemoteControllerReceiverImpl;
class RemoteControllerReceiver
{
protected:

	RemoteControllerReceiverImpl* m_impl;
	IRobotStatusProvider* m_robotStatusProvider;


	void _setupCaps();
public:



	//RemoteController class variables
	RemoteControllerReceiver();
	virtual~RemoteControllerReceiver();

	void Init(core::string robotdll);

	ERobotControllerStatus GetRobotStatus();

	void Render(ServiceRenderContext* context);
};

	}
}




#endif
