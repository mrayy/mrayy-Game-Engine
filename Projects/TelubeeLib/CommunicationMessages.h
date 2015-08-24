
#ifndef __COMMUNICATIONMESSAGES__
#define __COMMUNICATIONMESSAGES__

namespace mray
{

	enum class EMessages
	{
		DepthData = 1,
		DepthSize = 2,
		IsStereo = 3,
		CameraConfig = 4,
		CalibrationDone = 5,
		ReportMessage = 6,
		IRSensorMessage = 7,
		BumpSensorMessage = 8,
		BatteryLevel = 9,
		ClockSync = 10,
		ReinitializeRobot = 11,
		RobotStatus=12,
		JointValues=13
	};

}


#endif