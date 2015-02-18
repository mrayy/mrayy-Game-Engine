

#include "StdAfx.h"
#include "IControllableComponent.h"


namespace mray
{
namespace VT
{

	/*
IMPLEMENT_PROPERTY_TYPE_HEADER(ControlValue,IControllableComponent,std::vector<float> ,mT("ControlValue"),EPBT_Basic,mT("Control Value"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ControlValue,IControllableComponent,std::vector<float> ,SetControlValue,GetControlValue,
								core::StringConverter::ArrayToString<float>,core::StringConverter::ParseArray<float>,false);
								*/
IControllableComponent::IControllableComponent()
{
/*	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		PropertyTypeControlValue::instance.setFlag(EPF_Serialize,false);
		dic->addPropertie(&PropertyTypeControlValue::instance);
	}*/
}


}
}