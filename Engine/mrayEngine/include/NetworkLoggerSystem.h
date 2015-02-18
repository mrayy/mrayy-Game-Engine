


/********************************************************************
	created:	2011/01/16
	created:	16:1:2011   23:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\NetworkLoggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	NetworkLoggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef NetworkLoggerSystem_h__
#define NetworkLoggerSystem_h__

#include "ISingleton.h"
#include "LogSubsystem.h"


namespace mray
{


	DECLARE_LOGGER_SYSTEM(Network)

#define gNetworkLoggerSystem mray::LoggerSystemNetwork::getInstance()

}

#endif // NetworkLoggerSystem_h__
