
/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   10:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GenericValues.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GenericValues
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GenericValues___
#define ___GenericValues___

#include "IValue.h"

namespace mray{
namespace video{
	class SColor;
	class RenderMaterial;
	class ITexture;
}

class MRAY_DLL IntValue:public IValue
{
public:
	int value;
public:
	IntValue(const core::string&name,int v);

	virtual EValueType getType(){return EVT_INT;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL FloatValue:public IValue
{
public:
	float value;
public:
	FloatValue(const core::string&name,float v);

	virtual EValueType getType(){return EVT_FLOAT;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL BoolValue:public IValue
{
public:
	bool value;
public:
	BoolValue(const core::string&name,bool v);

	virtual EValueType getType(){return EVT_BOOLEAN;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL StringValue:public IValue
{
public:
	core::string value;
public:
	StringValue(const core::string&name,const core::string& v);

	virtual EValueType getType(){return EVT_STRING;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}


#endif //___GenericValues___
