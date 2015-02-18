
#include "stdafx.h"
#include "IGUICatagoryPanel.h"



namespace mray
{
namespace GUI
{


const GUID IGUICatagoryPanel::ElementType("GUICatagoryPanel");

IGUICatagoryPanel::IGUICatagoryPanel(IGUIManager* creator)
:IGUIPanelElement(ElementType,creator)
{
}
IGUICatagoryPanel::~IGUICatagoryPanel()
{
}



}
}


