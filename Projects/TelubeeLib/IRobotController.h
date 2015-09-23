
#ifndef IRobotController_h__
#define IRobotController_h__

#include <string>
#include <vector>


namespace mray
{
	namespace TBee
	{
		class RobotCapabilities;
	}
}

class ITelubeeRobotListener;
struct RobotStatus;

struct RobotStatus
{
	bool connected;
	float speed[2];	//speed x,y axis
	float rotation;
	float headRotation[4];  	//head rotation  [w,x,y,z] quaternion
	float headPos[3];			//head position [x,y,z]

	RobotStatus()
	{
		speed[0] = speed[1] = 0;
		headPos[0] = headPos[1] = headPos [2]= 0;
		headRotation[0] = 1;
		headRotation[1] = headRotation[2] = headRotation[3] = 0;
		connected = false;
		rotation = 0;
	}
};

class IRobotStatusProvider
{
public:
	virtual void GetRobotStatus(RobotStatus& st) = 0;
};

class IRobotController;


class ITelubeeRobotListener
{
public:

	virtual void OnCollisionData(IRobotController* c, float left, float right){}
	void OnReportMessage(IRobotController* c,int code, const std::string& msg){}

};
enum ERobotControllerStatus
{
	EStopped,		// The robot is not inited
	EIniting,		// The robot is initializing
	EStopping,		// The robot is shutting down
	EDisconnected,	// The robot inited and waiting for connection
	EDisconnecting,	// The robot is disconnecting
	EConnected,		// The robot is connected
	EConnecting,	// The robot is connecting
};
class IRobotController
{
public:

	static const std::string CMD_Start;
	static const std::string CMD_Stop;
	static const std::string CMD_IsStarted;
	static const std::string CMD_GetSensorCount;
	static const std::string CMD_GetSensorValue;
	static const std::string CMD_GetBatteryLevel;
	static const std::string CMD_GetBatteryCharge;

protected:
public:
	IRobotController(){}
	virtual ~IRobotController(){}

	virtual void SetListener(ITelubeeRobotListener* l) = 0;
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) = 0;
	virtual void ConnectRobot() = 0;
	virtual void DisconnectRobot() = 0;
	//virtual bool IsConnected() = 0;
	virtual void UpdateRobotStatus(const RobotStatus& st) = 0;
	virtual std::string ExecCommand(const std::string& cmd, const std::string& args){ return ""; }


	//New Functions 27/7/2015
	virtual ERobotControllerStatus GetRobotStatus() = 0;
	virtual void ShutdownRobot() = 0;
	virtual bool GetJointValues(std::vector<float>& values) = 0;
	virtual void ManualControlRobot() {};

	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return 0; }
};




#endif // IRobotController_h__

