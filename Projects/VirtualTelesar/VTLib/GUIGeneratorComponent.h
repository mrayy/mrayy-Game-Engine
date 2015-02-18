

/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   3:22
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GUIGeneratorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GUIGeneratorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	this component traverses over all components which can generate a UI Layout
*********************************************************************/
#ifndef ___GUIGeneratorComponent___
#define ___GUIGeneratorComponent___

#include "IVTComponent.h"


namespace mray
{
namespace VT
{

class GUIGeneratorComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:

public:
	GUIGeneratorComponent(){}
	virtual~GUIGeneratorComponent(){}
	
	
};

}
}

#endif
