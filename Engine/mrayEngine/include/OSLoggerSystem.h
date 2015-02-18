


/********************************************************************
	created:	2011/01/16
	created:	16:1:2011   23:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\OSLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	OSLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef OSLoggerSystem_h__
#define OSLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"


namespace mray
{


	DECLARE_LOGGER_SYSTEM(OS)

#define gOSLoggerSystem mray::LoggerSystemOS::getInstance()

}

#endif // OSLoggerSystem_h__
