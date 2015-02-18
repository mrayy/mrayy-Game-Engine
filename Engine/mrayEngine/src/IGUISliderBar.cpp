
#include "stdafx.h"
#include "IGUISliderBar.h"


namespace mray
{
namespace GUI
{

const GUID IGUISliderBar::ElementType("GUISliderBar");



//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Value, IGUISliderBar, float, mT("Value"), EPBT_Basic, mT("Slider current value"), 0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Value, IGUISliderBar, float, SetValue, GetValue, core::StringConverter::toString, core::StringConverter::toFloat, false)


IMPLEMENT_PROPERTY_TYPE_HEADER(MinValue, IGUISliderBar, float, mT("MinValue"), EPBT_Basic, mT("Slider Min value"), 0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(MinValue, IGUISliderBar, float, SetMinValue, GetMinValue, core::StringConverter::toString, core::StringConverter::toFloat, false)


IMPLEMENT_PROPERTY_TYPE_HEADER(MaxValue, IGUISliderBar, float, mT("MaxValue"), EPBT_Basic, mT("Slider Max value"), 0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(MaxValue, IGUISliderBar, float, SetMaxValue, GetMaxValue, core::StringConverter::toString, core::StringConverter::toFloat, false)


IGUISliderBar::IGUISliderBar(IGUIManager* creator):IGUIElement(ElementType,creator),
OnChanged(mT("OnChanged"))
{
	AddEvent(&OnChanged);
	fillProperties();
}

IGUISliderBar::~IGUISliderBar()
{
}


void IGUISliderBar::fillProperties()
{
	CPropertieDictionary *dic = 0;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeValue::instance, mT("Behavior"));
		dic->addPropertie(&PropertyTypeMinValue::instance, mT("Behavior"));
		dic->addPropertie(&PropertyTypeMaxValue::instance, mT("Behavior"));

	}
}

}
}

