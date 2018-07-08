


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
#include "RobotCapabilities.h"
#include <windows.h>

class RobotSerialPortImpl;
class RobotSerialPort:public IRobotController
{
protected:

	mray::TBee::RobotCapabilities m_caps;

	RobotSerialPortImpl* m_impl;
	float pan, tilt, roll;
	bool baseConnected;
	int m_baseCounter;
	int m_headCounter;

	float m_leftArm[7];
	float m_rightArm[7];

	float m_leftHand[3];
	float m_rightHand[3];

	ERobotControllerStatus _status;
	IRobotStatusProvider* m_robotStatusProvider;
	HANDLE m_robotThread;

	int base_control(int velocity_x, int velocity_y, int rotation, int control);
// 	int yamahaInitialize();
// 	int yamahaXY_control(float pos_x, float pos_y, int control);
	int head_control(float pan, float tilt, float roll);

	void _ProcessRobot();
	static DWORD WINAPI timerThreadRobot(RobotSerialPort *robot, LPVOID pdata);

	std::string ScanePorts();

	void _processData();

	void _setupCaps();
public:
	RobotSerialPort();
	virtual~RobotSerialPort();

	void SetListener(ITelubeeRobotListener* l);
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	void ConnectRobot();
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

	virtual ERobotControllerStatus GetRobotStatus();
	virtual void ShutdownRobot() ;
	virtual bool GetJointValues(std::vector<float>& values) ;
	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return &m_caps; }


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args);
	virtual void ParseParameters(const std::map<std::string, std::string>& valueMap);

	virtual void tuningMode() {};

};





#endif
