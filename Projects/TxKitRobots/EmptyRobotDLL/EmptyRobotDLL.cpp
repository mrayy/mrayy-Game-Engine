// EmptyRobotDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EmptyRobotDll.h"
#include "RobotController.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the EmptyRobotDll.dll
class  CEmptyRobotDll
{
protected:
	IRobotController* m_impl;
public:

	static CEmptyRobotDll* instance;

	CEmptyRobotDll(void);
	// TODO: add your methods here.

	virtual~CEmptyRobotDll();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

};

CEmptyRobotDll* CEmptyRobotDll::instance = 0;

// This is the constructor of a class that has been exported.
// see EmptyRobotDll.h for the class definition
CEmptyRobotDll::CEmptyRobotDll()
{
	printf("************************* TELUBE Robot Agent ************************* \n");
	m_impl = new RobotController();

}


CEmptyRobotDll::~CEmptyRobotDll(void)
{
	delete m_impl;
	printf("TELUBE Robot Communicator was Destroyed!\n");
}

namespace mray
{

	EmptyRobotDll_API void   DLL_RobotInit()
	{
		CEmptyRobotDll::instance = new CEmptyRobotDll();
	}

	EmptyRobotDll_API IRobotController*   DLL_GetRobotController()
	{
		return CEmptyRobotDll::instance->GetRobotController();
	}

	EmptyRobotDll_API void   DLL_RobotDestroy()
	{
		delete CEmptyRobotDll::instance;
		CEmptyRobotDll::instance = 0;
	}
}