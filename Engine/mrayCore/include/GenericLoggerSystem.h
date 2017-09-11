

/********************************************************************
	created:	2011/01/02
	created:	2:1:2011   23:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GenericLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GenericLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GenericLoggerSystem_h__
#define GenericLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{

DECLARE_LOGGER_SYSTEM(Generic)

#define gGenericLoggerSystem mray::LoggerSystemGeneric::getInstance()

}


#endif // GenericLoggerSystem_h__
