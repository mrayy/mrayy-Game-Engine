// RobotControlModule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RobotControlModule.h"


// This is an example of an exported variable
ROBOTCONTROLMODULE_API int nRobotControlModule=0;

// This is an example of an exported function.
ROBOTCONTROLMODULE_API int fnRobotControlModule(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see RobotControlModule.h for the class definition
CRobotControlModule::CRobotControlModule()
{
	return;
}
