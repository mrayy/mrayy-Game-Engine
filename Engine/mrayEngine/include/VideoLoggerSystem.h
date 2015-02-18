


/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   10:15
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VideoLoggerSystem.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VideoLoggerSystem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VideoLoggerSystem__
#define __VideoLoggerSystem__

#include "ISingleton.h"
#include "LogSubsystem.h"

namespace mray
{


	DECLARE_LOGGER_SYSTEM(Video)

#define gVideoLoggerSystem mray::LoggerSystemVideo::getInstance()

}

#endif
