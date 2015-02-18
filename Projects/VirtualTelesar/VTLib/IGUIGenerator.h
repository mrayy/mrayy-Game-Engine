

/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   3:24
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IGUIGenerator.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IGUIGenerator
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IGUIGenerator___
#define ___IGUIGenerator___



namespace mray
{
namespace VT
{


class IGUIGenerator
{
protected:

public:
	IGUIGenerator(){}
	virtual~IGUIGenerator(){}
	
	virtual GUI::IGUIElement* GetGeneratedUIElement()=0;
	
};

}
}

#endif
