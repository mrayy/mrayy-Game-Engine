

/********************************************************************
	created:	2012/08/26
	created:	26:8:2012   13:21
	filename: 	d:\Development\mrayEngine\Tests\PhysicsTest\CarController.h
	file path:	d:\Development\mrayEngine\Tests\PhysicsTest
	file base:	CarController
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CarController___
#define ___CarController___

#include "IThreadFunction.h"
#include "IThread.h"

#include <queue>

class ofSerial;



namespace mray
{

class CarController:public OS::IThreadFunction
{
protected:
	ofSerial* m_serialPort;

	OS::IThread* m_thread;
	OS::IMutex* m_mutex;

	float m_lastT[10];

	std::queue<core::string> m_sendList;

	void SendCommand(const uchar*buff,int index);


	virtual void setup(){}
	virtual void execute(OS::IThread*caller,void*arg);
public:
	CarController(const core::string& portName);
	virtual~CarController();

	void SetSpeed(const math::vector2d& speed);
	void SetLookingDirection(const math::vector3d& dir);


	
	
};

}

#endif
