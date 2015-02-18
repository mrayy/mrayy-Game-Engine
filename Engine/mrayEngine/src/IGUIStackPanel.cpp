
#include "stdafx.h"
#include "IGUIStackPanel.h"
#include "EnumManager.h"


namespace mray
{
namespace GUI
{
const GUID IGUIStackPanel::ElementType("GUIStackPanel");

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Direction,IGUIStackPanel,EStackDirection,mT("Direction"),EPBT_Basic,mT("Panel Direction"),IGUIStackPanel::EVertical);
IGUIStackPanel::PropertyTypeDirection IGUIStackPanel::PropertyTypeDirection::instance;
IGUIStackPanel::EStackDirection IGUIStackPanel::PropertyTypeDirection::GetValue(CPropertieSet*object)const
{
	IGUIStackPanel* o=dynamic_cast<IGUIStackPanel*>(object);
	if(!o)
		return m_default;
	return o->GetStackDirection();
}
bool IGUIStackPanel::PropertyTypeDirection::SetValue(CPropertieSet*object,const IGUIStackPanel::EStackDirection& v)
{
	IGUIStackPanel* o=dynamic_cast<IGUIStackPanel*>(object);
	if(!o)return false;
	return o->SetStackDirection(v);
}
core::string IGUIStackPanel::PropertyTypeDirection::toString(CPropertieSet*object)const
{
	if(GetEnum())
		return GetEnum()->ToString(GetValue(object));
	return core::StringConverter::toString((int)GetValue(object),core::string(mT("IGUIStackPanel::EStackDirection")));
}
bool IGUIStackPanel::PropertyTypeDirection::parse(CPropertieSet*reciver,const core::string&str)
{
	if(GetEnum())
	{
		if(!CheckIsValidValue(str))
			return false;
	}
	int v=0;
	if(GetEnum())
		v= GetEnum()->Parse(str);
	else
		core::StringConverter::parse(str,v,mT("IGUIStackPanel::EStackDirection"));
	return SetValue(reciver,(IGUIStackPanel::EStackDirection)v);
}
bool IGUIStackPanel::PropertyTypeDirection::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}


//IMPLEMENT_PROPERTY_TYPE_GENERIC(Direction,IGUIStackPanel,EStackDirection,SetDirection,GetDirection,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Offset,IGUIStackPanel,float,mT("Offset"),EPBT_Basic,mT("Offset between elements"),2);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Offset,IGUIStackPanel,float,SetOffset,GetOffset,core::StringConverter::toString,core::StringConverter::toFloat,false)

IGUIStackPanel::IGUIStackPanel(IGUIManager* creator)
:IGUIPanelElement(ElementType,creator),m_direction(EHorizontal),m_offset(0)
{
}
IGUIStackPanel::~IGUIStackPanel()
{
}


}
}

