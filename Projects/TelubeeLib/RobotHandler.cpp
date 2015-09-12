

#include "stdafx.h"
#include "RobotHandler.h"



namespace mray
{
namespace TBee
{

	typedef void(*dllFunctionPtr)();
	typedef IRobotController*(*dllLoadRobotFunctionPtr)();


	RobotHandler::RobotHandler()
	{
		m_listener = 0;
		m_localControl = 0;
		m_robotLib = OS::IDllManager::getInstance().getLibrary("Robot.dll");
		if (!m_robotLib)
		{
			gLogManager.log("Failed to load Robot.dll!! Please make sure Robot.dll is placed next to application.", ELL_WARNING);
			m_robotController = 0;
			return;
		}
		dllFunctionPtr libInitPtr;
		dllLoadRobotFunctionPtr robotLoadPtr;
		libInitPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotInit");
		robotLoadPtr = (dllLoadRobotFunctionPtr)m_robotLib->getSymbolName("DLL_GetRobotController");

		libInitPtr();


		m_robotController = robotLoadPtr();

	}
	RobotHandler::~RobotHandler()
	{
		dllFunctionPtr libDestroyPtr;
		libDestroyPtr = 0;
		if (m_robotLib)
		{
			libDestroyPtr = (dllFunctionPtr)m_robotLib->getSymbolName("DLL_RobotDestroy");
		}
		//delete m_robotController;
		if (libDestroyPtr)
			libDestroyPtr();
	}
	void RobotHandler::Initialize()
	{
		if (m_robotController && m_robotController->GetRobotStatus()==ERobotControllerStatus::EStopped)
		{
			gLogManager.log("RobotHandler::Initialize() - Initing Robot", ELL_INFO);
			m_robotController->InitializeRobot(this);
			gLogManager.log("RobotHandler::Initialize() - Finished initing Robot", ELL_INFO);
		}
		if (m_listener)
			m_listener->OnCalibrationDone(this);
	}
	void RobotHandler::GetRobotStatus(RobotStatus& st)
	{

		if (m_listener)
		{
			m_listener->RequestData(this, st);
			_RobotStatus(st);
		}
		else{
			memcpy(&st, &m_robotStatus, sizeof(m_robotStatus));
		}
	}
	void RobotHandler::SetRobotData(const RobotStatus &st)
	{
		//if (m_localControl)
		_RobotStatus(st);
	}

	void RobotHandler::_RobotStatus(const RobotStatus& st)
	{
		if (!m_robotController)
			return;
		ERobotControllerStatus status = m_robotController->GetRobotStatus();
		if ((st.connected || m_localControl) && status != ERobotControllerStatus::EConnected)
		{
			if (status == ERobotControllerStatus::EStopped)
				m_robotController->InitializeRobot(this);
			m_robotController->ConnectRobot();
		}


		if ((!st.connected && !m_localControl) && status != ERobotControllerStatus::EDisconnected)
		{
			if (status != ERobotControllerStatus::EStopped)
			{
				m_robotController->DisconnectRobot();
			}
		}

		m_robotController->UpdateRobotStatus(st);
		if (m_listener)
			m_listener->OnRobotStatus(this, st);
	}

	void RobotHandler::OnCollisionData(float left, float right)
	{
		if (m_listener)
		{
			m_listener->OnCollisionData(this, left, right);
		}
	}

	void RobotHandler::OnReportMessage(int code, const std::string& msg)
	{
		if (m_listener)
		{
			m_listener->OnReportMessage(this, code, msg);
		}

	}
}
}

