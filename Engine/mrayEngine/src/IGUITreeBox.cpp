

#include "stdafx.h"
#include "IGUITreeBox.h"


namespace mray
{
namespace GUI
{

	const GUID IGUITreeBox::ElementType("GUITreeBox");

IGUITreeBox::IGUITreeBox(IGUIManager*mngr):IGUIElement(ElementType,mngr),
	OnExpanded(mT("OnExpanded")),OnCollapsed(mT("OnCollapsed"))
{
	AddEvent(&OnExpanded);
	AddEvent(&OnCollapsed);
}
IGUITreeBox::~IGUITreeBox()
{
}

}
}
