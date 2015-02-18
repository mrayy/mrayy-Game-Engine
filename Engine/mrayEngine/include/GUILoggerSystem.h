


/********************************************************************
	created:	2011/01/02
	created:	2:1:2011   23:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUILoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUILoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUILoggerSystem_h__
#define GUILoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


DECLARE_LOGGER_SYSTEM(GUI)

#define gGUILoggerSystem mray::LoggerSystemGUI::getInstance()

}
#endif // GUILoggerSystem_h__
