
#include "stdafx.h"
#include "RobotArms.h"

namespace mray
{



RobotArms::RobotArms()
{
	m_leftArm = new ArmsController("leftArm.pid");
	m_rightArm = new ArmsController("rightArm.pid");
	m_leftEnabled = false;
	m_leftEnabled = false;
	_connected = false;
}
RobotArms::~RobotArms()
{
	delete m_leftArm;
	delete m_rightArm;
}

float* RobotArms::GetHandSensor(TargetArm hand) {
	if (hand == TargetArm::Right)
		return m_rightArm->GetHandSensor();
	else 
		return  m_leftArm->GetHandSensor();
}

bool RobotArms::Connect(const core::string& lPort, const core::string& rPort)
{
	Disconnect();
	bool left = true;
	bool right = true;
	m_leftEnabled = false;
	m_leftEnabled = false;
	if (lPort != "")
	{
		m_leftEnabled = true;
		left = m_leftArm->Connect(lPort,true);
	}
	if (rPort != "")
	{
		m_rightEnabled = true;
		right = m_rightArm->Connect(rPort,false);
	}
	_connected = left & right;
	//m_serial->owner = this;
	return _connected;
}
bool RobotArms::IsConnected()
{
	return _connected;
}
void RobotArms::Disconnect()
{
	_connected = false;
	if (!m_leftArm->IsConnected() && !m_rightArm->IsConnected())
		return;
	m_leftArm->SafeShutdown(3000);
	m_rightArm->SafeShutdown(3000);
	_sleep(3000);
	m_leftArm->Disconnect();
	m_rightArm->Disconnect();
}


void RobotArms::SetArmAngles(TargetArm arm, float *angles, int n)
{
	if (arm == TargetArm::Left)
		m_leftArm->SetArmAngles(angles, n);
	else
		m_rightArm->SetArmAngles(angles, n);
}

void RobotArms::SetHand(TargetArm arm, float* angles, int n)
{
	if (arm == TargetArm::Left)
		m_leftArm->SetHand(angles, n);
	else
		m_rightArm->SetHand(angles, n);
}
void RobotArms::Start(bool leftArm, bool rightArm, bool enableReadingAngles)
{
	m_leftArm->Start(leftArm, enableReadingAngles);
	m_rightArm->Start(rightArm, enableReadingAngles);
}
void RobotArms::Stop(bool force )
{
	m_leftArm->Stop(force);
	m_rightArm->Stop(force);
}

void RobotArms::Update(float dt)
{
	m_leftArm->Update(dt);
	m_rightArm->Update(dt);
}

}



