


/********************************************************************
	created:	2011/01/17
	created:	17:1:2011   0:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SoundLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SoundLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SoundLoggerSystem_h__
#define SoundLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


	DECLARE_LOGGER_SYSTEM(Sound)

#define gSoundLoggerSystem mray::LoggerSystemSound::getInstance()

}
#endif // SoundLoggerSystem_h__
