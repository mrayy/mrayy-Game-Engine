
#include "stdafx.h"
#include "GUIPropertyGrid.h"

#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "IGUITheme.h"
#include "GUIPropEditBoxGridItem.h"
#include "GUIPropComboListGridItem.h"
#include "GUIPropCheckBoxGridItem.h"
#include "GUICatagoryPanel.h"
#include "GUIPropGridStack.h"
#include "GUIStackPanel.h"
#include "GUIPropGridItem.h"
#include "GUISliderbarComponent.h"

#include "BasicTypesGUID.h"

namespace mray{
namespace GUI{

	IMPLEMENT_ELEMENT_FACTORY(GUIPropertyGrid);

GUIPropertyGrid::GUIPropertyGrid(IGUIManager* manager):IGUIPropertyGrid(manager),m_activeTab(0)
{
	m_FontAttributes.fontSize=12;
	m_FontAttributes.fontColor=video::DefaultColors::Black;
	m_FontAttributes.hasShadow=false;
	m_stackPanel=new GUIStackPanel(manager);
	m_stackPanel->SetStackDirection(GUIStackPanel::EVertical);

	m_slider=new GUISliderbarComponent();
	m_slider->owner=this;

	m_catagRegion=new GUIElementRegion(this);
	m_scrollRegion=new GUIElementRegion(this);

	m_stackPanel->AttachToRegion(m_catagRegion);
}
GUIPropertyGrid::~GUIPropertyGrid()
{
	SetPropertyObject(0);
	delete m_catagRegion;
	delete m_scrollRegion;
}


bool GUIPropertyGrid::_UpdateRegion(const math::rectf*vp)
{
	if(IGUIElement::_UpdateRegion(vp))
	{
		math::rectf rc=m_defaultRegion->GetRect();
		rc.BRPoint.x-=25;
		m_catagRegion->SetRect(rc);
		rc.ULPoint.x=rc.BRPoint.x;
		rc.BRPoint.x+=25;
		m_scrollRegion->SetRect(rc);

		m_stackPanel->_RectsDirt();
		return true;
	}
	return false;
}

void GUIPropertyGrid::SetPropertyObject(CPropertieSet* obj)
{
	IGUIPropertyGrid::SetPropertyObject(obj);

	m_stackPanel->ClearElements();
	m_categories.clear();
	m_objProps.clear();
	if(!m_object)
		return;
	CPropertieDictionary* dic= m_object->GetDictionary();
	if(!dic)
		return;

	RTTI* pr=(RTTI*)dic->GetRTTI();
	while(true)
	{
		if(dic)
		{
			const TPropertieGroupList& propList=dic->getGroups();
			TPropertieGroupList::const_iterator git=propList.begin();
			for(;git!=propList.end();++git)
			{
				CatagoryObj ctab(0,0);
				CategoryList::const_iterator tit= m_categories.find(git->first);
				if(tit==m_categories.end())
				{
					ctab.tab=new GUICatagoryPanel(GetCreator());
					ctab.tab->SetText(git->second.name);
					ctab.stack=new GUIPropGridStack(this);
					ctab.tab->AddElement(ctab.stack);

					ctab.tab->SetSize(math::vector2d(200,30));
					m_categories[git->first]=ctab;

					m_stackPanel->AddElement(ctab.tab);
				}else
					ctab=tit->second;


				const TPropertieList& props= git->second.properties;
				TPropertieList::const_iterator pit=props.begin();
				for(;pit!=props.end();++pit)
				{
					IGUIPropGridItem* itm=0;
					if(pit->second->GetEnum())
						itm=new GUIPropComboListGridItem(this);
					else if(pit->second->getPropertyType()==typeof(bool))
						itm=new GUIPropCheckBoxGridItem(this);
					else
						itm=new GUIPropEditBoxGridItem(this);
					itm->SetProperty(pit->second,m_object);

					ctab.stack->AddProperty(new GUIPropGridItem(this,itm));
				}
				m_objProps.insert(props.begin(),props.end());
			}
		}
		RTTI* r=pr->getParent();
		if(!r)
			break;
		dic=CPropertieSet::GetDictionary(r->getTypeName());
		pr=r;
	}

}

void GUIPropertyGrid::Draw(const math::rectf*vp)
{

	if(!IsVisible())
		return;
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	GUI::IFont* font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);

	const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();

	skin->drawSizableBox(creator->GetRenderQueue(),clipRect,0,mT("Frame"),video::DefaultColors::Gray);

	//m_stackPanel->SetPosition(clipRect.ULPoint);
	//m_stackPanel->SetSize(clipRect.getSize());
	m_stackPanel->Draw(vp);
	
