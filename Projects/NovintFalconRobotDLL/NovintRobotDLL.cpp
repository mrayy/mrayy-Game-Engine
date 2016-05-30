// NovintRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NovintRobotDLL.h"
#include "NovintController.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the NovintRobotDLL.dll
class  CNovintRobotDLL
{
protected:
	IRobotController* m_impl;
public:

	static CNovintRobotDLL* instance;

	CNovintRobotDLL(void);
	// TODO: add your methods here.

	virtual~CNovintRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

};

CNovintRobotDLL* CNovintRobotDLL::instance = 0;

// This is the constructor of a class that has been exported.
// see NovintRobotDLL.h for the class definition
CNovintRobotDLL::CNovintRobotDLL()
{
	printf("************************* Novint Falcon Robot Agent ************************* \n");
	m_impl = new NovintController();

}


CNovintRobotDLL::~CNovintRobotDLL(void)
{
	delete m_impl;
	printf("NovintController Robot Communicator was Destroyed!\n");
}

namespace mray
{

	NovintROBOTDLL_API void   DLL_RobotInit()
	{
		CNovintRobotDLL::instance = new CNovintRobotDLL();
	}

	NovintROBOTDLL_API IRobotController*   DLL_GetRobotController()
	{
		return CNovintRobotDLL::instance->GetRobotController();
	}

	NovintROBOTDLL_API void   DLL_RobotDestroy()
	{
		delete CNovintRobotDLL::instance;
		CNovintRobotDLL::instance = 0;
	}
}