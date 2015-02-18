
#include "stdafx.h"
#include "IGUICheckBox.h"


namespace mray
{
namespace GUI
{
	const GUID IGUICheckBox::ElementType("GUICheckBox");

IGUICheckBox::IGUICheckBox(IGUIManager* creator)
	:IGUIElement(ElementType,creator),
	OnChangeCheck(mT("OnChangeCheck"))
{
	AddEvent(&OnChangeCheck);
}
IGUICheckBox::~IGUICheckBox()
{
}


}
}

