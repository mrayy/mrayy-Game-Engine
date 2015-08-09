
#include "stdafx.h"
#include "CRobotConnector.h"
#include "RemoteRobotCommunicator.h"
#include "INetwork.h"
#include "AppData.h"
#include "IHeadController.h"
#include "IInputController.h"


#include "WiiboardInputController.h"
#include "OculusBaseController.h"
#include "JoystickInputController.h"
#include "KeyboardHeadController.h"
#include "NodeHeadController.h"
#include "OculusHeadController.h"
#include "OptiTrackHeadController.h"
#include "IRobotController.h"

namespace mray
{
namespace TBee
{
CRobotConnector::CRobotConnector()
{
	m_connected = false;
	m_status = false;
	m_communicator = 0;
	m_headController = 0;
	m_robotController = 0;

	m_videoPort = -1;
	m_audioPort = -1;
	m_handsPort = -1;
	m_clockPort = -1;
	m_rtcp = false;
	m_controller = 0;



}
CRobotConnector::~CRobotConnector()
{
	DisconnectRobot();
	delete m_communicator;
	if (m_controller)
		delete m_controller;
}
bool CRobotConnector::IsRobotConnected()
{
	return m_status;
}
void CRobotConnector::ConnectRobot()
{
	if (!m_communicator)
		return;
	if (m_connected)
		m_communicator->Disconnect();
	m_connected = m_communicator->Connect(m_robotIP, m_commPort);
	m_communicator->ClearData(true);
	//	m_roboComm->Connect("127.0.0.1",3000);
	m_communicator->SetUserID("yamens");
	m_communicator->ConnectUser(true);
	network::NetAddress addr;
	char ipLst[8][16];
	network::INetwork::getInstance().getLocalIPs(ipLst);
	addr.port = m_videoPort;
	addr.setIP(ipLst[0]);
	core::string addrStr = addr.toString();
	addrStr += "," + core::StringConverter::toString(m_videoPort);
	addrStr += "," + core::StringConverter::toString(m_audioPort);
	addrStr += "," + core::StringConverter::toString(m_handsPort);
	addrStr += "," + core::StringConverter::toString(m_clockPort);
	addrStr += "," + core::StringConverter::toString(m_rtcp);
	m_communicator->SetData("Connect", addrStr,true);
	addrStr =  core::StringConverter::toString(m_commPort);
	m_communicator->SetData("CommPort", addrStr, true);

	if (m_controller)
	{
		m_controller->ConnectRobot();
	}
}
void CRobotConnector::ConnectRobotIP(const core::string& ip, uint videoport, uint audioPort, uint handsPort, uint clockPort, uint commPort, bool rtcp)
{
	m_commPort = commPort;
	m_videoPort = videoport;
	m_audioPort = audioPort;
	m_handsPort = handsPort;
	m_clockPort = clockPort;
	m_rtcp = rtcp;
	m_robotIP = ip;
	ConnectRobot();
}


void CRobotConnector::DisconnectRobot()
{
	if (!m_communicator)
		return;
	if (!m_connected)
		return;
	network::NetAddress addr;
	char ipLst[8][16];
	network::INetwork::getInstance().getLocalIPs(ipLst);
	addr.port = m_videoPort;
	addr.setIP(ipLst[0]);
	core::string addrStr = addr.toString();
	addrStr += "," + core::StringConverter::toString(m_videoPort);
	m_communicator->SetData("Disconnect", addrStr, true);
	m_communicator->Update(0);//only once
	EndUpdate();
	m_communicator->Disconnect();
	m_connected = false;
}
void CRobotConnector::StartUpdate()
{
	if (!m_communicator)
		return;
	if (!m_connected)
		return;
	m_status = true;

	m_communicator->ConnectRobot(true);

}
math::vector3d CRobotConnector::GetCurrentHeadRotation()
{
	return core::StringConverter::toVector3d(m_communicator->GetData("HeadRotation"));
}
void CRobotConnector::EndUpdate()
{
	if (!m_communicator)
		return;
	m_status = false;
	m_communicator->SetData("HeadRotation", core::StringConverter::toString(math::quaternion::Identity), false);
	m_communicator->Update(0);//only once
	m_communicator->ConnectRobot(false);

	if (m_controller)
	{
		RobotStatus st;
		st.connected = m_connected;
		st.headPos[0] = 0;
		st.headPos[1] = 0;
		st.headPos[2] = 0;

		st.headRotation[0] = 0;
		st.headRotation[1] = 0;
		st.headRotation[2] = 0;
		st.headRotation[3] = 0;

		st.rotation = 0;

		st.speed[0] = 0;
		st.speed[1] = 0;

		m_controller->UpdateRobotStatus(st);

		m_controller->DisconnectRobot();
	}
}
void CRobotConnector::LoadXML(xml::XMLElement* e)
{

	xml::XMLAttribute*attr = e->getAttribute("IP");
	if (attr)
	{
		m_robotIP = attr->value;
	}
}
void CRobotConnector::SetData(const core::string& key, const core::string& val,bool status)
{
	if (!m_communicator)
		return;
	m_communicator->SetData(key, val,status);
}
void CRobotConnector::RemoveData(const core::string& key)
{
	if (!m_communicator)
		return;
	m_communicator->RemoveData(key);

}

void CRobotConnector::HandleController()
{
	if (!m_robotController)
	{
		m_speed = 0;
		m_rotation = 0;
	}
	else
	{
		m_speed = m_robotController->GetSpeed();
		if (m_speed.x < 0)
			m_speed *= 0.1f;
		m_rotation = m_robotController->GetRotation();;
	}


	if (m_headController)
	{
		if (!m_headController->GetHeadOrientation(m_headRotation, false))
			m_headRotation = math::quaternion::Identity;
		// 		m_headRotation.y = -m_headRotation.y;// REMOVE
		// 		math::Swap(m_headRotation.x, m_headRotation.y); // REMOVE
		if (!m_headController->GetHeadPosition(m_headPosition, false))
			m_headPosition = math::vector3d::Zero;

	}
}
void CRobotConnector::UpdateStatus()
{
	HandleController();
	if (!m_communicator)
		return;
	if (/*!m_status ||*/ !m_connected)
		return;
	m_communicator->SetData("HeadRotation", core::StringConverter::toString(m_headRotation), false);
	m_communicator->SetData("HeadPosition", core::StringConverter::toString(m_headPosition), false);
	m_communicator->SetData("Speed", core::StringConverter::toString(math::vector2d(m_speed.x,m_speed.y)), false);
	m_communicator->SetData("Rotation", core::StringConverter::toString(m_rotation/3), false);

	if (m_controller && m_status)
	{
		RobotStatus st;
		st.connected = m_connected;
		st.headPos[0] = m_headPosition.x;
		st.headPos[1] = m_headPosition.y;
		st.headPos[2] = m_headPosition.z;

		st.headRotation[0] = m_headRotation.w;
		st.headRotation[1] = m_headRotation.x;
		st.headRotation[2] = m_headRotation.y;
		st.headRotation[3] = m_headRotation.z;

		st.rotation = m_rotation / 3.0f;

		st.speed[0] = m_speed.x;
		st.speed[1] = m_speed.y;

		m_controller->UpdateRobotStatus(st);
	}
}

void CRobotConnector::InitController(CRobotConnector* c)
{
	switch (AppData::Instance()->headController)
	{
	case EHeadControllerType::Keyboard:
		c->SetHeadController(new KeyboardHeadController);
		break;;
	case EHeadControllerType::Oculus:
		c->SetHeadController(new OculusHeadController);
		break;;
	case EHeadControllerType::OptiTrack:
		c->SetHeadController(new OptiTrackHeadController(1));
		break;;
	case EHeadControllerType::SceneNode:
		c->SetHeadController(new NodeHeadController());
		break;;
	default:
		break;
	}

	switch (AppData::Instance()->robotController)
	{
	case ERobotControllerType::Keyboard:
		break;;
	case ERobotControllerType::Oculus:
		c->SetRobotController(new OculusBaseController);
		break;;
	case ERobotControllerType::Joystick:
		c->SetRobotController(new JoystickInputController);
		break;;
	case ERobotControllerType::Wiiboard:
		c->SetRobotController(new WiiboardInputController);
		break;;
	default:
		break;
	}

}

}
}