


/********************************************************************
	created:	2012/01/03
	created:	3:1:2012   10:27
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleXMLParser.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleXMLParser
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleXMLParser__
#define __VehicleXMLParser__

#include "compileConfig.h"

namespace mray
{
	namespace OS
	{
		class IStream;
	}

	namespace xml
	{
		class XMLElement;
	}
namespace physics
{
	class VehicleDesc;
	class VehicleMotorDesc;
	class VehicleGearDesc;
	class VehicleWheelDesc;

class MRAY_DLL VehicleXMLParser
{
private:
protected:
public:
	VehicleXMLParser();
	virtual~VehicleXMLParser();

	VehicleDesc* LoadVehicleDesc(OS::IStream* path);
	VehicleDesc* LoadVehicleDesc(xml::XMLElement* node);

	VehicleMotorDesc* LoadMotorDesc(xml::XMLElement* node);
	VehicleGearDesc* LoadGearDesc(xml::XMLElement* node);
	VehicleWheelDesc* LoadWheelDesc(xml::XMLElement* node);

	std::vector<VehicleWheelDesc*> LoadWheels(xml::XMLElement* node);
};

}
}

#endif
