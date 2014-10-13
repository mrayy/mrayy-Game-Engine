


/********************************************************************
	created:	2013/12/04
	created:	4:12:2013   14:33
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\RobotSerialPort.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
	file base:	RobotSerialPort
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotSerialPort__
#define __RobotSerialPort__

#include <string>
#include "IRobotController.h"
#include "RoombaController.h"
#include <windows.h>

class RobotSerialPortImpl;
class RobotSerialPort:public IRobotController
{
protected:

	RobotSerialPortImpl* m_impl;
	float robotX, robotY, robotZ;
	int robot_vx, robot_vy, robot_rot;
	float pan, tilt, roll;
	bool baseConnected;
	int omni_control(int velocity_x, int velocity_y, int rotation, int control);
// 	int yamahaInitialize();
// 	int yamahaXY_control(float pos_x, float pos_y, int control);
	int head_control(float pan, float tilt, float roll);

	static DWORD WINAPI timerThreadHead(RobotSerialPort *robot, LPVOID pdata);
	static DWORD WINAPI timerThreadBase(RobotSerialPort *robot, LPVOID pdata);

	std::string ScanePorts();

public:
	RobotSerialPort();
	virtual~RobotSerialPort();

	void SetListener(ITelubeeRobotListener* l);
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	void ConnectRobot();
	void DisconnectRobot();
	bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

};



#endif
