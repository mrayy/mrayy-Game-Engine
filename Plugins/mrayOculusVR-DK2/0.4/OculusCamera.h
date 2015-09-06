/********************************************************************
	created:	2013/10/30
	created:	30:10:2013   21:33
	filename: 	C:\Development\mrayEngine\Plugins\mrayOculusVR\OculusCamera.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOculusVR
	file base:	OculusCamera
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusCamera__
#define __OculusCamera__


#include "CameraNode.h"

namespace mray
{
namespace scene
{

class OculusCamera:public CameraNode
{
protected:
public:
	OculusCamera();
	virtual~OculusCamera();

	void SetPerspectiveOffset(const math::vector2d& offset);
};

}
}


#endif