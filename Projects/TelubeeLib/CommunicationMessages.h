
#ifndef __COMMUNICATIONMESSAGES__
#define __COMMUNICATIONMESSAGES__

namespace mray
{
namespace TBee
{

enum class EMessages
{
	MessageUnkown=0,
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
	RobotStatus = 12,
	JointValues = 13,
	Detect = 14,
	Presence = 15,
	NetValue = 16,
	AudioConfig = 17,
	CustomMessage = 18,	// When the message contents is not defined other place here

};

core::string ToStringMessage(EMessages msg);
EMessages ParseMessage(const core::string& msg);

}
}


#endif