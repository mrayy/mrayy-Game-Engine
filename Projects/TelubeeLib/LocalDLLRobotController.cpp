
#include "stdafx.h"
#include "LocalDLLRobotController.h"
#include "IDllManager.h"


namespace mray
{
namespace TBee
{


	typedef void(*dllFunctionPtr)();
	typedef IRobotController*(*dllLoadRobotFunctionPtr)();
LocalDLLRobotController::LocalDLLRobotController()
{
	m_robotLib = OS::IDllManager::getInstance().getLibrary("Robot.dll");
	if (m_robotLib)
	{
		dllFunctionPtr libInitPtr;
		dllLoadRobotFunctionPtr robotLoadPtr;
		libInitPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotInit");
		robotLoadPtr = (dllLoadRobotFunctionPtr)m_robotLib->getSymbolName("DLL_GetRobotController");

		libInitPtr();
		m_controller = robotLoadPtr();
	}else
		m_controller=0;

}

LocalDLLRobotController::~LocalDLLRobotController()
{
	dllFunctionPtr libDestroyPtr;
	libDestroyPtr = 0;
	if (m_robotLib)
	{
		libDestroyPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotDestroy");
	}
	if (libDestroyPtr)
		libDestroyPtr();

}


void LocalDLLRobotController::SetListener(ITelubeeRobotListener* l)
{
	if (m_controller)
		m_controller->SetListener(l);
}

void LocalDLLRobotController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	if (m_controller)
		m_controller->InitializeRobot(robotStatusProvider);
}

void LocalDLLRobotController::ConnectRobot()
{
	if (m_controller)
		m_controller->ConnectRobot();

}

void LocalDLLRobotController::DisconnectRobot()
{
	if (m_controller)
		m_controller->DisconnectRobot();
}

void LocalDLLRobotController::UpdateRobotStatus(const RobotStatus& st)
{
	if (m_controller)
		m_controller->UpdateRobotStatus(st);
}

std::string LocalDLLRobotController::ExecCommand(const std::string& cmd, const std::string& args)
{
	if (m_controller)
		return m_controller->ExecCommand(cmd,args);
	return "";
}



ERobotControllerStatus LocalDLLRobotController::GetRobotStatus()
{
	if (m_controller)
		return m_controller->GetRobotStatus();
	return ERobotControllerStatus::EDisconnected;

}

void LocalDLLRobotController::ShutdownRobot()
{
	if (m_controller)
		m_controller->ShutdownRobot();

}

bool LocalDLLRobotController::GetJointValues(std::vector<float>& values)
{

	if (m_controller)
		return m_controller->GetJointValues(values);
	return false;
}

void LocalDLLRobotController::ManualControlRobot()
{
	if (m_controller)
		m_controller->ManualControlRobot();

}


}
}

