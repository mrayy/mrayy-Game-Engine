

/********************************************************************
	created:	2014/01/18
	created:	18:1:2014   20:44
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\OculusHeadController.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	OculusHeadController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusHeadController__
#define __OculusHeadController__


#include "IHeadController.h"


namespace mray
{

namespace TBee
{
class OculusHeadController :public IHeadController
{
protected:
	math::quaternion m_initial;
public:
	OculusHeadController();
	virtual~OculusHeadController();

	virtual bool GetHeadOrientation(math::quaternion& v, bool abs);
	virtual bool GetHeadPosition(math::vector3d& v, bool abs);

	virtual void Recalibrate();
};

}
}


#endif
