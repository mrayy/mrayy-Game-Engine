// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TELUBEEROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TELUBEEROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef TELUBEEROBOTDLL_EXPORTS
#define TELUBEEROBOTDLL_API __declspec(dllexport)
#else
#define TELUBEEROBOTDLL_API __declspec(dllimport)
#endif

/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotDLL\TelubeeRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotDLL
	file base:	TelubeeRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TelubeeRobotDLL__
#define __TelubeeRobotDLL__

#include "IRobotController.h"

struct RobotStatus
{
	bool connected;
	float speedX, speedY;
	float rotation;
	float tilt, yaw, roll;	//head rotation
	float X, Y, Z;			//head position
	bool homing;

	RobotStatus()
	{
		homing = false;
		connected = false;
		speedX = speedY = 0;
		rotation = 0;
		tilt = yaw = roll = 0;
		X = Y = Z = 0;
	}
};
class IRobotController;

class ITelubeeRobotListener
{
public:

	virtual void OnRotationArrived(float x,float y,float z){}

	virtual void OnCollisionData(float left, float right){}
};

class RobotSerialPort;
// This class is exported from the TelubeeRobotDLL.dll
class TELUBEEROBOTDLL_API CNissanRobotDLL 
{
protected:
	IRobotController* m_impl;
public:
	CNissanRobotDLL(void);
	// TODO: add your methods here.

	virtual~CNissanRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

	void Destroy();

	static CNissanRobotDLL*instance;
};




#endif