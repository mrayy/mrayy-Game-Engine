// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RemoteROBOTDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RemoteROBOTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\RemoteRobotDLL\RemoteRobotDLL.h
	file path:	C:\Development\mrayEngine\Projects\RemoteRobotDLL
	file base:	RemoteRobotDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RemoteRobotDLL__
#define __RemoteRobotDLL__

class IRobotController;

#ifdef RemoteROBOTDLL_EXPORTS
#define RemoteROBOTDLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define RemoteROBOTDLL_EXPORTS __declspec(dllimport)
#endif



namespace mray
{
	RemoteROBOTDLL_EXPORTS void  DLL_RobotInit();
	RemoteROBOTDLL_EXPORTS IRobotController*  DLL_GetRobotController();
	RemoteROBOTDLL_EXPORTS void  DLL_RobotDestroy();
}




#endif