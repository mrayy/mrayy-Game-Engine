

#include "stdafx.h"
#include "OculusBaseController.h"
#include "AppData.h"
#include "OculusDevice.h"

namespace mray
{
namespace TBee
{

OculusBaseController::OculusBaseController()
{
	m_currentRotation = 0;
}
OculusBaseController::~OculusBaseController()
{
}


bool OculusBaseController::IsButtonPressed(EInputButton button)
{
	return false;
}
math::vector2d OculusBaseController::GetSpeed()
{
#ifdef OCULUS_DK1
	return math::vector3d::Zero;
#else
	if (!AppData::Instance()->oculusDevice)
		return 0;
	math::vector3d p = AppData::Instance()->oculusDevice->GetPosition();
	math::vector3d diff = p - m_headPosCalib;

	float x = diff.z;
	float y = diff.x;

	float minOffset = 0.05;
	float maxOffset = 0.15;

	x = math::sign(x)*math::sqr(math::Min<float>(1.0f, math::Max<float>(0, abs(x) - minOffset) / (maxOffset-minOffset)));
	y = math::sign(y)*math::sqr(math::Min<float>(1.0f, math::Max<float>(0, abs(y) - minOffset) / (maxOffset - minOffset)));

	m_currentSpeed += math::vector2d(-x, y)*gEngine.getFPS()->dt() * 2;
	m_currentSpeed -= m_currentSpeed*gEngine.getFPS()->dt()*1;
	m_currentSpeed.x = math::clamp<float>(m_currentSpeed.x, -1, 1);
	m_currentSpeed.y = math::clamp<float>(m_currentSpeed.y, -1, 1);
#endif

	return m_currentSpeed;
}

float OculusBaseController::GetRotation()
{
	if (!AppData::Instance()->oculusDevice)
		return 0;

	float y = AppData::Instance()->oculusDevice->GetOrientation().getYaw();


	float minAngle = 15;
	float maxAngle = 60;

	y = math::sign(y)*math::sqr(math::Min<float>(1.0f, math::Max<float>(0, abs(y) - minAngle) / (maxAngle - minAngle)));
	//m_currentRotation = y;
	m_currentRotation += math::clamp<float>(2 * y, -1, 1)*gEngine.getFPS()->dt() * 2;
	m_currentRotation -= m_currentRotation*gEngine.getFPS()->dt()*1.5f;
	m_currentRotation = math::clamp<float>(m_currentRotation, -1, 1);


	float r = m_currentRotation;
	return -r;
}
void OculusBaseController::Recalibrate()
{
	if (AppData::Instance()->oculusDevice)
	{
		m_headPosCalib = AppData::Instance()->oculusDevice->GetPosition();
	}
}

}
}


