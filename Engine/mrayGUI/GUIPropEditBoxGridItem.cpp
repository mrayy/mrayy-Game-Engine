
#include "stdafx.h"
#include "GUIPropEditBoxGridItem.h"

#include "IProperty.h"
#include "CPropertieSet.h"
#include "IGUIPropertyGrid.h"

#include "BasicTypesGUID.h"

namespace mray
{
namespace GUI
{

GUIPropEditBoxGridItem::GUIPropEditBoxGridItem(IGUIPropertyGrid* grid)
:IGUIPropGridItem(grid)
{
	m_component=new GUIEditBoxComponent();
	m_component->owner=grid;
	m_component->listener=this;
}
GUIPropEditBoxGridItem::~GUIPropEditBoxGridItem()
{
	delete m_component;
}

void GUIPropEditBoxGridItem::SetProperty(IProperty* prop,CPropertieSet* object)
{
	IGUIPropGridItem::SetProperty(prop,object);
	m_component->text=m_property->toString(object);
	const BasicTypeInfo* ifo= BasicTypesGUID::GetTypeInfo(m_property->getPropertyType().ID());
	m_component->acceptsNumbers=true;
	if(ifo && ifo->IsNumeric())
		m_component->acceptsChars=false;
	else
		m_component->acceptsChars=true;

	m_component->enabled= m_property->getFlag(EPF_Write) || 
		m_property->getAccessMode()==EPA_RW || m_property->getAccessMode()==EPA_WRITE;
}
void GUIPropEditBoxGridItem::OnTextChangeCallback(GUIEditBoxComponent*caller)
{
	m_property->parse(m_object,caller->text.GetAsString());
}

math::rectf GUIPropEditBoxGridItem::Draw(const math::rectf& rc)
{
	if(!m_component->active)
	{
		m_component->text=m_property->toString(m_object);
	}
	m_component->rc=rc;
	m_component->Draw();
	return rc;
}

bool GUIPropEditBoxGridItem::HandleMouseEvent(MouseEvent*e)
{
	return m_component->OnMouseEvent(e);
}
bool GUIPropEditBoxGridItem::HandleKeyboardEvent(KeyboardEvent*e)
{
	return m_component->OnKeyboardEvent(e);
}




}
}

