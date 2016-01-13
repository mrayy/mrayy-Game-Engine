

/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\PhantomRobotDLL\PhantomControllerDLL
	file base:	PhantomControllerDLL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhantomControllerDLL_h__
#define PhantomControllerDLL_h__

class IRobotController;

#ifdef PHANTOMROBOTDLL_EXPORTS
#define PHANTOMROBOTDLL_API extern "C" __declspec(dllexport)
#else
#define PHANTOMROBOTDLL_API __declspec(dllimport)
#endif


namespace mray
{
	PHANTOMROBOTDLL_API void  DLL_RobotInit();
	PHANTOMROBOTDLL_API IRobotController*  DLL_GetRobotController();
	PHANTOMROBOTDLL_API void  DLL_RobotDestroy();
}
#endif // PhantomControllerDLL_h__