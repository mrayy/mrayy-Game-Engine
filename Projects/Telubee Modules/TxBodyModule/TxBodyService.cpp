
#include "stdafx.h"
#include "TxBodyService.h"
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

IMPLEMENT_RTTI(TxBodyService, IServiceModule)
	const std::string TxBodyService::ModuleName("TxBodyService");


class TxBodyServiceImpl :public IServiceContextListener, public IRobotHandlerListener
{
public:

/*

	class RobotControlThread :public OS::IThreadFunction
	{
		TxBodyServiceImpl* owner;
	public:
		RobotControlThread(TxBodyServiceImpl* o){
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
	}*/
	RobotStatus *m_robotData;

	math::vector2d m_collision;
	RobotHandler* m_RobotHandler;

	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	OS::IThreadPtr m_thread;
	OS::IMutex* m_dataMutex;
	std::map<std::string, std::string> m_robotValueMap;
	bool m_connected;
	core::string m_robotDll;



	virtual bool RequestData(RobotHandler* r, RobotStatus& status)
	{
// 		if (m_connected && m_RobotHandler->GetRobotController()->GetRobotStatus() == ERobotControllerStatus::EStopped)
// 			m_RobotHandler->Initialize();
// 		TBee::SharedMemoryLock m(m_context->sharedMemory);
// 		OS::ScopedLock l(m_dataMutex);
		if (m_dataMutex->tryLock())
		{
			memcpy(&status, &m_context->sharedMemory->robotData, sizeof(status));
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
			m_context->commChannel->SendTo(m_context->GetTargetClientAddr(), (char*)buffer, len);
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
		m_context->commChannel->SendTo(m_context->GetTargetClientAddr(), (char*)buffer, len);
	}
	virtual void OnRobotStatus(RobotHandler* sender, const RobotStatus& status)
	{
	};
public:
	TxBodyServiceImpl()
	{
		m_RobotHandler = 0;
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_connected = false;
		m_robotDll = "Robot.dll";

		FILE* dataFile;
		dataFile = fopen("TorsoAngles.txt", "w");
		fclose(dataFile);
	}

	~TxBodyServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_status = EServiceStatus::Inited;
		m_context = context;

		m_robotData = &context->sharedMemory->robotData;

		//gLogManager.log("Initializing RobotHandler",ELL_INFO);
		m_RobotHandler = new RobotHandler(m_robotDll);
		m_RobotHandler->GetRobotController()->ParseParameters(m_robotValueMap);
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
			if (st == EIniting)msg += "Initing";
			if (st == EStopped)msg += "Stopped";
			if (st == EStopping)msg += "Stopping";
			if (st == EDisconnected)msg += "Disconnected";
			if (st == EConnecting)msg += "Connecting";
			if (st == EDisconnecting)msg += "Disconnecting";
			if (st == EConnected)msg += "Connected";
			context->RenderText(msg, 5, 0);
		}
		msg = core::string("User Controlling: ") + (m_robotData->connected ? "Yes" : "No");
		context->RenderText(msg, 5, 0);
		msg = core::string("Controlling: ") + (m_RobotHandler->IsLocalControl() ? "Local" : "Remote");
		context->RenderText(msg, 5, 0);
		msg = core::string("Sensors : ") + core::StringConverter::toString(math::vector2d(m_collision));
		context->RenderText(msg, 5, 0);
		if (m_robotData->connected || m_RobotHandler->IsLocalControl())
		{
			sprintf_s(buffer, "%-2.2f, %-2.2f", m_robotData->speed[0], m_robotData->speed[1]);
			msg = core::string("Speed: ") + buffer;
			context->RenderText(msg, 10, 0);

			msg = core::string("Rotation: ") + core::StringConverter::toString(m_robotData->rotation,2);
			context->RenderText(msg, 10, 0);

			math::vector3d angles;
			math::quaternion q(m_robotData->headRotation[0], m_robotData->headRotation[3],
				m_robotData->headRotation[1], m_robotData->headRotation[2]);
			q.toEulerAngles(angles);
			angles.set(angles.y, angles.z, angles.x);
			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", angles.x, angles.y, angles.z);
			msg = core::string("Head Rotation: ") + buffer;
			context->RenderText(msg, 10, 0);

			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", m_robotData->headPos[0], m_robotData->headPos[1], m_robotData->headPos[2]);
			msg = core::string("Head Position: ") + buffer;
			context->RenderText(msg, 10, 0);

		}


		{
			msg = "Robot Started: " + m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_IsStarted, "");
			context->RenderText(msg, 10, 0);

