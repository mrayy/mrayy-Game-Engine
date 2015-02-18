

/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   12:10
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\OculusLensParams.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	OculusLensParams
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusLensParams__
#define __OculusLensParams__




namespace mray
{
namespace video
{

	
class OculusLensParams
{
protected:
public:
	OculusLensParams():DistortionScale(1)
	{}
	virtual~OculusLensParams(){}

	math::vector2d LensCenter;
	float DistortionScale;
	math::vector4d LensParams;
};

}
}

#endif
