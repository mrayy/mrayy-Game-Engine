

#include "stdafx.h"
#include "CalibHeadController.h"






namespace mray
{
namespace TBee
{

#define  SAMPLES_COUNT 40

void CalibHeadController::CalibrationInfo::LoadFromXML(xml::XMLElement* e)
{
	headOrintation = core::StringConverter::toQuaternion(e->getValueString("HeadOrientation"));
	headPosition = core::StringConverter::toVector3d(e->getValueString("HeadPosition"));
	staticHeadOffset = core::StringConverter::toVector3d(e->getValueString("StaticHeadOffset"));
	calibrated = true;
}
void CalibHeadController::CalibrationInfo::WriteToXML(xml::XMLElement* elem)
{
	xml::XMLElement* e = new xml::XMLElement("Calibration");
	elem->addSubElement(e);
	e->addAttribute("HeadOrientation", core::StringConverter::toString(headOrintation));
	e->addAttribute("HeadPosition", core::StringConverter::toString(headPosition));
	e->addAttribute("StaticHeadOffset", core::StringConverter::toString(staticHeadOffset));
}

////////////////////////////////////////////////////////////////////////////////////////////

CalibHeadController::CalibHeadController(IHeadController* o) :m_otherController(o)
{
	m_lostPos = 0;
	m_lostOri = 0;
}

CalibHeadController::~CalibHeadController()
{
	delete m_otherController;
}

bool CalibHeadController::GetHeadOrientation(math::quaternion& v, bool abs)
{
	math::quaternion q;
	if (!m_otherController->GetHeadOrientation(q,abs) )
	{
		m_lostOri = SAMPLES_COUNT;
		v = m_lastQuaternion*m_calibration.headOrintation.inverse();
		m_trackedQuaternion = m_lastQuaternion;
		return true;
	}

	if (m_lostOri > 0)
	{
		v.Slerp(m_trackedQuaternion, q, (1 - (float)m_lostOri / (float)SAMPLES_COUNT));
		m_lostOri--;
	}
	else
	{
		v = q;
		m_trackedQuaternion = q;
	}
	m_lastQuaternion = v;
	if (!abs)
		v = v*m_calibration.headOrintation.inverse();;

	return true;
}

bool CalibHeadController::GetHeadPosition(math::vector3d& v, bool abs)
{
	math::vector3d pos;
	if (!m_otherController->GetHeadPosition(pos, abs))
	{
		m_lostPos = SAMPLES_COUNT;;
		v = m_lastPos - m_calibration.headPosition - m_calibration.staticHeadOffset;
		m_trackedPos = m_lastPos;
		return true;
	}
	if (m_lostPos > 0)
	{
		v = math::lerp(m_trackedPos, pos, (1 - (float)m_lostPos / SAMPLES_COUNT));
		m_lostPos--;
	}
	else
	{
		m_trackedPos = pos;
		v = pos;
	}
	m_lastPos = v;

	if (!abs)
		v = v - m_calibration.headPosition - m_calibration.staticHeadOffset;
	return true;
}


void CalibHeadController::Calibrate()
{
	m_calibration.calibrated = true;
	m_otherController->GetHeadPosition(m_calibration.headPosition,false);
	m_otherController->GetHeadOrientation(m_calibration.headOrintation, false);
}


void CalibHeadController::LoadFromXML(xml::XMLElement* e)
{
	m_calibration.LoadFromXML(e);
}

void CalibHeadController::WriteToXML(xml::XMLElement* e)
{
	m_calibration.WriteToXML(e);
}

}
}
