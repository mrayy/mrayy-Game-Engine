



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\PhantomRobotDLL\PhantomCommunicator
	file base:	PhantomCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhantomCommunicator_h__
#define PhantomCommunicator_h__

#include "IRobotController.h"

namespace mray
{
class PhantomCommunicatorImpl;

}
class PhantomCommunicator :public IRobotController
{
protected:
	mray::PhantomCommunicatorImpl* m_impl;
public:

	PhantomCommunicator();
	virtual~PhantomCommunicator();

	void SetListener(ITelubeeRobotListener* l);
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider);
	void ConnectRobot();
	void DisconnectRobot();
	bool IsConnected();
	virtual void ShutdownRobot();
	virtual ERobotControllerStatus GetRobotStatus();
	void UpdateRobotStatus(const RobotStatus& st);
	virtual bool GetJointValues(std::vector<float>& values) { return false; };
	virtual void tuningMode() {};


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args);
};

#endif // PhantomCommunicator_h__

