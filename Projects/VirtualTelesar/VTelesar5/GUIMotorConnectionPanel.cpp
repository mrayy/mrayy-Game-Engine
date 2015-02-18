

#include "stdafx.h"
#include "GUIMotorConnectionPanel.h"
#include "MotorConnection.h"

#include "IGUIManager.h"
#include "IGUIStackPanel.h"
#include "GUIMotorConnector.h"
#include "IGUIPanel.h"
#include "GUISliderbarComponent.h"

namespace mray
{
namespace GUI
{

const GUID GUIMotorConnectionPanel::ElementType("MotorConnectionPanel");

GUIMotorConnectionPanel::GUIMotorConnectionPanel(IGUIManager* creator)
	:IGUIPanelElement(ElementType,creator)
{

	m_maxWidth=300;
	m_status=EHidden;

	m_panel=(IGUIPanel*)creator->CreateElement(IGUIPanel::ElementType);
	m_stackPanel=(IGUIStackPanel*)creator->CreateElement(IGUIStackPanel::ElementType);
	m_slider=new GUISliderbarComponent();;
	m_panel->AddElement(m_stackPanel);
	m_panel->SetDocking(EED_Fill);
	AddElement(m_panel);
	m_stackPanel->SetPosition(math::vector2d(10,10));
	m_stackPanel->SetOffset(10);
	m_stackPanel->SetStackDirection(IGUIStackPanel::EVertical);
	m_stackPanel->SetDocking(EED_Fill);

	m_slider->m_direction=GUISliderbarComponent::EVertical;
	m_slider->	SetMinValue(0);
	m_slider->	SetMaxValue(100);
	m_slider->owner=this;

	m_panel->OnMouseEnter.Connect("",this,&GUIMotorConnectionPanel::m_panel_OnMouseMove);
	m_panel->OnMouseLeave.Connect("",this,&GUIMotorConnectionPanel::m_panel_OnMouseLeave);
	creator->OnChangeMouseOn.Connect("",this,&GUIMotorConnectionPanel::creator_OnChangeMouseOn);
}

GUIMotorConnectionPanel::~GUIMotorConnectionPanel()
{
}


void GUIMotorConnectionPanel::AddConnection(VT::MotorConnection* connection)
{
	GUIMotorConnector*mc=new GUIMotorConnector(GetCreator());
	mc->SetConnector(connection);
	mc->SetSize(math::vector2d(m_maxWidth-30,30));
	mc->SetHorizontalAlignment(EHA_Center);
	mc->SetSourceList(m_items);
	m_connections.push_back(mc);
	m_stackPanel->AddElement(mc);
}

bool GUIMotorConnectionPanel::_OnMouseEvent(MouseEvent*evt)
{
	if(m_slider->OnMouseEvent(evt))
		return true;
	return IGUIPanelElement::_OnMouseEvent(evt);
}

bool GUIMotorConnectionPanel::_OnKeyboardEvent(KeyboardEvent*evt)
{
	if(m_slider->OnKeyboardEvent(evt))
		return true;
	return IGUIPanelElement::_OnKeyboardEvent(evt);

}

void GUIMotorConnectionPanel::m_panel_OnMouseMove(IObject*,PVOID)
{
	//m_status=EShown;
}

void GUIMotorConnectionPanel::m_panel_OnMouseLeave(IObject*,PVOID)
{
	//m_status=EHidden;
}
void GUIMotorConnectionPanel::creator_OnChangeMouseOn(IGUIElement*e)
{
	if(e==this || this->IsChild(e,true))
		m_status=EShown;
	else
		m_status=EHidden;
}

void GUIMotorConnectionPanel::Update(float dt)
{
	IGUIPanelElement::Update(dt);

	if(m_status==EHidden)
	{
		if(GetSize().x>50)
		{
			SetSize(math::vector2d(GetSize().x-m_maxWidth*dt,GetSize().y));
			if(GetSize().x<50)
				SetSize(math::vector2d(50,GetSize().y));

		}
	}else
	{
		if(GetSize().x<m_maxWidth)
		{
			SetSize(math::vector2d(GetSize().x+m_maxWidth*dt,GetSize().y));
			if(GetSize().x>m_maxWidth)
				SetSize(math::vector2d(m_maxWidth,GetSize().y));
		}
	}
}
void GUIMotorConnectionPanel::OnSchemeChanged(VT::ICommunicationLayer* ,const std::vector<core::string>& names )
{
	m_items=names;
	std::list<GUIMotorConnector*>::iterator it=m_connections.begin();
	std::list<GUIMotorConnector*>::iterator end=m_connections.end();
	for(;it!=end;++it)
		(*it)->SetSourceList(m_items);
}

void GUIMotorConnectionPanel::Draw(video::IRenderArea*vp)
{
	if(!IsVisible())
		return;
	(GetCreator()->GetMouseOn());
	math::rectf rc(0, vp->GetSize());
	IGUIPanelElement::Draw(&rc);
}

}
}


