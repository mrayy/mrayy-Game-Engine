
#ifndef IRobotController_h__
#define IRobotController_h__

#include <string>

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
	virtual void GetRobotStatus(RobotStatus& st)const = 0;
};

class ITelubeeRobotListener
{
public:

	virtual void OnCollisionData(float left, float right){}
	void OnReportMessage(int code, const std::string& msg){}
};

class IRobotController
{
protected:
public:
	IRobotController(){}
	virtual ~IRobotController(){}

	virtual void SetListener(ITelubeeRobotListener* l) = 0;
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) = 0;
	virtual void ConnectRobot() = 0;
	virtual void DisconnectRobot() = 0;
	virtual bool IsConnected() = 0;
	virtual void UpdateRobotStatus(const RobotStatus& st) = 0;

};


#endif // IRobotController_h__

