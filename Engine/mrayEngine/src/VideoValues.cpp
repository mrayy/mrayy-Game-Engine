#include "stdafx.h"

#include "VideoValues.h"
#include "StringConverter.h"
#include "ShaderResourceManager.h"
#include "MaterialResourceManager.h"
#include "TextureResourceManager.h"
#include "XMLElement.h"

namespace mray{

MaterialValue::MaterialValue(const core::string&name,GCPtr<video::RenderMaterial> v)
	:IValue(name),value(v)
{
}


const mchar* MaterialValue::toString()const{return mT("");}
void MaterialValue::parse(const core::string&v){
}
IValue* MaterialValue::duplicate()
{
	return new MaterialValue(m_name,value);
}
void MaterialValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		value=gMaterialResourceManager.getMaterial(attr->value);
}
xml::XMLElement*  MaterialValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	if(value)
		e->addAttribute(mT("Value"),value->getResourceName());
	return e;
}

//////////////////////////////////////////////////////////////////////////

TextureValue::TextureValue(const core::string&name,GCPtr<video::TextureUnit> v)
:IValue(name),value(v)
{
}
TextureValue::~TextureValue(){

}


const mchar* TextureValue::toString()const{return mT("");}
void TextureValue::parse(const core::string&v){
}
IValue* TextureValue::duplicate()
{
	return new TextureValue(m_name,value);
}
void TextureValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLElement*e=elem->getSubElement(mT("TextureUnit"));
	if(e)
	{
		value=new video::TextureUnit();
		value->loadXMLSettings(e);
	}
}
xml::XMLElement*  TextureValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	if(value)
		value->exportXMLSettings(e);
	return e;
}
//////////////////////////////////////////////////////////////////////////

ColorValue::ColorValue(const core::string&name,const video::SColor& v)
:IValue(name),value(v)
{
}


const mchar* ColorValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void ColorValue::parse(const core::string&v){
	core::StringConverter::parse(v,value);
}
IValue* ColorValue::duplicate()
{
	return new ColorValue(m_name,value);
}

void ColorValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  ColorValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////

ShaderValue::ShaderValue(const core::string&name,GCPtr<video::GPUShader> v)
:IValue(name),value(v)
{
}


const mchar* ShaderValue::toString()const{
	if(value)
	{
		return value->GetName().c_str();
	}
	return mT("");
}
void ShaderValue::parse(const core::string&v){
	value=gShaderResourceManager.getResource(v);
}
IValue* ShaderValue::duplicate()
{
	return new ShaderValue(m_name,value);
}
void ShaderValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		value=gShaderResourceManager.getResource(attr->value);
}
xml::XMLElement*  ShaderValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	if(value)
		e->addAttribute(mT("Value"),value->GetName());
	return e;
}

}