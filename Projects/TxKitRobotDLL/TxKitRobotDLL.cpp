// TxKitRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TxKitRobotDLL.h"
#include "RobotSerialPort.h"
#include <stdio.h>
#include "IRobotController.h"

class RobotSerialPort;
// This class is exported from the TxKitRobotDLL.dll
class  CTxKitRobotDLL
{
protected:
	IRobotController* m_impl;
public:

	static CTxKitRobotDLL* instance;

	CTxKitRobotDLL(void);
	// TODO: add your methods here.

	virtual~CTxKitRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}

};

CTxKitRobotDLL* CTxKitRobotDLL::instance = 0;

// This is the constructor of a class that has been exported.
// see TxKitRobotDLL.h for the class definition
CTxKitRobotDLL::CTxKitRobotDLL()
{
	printf("************************* TELUBE Robot Agent ************************* \n");
	m_impl = new RobotSerialPort();

}


CTxKitRobotDLL::~CTxKitRobotDLL(void)
{
	delete m_impl;
	printf("TELUBE Robot Communicator was Destroyed!\n");
}

namespace mray
{

	TxKitRobotDLL_API void   DLL_RobotInit()
	{
		CTxKitRobotDLL::instance = new CTxKitRobotDLL();
	}

	TxKitRobotDLL_API IRobotController*   DLL_GetRobotController()
	{
		return CTxKitRobotDLL::instance->GetRobotController();
	}

	TxKitRobotDLL_API void   DLL_RobotDestroy()
	{
		delete CTxKitRobotDLL::instance;
		CTxKitRobotDLL::instance = 0;
	}
}