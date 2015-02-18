


/********************************************************************
	created:	2011/01/21
	created:	21:1:2011   15:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TypedProperty.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TypedProperty
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TypedProperty_h__
#define TypedProperty_h__

#include "IProperty.h"

namespace mray
{
/*
template <class T>
class TypedProperty:public IProperty
{
private:
protected:
public:
	TypedProperty(const core::string&name,EPropertyBasicType type,const GUID& propType,const  core::string&info=mT(""))
		:IProperty(name,type,propType,info)
	{
	}
	virtual ~TypedProperty(){}


	virtual T GetValue(CPropertieSet*object)const=0;
	virtual bool SetValue(CPropertieSet*object,const T& v)=0;

	virtual const T& GetDefaultValue()const=0;
	virtual bool isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}
};

*/

#define DECLARE_PROPERTY_TYPE(type,dataType,SPECS)\
class SPECS PropertyType##type:public IProperty\
{\
protected:\
	dataType m_default;\
public:\
	static PropertyType##type instance;\
	PropertyType##type();\
	virtual dataType GetValue(CPropertieSet*object)const;\
	virtual bool SetValue(CPropertieSet*object,const dataType& v);\
	virtual core::string toString(CPropertieSet*object)const;\
	virtual bool parse(CPropertieSet*reciver,const core::string&str);\
	virtual const dataType& GetDefaultValue()const{return m_default;}\
	virtual bool isDefault(CPropertieSet*reciver)const;\
};

#define IMPLEMENT_PROPERTY_TYPE_HEADER(type,ObjectType,dataType,Name,proptype,Description,Default)\
	ObjectType::PropertyType##type::PropertyType##type()\
	:IProperty(Name,proptype,mray::GUID(#dataType),Description),m_default(Default){}
		//:TypedProperty(Name,proptype,mray::GUID(#dataType),Description)

#define IMPLEMENT_PROPERTY_TYPE_GENERIC(type,ObjectType,dataType,SetFunction,GetFunction,toStringFunc,parseFunc,useCheck)\
ObjectType::PropertyType##type ObjectType::PropertyType##type::instance;\
\
dataType ObjectType::PropertyType##type::GetValue(CPropertieSet*object)const\
{\
	ObjectType* o=dynamic_cast<ObjectType*>(object);\
	if(!o)\
		return m_default;\
	return o->GetFunction();\
}\
bool ObjectType::PropertyType##type::SetValue(CPropertieSet*object,const dataType& v)\
{\
	ObjectType* o=dynamic_cast<ObjectType*>(object);\
	if(!o)return false;\
	return o->SetFunction(v);\
}\
core::string ObjectType::PropertyType##type::toString(CPropertieSet*object)const\
{\
	return toStringFunc(GetValue(object));\
}\
bool ObjectType::PropertyType##type::parse(CPropertieSet*reciver,const core::string&str)\
{\
	if(useCheck)\
	{\
		if(!CheckIsValidValue(str))\
			return false;\
	}\
	return SetValue(reciver,parseFunc(str));\
}\
bool ObjectType::PropertyType##type::isDefault(CPropertieSet*reciver)const\
{\
	if(GetValue(reciver)==GetDefaultValue())\
		return true;\
	return false;\
}


#define DECLARE_SETGET_PROP(Name,dataType)\
	virtual bool Set##Name(const dataType& v);\
	virtual const dataType& Get##Name()const;

#define IMPLEMENT_SETGET_PROP(object,Name,dataType,value,default,setter,getter)\
	bool object::Set##Name(const dataType& v){value=v;return true;}\
	const dataType& object::Get##Name()const{return value;}\
	IMPLEMENT_PROPERTY_TYPE_HEADER(Name,object,dataType,mT(#Name),EPBT_Basic,mT(""),default);\
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,object,dataType,Set##Name,Get##Name,getter,setter,false);



}
#endif // TypedProperty_h__
