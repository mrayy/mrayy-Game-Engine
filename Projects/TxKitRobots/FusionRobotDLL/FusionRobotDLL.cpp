// FusionRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FusionRobotDLL.h"
#include "RobotSerialPort.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the FusionRobotDLL.dll
class  CFusionRobotDLL
{
protected:
	IRobotController* m_impl;
public:

	static CFusionRobotDLL* instance;

	CFusionRobotDLL(void);
	// TODO: add your methods here.

	virtual~CFusionRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

};

CFusionRobotDLL* CFusionRobotDLL::instance = 0;

// This is the constructor of a class that has been exported.
// see FusionRobotDLL.h for the class definition
CFusionRobotDLL::CFusionRobotDLL()
{
	printf("************************* TELUBE Robot Agent ************************* \n");
	m_impl = new RobotSerialPort();

}


CFusionRobotDLL::~CFusionRobotDLL(void)
{
	delete m_impl;
	printf("TELUBE Robot Communicator was Destroyed!\n");
}

namespace mray
{

	FusionROBOTDLL_API void   DLL_RobotInit()
	{
		CFusionRobotDLL::instance = new CFusionRobotDLL();
	}

	FusionROBOTDLL_API IRobotController*   DLL_GetRobotController()
	{
		return CFusionRobotDLL::instance->GetRobotController();
	}

	FusionROBOTDLL_API void   DLL_RobotDestroy()
	{
		delete CFusionRobotDLL::instance;
		CFusionRobotDLL::instance = 0;
	}
}