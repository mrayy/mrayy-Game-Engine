

#include "stdafx.h"
#include "GUISceneSpacePanel.h"

#include "GUIElementRegion.h"

#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "SessionRenderer.h"

#include "CSubProject.h"
#include "SessionDetails.h"

namespace mray
{
namespace GUI
{
	const GUID GUISceneSpacePanel::ElementType = "GUISceneSpacePanel";


GUISceneSpacePanel::GUISceneSpacePanel(IGUIManager* m)
:IGUIPanelElement(ElementType, m), m_topElement(0), m_bottomElement(0), m_leftElement(0), m_rightElement(0), m_sessionRenderer(0)
{
	m_currentSub = 0;

	GUI::GUIOverlay* o = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIScenePanelLayout.GUI");
	if (o)
	{
		o->CreateElements(m, this, this, this);
	}

	gAppData.subProjectChange.AddListener(this);
}

GUISceneSpacePanel::~GUISceneSpacePanel(){
}


void GUISceneSpacePanel::_OnSubProjectChange(kmd::CSubProject* sp)
{
	if (!m_currentSub || sp->GetSession() != m_currentSub->GetSession())
	{
		m_targetColor = sp->GetSession()->GetColor();
		Transition->Start(m_targetColor);
		Theme->SetText(sp->GetSession()->GetSessionName());
		Info->SetText(sp->GetSession()->GetDescription());
	}
	Speaker->SetText(sp->GetName());
	m_currentSub = sp;
}

void GUISceneSpacePanel::Update(float dt){
	if (!IsVisible())
		return;
	IGUIPanelElement::Update(dt);

	video::SColor c = Background-> GetColor();
	c += (m_targetColor - c)*dt;
	c.A = 1;
	Background->SetColor(c);

}

void GUISceneSpacePanel::Draw(const math::rectf*vp)
{
	if (!IsVisible())
		return;
	math::vector2d pos = GetPosition();
	math::vector2d sz = GetSize();
	if (m_leftElement)
	{
		pos.x = m_leftElement->GetDefaultRegion()->GetClippedRect().BRPoint.x;
	}
	if (m_topElement)
	{
		pos.y = m_topElement->GetDefaultRegion()->GetClippedRect().BRPoint.y;
	}
	if (m_rightElement)
	{
		sz.x = m_rightElement->GetDefaultRegion()->GetClippedRect().ULPoint.x - pos.x;
	}
	else
	{
		sz.x = vp->getSize().x - pos.x;
	}
	if (m_bottomElement)
	{
		sz.y = m_bottomElement->GetDefaultRegion()->GetClippedRect().ULPoint.y-pos.y;
	}
	else
	{
		sz.y = vp->getSize().y - pos.y;
	}
	SetPosition(pos);
	SetSize(sz);
	IGUIPanelElement::Draw(vp);

	math::rectf oldS= gEngine.getDevice()->getScissorRect();
	gEngine.getDevice()->setScissorRect(GetDefaultRegion()->GetClippedRect());
	if (m_sessionRenderer)
	{
		m_sessionRenderer->SetRenderingVP(GetDefaultRegion()->GetClippedRect());
		m_sessionRenderer->Draw(GetAlpha());
	}
	gEngine.getDevice()->setScissorRect(oldS);
}


IMPLEMENT_RTTI(GUISceneSpacePanel, IGUIPanelElement);
IMPLEMENT_ELEMENT_FACTORY(GUISceneSpacePanel);

}
}
