// TelubeeRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RemoteRobotDLL.h"
#include "RemoteController.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the TelubeeRobotDLL.dll
class  CTelubeeRobotDLL
{
protected:
	RemoteController* m_impl;
public:

	static CTelubeeRobotDLL* instance;

	CTelubeeRobotDLL(void);
	// TODO: add your methods here.

	virtual~CTelubeeRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

};

CTelubeeRobotDLL* CTelubeeRobotDLL::instance = 0;

// This is the constructor of a class that has been exported.
// see TelubeeRobotDLL.h for the class definition
CTelubeeRobotDLL::CTelubeeRobotDLL()
{
	printf("************************* Remote Robot Agent ************************* \n");
	m_impl = new RemoteController();

}


CTelubeeRobotDLL::~CTelubeeRobotDLL(void)
{
	delete m_impl;
	printf("Remote Robot Communicator was Destroyed!\n");
}

namespace mray
{

	RemoteROBOTDLL_EXPORTS void DLL_RobotInit()
	{
		CTelubeeRobotDLL::instance = new CTelubeeRobotDLL();
	}

	RemoteROBOTDLL_EXPORTS IRobotController* DLL_GetRobotController()
	{
		return CTelubeeRobotDLL::instance->GetRobotController();
	}

	RemoteROBOTDLL_EXPORTS void DLL_RobotDestroy()
	{
		delete CTelubeeRobotDLL::instance;
		CTelubeeRobotDLL::instance = 0;
	}
}