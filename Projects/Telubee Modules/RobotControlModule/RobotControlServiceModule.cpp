
#include "stdafx.h"
#include "RobotControlServiceModule.h"
#include "TBeeServiceContext.h"
#include "IRobotController.h"
#include "RobotHandler.h"
#include "CommunicationMessages.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "ModuleSharedMemory.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "MutexLocks.h"

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(RobotControlServiceModule, IServiceModule)
	const std::string RobotControlServiceModule::ModuleName("RobotControlServiceModule");


class RobotControlServiceModuleImpl :public IServiceContextListener, public IRobotHandlerListener
{
public:


	class RobotControlThread :public OS::IThreadFunction
	{
		RobotControlServiceModuleImpl* owner;
	public:
		RobotControlThread(RobotControlServiceModuleImpl* o){
			owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (!owner->_ProcessThread())
				{
					break;
				}
			}
		}
	};
	RobotStatus m_robotData;

	math::vector2d m_collision;
	RobotHandler* m_RobotHandler;

	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	OS::IThreadPtr m_thread;
	OS::IMutex* m_dataMutex;

	bool m_connected;


	bool _ProcessThread()
	{
		if (m_status == EServiceStatus::Idle)
			return false;
		if (m_status != EServiceStatus::Running)
		{
			OS::IThreadManager::getInstance().sleep(100);
			return true;
		}

		m_RobotHandler->SetRobotData(m_robotData);
		return true;
	}

	virtual bool RequestData(RobotHandler* r, RobotStatus& status)
	{
// 		if (m_connected && m_RobotHandler->GetRobotController()->GetRobotStatus() == ERobotControllerStatus::EStopped)
// 			m_RobotHandler->Initialize();
// 		TBee::SharedMemoryLock m(m_context->sharedMemory);
// 		OS::ScopedLock l(m_dataMutex);
		if (m_dataMutex->tryLock())
		{
			memcpy(&status, &m_robotData, sizeof(status));
			m_dataMutex->unlock();
			return true;
		}
		return false;
		//if (m_connected)
		//	r->SetRobotData(m_robotData);
	}

public:

	virtual void OnCalibrationDone(RobotHandler* sender)
	{

		if (m_context->commChannel)
		{
			const int BufferLen = 64;
			uchar buffer[BufferLen];
			OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
			int reply = (int)EMessages::DepthSize;
			int len = stream.write(&reply, sizeof(reply));
			m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
		}
	};

	virtual void OnCollisionData(RobotHandler* sender, float left, float right)
	{
		m_collision.x = left;
		m_collision.y = right;
	}


	virtual void OnReportMessage(RobotHandler* sender, int code, const core::string& msg){
		const int BufferLen = 512;
		uchar buffer[BufferLen];
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		int reply = (int)EMessages::ReportMessage;
		int len = stream.write(&reply, sizeof(reply));
		len += wrtr.binWriteInt(code);
		len += wrtr.binWriteString(msg);
		m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
	}
	virtual void OnRobotStatus(RobotHandler* sender, const RobotStatus& status)
	{
	};
public:

	RobotControlServiceModuleImpl()
	{
		m_RobotHandler = 0;
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_connected = false;

	}

	~RobotControlServiceModuleImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_status = EServiceStatus::Inited;
		m_context = context;

		//gLogManager.log("Initializing RobotHandler",ELL_INFO);
		m_RobotHandler = new RobotHandler();
		m_RobotHandler->SetListener(this);

		m_dataMutex = OS::IThreadManager::getInstance().createMutex();

		//m_RobotHandler->Initialize();

		context->AddListener(this);
//  		m_thread = OS::IThreadManager::getInstance().createThread(new RobotControlThread(this), OS::ETP_Normal);
//  		m_thread->start(0);
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_status = EServiceStatus::Idle;
		m_context->RemoveListener(this);

