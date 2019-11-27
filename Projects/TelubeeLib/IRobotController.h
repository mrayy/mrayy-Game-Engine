
#ifndef IRobotController_h__
#define IRobotController_h__

#include <string>
#include <vector>
#include <map>


namespace mray
{
	namespace TBee
	{
		class RobotCapabilities;
		class ServiceRenderContext;
	}
}

class ITelubeeRobotListener;
struct RobotStatus;

class JointData
{
public:
	 JointData()
	 {
	}

	mray::math::vector3d pos;
	mray::math::quaternion ori;
};

struct RobotStatus
{
	bool connected;
	float speed[2];	//speed x,y axis
	float rotation;
	JointData head;
	JointData leftHand;
	JointData rightHand;

	float customAngles[256];

	RobotStatus()
	{
		speed[0] = speed[1] = 0;
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
	virtual void ForceShutdownRobot() { ShutdownRobot(); }
	virtual bool GetJointValues(std::vector<float>& values) = 0;
	virtual void ManualControlRobot() {};

	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return 0; }

	virtual void ParseParameters(const std::map<std::string, std::string>& valueMap){}

	virtual void tuningMode() = 0;
	virtual void DebugRender(mray::TBee::ServiceRenderContext* context) {};
};




#endif // IRobotController_h__

