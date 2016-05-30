
#ifndef __NOVINTCONTROLLER__
#define __NOVINTCONTROLLER__

#include <string>
#include "IRobotController.h"
#include "RobotCapabilities.h"
#include "PIDController.h"
#include <windows.h>

#include <hdl/hdl.h>
#include <hdlu/hdlu.h>


class NovintController :public IRobotController
{
protected:
	// Handle to device
	HDLDeviceHandle m_deviceHandle;
	ERobotControllerStatus _status;
	IRobotStatusProvider* m_robotStatusProvider;
	ITelubeeRobotListener* listener;
	HANDLE m_robotThread;
	bool isDone;
	bool threadStart;

	mray::PIDController* _pidController[3];

	mray::math::vector3d _position;

	mray::TBee::RobotCapabilities m_caps;

	static DWORD timerThreadRobot(NovintController *robot, LPVOID pdata);

	void _ProcessRobot();
	void _processData();
	void _updatePosition();
	void testHDLError(const char* str);
	static HDLServoOpExitCode ContactCB(void* pUserData);

public:
	NovintController();
	virtual ~NovintController();

	void SetListener(ITelubeeRobotListener* l);
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider);
	void ConnectRobot();
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

	virtual ERobotControllerStatus GetRobotStatus();
	virtual void ShutdownRobot();
	virtual bool GetJointValues(std::vector<float>& values);
	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return &m_caps; }


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args);
	virtual void ParseParameters(const std::map<std::string, std::string>& valueMap);

	virtual void tuningMode() {};

};



#endif
