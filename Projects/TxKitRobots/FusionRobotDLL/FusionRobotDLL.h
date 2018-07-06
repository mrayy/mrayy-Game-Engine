// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FusionROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FusionROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\FusionRobotDLL\FusionRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\FusionRobotDLL
	file base:	FusionRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FusionRobotDLL__
#define __FusionRobotDLL__


class IRobotController;

#ifdef FusionRobotDLL_EXPORTS
#define FusionROBOTDLL_API extern "C" __declspec(dllexport)
#else
#define FusionROBOTDLL_API __declspec(dllimport)
#endif


namespace mray
{
FusionROBOTDLL_API void  DLL_RobotInit();
FusionROBOTDLL_API IRobotController*  DLL_GetRobotController();
FusionROBOTDLL_API void  DLL_RobotDestroy();
}

#endif