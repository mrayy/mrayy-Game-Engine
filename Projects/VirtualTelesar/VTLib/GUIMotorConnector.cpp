

#include "stdafx.h"
#include "GUIMotorConnector.h"
#include "IGUIManager.h"

#include "IGUIComboList.h"
#include "IGUIStaticText.h"
#include "MotorConnection.h"
#include "GUIElementRegion.h"
#include "IGUIRenderer.h"
#include "FontResourceManager.h"


namespace mray
{
namespace GUI
{
const GUID GUIMotorConnector::ElementType("GUIMotorConnector");
GUIMotorConnector::GUIMotorConnector(GUI::IGUIManager* crtr)
	:IGUIPanelElement(ElementType,crtr)
{
	m_SourceText=(IGUIComboList*) crtr->CreateElement(GUI::IGUIComboList::ElementType);
	m_TargetText=(IGUIStaticText*) crtr->CreateElement(GUI::IGUIStaticText::ElementType);
	AddElement(m_SourceText);
	AddElement(m_TargetText);
	m_connector=0;
	m_mouseState=EMS_MouseOut;

	m_SourceText->SetShowItemsCount(10);

	m_TargetText->SetText(core::string("Target"));
	m_TargetText->SetBackground(false);
	m_TargetText->GetFontAttributes()->hasShadow=false;
	m_TargetText->GetFontAttributes()->fontColor.Set(0,0,0,1);

	m_SourceText->OnSelectChange.Connect("",this,&GUIMotorConnector::m_SourceText_SelectionChanged);
}
GUIMotorConnector::~GUIMotorConnector()
{
}

bool GUIMotorConnector::_OnMouseEvent(MouseEvent*evt)
{
	if(!m_connectedRect.IsPointInside(evt->pos))
	{
		m_mouseState=EMS_MouseOut;
		return false;
	}
	

	if(evt->event==MET_LEFTDOWN)
	{
		m_mouseState=EMS_Clicked;
		m_creator->SetFocus(this);
	}else if(evt->event==MET_LEFTUP)
	{
		if(m_connector)
			m_connector->SetConencted(!m_connector->IsConnected());
		m_mouseState=EMS_MouseOn;
	}else
		m_mouseState=EMS_MouseOn;
	evt->setRecived();
	return true;
}

bool GUIMotorConnector::_OnKeyboardEvent(KeyboardEvent*evt)
{
	if(!m_connector)
		return false;

	return false;
}

void GUIMotorConnector::m_SourceText_SelectionChanged(IObject*,PVOID)
{
	IListItem* item=m_SourceText->GetItem(m_SourceText->GetSelectedItem());
	if(m_connector && item)
		m_connector->SetSourceJoint(item->toString().GetAsString());
	OnSourceChanged(this);
}


void GUIMotorConnector::SetConnector(VT::MotorConnection* c)
{
	m_connector=c;
	if(m_connector)
	{
		m_TargetText->SetText(m_connector->GetTargetJoint());
		core::UTFString str=c->GetSourceJoint();
		bool found=false;
		for(int i=0;i<m_SourceText->GetItemsCount();++i)
		{
			if(m_SourceText->GetItem(i)->toString()==str)
			{
				m_SourceText->SetSelectedItem(i);
				found=true;
				break;
			}
		}
		if(!found)
		{
			m_SourceText->SetSelectedItem(m_SourceText->AddItem(new StringListItem(c->GetSourceJoint())));
		}
	}else
	{
		m_TargetText->SetText(core::string("None"));
		m_SourceText->SetSelectedItem(-1);
	}
}

void GUIMotorConnector::Draw(const math::rectf*vp)
{
	bool state=false;
	CalculateSizes();
	if(m_connector)
	{
		state=m_connector->IsConnected();
	}

	video::IVideoDevice* device=GetCreator()->GetDevice();

	math::rectf rc=m_connectedRect;
	rc.clipRect(GetDefaultRegion()->GetClippedRect());
	math::rectf oldScissor=device->getScissorRect();
	device->setScissorRect(rc);
	
	video::SColor clr;if(m_mouseState==EMS_MouseOn)
		clr=state? video::SColor(0,0.557,0.404,1):video::SColor(0.557,0.2,0.2,1);
	else if(m_mouseState==EMS_Clicked)
		clr=state? video::SColor(0,0.557,0.557,1):video::SColor(0.557,0.4,0.4,1);
	else 
		clr=state? video::SColor(0,0.8745,0.6353,1):video::SColor(0.8745,0.1,0.1,1);


	device->draw2DRectangle(m_connectedRect,clr,true);


	static FontAttributes attrs;
	attrs.hasShadow=false;
	attrs.fontSize=14;
	attrs.fontColor=video::DefaultColors::Black;
	//if(attrs)
	{
		IFont* f= gFontResourceManager.getFontByName(attrs.fontName);
		core::string text;
		if(state)
			text="Connected";
		else text="Disconnected";
		if(f)
		{
			f->print(rc,&attrs,0,text,GetCreator()->GetRenderQueue());
		}
	}

	if(m_mouseState!=EMS_MouseOut)
	{
		device->setLineWidth(3);
		device->draw2DRectangle(m_connectedRect,video::SColor(0.13,0.557,0.557,1),false,0);
	}
	GetCreator()->GetRenderQueue()->Flush();

	device->setScissorRect(oldScissor);

	IGUIPanelElement::Draw(vp);
}
void GUIMotorConnector::Update(float dt)
{
	IGUIPanelElement::Update(dt);
}

void GUIMotorConnector::CalculateSizes()
{
	math::rectf rc=GetDefaultRegion()->GetRect();

	float width=rc.getWidth()/5.0f;
	float height=rc.getHeight();
	m_SourceText->SetSize(math::vector2d(width*2,height));
	m_connectedRect.ULPoint=math::vector2d(width*2,0);
	m_connectedRect.BRPoint=m_connectedRect.ULPoint+math::vector2d(width,height);

	m_TargetText->SetPosition(math::vector2d(m_connectedRect.BRPoint.x,0));
	m_TargetText->SetSize(math::vector2d(width*2,height));
	
	m_SourceText->GetFontAttributes()->fontSize=14;
	m_SourceText->GetFontAttributes()->hasShadow=false;
	m_SourceText->GetFontAttributes()->fontColor=video::DefaultColors::Black;

	m_TargetText->GetFontAttributes()->fontSize=14;
	m_TargetText->GetFontAttributes()->hasShadow=false;
	m_TargetText->GetFontAttributes()->fontColor=video::DefaultColors::Black;

	m_connectedRect+=rc.ULPoint;

}

void GUIMotorConnector::SetSourceList(const std::vector<core::string>& lst)
{
	int i=m_SourceText->GetSelectedItem();
	GUI::IListItem* item= m_SourceText->GetItem(i);
	core::string selectedItem;
	if(item)
	{
		selectedItem=item->toString().GetAsString();
	}
	m_SourceText->ClearItems();
	for(int i=0;i<lst.size();++i)
	{
		m_SourceText->AddItem(new GUI::StringListItem(lst[i]));
		if(lst[i]==m_connector->GetSourceJoint())
		{
			m_SourceText->SetSelectedItem(i);
		}else if(lst[i]==m_connector->GetTargetJoint() && m_connector->GetSourceJoint()=="")
		{
			m_connector->SetSourceJoint(lst[i]);
			m_SourceText->SetSelectedItem(i);
		}
	}
}

}
}