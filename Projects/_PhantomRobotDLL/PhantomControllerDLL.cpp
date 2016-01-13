

#include "stdafx.h"
#include "PhantomControllerDLL.h"
#include "PhantomCommunicator.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the TelubeeRobotDLL.dll
class  CTelubeeRobotDLL
{
protected:
	IRobotController* m_impl;
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
	printf("************************* Phantom Robot Agent ************************* \n");
	m_impl = new PhantomCommunicator();

}


CTelubeeRobotDLL::~CTelubeeRobotDLL(void)
{
	delete m_impl;
	printf("Phantom Robot Communicator was Destroyed!\n");
}

namespace mray
{

	PHANTOMROBOTDLL_API void   DLL_RobotInit()
	{
		CTelubeeRobotDLL::instance = new CTelubeeRobotDLL();
	}

	PHANTOMROBOTDLL_API IRobotController*   DLL_GetRobotController()
	{
		return CTelubeeRobotDLL::instance->GetRobotController();
	}

	PHANTOMROBOTDLL_API void   DLL_RobotDestroy()
	{
		delete CTelubeeRobotDLL::instance;
		CTelubeeRobotDLL::instance = 0;
	}
}


