

/********************************************************************
	created:	2013/03/04
	created:	4:3:2013   13:13
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\ISensorModulator.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	ISensorModulator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _ISensorModulator_
#define _ISensorModulator_


namespace mray
{
namespace VT
{

	class IFeedbackResult;

	template <class T>
class ISensorModulator
{
protected:
public:
	ISensorModulator(){}
	virtual~ISensorModulator(){}

	virtual T ModulateValue(const T& v)=0;


	virtual void LoadFromXML(xml::XMLElement* e)=0;
};

typedef ISensorModulator<float> ISensorModulatorF;
typedef ISensorModulator<math::vector3d> ISensorModulatorV3;

}
}

#endif
