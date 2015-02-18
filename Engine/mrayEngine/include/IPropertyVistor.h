

/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   12:34
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPropertyVistor.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPropertyVistor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPropertyVistor__
#define __IPropertyVistor__


namespace mray
{
	class IObject;
	class IProperty;
	class CPropertieSet;
	class CPropertieDictionary;
class IPropertyVistor
{
private:
protected:
public:
	IPropertyVistor(){}
	virtual~IPropertyVistor(){}

	virtual void Visit(IObject*prop)=0;
	virtual void Visit(IProperty*prop)=0;
	virtual void Visit(CPropertieSet*prop)=0;
	virtual void Visit(CPropertieDictionary*prop)=0;

};


}

#endif
