

#include "stdafx.h"
#include "GUIPropCheckBoxGridItem.h"

#include "IProperty.h"
#include "CPropertieSet.h"
#include "IGUIPropertyGrid.h"

namespace mray
{
namespace GUI
{

GUIPropCheckBoxGridItem::GUIPropCheckBoxGridItem(IGUIPropertyGrid* grid)
	:IGUIPropGridItem(grid)
{
	m_component=new GUICheckBoxComponent();
	m_component->owner=grid;
	m_component->listener=this;
	m_component->text.Set(mT(""));
	m_component->m_BGOpaque=false;
}
GUIPropCheckBoxGridItem::~GUIPropCheckBoxGridItem()
{
	delete m_component;
}

void GUIPropCheckBoxGridItem::SetProperty(IProperty* prop,CPropertieSet* object)
{
	IGUIPropGridItem::SetProperty(prop,object);
	m_component->m_checked=core::StringConverter::toBool(prop->toString(object));
	m_component->enabled= m_property->getFlag(EPF_Write) || 
		m_property->getAccessMode()==EPA_RW || m_property->getAccessMode()==EPA_WRITE;
}
void GUIPropCheckBoxGridItem::OnChangeCheck(GUICheckBoxComponent*caller)
{
	m_property->parse(m_object,caller->m_checked? mT("true"):mT("false"));
}

math::rectf GUIPropCheckBoxGridItem::Draw(const math::rectf& rc)
{
	if(!m_component->active)
	{
		m_component->m_checked=core::StringConverter::toBool(m_property->toString(m_object));
	}
	m_component->rc=rc;
	m_component->Draw();
	return rc;
}

bool GUIPropCheckBoxGridItem::HandleMouseEvent(MouseEvent*e)
{
	return m_component->OnMouseEvent(e);
}
bool GUIPropCheckBoxGridItem::HandleKeyboardEvent(KeyboardEvent*e)
{
	return m_component->OnKeyboardEvent(e);
}




}
}





