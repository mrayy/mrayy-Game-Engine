


/********************************************************************
	created:	2011/01/03
	created:	3:1:2011   22:15
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AnimationLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AnimationLoggerSystem_h__
#define AnimationLoggerSystem_h__


#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


	DECLARE_LOGGER_SYSTEM(Animation)

#define gAnimationLoggerSystem mray::LoggerSystemAnimation::getInstance()

}
#endif // AnimationLoggerSystem_h__
