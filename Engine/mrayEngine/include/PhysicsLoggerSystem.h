


/********************************************************************
	created:	2011/01/16
	created:	16:1:2011   23:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PhysicsLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PhysicsLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhysicsLoggerSystem_h__
#define PhysicsLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{
	DECLARE_LOGGER_SYSTEM(Physics)

	#define gPhysicsLoggerSystem mray::LoggerSystemPhysics::getInstance()
}

#endif // PhysicsLoggerSystem_h__
