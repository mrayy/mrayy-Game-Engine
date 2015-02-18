

/********************************************************************
	created:	2013/03/04
	created:	4:3:2013   13:15
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SensorCurve.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	SensorCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _SensorCurve_
#define _SensorCurve_

#include "ISensorModulator.h"

namespace mray
{
namespace VT
{
	class SensorCurveImpl;
class SensorCurve:public ISensorModulatorF
{
protected:
	SensorCurveImpl* m_impl;
public:
	SensorCurve();
	virtual~SensorCurve();

	const math::vector2d& GetInLimit();
	const math::vector2d& GetOutLimit();
	float SampleValue01(float v);
	float ModulateValue(const float& v);
	const std::vector<float>& GetKeys();

	void LoadFromXML(xml::XMLElement* e);
};


}
}

#endif
