#include "stdafx.h"

#include "IGUIElement.h"
#include "IGUIPanelElement.h"

#include "Event.h"

#include "MouseEvent.h"
#include "KeyboardEvent.h"

#include "IGUIElementVisitor.h"
#include "EnumManager.h"
#include "GUIElementRegion.h"
#include "IRenderArea.h"
#include "FontAttributes.h"

namespace mray
{
namespace GUI
{


IGUIElement::IGUIElement(const GUID& type,IGUIManager* creator)
:OnMouseEnter(mT("MouseEnter")),OnMouseLeave(mT("MouseLeave")),OnMouseMove(mT("MouseMove")),
	OnReceiveFocus(mT("ReceiveFocus")),OnLostFocus(mT("LostFocus")),OnMouseDown(mT("MouseDown")),OnMouseUp(mT("MouseUp")),
	m_type(&type),m_creator(creator),m_eventReciver(0),
	m_visible(true),m_enabled(true),m_docking(EED_None),m_horizontalAlign(EHA_Left),m_verticalAlign(EVA_Top),m_id(0),m_attachedRegion(0),
	m_derivedPosDirt(false),
	m_derivedSizeDirt(false),
	m_clippedRectDirt(false),
	m_unclippedRectDirt(false),
	m_locked(false),m_text(mT(""))
{
	m_defaultRegion=new GUIElementRegion(this);

	m_anchor[EEA_Top] = false;
	m_anchor[EEA_Left] = false;
	m_anchor[EEA_Bottom] = false;
	m_anchor[EEA_Right] = false;
	fillProperties();
}
IGUIElement::~IGUIElement()
{
	delete m_defaultRegion;
}

void IGUIElement::fillProperties()
{
	CPropertieDictionary *dic=0;
	m_color=1;
	m_deriveAlpha=1;
	m_derivedAlphaValue=1;

	if(CreateDictionary(&dic))
	{
		PropertyTypeHorizontalAlignment::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EHorizontalAlignment")));
		PropertyTypeVerticalAlignment::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EVerticalAlignment")));
		PropertyTypeDocking::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EElementDock")));


		dic->addPropertie(&PropertyTypeLocked::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeName::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeID::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeVisible::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeEnabled::instance,mT("Design"));
		dic->addPropertie(&PropertyTypePosition::instance, mT("Design"));
		dic->addPropertie(&PropertyTypeAnchorPosition::instance, mT("Design"));
		dic->addPropertie(&PropertyTypeSize::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeHorizontalAlignment::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeVerticalAlignment::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeDocking::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeText::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeColor::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeDeriveAlpha::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeAlpha::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeAnchorLeft::instance, mT("Design"));
		dic->addPropertie(&PropertyTypeAnchorRight::instance, mT("Design"));
		dic->addPropertie(&PropertyTypeAnchorTop::instance, mT("Design"));
		dic->addPropertie(&PropertyTypeAnchorBottom::instance, mT("Design"));

	}

	AddEvent(&OnMouseEnter);
	AddEvent(&OnMouseLeave);
	AddEvent(&OnMouseMove);
	AddEvent(&OnMouseDown);
	AddEvent(&OnMouseUp);

	AddEvent(&OnReceiveFocus);
	AddEvent(&OnLostFocus);
}

void IGUIElement::_UpdateDocking(const math::rectf*vp)
{
	if(!m_attachedRegion && !vp)
		return;
	m_clippedRectDirt=true;
	math::rectf parentRC;
	if(m_attachedRegion)
		parentRC=m_attachedRegion->GetRect();
	else
		parentRC.BRPoint=vp->getSize();
	math::rectf m_unclippedRect;//=m_defaultRegion->GetRect();
	switch (m_docking)
	{
	case EED_Top:
		{
			m_unclippedRect.ULPoint=parentRC.ULPoint;
			m_unclippedRect.BRPoint.x=parentRC.BRPoint.x;
			m_unclippedRect.BRPoint.y=parentRC.ULPoint.y+GetSize().y;
		}break;
	case EED_Bottom:
		{
			m_unclippedRect.ULPoint.x=parentRC.ULPoint.x;
			m_unclippedRect.ULPoint.y=parentRC.BRPoint.y-GetSize().y;
			m_unclippedRect.BRPoint=parentRC.BRPoint;
		}break;
	case EED_Left:
		{
			m_unclippedRect.ULPoint=parentRC.ULPoint;
			m_unclippedRect.BRPoint.y=parentRC.BRPoint.y;
			m_unclippedRect.BRPoint.x=parentRC.ULPoint.x+GetSize().x;
		}break;
	case EED_Right:
		{
			m_unclippedRect.ULPoint.x=parentRC.BRPoint.x-GetSize().x;
			m_unclippedRect.ULPoint.y=parentRC.ULPoint.y;
			m_unclippedRect.BRPoint=parentRC.BRPoint;
		}break;
	case EED_Fill:
		{
			m_unclippedRect=parentRC;
		}break;
	}
	m_defaultRegion->SetRect(m_unclippedRect);
}

void IGUIElement::_UpdateAlignment(const math::rectf*vp)
{
	const math::vector2d& pos=GetDerivedPosition();
	const math::vector2d& size=GetSize();

	math::rectf m_unclippedRect;//=m_defaultRegion->GetRect();
	math::rectf parentRC;
	bool parentExist=m_attachedRegion!=0 || vp!=0;
	if(parentExist)
	{
		if(m_attachedRegion)
			parentRC=m_attachedRegion->GetClippedRect();
		else parentRC.BRPoint=vp->getSize();
	}


	if(m_horizontalAlign == EHA_Left || !parentExist)
	{
		m_unclippedRect.ULPoint.x=pos.x;
		m_unclippedRect.BRPoint.x=pos.x+size.x;
	}else if(m_horizontalAlign==EHA_Center)
	{
		m_unclippedRect.ULPoint.x=parentRC.ULPoint.x + (parentRC.getWidth()-size.x)*0.5f + m_position.x;
		m_unclippedRect.BRPoint.x=m_unclippedRect.ULPoint.x+size.x;
	}else{
		m_unclippedRect.ULPoint.x=parentRC.BRPoint.x-pos.x;
		m_unclippedRect.BRPoint.x=m_unclippedRect.ULPoint.x+size.x;
	}

	if(m_verticalAlign == EVA_Top || !parentExist)
	{
		m_unclippedRect.ULPoint.y=pos.y;
		m_unclippedRect.BRPoint.y=pos.y+size.y;
	}else if(m_verticalAlign==EVA_Center)
	{
		m_unclippedRect.ULPoint.y=parentRC.ULPoint.y + (parentRC.getHeight()-size.y)*0.5f + m_position.y;
		m_unclippedRect.BRPoint.y=m_unclippedRect.ULPoint.y+size.y;
	}else{
		m_unclippedRect.ULPoint.y=parentRC.BRPoint.y-pos.y;
		m_unclippedRect.BRPoint.y=m_unclippedRect.ULPoint.y+size.y;
	}
	m_defaultRegion->SetRect(m_unclippedRect);
}

void IGUIElement::_UpdateAnchor(const math::rectf*vp)
{
	
	const math::vector2d& pos = GetDerivedPosition();
	const math::vector2d& size = GetSize();

	math::rectf m_unclippedRect;//=m_defaultRegion->GetRect();
	math::rectf parentRC;
	bool parentExist = m_attachedRegion != 0 || vp != 0;
	if (parentExist)
	{
		if (m_attachedRegion)
			parentRC = m_attachedRegion->GetClippedRect();
		else parentRC.BRPoint = vp->getSize();
	}
	else
		return;

	if (m_anchor[EEA_Left])
	{
		m_unclippedRect.ULPoint.x = pos.x;
	}
	if (m_anchor[EEA_Right])
	{
		m_unclippedRect.BRPoint.x = parentRC.BRPoint.x - m_anchorPosition.x;
	}
	if (m_anchor[EEA_Top])
	{
		m_unclippedRect.ULPoint.y = pos.y;
	}
	if (m_anchor[EEA_Bottom])
	{
		m_unclippedRect.BRPoint.y = parentRC.BRPoint.y - m_anchorPosition.y;
	}


	if (!m_anchor[EEA_Left])
	{
		m_unclippedRect.ULPoint.x = m_unclippedRect.BRPoint.x - size.x;
	}
	if (!m_anchor[EEA_Right])
	{
		m_unclippedRect.BRPoint.x = m_unclippedRect.ULPoint.x + size.x;
	}
	if (!m_anchor[EEA_Top])
	{
		m_unclippedRect.ULPoint.y = m_unclippedRect.BRPoint.y - size.y;
	}
	if (!m_anchor[EEA_Bottom])
	{
		m_unclippedRect.BRPoint.y = m_unclippedRect.ULPoint.y + size.y;
	}
	m_defaultRegion->SetRect(m_unclippedRect);
}

void IGUIElement::SetCreator(IGUIManager* mngr)
{
	m_creator=mngr;
	OnSetCreator(GUIElementEvent(this));
}
IGUIManager* IGUIElement::GetCreator()
{
	return m_creator;
}

const bool& IGUIElement::IsLocked()
{
	return m_locked;
}
bool IGUIElement::SetLocked(const bool& locked)
{
	m_locked=locked;
	return true;
}
bool IGUIElement::SetID(const uint&id)
{
	if(m_locked)
		return false;
	m_id=id;
	return true;
}
const uint& IGUIElement::GetID()
{
	return m_id;
}

bool IGUIElement::SetName(const core::string& name)
{
	if(m_locked)
		return false;
	m_name=name;
	return true;
}
const core::string&  IGUIElement::GetName() const
{
	return m_name;
}

const GUID& IGUIElement::GetType()const
{
	return *m_type;
}

bool IGUIElement::SetText(const core::UTFString&t)
{
	if(m_locked)
		return false;
	m_text=t;
	return true;
}
const core::UTFString& IGUIElement::GetText()const 
{
	return m_text;
}
bool IGUIElement::SetVisible(bool v)
{
	if(m_locked)
		return false;
	m_visible=v;
	OnChangeVisible(this);
	return true;
}
bool IGUIElement::IsVisible(bool parentSearch)const
{
	return m_visible && (parentSearch ? (m_attachedRegion? 
		m_attachedRegion->GetOwner()->IsVisible(true):true):true);
}
const bool& IGUIElement::IsVisible()const
{
	return m_visible;
}
void IGUIElement::_UpdateDerivedAlpha()
{
	if(m_attachedRegion && m_attachedRegion->GetOwner())
		m_derivedAlphaValue=m_color.A* m_attachedRegion->GetOwner()->GetDerivedAlpha();
}
bool IGUIElement::SetAlpha(float c)
{
	m_color.A=c;
	m_derivedAlphaValue=c;
	_UpdateDerivedAlpha();
	return true;
}

bool IGUIElement::Show()
{
	return SetVisible(true);
}
bool IGUIElement::Hide()
{
	return SetVisible(false);
}

bool IGUIElement::SetEnabled(bool e)
{
	if(m_locked)
		return false;
	m_enabled=e;
	OnChangeEnable(this);
	return true;
}
bool IGUIElement::IsEnabled(bool parentSearch)const
{
	return m_enabled && (parentSearch ? (m_attachedRegion? m_attachedRegion->GetOwner()->IsEnabled(true):true):true);

}
const bool& IGUIElement::IsEnabled()const
{
	return m_enabled;
}

bool IGUIElement::Enable()
{
	return SetEnabled(true);
}
bool IGUIElement::Disable()
{
	return SetEnabled(false);
}



bool IGUIElement::MoveToTop()
{
	if(m_locked)
		return false;
	if(!m_attachedRegion)
		return false;
	return ((IGUIPanelElement*)m_attachedRegion->GetOwner())->MoveElementToTop(this);
}
bool IGUIElement::MoveToBottom()
{
	if(m_locked)
		return false;
	if(!m_attachedRegion)
		return false;
	return ((IGUIPanelElement*)m_attachedRegion->GetOwner())->MoveElementToBottom(this);
}


bool IGUIElement::IsPointInside(const math::vector2d& pos)
{
	const math::rectf& r=m_defaultRegion->GetClippedRect();
	return r.IsPointInside(pos);
}

bool IGUIElement::SetPosition(const math::vector2d& pos)
{
	if(m_locked)
		return false;
	m_position=pos;
	OnMoved(GUIElementEvent(this));
	return true;
}
bool IGUIElement::Move(const math::vector2d& dpos)
{
	return SetPosition(GetPosition()+dpos);
}
const math::vector2d& IGUIElement::GetPosition()const
{
	return m_position;
}

bool IGUIElement::SetAnchorPosition(const math::vector2d& pos)
{
	if (m_locked)
		return false;
	m_anchorPosition = pos;
	return true;
}

const math::vector2d& IGUIElement::GetAnchorPosition()const
{
	return m_anchorPosition;
}
const math::vector2d& IGUIElement::GetDerivedPosition()
{
	if(m_derivedPosDirt)
	{
		m_derivedPosition=GetPosition();
		if(m_attachedRegion)
			m_derivedPosition+=m_attachedRegion->GetRect().ULPoint;
		m_derivedPosDirt=false;
	}
	return m_derivedPosition;
}

bool IGUIElement::SetSize(const math::vector2d& sz)
{
	if(m_locked)
		return false;
	m_size=sz;
	OnResized(GUIElementEvent(this));
	return true;
}
const math::vector2d& IGUIElement::GetSize()const
{
	return m_size;
}
const math::vector2d& IGUIElement::GetDerivedSize()
{

	if (m_derivedSizeDirt)
	{
		m_derivedSize = GetSize();
		if (m_attachedRegion)
			m_derivedSize += m_attachedRegion->GetRect().getSize();
		m_derivedSizeDirt = false;
	}
	return m_derivedSize;
}
/*
const math::rectf& IGUIElement::GetUnclippedRect()
{
	if(m_unclippedRectDirt || m_docking!=EED_None)
	{
		if(m_docking==EED_None){
			_UpdateAlignment();
		}else
		{
			if(m_unclippedRectDirt || m_parent && m_parent->m_clippedRectDirt)
				_UpdateDocking();
		}

		m_unclippedRectDirt=false;
	}
	return m_unclippedRect;
}
const math::rectf& IGUIElement::GetClippedRect()
{
	if(m_clippedRectDirt || m_unclippedRectDirt)
	{
		m_clippedRect=GetUnclippedRect();
		if(m_parent)
			m_clippedRect.clipRect(m_parent->GetClippedRect());

		m_clippedRectDirt=false;
	}
	return m_clippedRect;
}
*/
bool IGUIElement::_UpdateRegion(const math::rectf*vp)
{
	bool res=false;
	if(!m_attachedRegion && vp && vp->getSize()!=m_defaultRegion->GetRect().getSize())
	{
		_RectsDirt();
	}
	if(m_unclippedRectDirt || m_docking!=EED_None)
	{

	
		m_clippedRectDirt=true;
		res=true;
	}
	if(m_clippedRectDirt)
	{
		if (m_docking == EED_None){
			if (m_anchor[EEA_Left] || m_anchor[EEA_Right] || m_anchor[EEA_Top] || m_anchor[EEA_Bottom])
				_UpdateAnchor(vp);
			else
				_UpdateAlignment(vp);
		}
		else
		{
			if (m_unclippedRectDirt || m_attachedRegion && m_clippedRectDirt)
			{
				_UpdateDocking(vp);
			}
		}
		if(m_attachedRegion )
			m_defaultRegion->SetClipRect(m_attachedRegion->GetClippedRect());
		m_unclippedRectDirt = false;
		m_clippedRectDirt=false;
		res=true;
	}
	return res;
}
const GUIElementRegion* IGUIElement::GetDefaultRegion()
{
	_UpdateRegion(0);
	return m_defaultRegion;
}
const GUIElementRegion* IGUIElement::GetDefaultRegion()const
{
	return m_defaultRegion;
}

void IGUIElement::AttachToRegion(GUIElementRegion* p)
{
	m_attachedRegion=p;
	m_unclippedRectDirt=true;
	OnChangeParent(GUIElementEvent(this));
}
IGUIPanelElement* IGUIElement::GetParent()const
{
	if(m_attachedRegion)
		return (IGUIPanelElement*)m_attachedRegion->GetOwner();
	return 0;
}


bool IGUIElement::SetAnchor(EElementAnchor anchor, bool set)
{
	if (m_locked)
		return false;
	m_anchor[anchor] = set;
	m_unclippedRectDirt = true;
	return true;
}
bool IGUIElement::GetAnchor(EElementAnchor anchor)const
{
	return m_anchor[anchor];
}

bool IGUIElement::SetVerticalAlignment(EVerticalAlignment e)
{
	if(m_locked)
		return false;
	m_verticalAlign=e;
	m_unclippedRectDirt=true;
	return true;
}
const EVerticalAlignment& IGUIElement::GetVerticalAlignment()const
{
	return m_verticalAlign;
}

bool IGUIElement::SetHorizontalAlignment(EHorizontalAlignment e)
{
	if(m_locked)
		return false;
	m_horizontalAlign=e;
	m_unclippedRectDirt=true;
	return true;
}
const EHorizontalAlignment& IGUIElement::GetHorizontalAlignment()const
{
	return m_horizontalAlign;
}

bool IGUIElement::SetDocking(EElementDock d)
{
	if(m_locked)
		return false;
	m_docking=d;
	m_unclippedRectDirt=true;
	return true;
}
const EElementDock& IGUIElement::GetDocking()const
{
	return m_docking;
}
bool IGUIElement::OnEvent(Event* e)
{
	if(!IsEnabled() || !IsVisible())return false;
	if(e->getType()==ET_Mouse)
	{
		return _OnMouseEvent(dynamic_cast<MouseEvent*>(e));
	}else
	if(e->getType()==ET_Keyboard)
	{
		return _OnKeyboardEvent(dynamic_cast<KeyboardEvent*>(e));
	}
	return false;
}

void IGUIElement::Update(float dt)
{
}
void IGUIElement::Draw(const math::rectf*vp)
{

}

void IGUIElement::LoadXMLSettings(xml::XMLElement*e)
{
	IObject::loadFromXML(e);
	FontAttributes* fontAttrs= GetFontAttributes();
	if(fontAttrs)
	{
		xml::XMLElement* elem=e->getSubElement("FontAttributes");
		if(elem)
			fontAttrs->loadXMLSettings(elem);
	}
}
void IGUIElement::OnVisit(IGUIElementVisitor*v)
{
	v->OnVisitElement(this);
}

IGUIElement* IGUIElement::Duplicate()
{
	IGUIElement* o=new IGUIElement(*m_type,m_creator);
	o->CopyProperties(this);
	return o;
}
 void IGUIElement::CopyProperties(const IGUIElement*o)
{
	//SetParent(o->GetParent());
	SetPosition(o->GetPosition());
	SetSize(o->GetSize());
	SetDocking(o->GetDocking());
	SetHorizontalAlignment(o->GetHorizontalAlignment());
	SetVerticalAlignment(o->GetVerticalAlignment());
	SetEnabled(o->IsEnabled());
	SetVisible(o->IsVisible());
	SetText(o->GetText());
}
void IGUIElement::_RectsDirt()
{
	m_clippedRectDirt=true;
	m_unclippedRectDirt=true;
	m_derivedPosDirt=true;
}

//internal methods
bool IGUIElement::_OnMouseEvent(MouseEvent*evt)
{
	return false;
}
bool IGUIElement::_OnKeyboardEvent(KeyboardEvent*evt)
{
	return false;
}

void IGUIElement::OnMoved(const GUIElementEvent& e)
{
	m_clippedRectDirt=true;
	if(e.element==this)
		m_unclippedRectDirt=true;
	m_derivedPosDirt=true;
}
void IGUIElement::OnResized(const GUIElementEvent& e)
{
	m_clippedRectDirt=true;
	if(e.element==this)
		m_unclippedRectDirt=true;
}
void IGUIElement::OnChangeVisible(const GUIElementEvent& e)
{
}
void IGUIElement::OnChangeEnable(const GUIElementEvent& e)
{
}
void IGUIElement::OnChangeParent(const GUIElementEvent& e)
{
	m_clippedRectDirt=true;
	m_derivedPosDirt=true;
}
void IGUIElement::OnSetCreator(const GUIElementEvent& e)
{
}

void IGUIElement::_notifyMouseEnter()
{
	OnMouseEnter(this,0);
	_OnEventFired(&OnMouseEnter,this,0);
}

void IGUIElement::_notifyMouseLeave()
{
	OnMouseLeave(this,0);
	_OnEventFired(&OnMouseLeave,this,0);
}
void IGUIElement::_notifyReceiveFocus()
{
	OnReceiveFocus(this,0);
	_OnEventFired(&OnReceiveFocus,this,0);
}

void IGUIElement::_notifyLostFocus()
{
	OnLostFocus(this,0);
	_OnEventFired(&OnLostFocus,this,0);
}

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_PROPERTY_TYPE_HEADER(Locked,IGUIElement,bool,mT("Locked"),EPBT_Basic,mT("Locking control yields not to modify it's design properties"),false);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Locked,IGUIElement,bool,SetLocked,IsLocked,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Name,IGUIElement,string,mT("Name"),EPBT_Basic,mT("Control's name"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,IGUIElement,core::string,SetName,GetName,,,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ID,IGUIElement,uint,mT("ID"),EPBT_Basic,mT("Control's ID"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ID,IGUIElement,uint,SetID,GetID,core::StringConverter::toString,core::StringConverter::toUInt,false)

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Visible,IGUIElement,bool,mT("Visible"),EPBT_Basic,mT("Show/hide element"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Visible,IGUIElement,bool,SetVisible,IsVisible,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Enabled,IGUIElement,bool,mT("Enable"),EPBT_Basic,mT("Enable/Disable element"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Enabled,IGUIElement,bool,SetEnabled,IsEnabled,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Position,IGUIElement,vector2d,mT("Position"),EPBT_Struct,mT("Control's position"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Position, IGUIElement, math::vector2d, SetPosition, GetPosition, core::StringConverter::toString, core::StringConverter::toVector2d, false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AnchorPosition, IGUIElement, vector2d, mT("AnchorPosition"), EPBT_Struct, mT("Control's position"), 0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AnchorPosition, IGUIElement, math::vector2d, SetAnchorPosition, GetAnchorPosition, core::StringConverter::toString, core::StringConverter::toVector2d, false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Size,IGUIElement,vector2d,mT("Size"),EPBT_Struct,mT("Control's Size"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Size,IGUIElement,math::vector2d,SetSize,GetSize,core::StringConverter::toString,core::StringConverter::toVector2d,false)

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_PROPERTY_TYPE_HEADER(HorizontalAlignment,IGUIElement,enum,mT("HorizontalAlignment"),EPBT_Enum,mT("Control's Horizontal Alignment"),EHA_Left);

IGUIElement::PropertyTypeHorizontalAlignment IGUIElement::PropertyTypeHorizontalAlignment::instance;
EHorizontalAlignment IGUIElement::PropertyTypeHorizontalAlignment::GetValue(CPropertieSet*object)const
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)
		return m_default;
	return o->GetHorizontalAlignment();
}
bool IGUIElement::PropertyTypeHorizontalAlignment::SetValue(CPropertieSet*object,const EHorizontalAlignment& v)
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)return false;
	return o->SetHorizontalAlignment(v);
}
core::string IGUIElement::PropertyTypeHorizontalAlignment::toString(CPropertieSet*object)const
{
	if(GetEnum())
		return GetEnum()->ToString(GetValue(object));
	return core::StringConverter::toString((int)GetValue(object),core::string(mT("EHorizontalAlignment")));
}
bool IGUIElement::PropertyTypeHorizontalAlignment::parse(CPropertieSet*reciver,const core::string&str)
{
	if(GetEnum())
	{
		if(!CheckIsValidValue(str))
			return false;
	}
	int v=0;
	if(GetEnum())
		v= GetEnum()->Parse(str);
	else
		core::StringConverter::parse(str,v,mT("EHorizontalAlignment"));
	return SetValue(reciver,(EHorizontalAlignment)v);
}

bool IGUIElement::PropertyTypeHorizontalAlignment::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_PROPERTY_TYPE_HEADER(VerticalAlignment,IGUIElement,enum,mT("VerticalAlignment"),EPBT_Enum,mT("Control's Vertical Alignment"),EVA_Top);

IGUIElement::PropertyTypeVerticalAlignment IGUIElement::PropertyTypeVerticalAlignment::instance;
EVerticalAlignment IGUIElement::PropertyTypeVerticalAlignment::GetValue(CPropertieSet*object)const
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)
		return m_default;
	return o->GetVerticalAlignment();
}
bool IGUIElement::PropertyTypeVerticalAlignment::SetValue(CPropertieSet*object,const EVerticalAlignment& v)
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)return false;
	return o->SetVerticalAlignment(v);
}
core::string IGUIElement::PropertyTypeVerticalAlignment::toString(CPropertieSet*object)const
{
	if(GetEnum())
		return GetEnum()->ToString(GetValue(object));
	return core::StringConverter::toString((int)GetValue(object),core::string(mT("EVerticalAlignment")));
}
bool IGUIElement::PropertyTypeVerticalAlignment::parse(CPropertieSet*reciver,const core::string&str)
{
	if(GetEnum())
	{
		if(!CheckIsValidValue(str))
			return false;
	}
	int v=0;
	if(GetEnum())
		v= GetEnum()->Parse(str);
	else
		core::StringConverter::parse(str,v,mT("EVerticalAlignment"));
	return SetValue(reciver,(EVerticalAlignment)v);
}
bool IGUIElement::PropertyTypeVerticalAlignment::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_PROPERTY_TYPE_HEADER(Docking,IGUIElement,enum,mT("Docking"),EPBT_Enum,mT("Control's Docking"),EED_None);

