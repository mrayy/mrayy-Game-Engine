

#include "stdafx.h"
#include "IGUIPropertyGrid.h"


namespace mray
{
namespace GUI
{


const GUID IGUIPropertyGrid::ElementType("GUIPropertyGrid");

IGUIPropertyGrid::IGUIPropertyGrid(IGUIManager*mngr)
:IGUIElement(ElementType,mngr),m_object(0)
{
}
IGUIPropertyGrid::~IGUIPropertyGrid()
{
}


}
}
