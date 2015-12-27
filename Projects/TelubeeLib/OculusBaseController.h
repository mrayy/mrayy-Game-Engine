
/********************************************************************
created:	2014/02/12
created:	12:2:2014   21:35
filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\OculusBaseController.h
file path:	C:\Development\mrayEngine\Projects\TelubeeLib
file base:	OculusBaseController
file ext:	h
author:		MHD Yamen Saraiji

purpose:
*********************************************************************/

#ifndef __OculusBaseController__
#define __OculusBaseController__

#if 0

#include "IInputController.h"

namespace mray
{
namespace TBee
{

class OculusBaseController :public IInputController
{
protected:

	math::vector3d m_headPosCalib;

	math::vector2d m_currentSpeed;
	float m_currentRotation;
public:
	OculusBaseController();
	virtual~OculusBaseController();


	virtual bool IsButtonPressed(EInputButton button);
	virtual math::vector2d GetSpeed();
	virtual float GetRotation();
	void Recalibrate();
};

}
}

#endif

#endif