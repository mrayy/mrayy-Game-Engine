





/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   23:19
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GPSComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GPSComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___GPSComponent___
#define ___GPSComponent___

#include "MountableComponent.h"


namespace mray
{
namespace VT
{

BEGIN_DECLARE_VTCOMPONENT(GPSComponent,MountableComponent)
protected:
	float m_noiseAmount;
	math::vector3d m_posValues;
	float m_refreshFreq;
	float m_invRefFreq;
	float m_timeAcc;
public:
	DECLARE_PROPERTY_TYPE(NoiseAmount,float,);
	DECLARE_PROPERTY_TYPE(RefreshFrequency,float,);
public:
	DECLARE_SETGET_PROP(NoiseAmount,float);
	DECLARE_SETGET_PROP(RefreshFrequency,float);

	math::vector3d GetGPSValues();
END_DECLARE_VTCOMPONENT(GPSComponent);


}
}

#endif

