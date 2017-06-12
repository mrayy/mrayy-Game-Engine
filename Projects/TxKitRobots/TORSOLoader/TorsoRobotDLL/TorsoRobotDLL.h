// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TorsoROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TorsoROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\TorsoRobotDLL\TorsoRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\TorsoRobotDLL
	file base:	TorsoRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TorsoRobotDLL__
#define __TorsoRobotDLL__

class IRobotController;

#ifdef TorsoROBOTDLL_EXPORTS
#define TorsoROBOTDLL_API extern "C" __declspec(dllexport)
#else
#define TorsoROBOTDLL_API __declspec(dllimport)
#endif



namespace mray
{
	TorsoROBOTDLL_API void  DLL_RobotInit();
	TorsoROBOTDLL_API IRobotController*  DLL_GetRobotController();
	TorsoROBOTDLL_API void  DLL_RobotDestroy();
}




#endif