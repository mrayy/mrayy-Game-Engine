

/********************************************************************
created:	2014/03/25
created:	25:3:2014   15:29
filename: 	C:\Development\mrayEngine\Projects\NissanCamera\CalibHeadController.h
file path:	C:\Development\mrayEngine\Projects\NissanCamera
file base:	CalibHeadController
file ext:	h
author:		MHD Yamen Saraiji
	
purpose:	
*********************************************************************/

#ifndef __CalibHeadController__
#define __CalibHeadController__


#include "IHeadController.h"

namespace mray
{
namespace TBee
{


class CalibHeadController :public IHeadController
{
protected:
	IHeadController* m_otherController;

	struct CalibrationInfo
	{
		CalibrationInfo()
		{
			calibrated = false;
		}
		math::vector3d headPosition;
		math::vector3d staticHeadOffset;
		math::quaternion headOrintation;

		bool calibrated;

		void LoadFromXML(xml::XMLElement* e);
		void WriteToXML(xml::XMLElement* e);
	};

	//frame counter to smooth down the result values when marker is lost
	int m_lostPos;
	int m_lostOri;

	math::vector3d m_lastPos;
	math::quaternion m_lastQuaternion;

	math::vector3d m_trackedPos;
	math::quaternion m_trackedQuaternion;
	CalibrationInfo m_calibration;

	math::Point3d<bool> m_lock;
public:
	CalibHeadController(IHeadController* o);
	virtual~CalibHeadController();

	void SetLockPosition(bool x, bool y, bool z){ m_lock.set(x, y, z); }
	virtual bool GetHeadOrientation(math::quaternion& q, bool abs);
	virtual bool GetHeadPosition(math::vector3d &v,bool abs);

	void Calibrate();

	void LoadFromXML(xml::XMLElement* e);
	void WriteToXML(xml::XMLElement* e);
};

}
}


#endif
