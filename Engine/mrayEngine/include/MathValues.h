
/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   10:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MathValues.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MathValues
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MathValues___
#define ___MathValues___


#include "IValue.h"


#include "Point4d.h"








namespace mray{

//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector2diValue:public IValue
{
public:
	math::vector2di value;
public:
	Vector2diValue(const core::string&name,const math::vector2di& v);

	virtual EValueType getType(){return EVT_VECTOR2Di;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector2dfValue:public IValue
{
public:
	math::vector2df value;
public:
	Vector2dfValue(const core::string&name,const math::vector2df& v);

	virtual EValueType getType(){return EVT_VECTOR2Df;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector3diValue:public IValue
{
public:
	math::vector3di value;
public:
	Vector3diValue(const core::string&name,const math::vector3di& v);

	virtual EValueType getType(){return EVT_VECTOR3Di;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector3dfValue:public IValue
{
public:
	math::vector3df value;
public:
	Vector3dfValue(const core::string&name,const math::vector3df& v);

	virtual EValueType getType(){return EVT_VECTOR3Df;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector4diValue:public IValue
{
public:
	math::vector4di value;
public:
	Vector4diValue(const core::string&name,const math::vector4di& v);

	virtual EValueType getType(){return EVT_VECTOR4Di;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Vector4dfValue:public IValue
{
public:
	math::vector4df value;
public:
	Vector4dfValue(const core::string&name,const math::vector4df& v);

	virtual EValueType getType(){return EVT_VECTOR4Df;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Line2dfValue:public IValue
{
public:
	math::line2d value;
public:
	Line2dfValue(const core::string&name,const math::line2d& v);

	virtual EValueType getType(){return EVT_LINE2D;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Line3dfValue:public IValue
{
public:
	math::line3d value;
public:
	Line3dfValue(const core::string&name,const math::line3d& v);

	virtual EValueType getType(){return EVT_LINE3D;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL box3dfValue:public IValue
{
public:
	math::box3d value;
public:
	box3dfValue(const core::string&name,const math::box3d& v);

	virtual EValueType getType(){return EVT_BOX3D;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Matrix3x3Value:public IValue
{
public:
	math::matrix3x3 value;
public:
	Matrix3x3Value(const core::string&name,const math::matrix3x3& v);

	virtual EValueType getType(){return EVT_MATRIX3x3;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL Matrix4x4Value:public IValue
{
public:
	math::matrix4x4 value;
public:
	Matrix4x4Value(const core::string&name,const math::matrix4x4& v);

	virtual EValueType getType(){return EVT_MATRIX4x4;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL PlaneValue:public IValue
{
public:
	math::Plane value;
public:
	PlaneValue(const core::string&name,const math::Plane& v);

	virtual EValueType getType(){return EVT_PLANE;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL QuaternionValue:public IValue
{
public:
	math::quaternion value;
public:
	QuaternionValue(const core::string&name,const math::quaternion& v);

	virtual EValueType getType(){return EVT_QUATERNION;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL RectiValue:public IValue
{
public:
	math::recti value;
public:
	RectiValue(const core::string&name,const math::recti& v);

	virtual EValueType getType(){return EVT_RECTi;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL RectfValue:public IValue
{
public:
	math::rectf value;
public:
	RectfValue(const core::string&name,const math::rectf& v);

	virtual EValueType getType(){return EVT_RECTf;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}


#endif //___MathValues___
