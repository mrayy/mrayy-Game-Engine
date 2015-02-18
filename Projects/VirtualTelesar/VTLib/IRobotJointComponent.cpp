

#include "stdafx.h"

#if 0
#include "IRobotJointComponent.h"



namespace mray
{
namespace VT
{

IMPLEMENT_RTTI(ICoupledJointInfo,IObject);

IMPLEMENT_PROPERTY_TYPE_HEADER(JointName,ICoupledJointInfo,core::string,mT("TargetJoint"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(JointName,ICoupledJointInfo,core::string,SetJointName,GetJointName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Ratio,ICoupledJointInfo,float,mT("Ratio"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Ratio,ICoupledJointInfo,float,SetRatio,GetRatio,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Axis,ICoupledJointInfo,ETargetControlAxis,mT("Axis"),EPBT_Basic,mT(""),Axis_Twist);


ICoupledJointInfo::PropertyTypeAxis ICoupledJointInfo::PropertyTypeAxis::instance;
ETargetControlAxis ICoupledJointInfo::PropertyTypeAxis::GetValue(CPropertieSet*object)const
{
	ICoupledJointInfo* o=dynamic_cast<ICoupledJointInfo*>(object);
	if(!o)
		return m_default;
	return o->GetJointAxis();
}
bool ICoupledJointInfo::PropertyTypeAxis::SetValue(CPropertieSet*object,const ETargetControlAxis& v)
{
	ICoupledJointInfo* o=dynamic_cast<ICoupledJointInfo*>(object);
	if(!o)return false;
	return o->SetJointAxis(v);
}
core::string ICoupledJointInfo::PropertyTypeAxis::toString(CPropertieSet*object)const
{
	ETargetControlAxis a=GetValue(object);
	switch (a)
	{
	case Axis_Twist:return mT("T");
	case Axis_Swing1:return mT("S1");
	case Axis_Swing2:return mT("S2");
	case Axis_XTranslate:return mT("X");
	case Axis_YTranslate:return mT("Y");
	case Axis_ZTranslate:return mT("Z");
	}
	return mT("");
}
bool ICoupledJointInfo::PropertyTypeAxis::parse(CPropertieSet*reciver,const core::string&str)
{
	ETargetControlAxis v;
	if(str==mT("T"))
		v=Axis_Twist;
	else if(str==mT("S1"))
		v=Axis_Swing1;
	else if(str==mT("S2"))
		v=Axis_Swing2;
	else if(str==mT("X"))
		v=Axis_XTranslate;
	else if(str==mT("Y"))
		v=Axis_YTranslate;
	else if(str==mT("Z"))
		v=Axis_ZTranslate;
	else 
		return false;
	return SetValue(reciver,v);
}
bool ICoupledJointInfo::PropertyTypeAxis::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}
ICoupledJointInfo::ICoupledJointInfo()
{

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeJointName::instance);
		dic->addPropertie(&PropertyTypeAxis::instance);
		dic->addPropertie(&PropertyTypeRatio::instance);
	}
}
	

}
}

#endif