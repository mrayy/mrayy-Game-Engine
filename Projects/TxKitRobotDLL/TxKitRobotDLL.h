// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TxKitRobotDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TxKitRobotDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\TxKitRobotDLL\TxKitRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\TxKitRobotDLL
	file base:	TxKitRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TxKitRobotDLL__
#define __TxKitRobotDLL__


class IRobotController;

#ifdef TxKitRobotDLL_EXPORTS
#define TxKitRobotDLL_API extern "C" __declspec(dllexport)
#else
#define TxKitRobotDLL_API __declspec(dllimport)
#endif


namespace mray
{
TxKitRobotDLL_API void  DLL_RobotInit();
TxKitRobotDLL_API IRobotController*  DLL_GetRobotController();
TxKitRobotDLL_API void  DLL_RobotDestroy();
}

#endif