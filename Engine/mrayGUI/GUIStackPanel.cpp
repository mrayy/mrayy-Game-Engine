
#include "stdafx.h"
#include "GUIStackPanel.h"
#include "GUISliderbarComponent.h"
#include "IGUIManager.h"

namespace mray
{
namespace GUI
{

IMPLEMENT_ELEMENT_FACTORY(GUIStackPanel);

GUIStackPanel::GUIStackPanel(IGUIManager*mngr)
:IGUIStackPanel(mngr)
{
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{

		dic->addPropertie(&PropertyTypeDirection::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeOffset::instance,mT("Design"));
	}
	m_startPos=0;
	m_sliderComp=new GUISliderbarComponent();
	m_sliderComp->owner=this;
}
GUIStackPanel::~GUIStackPanel()
{
}

bool GUIStackPanel::SetStackDirection(IGUIStackPanel::EStackDirection dir)
{
	if(!IGUIStackPanel::SetStackDirection(dir))
		return false;
	if(dir==EHorizontal)
		m_sliderComp->m_direction=GUISliderbarComponent::EHorizontal;
	else
		m_sliderComp->m_direction=GUISliderbarComponent::EVertical;
	return true;
}

/*
const math::rectf& GUIStackPanel::GetUnclippedRect()
{
	IGUIPanelElement::GetUnclippedRect();
	/ *if(m_unclippedRect.getHeight()>m_maxHeight)
	{
		m_unclippedRect.BRPoint.y=m_unclippedRect.ULPoint.y+m_maxHeight;
		m_clippedRectDirt=true;
	}* /
	return m_unclippedRect;
}*/

void GUIStackPanel::SetScrollOffset(float offset)
{ 
	m_startPos = offset;
	m_startPos = math::clamp<float>(m_startPos, 0, (m_sliderComp->itemsCount - m_sliderComp->pageSize));
	if ((m_sliderComp->itemsCount - m_sliderComp->pageSize) > 0)
		m_sliderComp->SetPercentValue(m_startPos / (m_sliderComp->itemsCount - m_sliderComp->pageSize));
}
bool GUIStackPanel::_OnMouseEvent(MouseEvent* e)
{
	if(GetDefaultRegion()->GetClippedRect().IsPointInside(e->pos))
		GetCreator()->SetMouseOn(this);
	if(m_sliderComp->OnMouseEvent(e))
	{
		m_startPos=m_sliderComp->GetPercentValue()*(m_sliderComp->itemsCount-m_sliderComp->pageSize);
		return true;
	}
	return IGUIPanelElement::_OnMouseEvent(e);
}
void GUIStackPanel::_RecalcPositions()
{
	const ElementList& elems= GetElements();
	ElementList::const_iterator it=elems.begin();
	math::vector2d pos;
	math::vector2d start;
	if(m_direction==EHorizontal)
	{
		pos.x=m_offset;
		start.x=m_startPos;
	}
	else
	{
		pos.y=m_offset;
		start.y=m_startPos;
	}
	m_acctualSize=0;
	float maxSz=0;
	for (;it!=elems.end();++it)
	{
		IGUIElement* elem=*it;
		if (!elem->IsVisible())
			continue;
		bool locked=elem->IsLocked();
		elem->SetLocked(false);
		elem->SetDocking(EED_None);
		elem->SetLocked(locked);
		if(m_direction==EHorizontal)
		{
			float y = elem->GetPosition().y;
			elem->SetPosition(math::vector2d((pos - start).x,y));
			maxSz = math::Max(maxSz, elem->GetSize().y);
			pos.x+=elem->GetSize().x+m_offset;
			elem->SetHorizontalAlignment(EHA_Left);
		}else
		{
			float x = elem->GetPosition().x;
			elem->SetPosition(math::vector2d(x,(pos - start).y));
			maxSz=math::Max(maxSz,elem->GetSize().x);
			pos.y+=elem->GetSize().y+m_offset;
			elem->SetVerticalAlignment(EVA_Top);
		}
	}
	if(m_direction==EHorizontal)
	{
		m_acctualSize.x=pos.x;
		m_acctualSize.y=maxSz;
		m_sliderComp->itemsCount=pos.x;
	}else
	{
		m_acctualSize.x=maxSz;
		m_acctualSize.y=pos.y;
		m_sliderComp->itemsCount=pos.y;
	}
	//m_size=m_acctualSize;

}
void GUIStackPanel::Draw(const math::rectf*vp)
{
	_RecalcPositions();
	_UpdateRegion(vp);
	IGUIPanelElement::Draw(vp);

	const math::rectf& rc=GetDefaultRegion()->GetClippedRect();
	m_sliderComp->rc=rc;
	bool show = false;
	if(m_direction==EHorizontal)
	{
		m_sliderComp->rc.ULPoint.y=math::Max(rc.ULPoint.y,rc.BRPoint.y-20);
		m_sliderComp->pageSize=rc.getWidth();
		if (m_acctualSize.x > rc.getWidth())
			show = true;
	}
	else
	{
		m_sliderComp->rc.ULPoint.x=math::Max(rc.ULPoint.x,rc.BRPoint.x-20);
		m_sliderComp->pageSize = rc.getHeight();
		if (m_acctualSize.y > rc.getHeight())
			show = true;
	}


	if (show)
	{
		math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
		GetCreator()->GetDevice()->setScissorRect(m_sliderComp->rc);
		m_sliderComp->Draw();
		GetCreator()->GetRenderQueue()->Flush();
		GetCreator()->GetDevice()->setScissorRect(oldScissor);
	}
}

bool GUIStackPanel::SetUseScroll(bool scroll)
{
	m_sliderComp->DrawScroll(scroll);
	return true;
}
bool GUIStackPanel::GetUseScroll()const
{
	return m_sliderComp->IsDrawScroll();
}

IGUIElement* GUIStackPanel::Duplicate()
{
	return new GUIStackPanel(GetCreator());
}
FontAttributes* GUIStackPanel::GetFontAttributes()
{
	return 0;
}

}
}
