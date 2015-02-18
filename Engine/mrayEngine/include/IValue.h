

/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   9:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IValue.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IValue
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IValue___
#define ___IValue___

#include "mString.h"


namespace mray{

namespace xml{
	class XMLElement;
}
enum EValueType{
	EVT_UNKOWN,
	EVT_INT,		//int
	EVT_FLOAT,		//float
	EVT_BOOLEAN,	//bool
	EVT_COLOR,		//color
	EVT_STRING,		//string
	EVT_VECTOR2Di,	//vector2di
	EVT_VECTOR2Df,	//vector2df
	EVT_VECTOR3Di,	//vector3di
	EVT_VECTOR3Df,	//vector3df
	EVT_VECTOR4Di,	//vector4di
	EVT_VECTOR4Df,	//vector4df
	EVT_LINE2D,		//line2d
	EVT_LINE3D,		//line3d
	EVT_BOX3D,		//box3d
	EVT_MATRIX3x3,		//matrix
	EVT_MATRIX4x4,		//matrix
	EVT_PLANE,		//plane
	EVT_QUATERNION,	//quaternion
	EVT_RECTi,		//reci
	EVT_RECTf,		//rectf
	EVT_MATERIAL,
	EVT_TEXTURE,
	EVT_SHADER,
	EVT_COUNT,		//---
};

class MRAY_DLL IValue
{
public:
	core::string m_name;
public:
	IValue(const core::string&name):m_name(name){}
	virtual~IValue(){}

	const core::string& getName(){return m_name;}
	void setName(const core::string&name){m_name=name;}

	virtual EValueType getType(){return EVT_UNKOWN;}

	virtual const mchar* toString()const{return mT("");}
	virtual void parse(const core::string&v){}
	virtual IValue* duplicate()=0;

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);


	static IValue* CreateValueByType(const core::string&name,EValueType t);
};
typedef std::map<core::string,IValue*> ValuesMap;

}


#endif //___IValue___
