// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TELUBEEROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TELUBEEROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotDLL\TelubeeRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotDLL
	file base:	TelubeeRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TelubeeRobotDLL__
#define __TelubeeRobotDLL__


class IRobotController;

#ifdef TELUBEEROBOTDLL_EXPORTS
#define TELUBEEROBOTDLL_API extern "C" __declspec(dllexport)
#else
#define TELUBEEROBOTDLL_API __declspec(dllimport)
#endif


namespace mray
{
TELUBEEROBOTDLL_API void  DLL_RobotInit();
TELUBEEROBOTDLL_API IRobotController*  DLL_GetRobotController();
TELUBEEROBOTDLL_API void  DLL_RobotDestroy();
}

#endif