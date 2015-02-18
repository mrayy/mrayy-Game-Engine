


/********************************************************************
	created:	2011/01/17
	created:	17:1:2011   0:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ScriptLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ScriptLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ScriptLoggerSystem_h__
#define ScriptLoggerSystem_h__


#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


	DECLARE_LOGGER_SYSTEM(Script)

#define gScriptLoggerSystem mray::LoggerSystemScript::getInstance()

}

#endif // ScriptLoggerSystem_h__
