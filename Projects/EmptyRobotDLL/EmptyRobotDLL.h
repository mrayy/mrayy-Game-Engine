// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EmptyRobotDll_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EmptyRobotDll_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\EmptyRobotDll\EmptyRobotDll.h
	file path:	C:\Development\mrayEngine\Projects\EmptyRobotDll
	file base:	EmptyRobotDll
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __EmptyRobotDll__
#define __EmptyRobotDll__


class IRobotController;

#ifdef EmptyRobotDll_EXPORTS
#define EmptyRobotDll_API extern "C" __declspec(dllexport)
#else
#define EmptyRobotDll_API __declspec(dllimport)
#endif


namespace mray
{
EmptyRobotDll_API void  DLL_RobotInit();
EmptyRobotDll_API IRobotController*  DLL_GetRobotController();
EmptyRobotDll_API void  DLL_RobotDestroy();
}

#endif