

#include "stdafx.h"
#include "GUICommentDetailsPanel.h"

#include "GUIElementRegion.h"
#include "FontResourceManager.h"
#include "IGUIManager.h"
#include "IGUIRenderer.h"
#include "DateTime.h"
#include "GUIOverlay.h"
#include "GUIOverlayManager.h"
#include "GUISessionSidePanel.h"
#include "KMDComment.h"
#include "SessionDetails.h"
#include "KMDUser.h"
#include "StringUtil.h"

namespace mray
{
namespace GUI
{



	const GUID GUICommentDetailsPanel::ElementType = "GUICommentDetailsPanel";

float GUICommentDetailsPanel_ShrinkSpeed = 150;

GUICommentDetailsPanel::GUICommentDetailsPanel(IGUIManager* m) :
	IGUIPanelElement(ElementType, m)
{
	m_active = false;
	m_sidePanel = 0;
	m_Comment = 0;
	GUI::GUIOverlay* o = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUICommentDetailsPanelLayout.GUI");
	if (o)
	{
		o->CreateElements(m, this, this, this);
		Details->GetFontAttributes()->fontSize = core::StringConverter::toInt(gAppData.GetValue("UI", "CommentSize", "18"));
		ProjectID->GetFontAttributes()->fontSize = core::StringConverter::toInt(gAppData.GetValue("UI", "ProjectIDSize", "18"));


		math::vector2d sz = TwitterImage->GetSize();
		sz = core::StringConverter::toInt(gAppData.GetValue("UI", "UserImageSize", core::StringConverter::toString(sz.x)));
		TwitterImage->SetSize(sz);

		math::vector2d pos = Details->GetPosition();
		pos.x = TwitterImage->GetPosition().x + sz.x + 10;
		Details->SetPosition(pos);

	}
}
GUICommentDetailsPanel::~GUICommentDetailsPanel()
{
}
void GUICommentDetailsPanel::LoadXMLSettings(xml::XMLElement*e)
{
	IGUIPanelElement::LoadXMLSettings(e);

}

void GUICommentDetailsPanel::SetComment(kmd::KMDComment* t)
{
	m_Comment = t;
	if (m_Comment)
	{
		if (t->user)			
		{
			TwitterImage->SetSourceImage(t->user->imageUrl);
			TwitterID->SetText(t->user->name);
		}
		ProjectID->SetText(t->project->GetSessionName());
		Details->SetText(t->text);
		CommentTime->SetText(core::DateTime::ToString(t->date));
		m_active = true;
	}
	else
		m_active = false;
}
bool GUICommentDetailsPanel::OnEvent(Event* e)
{
	if (!IsVisible())
		return false;
	if (e->getType() == ET_Mouse)
	{
		MouseEvent* evt = dynamic_cast<MouseEvent*>(e);
		const GUIElementRegion* r = GetDefaultRegion();
		if (r->GetClippedRect().IsPointInside(evt->pos))
		{
			m_active = true;
			return true;
		}
		else	{

			m_active = false;
		}
	}
	return false;
}

void GUICommentDetailsPanel::Update(float dt)
{
	if (!IsVisible())
		return;
	IGUIPanelElement::Update(dt);
	return;
#define DECREASE(x,l,v) if(x>(l)){ x-=(v);} if(x<(l)){x=(l);}
#define INCREASE(x,l,v) if(x<(l)){ x+=(v);} if(x>(l)){x=(l);}

	math::vector2d sz = this->GetSize();
	float a = GetAlpha();
//	if (!m_active)
	{
		DECREASE(sz.y, 120, 200*dt);
		DECREASE(a, 0.2, dt);
	}/*
	else
	{
		INCREASE(sz.y, 160, 200 * dt);
		INCREASE(a, 1, dt);
	}*/
	SetSize(sz);
	SetAlpha(a);
}
void GUICommentDetailsPanel::Draw(const math::rectf*vp)
{
	if (!IsVisible())
		return;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();


	const GUIElementRegion* r = GetDefaultRegion();

//	dev->draw2DRectangle(r->GetClippedRect(), GetColor());


//	core::DateTime dt = core::DateTime::Now();

	if (m_sidePanel)
	{
		const math::rectf& rc = m_sidePanel->GetDefaultRegion()->GetClippedRect();
		math::vector2d pos = GetPosition();
		pos.x = rc.BRPoint.x;
		this->SetPosition(pos);
	}
	IGUIPanelElement::Draw(vp);
}

IMPLEMENT_RTTI(GUICommentDetailsPanel, IGUIPanelElement);
IMPLEMENT_ELEMENT_FACTORY(GUICommentDetailsPanel);

}
}