	m_slider->rc=m_scrollRegion->GetClippedRect();
	m_slider->Draw();

/*
	CategoryList::iterator it=m_categories.begin();
	math::rectf rcName;
	math::rectf rcValue;
	rcName.ULPoint=clipRect.ULPoint;
	rcName.BRPoint=rcName.ULPoint+math::vector2d(clipRect.getWidth(),dimY);
	rcValue=rcName;

	rcName.BRPoint.x-=clipRect.getWidth()/2;
	rcValue.ULPoint.x=rcName.BRPoint.x;
	
	for(int i=0;it!=m_categories.end()&& i<totalCount;++it,++i)
	{
		IGUIPropGridItem* itm=*it;
		IProperty* p= itm->GetProperty();
		core::string name=p->getName();
		//core::string val=p->toString(m_object);

		video::SColor clr=video::DefaultColors::White;
		clr.A=0.4;

//		creator->GetRenderQueue()->AddQuad(0,rcName,math::rectf(0,0,1,1),clr);
		creator->GetRenderQueue()->AddQuad(0,rcName+math::rectf(1,1,-1,-1),math::rectf(0,0,1,1),clr);
		clr.A=0.8;
		//		creator->GetRenderQueue()->AddQuad(0,rcValue,math::rectf(0,0,1,1),clr);
		//creator->GetRenderQueue()->AddQuad(0,rcValue+math::rectf(1,1,-1,-1),math::rectf(0,0,1,1),clr);


		font->print(rcName,&m_FontAttributes,&rcName,name,creator->GetRenderQueue());

		math::rectf ir= itm->Draw(rcValue+math::rectf(1,1,-1,-1));
//		font->print(rcValue,&m_FontAttributes,&rcValue,val,creator->GetRenderQueue());

		rcName.ULPoint.y+=ir.getHeight();
		rcName.BRPoint.y+=ir.getHeight();

		rcValue.ULPoint.y+=ir.getHeight();
		rcValue.BRPoint.y+=ir.getHeight();
	}
*/
/*
	m_Color.A=AlphaEffect ? Alpha:Color().A;
	video::SColor secColor=Color();
	secColor.A=AlphaEffect ? Alpha:Color().A;

	skin->drawBox(getManager()->GetRenderQueue(),tRect,secColor,0,&clipRect,false);

	IFont*Font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!Font)return;
	math::rectf ElementRect=tRect;
	float charHeight=Font->getCharDimension('A',m_FontAttributs.fontSize).y+2;
	
	video::SColor clr=m_FontAttributs.fontColor;
	m_FontAttributs.fontColor.A=AlphaEffect ? Alpha:m_FontAttributs.fontColor.A;
	if(!Enable)
		m_FontAttributs.fontColor=video::SColor(128,128,128,AlphaEffect ? Alpha:m_FontAttributs.fontColor.A);
	if(ShowCaption){
		
		ElementRect=tRect;

		ElementRect.BRPoint.y=ElementRect.ULPoint.y+charHeight+4;
		Font->print(ElementRect,&m_FontAttributs,&clipRect,(utf8*)mT("PropertyGrid"),guiMngr->GetRenderQueue());
	}
	if(!m_selectedElement)return;
	TPropertieListIT it=m_selectedElement->getProperties();
	TPropertieListIT end=m_selectedElement->getPropertiesEnd();

	math::rectf CaptionRect=ElementRect;
	CaptionRect.BRPoint.x+=getSize().x*0.33f;
	ElementRect.ULPoint.x+=getSize().x*0.33f;

	video::SColor nameColor(230,230,230,AlphaEffect ? Alpha:Color().A);
	video::SColor valueColor(255,255,255,AlphaEffect ? Alpha:Color().A);
	video::SColor BorderColor(20,20,20,valueColor.A);
	for(;it!=end;++it){
		ElementRect.ULPoint.y+=charHeight+2;
		ElementRect.BRPoint.y+=charHeight+2;

		CaptionRect.ULPoint.y+=charHeight+2;
		CaptionRect.BRPoint.y+=charHeight+2;

		math::rectf tmpRect=CaptionRect;
		tmpRect.clipRect(clipRect);
		skin->drawBox(getManager()->GetRenderQueue(),tmpRect,nameColor,BorderColor);
//		Font->print(tmpRect,&m_FontAttributs,0,core::StringConverter::toString(it->second->getName()),guiMngr->GetRenderQueue());

		tmpRect=ElementRect;
		tmpRect.clipRect(clipRect);
		skin->drawBox(getManager()->GetRenderQueue(),tmpRect,valueColor,BorderColor);
//		Font->print(tmpRect,&m_FontAttributs,0,it->second->toString(),guiMngr->GetRenderQueue());
		
	}
	m_FontAttributs.fontColor=clr;*/
}
#if 0
bool GUIPropertyGrid::_OnMouseEvent(MouseEvent*evt)
{
	/*
	PropItemsList::iterator it=m_items.begin();
	for(;it!=m_items.end();++it)
	{
		if((*it)->HandleMouseEvent(evt))
		{
			if(m_activeItem)
				m_activeItem->SetActive(false);
			m_activeItem=*it;
			m_activeItem->SetActive(true);
			return true;
		}
	}
	if(evt->event==MET_LEFTDOWN)
	{
		if(m_activeItem)
			m_activeItem->SetActive(false);
		m_activeItem=0;
	}
	return m_stackPanel->_OnMouseEvent(evt);*/
	return IGUIElement::_OnMouseEvent(evt);
}
bool GUIPropertyGrid::_OnKeyboardEvent(KeyboardEvent*evt)
{
	return IGUIElement::_OnKeyboardEvent(evt);
	/*return m_stackPanel->_OnKeyboardEvent(evt);
	if(m_activeItem)
		return m_activeItem->HandleKeyboardEvent(evt);
	
	PropItemsList::iterator it=m_items.begin();
	for(;it!=m_items.end();++it)
	{
		if((*it)->HandleKeyboardEvent(evt))
			return true;
	}*/
	return false;
}
#endif

bool GUIPropertyGrid::OnEvent(Event* e)
{
	if(e->getType()==ET_Mouse)
	{
		if(m_slider->OnMouseEvent((MouseEvent*)e))
			return true;
	}
	if(m_stackPanel->OnEvent(e))
		return true;
	return IGUIElement::OnEvent(e);
}

IGUIElement* GUIPropertyGrid::Duplicate()
{
	return new GUIPropertyGrid(GetCreator());
}
FontAttributes* GUIPropertyGrid::GetFontAttributes()
{
	return &m_FontAttributes;
}


}
}