//  		m_thread->terminate();
//  		OS::IThreadManager::getInstance().killThread(m_thread);
//  		m_thread = 0;

		if (m_RobotHandler->GetRobotController())
			m_RobotHandler->GetRobotController()->ShutdownRobot();

		delete m_RobotHandler;
		m_RobotHandler = 0;

		delete m_dataMutex;
		m_dataMutex = 0;

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

// 		m_RobotHandler->Initialize();
// 		if (m_RobotHandler->GetRobotController())
// 			m_RobotHandler->GetRobotController()->ConnectRobot();

		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		
		if (m_RobotHandler->GetRobotController())
			m_RobotHandler->GetRobotController()->DisconnectRobot();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;


		if (m_RobotHandler->IsLocalControl())
		{
			math::vector2d speed;
			float rotation;
			/*
			controllers::IJoysticController* joystick = m_inputManager->getJoystick(0);
			if (joystick)
			{
				RobotStatus st;
				controllers::JoysticAxis x = joystick->getAxisState(0);
				controllers::JoysticAxis y = joystick->getAxisState(1);
				controllers::JoysticAxis r = joystick->getAxisState(3);

				st.speed[0] = x.abs;
				st.speed[1] = y.abs;
				st.rotation = r.abs;

				st.connected = true;
				st.headRotation[0] = 1;
				st.headRotation[1] = 0;
				st.headRotation[2] = 0;
				st.headRotation[3] = 0;
				m_RobotHandler->SetRobotData(st);
				}*/

			if (m_RobotHandler->GetRobotController() && m_RobotHandler->GetRobotController()->GetRobotStatus() == ERobotControllerStatus::EConnected && m_connected)
			{
#if SEND_ROBOT_SENSORS
				const int BufferLen = 128;
				uchar buffer[BufferLen];
				//tell the client if we are sending stereo or single video images

				OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
				OS::StreamWriter wrtr(&stream);
				{
					int reply = (int)EMessages::BumpSensorMessage;
					int len = stream.write(&reply, sizeof(reply));
					bool leftBump = core::StringConverter::toBool(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "0"));
					bool rightBump = core::StringConverter::toBool(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "1"));
					int count = 2;
					len += stream.write(&count, sizeof(count));
					len += stream.write(&leftBump, sizeof(leftBump));
					len += stream.write(&rightBump, sizeof(rightBump));
					m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
				}
				{
					stream.seek(0, OS::ESeek_Set);
					int reply = (int)EMessages::IRSensorMessage;
					int len = stream.write(&reply, sizeof(reply));
					float ir[6];
					ir[0] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "2"));
					ir[1] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "3"));
					ir[2] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "4"));
					ir[3] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "5"));
					ir[4] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "6"));
					ir[5] = core::StringConverter::toFloat(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "7"));
					int count = 6;
					len += stream.write(&count, sizeof(count));
					for (int i = 0; i < 6; ++i)
						len += wrtr.binWriteFloat(ir[i]);
					m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
				}
				{
					stream.seek(0, OS::ESeek_Set);
					int reply = (int)EMessages::BatteryLevel;
					int len = stream.write(&reply, sizeof(reply));
					int batt = core::StringConverter::toInt(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetBatteryLevel, ""));
					len += wrtr.txtWriteInt(batt);
					m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
				}
