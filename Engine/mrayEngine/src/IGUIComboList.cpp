
#include "stdafx.h"
#include "IGUIComboList.h"




namespace mray
{
namespace GUI
{

const GUID IGUIComboList::ElementType("GUIComboList");


IGUIComboList::IGUIComboList(IGUIManager* manager):IGUIElement(ElementType,manager),
OnSelectChange(mT("SelectChange"))
{
	AddEvent(&OnSelectChange);
}
IGUIComboList::~IGUIComboList()
{
}


}
}