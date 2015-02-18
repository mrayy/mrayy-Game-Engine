
#include "stdafx.h"
#include "IGUIEditBox.h"


namespace mray
{
namespace GUI
{

const GUID IGUIEditBox::ElementType("GUIEditbox");

IGUIEditBox::IGUIEditBox(IGUIManager*mngr):IGUIElement(ElementType,mngr)
{

	AddEvent(&OnKeyDown);
	AddEvent(&OnTextChange);
}
IGUIEditBox::~IGUIEditBox()
{
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Background,IGUIEditBox,bool,mT("Background"),EPBT_Basic,mT("Draw background"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Background,IGUIEditBox,bool,setBackground,getBackground,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Border,IGUIEditBox,bool,mT("Border"),EPBT_Basic,mT("Draw borders"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Border,IGUIEditBox,bool,setBorder,getBorder,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(HideText,IGUIEditBox,bool,mT("HideText"),EPBT_Basic,mT("Hide plain text"),false);
IMPLEMENT_PROPERTY_TYPE_GENERIC(HideText,IGUIEditBox,bool,setHideText,getHideText,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(HideChar,IGUIEditBox,string,mT("HideChar"),EPBT_Basic,mT("Hide character"),mT("*"));
IMPLEMENT_PROPERTY_TYPE_GENERIC(HideChar,IGUIEditBox,core::string,setHideChar,getHideChar,,,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(RegularExpression,IGUIEditBox,string,mT("RegularExpression"),EPBT_Basic,mT("Regular expression to match with the input text"),mT("*"));
IMPLEMENT_PROPERTY_TYPE_GENERIC(RegularExpression,IGUIEditBox,core::string,setRegex,getRegex,,,false)



}
}