IGUIElement::PropertyTypeDocking IGUIElement::PropertyTypeDocking::instance;
EElementDock IGUIElement::PropertyTypeDocking::GetValue(CPropertieSet*object)const
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)
		return m_default;
	return o->GetDocking();
}
bool IGUIElement::PropertyTypeDocking::SetValue(CPropertieSet*object,const EElementDock& v)
{
	IGUIElement* o=dynamic_cast<IGUIElement*>(object);
	if(!o)return false;
	return o->SetDocking(v);
}
core::string IGUIElement::PropertyTypeDocking::toString(CPropertieSet*object)const
{
	if(GetEnum())
		return GetEnum()->ToString(GetValue(object));
	return core::StringConverter::toString((int)GetValue(object),core::string(mT("EElementDock")));
}
bool IGUIElement::PropertyTypeDocking::parse(CPropertieSet*reciver,const core::string&str)
{
	if(GetEnum())
	{
		if(!CheckIsValidValue(str))
			return false;
	}
	int v=0;
	if(GetEnum())
		v= GetEnum()->Parse(str);
	else
		core::StringConverter::parse(str,v,mT("EElementDock"));
	return SetValue(reciver,(EElementDock)v);
}
bool IGUIElement::PropertyTypeDocking::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Text,IGUIElement,UTFString,mT("Text"),EPBT_Basic,mT("Control's Text"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Text,IGUIElement,core::UTFString,SetText,GetText,core::StringConverter::toString,,true)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Color,IGUIElement,video::SColor,mT("Color"),EPBT_Struct,mT("Control's Color"),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Color,IGUIElement,video::SColor,SetColor,GetColor,core::StringConverter::toString,core::StringConverter::toColor,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Alpha,IGUIElement,float,mT("Alpha"),EPBT_Basic,mT("Control's Alpha"),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Alpha,IGUIElement,float,SetAlpha,GetAlpha,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(DeriveAlpha,IGUIElement,bool,mT("DeriveAlpha"),EPBT_Basic,mT("Derive alpha value?"),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(DeriveAlpha, IGUIElement, bool, SetDeriveAlpha, GetDeriveAlpha, core::StringConverter::toString, core::StringConverter::toBool, false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AnchorLeft, IGUIElement, bool, mT("AnchorLeft"), EPBT_Basic, mT("Left anchor"), 1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AnchorLeft, IGUIElement, bool, SetAnchorLeft, GetAnchorLeft, core::StringConverter::toString, core::StringConverter::toBool, true)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AnchorRight, IGUIElement, bool, mT("AnchorRight"), EPBT_Basic, mT("Right anchor"), 1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AnchorRight, IGUIElement, bool, SetAnchorRight, GetAnchorRight, core::StringConverter::toString, core::StringConverter::toBool, true)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AnchorTop, IGUIElement, bool, mT("AnchorTop"), EPBT_Basic, mT("Top anchor"), 1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AnchorTop, IGUIElement, bool, SetAnchorTop, GetAnchorTop, core::StringConverter::toString, core::StringConverter::toBool, false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AnchorBottom, IGUIElement, bool, mT("AnchorBottom"), EPBT_Basic, mT("Bottom anchor"), 1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AnchorBottom, IGUIElement, bool, SetAnchorBottom, GetAnchorBottom, core::StringConverter::toString, core::StringConverter::toBool, false)

}
}

