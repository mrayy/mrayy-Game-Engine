
#include "stdafx.h"
#include "GUIPropComboListGridItem.h"

#include "IGUIPropertyGrid.h"
#include "EnumManager.h"


namespace mray
{
namespace GUI
{

GUIPropComboListGridItem::GUIPropComboListGridItem(IGUIPropertyGrid* grid)
	:IGUIPropGridItem(grid)
{
	m_component=new GUIComboListComponent(grid);
	m_component->listener=(GUIComboListComponent::IListener*)this;
}
GUIPropComboListGridItem::~GUIPropComboListGridItem()
{
	delete m_component;
}

void GUIPropComboListGridItem::OnChangeSelection(GUIListBoxComponent*caller)
{
	if(caller->GetSelectedItem()!=-1)
	{
		m_property->parse(m_object,caller->items[caller->GetSelectedItem()]->toString().GetAsString());
	}
}

void GUIPropComboListGridItem::SetProperty(IProperty* prop,CPropertieSet* object)
{
	IGUIPropGridItem::SetProperty(prop,object);
	m_component->text=m_property->toString(object);

	m_component->items.clear();
	const EnumInfo* enIfo=prop->GetEnum();
	if(enIfo)
	{
		EnumNameValMap::const_iterator it= enIfo->NameVal.begin();
		for(;it!=enIfo->NameVal.end();++it)
		{
			m_component->items.push_back(new StringListItem(it->first,(void*)it->second));
		}
	}

	m_component->enabled= m_property->getFlag(EPF_Write) || 
		m_property->getAccessMode()==EPA_RW || m_property->getAccessMode()==EPA_WRITE;
}

math::rectf GUIPropComboListGridItem::Draw(const math::rectf& rc)
{
	m_component->rc=rc;
	m_component->Draw();
	math::rectf r=rc;
	
	if(m_component->ItemsVisible())
	{
		float h=m_component->GetListBoxRect(rc,m_component->items.size()).getHeight();
		r.BRPoint.y+=h;
	}
	return r;
}

bool GUIPropComboListGridItem::HandleMouseEvent(MouseEvent*e)
{
	return m_component->OnMouseEvent(e);
}
bool GUIPropComboListGridItem::HandleKeyboardEvent(KeyboardEvent*e)
{
	return m_component->OnKeyboardEvent(e);
}



}
}
