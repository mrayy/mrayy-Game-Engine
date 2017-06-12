


/********************************************************************
	created:	2013/12/04
	created:	4:12:2013   14:33
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\RobotController.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
	file base:	RobotController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotController__
#define __RobotController__

#include <string>
#include "IRobotController.h"
#include <windows.h>

class RobotControllerImpl;
class RobotController:public IRobotController
{
protected:

	RobotControllerImpl* m_impl;
	int robot_vx, robot_vy, robot_rot;
	float pan, tilt, roll;
	ERobotControllerStatus _status;
	IRobotStatusProvider* m_robotStatusProvider;

	HANDLE m_robotThread;

	int head_control(float pan, float tilt, float roll);

	void _ProcessRobot();
	static DWORD WINAPI timerThreadRobot(RobotController *robot, LPVOID pdata);

	void _processData();
public:
	RobotController();
	virtual~RobotController();

	void SetListener(ITelubeeRobotListener* l);
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	void ConnectRobot();
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

	virtual ERobotControllerStatus GetRobotStatus();
	virtual void ShutdownRobot() ;
	virtual bool GetJointValues(std::vector<float>& values) ;
	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return 0; }


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args);
	virtual void ParseParameters(const std::map<std::string, std::string>& valueMap);

	virtual void tuningMode() {};

};





#endif