#endif 
			}
		}
	}
	void DebugRender(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

		if (!m_RobotHandler->GetRobotController())
			return;


		char buffer[512];
		float yoffset = 50;

		core::string msg;
		{
			ERobotControllerStatus st = m_RobotHandler->GetRobotController()->GetRobotStatus();
			msg = core::string("Robot Status: ");
			if (st == EStopped)msg += "Stopped";
			if (st == EDisconnected)msg += "Disconnected";
			if (st == EConnecting)msg += "Connecting";
			if (st == EDisconnecting)msg += "Disconnecting";
			if (st == EConnected)msg += "Connected";
			context->RenderText(msg, 50, 0);
		}
		msg = core::string("User Controlling: ") + (m_robotData.connected ? "Yes" : "No");
		context->RenderText(msg, 50, 0);
		msg = core::string("Controlling: ") + (m_RobotHandler->IsLocalControl() ? "Local" : "Remote");
		context->RenderText(msg, 50, 0);
		msg = core::string("Sensors : ") + core::StringConverter::toString(math::vector2d(m_collision));
		context->RenderText(msg, 50, 0);
		if (m_robotData.connected || m_RobotHandler->IsLocalControl())
		{

			sprintf_s(buffer, "%-2.2f, %-2.2f", m_robotData.speed[0], m_robotData.speed[1]);
			msg = core::string("Speed: ") + buffer;
			context->RenderText(msg, 100, 0);

			msg = core::string("Rotation: ") + core::StringConverter::toString(m_robotData.rotation,2);
			context->RenderText(msg, 100, 0);

			math::vector3d angles;
			math::quaternion q(m_robotData.headRotation[0], m_robotData.headRotation[3],
				m_robotData.headRotation[1], m_robotData.headRotation[2]);
			q.toEulerAngles(angles);
			angles.set(angles.y, angles.z, angles.x);
			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", angles.x, angles.y, angles.z);
			msg = core::string("Head Rotation: ") + buffer;
			context->RenderText(msg, 100, 0);

			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", m_robotData.headPos[0], m_robotData.headPos[1], m_robotData.headPos[2]);
			msg = core::string("Head Position: ") + buffer;
			context->RenderText(msg, 100, 0);

		}


		{
			msg = "Robot Started: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_IsStarted, "");
			context->RenderText(msg, 100, 0);
			if (false)
			{
				msg = "Bump Left: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "0");
				context->RenderText(msg, 100, 0);
				msg = "Bump Right: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "1");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Left: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "2");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Front Left: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "3");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Center Left: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "4");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Center Right: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "5");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Front Right: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "6");
				context->RenderText(msg, 100, 0);
				msg = "Sensor Light Right: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, "7");
				context->RenderText(msg, 100, 0);
				msg = "Battery Level: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetBatteryLevel, "");
				context->RenderText(msg, 100, 0);
				msg = "Battery Status: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetBatteryCharge, "");
				context->RenderText(msg, 100, 0);
			}
			std::vector<float> jvalues;

			m_RobotHandler->GetRobotController()->GetJointValues(jvalues);
			context->RenderText(core::string("Robot Joint Values:"), 50, 0);
						
			msg = "";
			context->RenderText("   \tIK\t/ Real", 100, 0);
			for (int i = 0; i < jvalues.size(); i += 2)
			{
				
				sprintf_s(buffer, "\t%-2.2f\t/ %-2.2f", jvalues[i], jvalues[i+1]);
				msg = core::string("J[") + core::StringConverter::toString(i / 2) + "]:" + buffer;
				context->RenderText(msg, 100, 0);
			}
			
		}

	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

		if (m_RobotHandler->GetRobotController() )
		{
			IRobotController* rc = m_RobotHandler->GetRobotController();
			int batt = core::StringConverter::toInt(rc->ExecCommand(IRobotController::CMD_GetBatteryLevel, ""));
			video::SColor fontColor = video::SColor(0, 1, 0, 1);
			if (batt < 20)
				fontColor = video::SColor(1, 0, 0, 1);
			
			core::string msg;
			msg = core::string("Battery Level : ") + core::StringConverter::toString(batt) + "%";
			context->RenderText(msg, 0, 0, fontColor);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		// change main thread priority to max
		HANDLE thread = GetCurrentThread();
		SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
		//Sleep until other threads loads
		Sleep(3000);
		m_RobotHandler->Initialize();
		// change it back to normal
		SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);
		m_connected = true;
	}
	virtual void OnUserDisconnected()
	{
		RobotStatus st;
		m_connected = false;
		//m_RobotHandler->SetRobotData(st);
		if (m_RobotHandler->GetRobotController() != 0)
		{
			ERobotControllerStatus status = m_RobotHandler->GetRobotController()->GetRobotStatus();
			m_RobotHandler->GetRobotController()->ShutdownRobot();
		}
	}
	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{
		const int BufferLen = 65537;
		uchar buffer[BufferLen];
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

		std::vector<core::string> vals;
		vals = core::StringUtil::Split(value, ",");


		if (msg.equals_ignore_case("query"))
		{
			if (m_RobotHandler->GetRobotController() != 0)
			{
				ERobotControllerStatus st = m_RobotHandler->GetRobotController()->GetRobotStatus();
				int reply = (int)EMessages::RobotStatus;
				int len = stream.write(&reply, sizeof(reply));
				len += stream.write(&st, sizeof(st));
				m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
			}
		}
		else if (msg.equals_ignore_case("RobotConnect") && vals.size() == 1)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData.connected = core::StringConverter::toBool(vals[0].c_str());
		}
		else if (msg.equals_ignore_case("Speed") && vals.size() == 2)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData.speed[0] = atof(vals[0].c_str());
			m_robotData.speed[1] = atof(vals[1].c_str());
			//limit the speed
			m_robotData.speed[0] = -math::clamp<float>(m_robotData.speed[0], -1, 1);
			m_robotData.speed[1] = math::clamp<float>(m_robotData.speed[1], -1, 1);

		}
		else if (msg.equals_ignore_case("HeadRotation") && vals.size() == 4)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData.headRotation[0] = atof(vals[0].c_str());
			m_robotData.headRotation[1] = atof(vals[1].c_str());
			m_robotData.headRotation[2] = atof(vals[2].c_str());
			m_robotData.headRotation[3] = atof(vals[3].c_str());


			//do head limits
			// 		m_robotData.tilt = math::clamp(m_robotData.tilt, -50.0f, 50.0f);
			// 		m_robotData.yaw = math::clamp(m_robotData.yaw, -70.0f, 70.0f);
			// 		m_robotData.roll = math::clamp(m_robotData.roll, -40.0f, 40.0f);
		}
		else if (msg.equals_ignore_case("HeadPosition") && vals.size() == 3)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData.headPos[0] = atof(vals[0].c_str());
			m_robotData.headPos[1] = atof(vals[1].c_str());
			m_robotData.headPos[2] = atof(vals[2].c_str());

		}
		else if (msg.equals_ignore_case("Rotation") && vals.size() == 1)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData.rotation = atof(vals[0].c_str());
			m_robotData.rotation = math::clamp<float>(m_robotData.rotation, -1, 1);
		}
		else if (msg.equals_ignore_case("jointVals"))
		{
			if (m_RobotHandler->GetRobotController() != 0)
			{
				std::vector<float> vals;
				m_RobotHandler->GetRobotController()->GetJointValues(vals);
				int reply = (int)EMessages::JointValues;
				int len = stream.write(&reply, sizeof(reply));
				int count = vals.size();
				len += stream.write(&count, sizeof(count));
				for (int i = 0; i < count; ++i)
					len += wrtr.binWriteFloat(vals[i]);
				m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
			}
		}
		else if (msg.equals_ignore_case("reinit"))
		{
			if (m_RobotHandler->GetRobotController() != 0)
			{
				ERobotControllerStatus status = m_RobotHandler->GetRobotController()->GetRobotStatus();
				m_RobotHandler->GetRobotController()->ShutdownRobot();
			}
		}
	}
};


RobotControlServiceModule::RobotControlServiceModule()
{
	m_impl = new RobotControlServiceModuleImpl();
}

RobotControlServiceModule::~RobotControlServiceModule()
{
	delete m_impl;
}


std::string RobotControlServiceModule::GetServiceName()
{
	return RobotControlServiceModule::ModuleName;
}

EServiceStatus RobotControlServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void RobotControlServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus RobotControlServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool RobotControlServiceModule::StopService()
{
	return m_impl->Stop();
}

void RobotControlServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void RobotControlServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void RobotControlServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void RobotControlServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool RobotControlServiceModule::LoadServiceSettings(xml::XMLElement* e)
{
	return true;
}

void RobotControlServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


