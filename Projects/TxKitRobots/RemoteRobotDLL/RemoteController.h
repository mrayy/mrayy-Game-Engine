#ifndef __RemoteController__
#define __RemoteController__

#include <string>
#include "IRobotController.h"




class RemoteControllerImpl;
class RemoteController :public IRobotController
{
protected:

	RemoteControllerImpl* m_impl;
	IRobotStatusProvider* m_robotStatusProvider;


	void _setupCaps();
public:



	//RemoteController class variables
	RemoteController();
	virtual~RemoteController();

	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	void SetListener(ITelubeeRobotListener* l);
	void ConnectRobot();
	void ManualControlRobot(); 
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

	ERobotControllerStatus GetRobotStatus();
	void ShutdownRobot();
	void tuningMode();
	bool GetJointValues(std::vector<float>& values);


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args){ return ""; }

	virtual void ParseParameters(const std::map<std::string, std::string>& valueMap);

	void _processData();
	void _updateRobot();
};





#endif
