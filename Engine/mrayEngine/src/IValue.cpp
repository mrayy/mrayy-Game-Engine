#include "stdafx.h"

#include "IValue.h"
#include "XMLElement.h"
#include "EnumManager.h"

#include "GenericValues.h"
#include "VideoValues.h"
#include "MathValues.h"


namespace mray{

void IValue::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
}
xml::XMLElement*  IValue::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Value"));
	elem->addSubElement(e);
	e->addAttribute(mT("Name"),m_name);
	e->addAttribute(mT("Type"),EnumManager::getInstance().getName(mT("EValueType"),getType()));

	return e;
}



IValue* IValue::CreateValueByType(const core::string&name,EValueType t)
{
	switch (t)
	{
	case EVT_BOOLEAN:
		return new BoolValue(name,false);
	case EVT_INT:
		return new IntValue(name,0);
	case EVT_FLOAT:
		return new FloatValue(name,0);
	case EVT_STRING:
		return new StringValue(name,mT(""));
	case EVT_VECTOR2Di:
		return new Vector2diValue(name,math::vector2di::Zero);
	case EVT_VECTOR2Df:
		return new Vector2diValue(name,math::vector2df::Zero);
	case EVT_VECTOR3Di:
		return new Vector3diValue(name,math::vector3di::Zero);
	case EVT_VECTOR3Df:
		return new Vector3diValue(name,math::vector3di::Zero);
	case EVT_VECTOR4Di:
		return new Vector4diValue(name,math::vector4di::Zero);
	case EVT_VECTOR4Df:
		return new Vector4diValue(name,math::vector4di::Zero);
	case EVT_LINE2D:
		return new Line2dfValue(name,math::line2d(0,0));
	case EVT_LINE3D:
		return new Line3dfValue(name,math::line3d(0,0));
	case EVT_BOX3D:
		return new box3dfValue(name,math::box3d(0));
	case EVT_MATRIX3x3:
		return new Matrix3x3Value(name,math::matrix3x3::Identity);
	case EVT_MATRIX4x4:
		return new Matrix4x4Value(name,math::matrix4x4::Identity);
	case EVT_PLANE:
		return new PlaneValue(name,math::Plane(0,math::vector3d::YAxis));
	case EVT_QUATERNION:
		return new QuaternionValue(name,math::quaternion::Identity);
	case EVT_RECTi:
		return new RectiValue(name,math::recti(0,0,0,0));
	case EVT_RECTf:
		return new RectfValue(name,math::rectf(0,0,0,0));

	case EVT_MATERIAL:
		return new MaterialValue(name,0);
	case EVT_TEXTURE:
		return new TextureValue(name,0);
	case EVT_COLOR:
		return new ColorValue(name,0);
	case EVT_SHADER:
		return new ShaderValue(name,0);
	}

	return 0;

}

}