

#include "stdafx.h"
#include "OculusHeadController.h"
#include "AppData.h"
#include "OculusDevice.h"

namespace mray
{
namespace TBee
{

OculusHeadController::OculusHeadController()
{
}
OculusHeadController::~OculusHeadController()
{
}


bool OculusHeadController::GetHeadOrientation(math::quaternion& v, bool abs)
{
	if (!AppData::Instance()->oculusDevice)
		return false;
	v = AppData::Instance()->oculusDevice->GetOrientation();
	if (!abs)
	{
		v=v*m_initial;
		v.Normalize();
	}
	return true;
}
bool OculusHeadController::GetHeadPosition(math::vector3d& v,bool abs)
{
#ifdef OCULUS_DK1
	return math::vector3d::Zero;
#else
	if (!AppData::Instance()->oculusDevice)
		return false;
	v=AppData::Instance()->oculusDevice->GetPosition();
	return true;
#endif
}

void OculusHeadController::Recalibrate()
{
	if (AppData::Instance()->oculusDevice)
	{
		AppData::Instance()->oculusDevice->ResetOrientation();
		m_initial = AppData::Instance()->oculusDevice->GetOrientation();
		m_initial = math::quaternion::Identity;
	}
	m_initial = m_initial.inverse();
}

}
}

