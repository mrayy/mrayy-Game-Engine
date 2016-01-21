#ifndef __RemoteController__
#define __RemoteController__

#include <string>
#include "IRobotController.h"


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

	ERobotControllerStatus GetRobotStatus();

	void Render();
};





#endif
