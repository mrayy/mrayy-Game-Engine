
#include "stdafx.h"
#include "NovintController.h"
#include "ILogManager.h"


using namespace mray;

NovintController::NovintController() 
{
	float kp = 300, ki = 50, kd = 1;

	_pidController[0] = new PIDController(kp, ki, kd, 0.2);
	_pidController[1] = new PIDController(kp, ki, kd, 0.2);
	_pidController[2] = new PIDController(kp*3.5f, ki, kd, 0.2);
	m_robotStatusProvider = 0;
	listener = 0;
	isDone = false;
	_status = ERobotControllerStatus::EStopped;
	m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);

}
NovintController::~NovintController()
{
	isDone = true;
	threadStart = false;
	Sleep(100);
	TerminateThread(m_robotThread, 0);
	ShutdownRobot();
}
void NovintController::testHDLError(const char* str)
{
	HDLError err = hdlGetError();
	if (err != HDL_NO_ERROR)
	{
		gLogManager.log(str, ELL_ERROR);
		return;
	}
}


DWORD NovintController::timerThreadRobot(NovintController *robot, LPVOID pdata){
	int count = 0;
	while (!robot->isDone){
		robot->_ProcessRobot();
		Sleep(1);
		if (!robot->threadStart)
			Sleep(100);
		else {
		}
	}
	return 0;
}
void NovintController::SetListener(ITelubeeRobotListener* l)
{
	listener = l;
}

HDLServoOpExitCode NovintController::ContactCB(void* pUserData) {
	// Get pointer to haptics object 
	NovintController* controller = static_cast< NovintController* >(pUserData); 
	//controller->_updatePosition();
	return HDL_SERVOOP_CONTINUE; 
}

void NovintController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	m_robotStatusProvider = robotStatusProvider;

	HDLError err = HDL_NO_ERROR;
	gLogManager.log("Trying to start Falcon device", ELL_INFO);

	// Passing "DEFAULT" or 0 initializes the default device based on the
	// [DEFAULT] section of HDAL.INI.   The names of other sections of HDAL.INI
	// could be passed instead, allowing run-time control of different devices
	// or the same device with different parameters.  See HDAL.INI for details.
	m_deviceHandle = hdlInitNamedDevice("DEFAULT");
	testHDLError("hdlInitDevice");

	if (m_deviceHandle == HDL_INVALID_HANDLE)
	{
		gLogManager.log("Could not open device - Device Failure", ELL_ERROR);
		return;
	}

	hdlStart();

	hdlMakeCurrent(m_deviceHandle);
	testHDLError("hdlMakeCurrent");

	double m_workspaceDims[6];

	hdlDeviceWorkspace(m_workspaceDims);
	testHDLError("hdlDeviceWorkspace");
	// Set up callback function 
	HDLOpHandle m_servoOp = hdlCreateServoOp(ContactCB, this, false);

	gLogManager.log("Falcon Device started successfully", ELL_INFO);
	_status = EDisconnected;

}


void NovintController::ConnectRobot()
{
	if (GetRobotStatus() != EDisconnected)
		return;
	gLogManager.log("Connecting Robot\n", ELL_INFO);
	_status = EConnecting;

}
void NovintController::DisconnectRobot()
{
	if (_status != EConnected)
		return;
	gLogManager.log("Disconnecting Robot\n", ELL_INFO);
	_status = EDisconnecting;
}
//bool IsConnected();
void NovintController::UpdateRobotStatus(const RobotStatus& st)
{

	if (GetRobotStatus() == EConnected && !st.connected)
	{
		DisconnectRobot();
	}
	else if (GetRobotStatus() != EConnected && st.connected)
	{
		ConnectRobot();
	}


	if (st.connected)
		threadStart = true;
	else
		threadStart = false;
	if (GetRobotStatus() != EConnected)
		return;

	_position.set(st.headPos[0], st.headPos[2], st.headPos[1]);
}

ERobotControllerStatus NovintController::GetRobotStatus()
{
	return _status;
}
void NovintController::ShutdownRobot()
{
	if (_status == EStopped || _status == EStopping)
		return;
	_status = EStopping;

	hdlStop();
	if (m_deviceHandle != HDL_INVALID_HANDLE)
	{
		hdlUninitDevice(m_deviceHandle);
		m_deviceHandle = HDL_INVALID_HANDLE;
	}
}
bool NovintController::GetJointValues(std::vector<float>& values)
{
	return false;
}


std::string NovintController::ExecCommand(const std::string& cmd, const std::string& args)
{
	return "";
}
void NovintController::ParseParameters(const std::map<std::string, std::string>& valueMap)
{

}


void NovintController::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}


void NovintController::_updatePosition()
{
	if (_status != EConnected)
		return;
	math::Point3d<double> targetPos = _position;
	math::Point3d<double> currentPos;
	math::Point3d<double> dpPos;

	double pos[3];
	hdlToolPosition(pos);

	currentPos.set(pos[0], pos[1], pos[2]);
	dpPos.x = _pidController[0]->SetValue(currentPos.x, targetPos.x);
	dpPos.y = _pidController[1]->SetValue(currentPos.y, targetPos.y);
	dpPos.z = _pidController[2]->SetValue(currentPos.z, targetPos.z);

	hdlSetToolForce(&dpPos.x);

}
void NovintController::_ProcessRobot()
{
	int ret = 0;

	if (_status != EDisconnecting || _status != EDisconnected)
		_processData();

	bool ok = false;
	switch (_status)
	{
	case ERobotControllerStatus::EIniting:
		break;
	case ERobotControllerStatus::EConnecting:
		ok = true;
		if (ok)
			_status = ERobotControllerStatus::EConnected;
		break;
	case ERobotControllerStatus::EConnected:

		_updatePosition();
		break;
	case ERobotControllerStatus::EDisconnecting:
		gLogManager.log("Disconnecting Robot", ELL_INFO);
		_status = EDisconnected;
		break;
	case ERobotControllerStatus::EDisconnected:
		break;
	case ERobotControllerStatus::EStopping:
		_status = ERobotControllerStatus::EStopped;
		break;
	case ERobotControllerStatus::EStopped:
		break;
	}
	if (threadStart){
	}


}
