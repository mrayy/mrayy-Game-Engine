#include "stdafx.h"
#include "GenericValues.h"
#include "StringConverter.h"

#include "XMLElement.h"



namespace mray{

IntValue::IntValue(const core::string&name,int v)
	:IValue(name),value(v)
{
}


core::string IntValue::toString()const{
	return core::StringConverter::toString(value);
}
void IntValue::parse(const core::string&v){
	core::StringConverter::parse(v,value);
	OnChanged(this);
}

IValue* IntValue::duplicate()
{
	return new IntValue(m_name,value);
}
void IntValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
 xml::XMLElement*  IntValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////


FloatValue::FloatValue(const core::string&name,float v)
:IValue(name),value(v)
{
}


core::string FloatValue::toString()const{
	return core::StringConverter::toString(value);
}
void FloatValue::parse(const core::string&v){
	core::StringConverter::parse(v, value);
	OnChanged(this);
}


IValue* FloatValue::duplicate()
{
	return new FloatValue(m_name,value);
}
void FloatValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  FloatValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

BoolValue::BoolValue(const core::string&name,bool v)
:IValue(name),value(v)
{
}


core::string BoolValue::toString()const{
	return core::StringConverter::toString(value);
}
void BoolValue::parse(const core::string&v){
	core::StringConverter::parse(v, value);
	OnChanged(this);
}
IValue* BoolValue::duplicate()
{
	return new BoolValue(m_name,value);
}
void BoolValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  BoolValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

StringValue::StringValue(const core::string&name,const core::string& v)
:IValue(name),value(v)
{
}


core::string StringValue::toString()const{
	return core::StringConverter::toString(value);
}
void StringValue::parse(const core::string&v){
	core::StringConverter::parse(v, value);
	OnChanged(this);
}
IValue* StringValue::duplicate()
{
	return new StringValue(m_name,value);
}
void StringValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		value=attr->value;
}
xml::XMLElement*  StringValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),value);
	return e;
}

//////////////////////////////////////////////////////////////////////////



}
