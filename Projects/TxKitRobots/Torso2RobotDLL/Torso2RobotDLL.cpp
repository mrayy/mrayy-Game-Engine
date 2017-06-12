// TORSORobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Torso2RobotDLL.h"
#include "torsoController.h"
#include <stdio.h>
#include "IRobotController.h"


// This class is exported from the TelubeeRobotDLL.dll



class  CTorsoRobotDLL
{
protected:
	torsoController* m_impl;
public:

	static CTorsoRobotDLL* instance;

	CTorsoRobotDLL(void);
	// TODO: add your methods here.

	virtual~CTorsoRobotDLL();

	IRobotController* GetRobotController()
	{
		return m_impl;
	}
};

CTorsoRobotDLL* CTorsoRobotDLL::instance = 0;

// This is the constructor of a class that has been exported.
// see TelubeeRobotDLL.h for the class definition
CTorsoRobotDLL::CTorsoRobotDLL()
{
	printf("------------------------ TORSO 2.0 Robot Agent ------------------------ \n");
	m_impl = new torsoController();

}


CTorsoRobotDLL::~CTorsoRobotDLL(void)
{
	delete m_impl;
	printf("TORSO Robot Communicator was Destroyed!\n");
}

namespace mray
{

	TorsoROBOTDLL_API void DLL_RobotInit()
	{
		CTorsoRobotDLL::instance = new CTorsoRobotDLL();
	}

	TorsoROBOTDLL_API IRobotController* DLL_GetRobotController()
	{
		return CTorsoRobotDLL::instance->GetRobotController();
	}

	TorsoROBOTDLL_API void DLL_RobotDestroy()
	{
		delete CTorsoRobotDLL::instance;
		CTorsoRobotDLL::instance = 0;
	}
}