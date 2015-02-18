


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

#include "IRobotController.h"

#include <vector>

	struct RobotStatus;
	class RobotSerialPortImpl;
	class ITelubeeRobotListener;
class RobotSerialPort:public IRobotController
{
protected:

	RobotSerialPortImpl* m_impl;
	float robotX, robotY, robotZ;
	float pan, tilt, roll;
	bool isHoming;
	int omni_control(int velocity_x, int velocity_y, int rotation, int control);
	int yamahaInitialize();
	int IAI_Initialize();
	int yamahaXY_control(float pos_x, float pos_y, int control);
	int head_control(float pan, float tilt, float roll);
	unsigned int lrc_generator(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range);
	char *RobotSerialPort::iai_command_send(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range); 
	char *RobotSerialPort::iai_pos(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range, unsigned int length, float pos);


	static DWORD WINAPI timerThreadHead(RobotSerialPort *robot, LPVOID pdata);
	static DWORD WINAPI timerThreadBase(RobotSerialPort *robot, LPVOID pdata);

	std::vector<char> m_headbBuffer;

public:
	RobotSerialPort();
	virtual~RobotSerialPort();

	int IAI_destroy();

	void SetListener(ITelubeeRobotListener* l);
	void ConnectRobot();
	void DisconnectRobot();
	bool IsConnected();
	void UpdateRobotStatus( RobotStatus& st);

	RobotSerialPortImpl* GetImpl(){ return m_impl; }

	void HeadDataArrived(char *buffer, int size);
	void RobotDataArrived(char *buffer, int size);

};



#endif
