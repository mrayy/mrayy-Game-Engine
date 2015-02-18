#include "stdafx.h"

#include "MathValues.h"
#include "StringConverter.h"

#include "XMLElement.h"

namespace mray{

Vector2diValue::Vector2diValue(const core::string&name,const math::vector2di& v)
	:IValue(name),value(v)
{
}


const mchar* Vector2diValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector2diValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Vector2diValue::duplicate()
{
	return new Vector2diValue(m_name,value);
}
void Vector2diValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector2diValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Vector2dfValue::Vector2dfValue(const core::string&name,const math::vector2df& v)
:IValue(name),value(v)
{
}


const mchar* Vector2dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector2dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}
IValue* Vector2dfValue::duplicate()
{
	return new Vector2dfValue(m_name,value);
}
void Vector2dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector2dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}


//////////////////////////////////////////////////////////////////////////

Vector3diValue::Vector3diValue(const core::string&name,const math::vector3di& v)
:IValue(name),value(v)
{
}


const mchar* Vector3diValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector3diValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Vector3diValue::duplicate()
{
	return new Vector3diValue(m_name,value);
}
void Vector3diValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector3diValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Vector3dfValue::Vector3dfValue(const core::string&name,const math::vector3df& v)
:IValue(name),value(v)
{
}


const mchar* Vector3dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector3dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}


IValue* Vector3dfValue::duplicate()
{
	return new Vector3dfValue(m_name,value);
}
void Vector3dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector3dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Vector4diValue::Vector4diValue(const core::string&name,const math::vector4di& v)
:IValue(name),value(v)
{
}


const mchar* Vector4diValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector4diValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Vector4diValue::duplicate()
{
	return new Vector4diValue(m_name,value);
}
void Vector4diValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector4diValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////

Vector4dfValue::Vector4dfValue(const core::string&name,const math::vector4df& v)
:IValue(name),value(v)
{
}


const mchar* Vector4dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Vector4dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Vector4dfValue::duplicate()
{
	return new Vector4dfValue(m_name,value);
}
void Vector4dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Vector4dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Line2dfValue::Line2dfValue(const core::string&name,const math::line2d& v)
:IValue(name),value(v)
{
}


const mchar* Line2dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Line2dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Line2dfValue::duplicate()
{
	return new Line2dfValue(m_name,value);
}
void Line2dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Line2dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Line3dfValue::Line3dfValue(const core::string&name,const math::line3d& v)
:IValue(name),value(v)
{
}


const mchar* Line3dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Line3dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Line3dfValue::duplicate()
{
	return new Line3dfValue(m_name,value);
}
void Line3dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Line3dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

box3dfValue::box3dfValue(const core::string&name,const math::box3d& v)
:IValue(name),value(v)
{
}


const mchar* box3dfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void box3dfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* box3dfValue::duplicate()
{
	return new box3dfValue(m_name,value);
}
void box3dfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  box3dfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Matrix3x3Value::Matrix3x3Value(const core::string&name,const math::matrix3x3& v)
:IValue(name),value(v)
{
}


const mchar* Matrix3x3Value::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Matrix3x3Value::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Matrix3x3Value::duplicate()
{
	return new Matrix3x3Value(m_name,value);
}
void Matrix3x3Value::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Matrix3x3Value::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

Matrix4x4Value::Matrix4x4Value(const core::string&name,const math::matrix4x4& v)
:IValue(name),value(v)
{
}


const mchar* Matrix4x4Value::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void Matrix4x4Value::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}

IValue* Matrix4x4Value::duplicate()
{
	return new Matrix4x4Value(m_name,value);
}
void Matrix4x4Value::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  Matrix4x4Value::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}

//////////////////////////////////////////////////////////////////////////

PlaneValue::PlaneValue(const core::string&name,const math::Plane& v)
:IValue(name),value(v)
{
}


const mchar* PlaneValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void PlaneValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}
IValue* PlaneValue::duplicate()
{
	return new PlaneValue(m_name,value);
}

void PlaneValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  PlaneValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////

QuaternionValue::QuaternionValue(const core::string&name,const math::quaternion& v)
:IValue(name),value(v)
{
}


const mchar* QuaternionValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void QuaternionValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}
IValue* QuaternionValue::duplicate()
{
	return new QuaternionValue(m_name,value);
}

void QuaternionValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  QuaternionValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////

RectiValue::RectiValue(const core::string&name,const math::recti& v)
:IValue(name),value(v)
{
}


const mchar* RectiValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void RectiValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}
IValue* RectiValue::duplicate()
{
	return new RectiValue(m_name,value);
}

void RectiValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  RectiValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////

RectfValue::RectfValue(const core::string&name,const math::rectf& v)
:IValue(name),value(v)
{
}


const mchar* RectfValue::toString()const{
	return core::StringConverter::toString(value).c_str();
}
void RectfValue::parse(const core::string&v){
	return core::StringConverter::parse(v,value);
}
IValue* RectfValue::duplicate()
{
	return new RectfValue(m_name,value);
}

void RectfValue::loadXMLSettings(xml::XMLElement* elem)
{
	IValue::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("Value"));
	if(attr)
		core::StringConverter::parse(attr->value,value);
}
xml::XMLElement*  RectfValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IValue::exportXMLSettings(elem);

	e->addAttribute(mT("Value"),core::StringConverter::toString(value));
	return e;
}
//////////////////////////////////////////////////////////////////////////




}