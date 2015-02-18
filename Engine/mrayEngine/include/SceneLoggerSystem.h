

/********************************************************************
	created:	2011/01/17
	created:	17:1:2011   0:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SceneLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SceneLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SceneLoggerSystem_h__
#define SceneLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


	DECLARE_LOGGER_SYSTEM(Scene)

#define gSceneLoggerSystem mray::LoggerSystemScene::getInstance()

}

#endif // SceneLoggerSystem_h__