			int sensorsCount = core::StringConverter::toInt(m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorCount, ""));
			for (int i = 0; i < sensorsCount;++i)
			{
				msg = "Sensor[" + core::StringConverter::toString(i) + "]: " + 
					m_RobotHandler->GetRobotController()->ExecCommand(IRobotController::CMD_GetSensorValue, core::StringConverter::toString(i));
				context->RenderText(msg, 10, 0);
			}
			std::vector<float> jvalues;

			m_RobotHandler->GetRobotController()->GetJointValues(jvalues);
			context->RenderText(core::string("Robot Joint Values:"), 5, 0);
						
			msg = "";
			context->RenderText("   \tIK\t/ Real", 10, 0);
			for (int i = 0; i < jvalues.size(); i += 2)
			{
				
				sprintf_s(buffer, "\t%-2.2f\t/ %-2.2f", jvalues[i], jvalues[i+1]);
				msg = core::string("J[") + core::StringConverter::toString(i / 2) + "]:" + buffer;
				context->RenderText(msg, 10, 0);
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
		m_RobotHandler->Initialize();
		m_connected = true;
	}
	virtual void OnUserDisconnected()
	{
		RobotStatus st;
		m_connected = false;
//		m_robotData->connected = 0;
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

		int arrived = false;

		if (msg.equals_ignore_case("query"))
		{
			if (m_RobotHandler->GetRobotController() != 0)
			{
				ERobotControllerStatus st = m_RobotHandler->GetRobotController()->GetRobotStatus();
				int reply = (int)EMessages::RobotStatus;
				int len = stream.write(&reply, sizeof(reply));
				len += stream.write(&st, sizeof(st));
				m_context->commChannel->SendTo(m_context->GetTargetClientAddr(), (char*)buffer, len);
			}
		}
#if 0 // these will be handled using shared memory
		else if (msg.equals_ignore_case("RobotConnect") && vals.size() == 1)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData->connected = core::StringConverter::toBool(vals[0].c_str());
		}
		else if (msg.equals_ignore_case("Speed") && vals.size() == 2)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData->speed[0] = atof(vals[0].c_str());
			m_robotData->speed[1] = atof(vals[1].c_str());
			//limit the speed
			m_robotData->speed[0] = math::clamp<float>(m_robotData->speed[0], -1, 1);
			m_robotData->speed[1] = math::clamp<float>(m_robotData->speed[1], -1, 1);
			m_robotData->speed[0] = (m_robotData->speed[0] ) ;
			m_robotData->speed[1] = (m_robotData->speed[1] ) ;

		}
		else if (msg.equals_ignore_case("Rotation") && vals.size() == 1)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData->rotation = atof(vals[0].c_str());
			m_robotData->rotation = math::clamp<float>(m_robotData->rotation, -1, 1);
		}
		else if (msg.equals_ignore_case("HeadRotation") && vals.size() == 4)
		{
			arrived = true;
			OS::ScopedLock l(m_dataMutex);
			m_robotData->headRotation[0] = atof(vals[0].c_str());
			m_robotData->headRotation[1] = atof(vals[1].c_str());
			m_robotData->headRotation[2] = atof(vals[2].c_str());
			m_robotData->headRotation[3] = atof(vals[3].c_str());
			/*
			math::quaternion q(m_robotData->headRotation[0], m_robotData->headRotation[1], m_robotData->headRotation[2], m_robotData->headRotation[3]);
			double a[3];
			quaternion2Euler(q, a, RotSeq::zxy);
			math::vector3d angles;
			angles.set(math::toDeg(a[0]), math::toDeg(a[1]), math::toDeg(a[2]));

			FILE* dataFile = 0;

			dataFile = fopen("TorsoAngles.txt", "a");
			fprintf(dataFile, "%f\t%f\t%f\n", angles.x, angles.y, angles.z);

			fclose(dataFile);*/
			//do head limits
			// 		m_robotData->tilt = math::clamp(m_robotData->tilt, -50.0f, 50.0f);
			// 		m_robotData->yaw = math::clamp(m_robotData->yaw, -70.0f, 70.0f);
			// 		m_robotData->roll = math::clamp(m_robotData->roll, -40.0f, 40.0f);
		}
		else if (msg.equals_ignore_case("HeadPosition") && vals.size() == 3)
		{
			OS::ScopedLock l(m_dataMutex);
			m_robotData->headPos[0] = atof(vals[0].c_str());
			m_robotData->headPos[1] = atof(vals[1].c_str());
			m_robotData->headPos[2] = atof(vals[2].c_str());

		}
#endif
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
				m_context->commChannel->SendTo(m_context->GetTargetClientAddr(), (char*)buffer, len);
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


TxBodyService::TxBodyService()
{
	m_impl = new TxBodyServiceImpl();
}

TxBodyService::~TxBodyService()
{
	delete m_impl;
}


std::string TxBodyService::GetServiceName()
{
	return TxBodyService::ModuleName;
}

EServiceStatus TxBodyService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxBodyService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxBodyService::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool TxBodyService::StopService()
{
	return m_impl->Stop();
}

void TxBodyService::DestroyService()
{
	m_impl->Destroy();
}


void TxBodyService::Update(float dt)
{
	m_impl->Update(dt);
}

void TxBodyService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxBodyService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxBodyService::LoadServiceSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e = elem->getSubElement("RobotParameters");
	if (e)
	{
		e = e->getSubElement("Value");
		while (e)
		{
			m_impl->m_robotValueMap[e->getAttribute("Name")->value] = e->getAttribute("Value")->value;
			e = e->nextSiblingElement("Value");
		}
	}
	xml::XMLAttribute*a=elem->getAttribute("RobotDll");
	if (a)
	{
		m_impl->m_robotDll = a->value;
	}
	return true;
}

void TxBodyService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


