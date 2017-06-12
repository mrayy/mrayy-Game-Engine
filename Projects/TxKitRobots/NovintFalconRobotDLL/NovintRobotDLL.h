// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NovintROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NovintROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\NovintRobotDLL\NovintRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\NovintRobotDLL
	file base:	NovintRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __NovintRobotDLL__
#define __NovintRobotDLL__


class IRobotController;

#ifdef NOVINTFALCONROBOTDLL_EXPORTS
#define NovintROBOTDLL_API extern "C" __declspec(dllexport)
#else
#define NovintROBOTDLL_API __declspec(dllimport)
#endif


namespace mray
{
NovintROBOTDLL_API void  DLL_RobotInit();
NovintROBOTDLL_API IRobotController*  DLL_GetRobotController();
NovintROBOTDLL_API void  DLL_RobotDestroy();
}

#endif