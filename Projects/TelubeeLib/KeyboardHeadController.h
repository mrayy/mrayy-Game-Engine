

/********************************************************************
	created:	2014/02/25
	created:	25:2:2014   20:06
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\KeyboardHeadController.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	KeyboardHeadController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __KeyboardHeadController__
#define __KeyboardHeadController__


#include "IHeadController.h"

namespace mray
{
namespace TBee
{	

class KeyboardHeadController:public IHeadController
{
protected:
public:
	KeyboardHeadController(){}
	virtual~KeyboardHeadController(){}

	virtual bool GetHeadOrientation(math::quaternion& v, bool abs);
	virtual bool GetHeadPosition(math::vector3d& v, bool abs);
};

}
}


#endif
