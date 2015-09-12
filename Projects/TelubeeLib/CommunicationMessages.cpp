
#include "stdafx.h"
#include "CommunicationMessages.h"
#include "mstring.h"


namespace mray
{
namespace TBee
{

	static const core::string EMessageStr[] =
	{
		"Unkown",
		"DepthData" ,
		"DepthSize" ,
		"IsStereo" ,
		"CameraConfig" ,
		"CalibrationDone" ,
		"ReportMessage" ,
		"IRSensorMessage" ,
		"BumpSensorMessage" ,
		"BatteryLevel" ,
		"ClockSync" ,
		"ReinitializeRobot" ,
		"RobotStatus" ,
		"JointValues" ,
		"Detect",
		"Presence "
	}; 
	core::string ToStringMessage(EMessages msg)
	{
		return EMessageStr[(int)msg];
	}
	EMessages ParseMessage(const core::string& msg)
	{
		int cnt = sizeof(EMessageStr) / sizeof(core::string);
		for (int i = 0; i < cnt; ++i)
		{
			if (msg.equals_ignore_case(EMessageStr[i]))
				return (EMessages)i;
		}
		return EMessages::MessageUnkown;
	}
}
}
