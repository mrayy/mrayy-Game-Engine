
#include "stdafx.h"
#include "IGUIStaticText.h"


namespace mray
{
namespace GUI
{

const GUID IGUIStaticText::ElementType("GUIStaticText");

IGUIStaticText::IGUIStaticText(IGUIManager* creator):IGUIElement(ElementType,creator)
{
}
IGUIStaticText::~IGUIStaticText()
{
}

}
}

