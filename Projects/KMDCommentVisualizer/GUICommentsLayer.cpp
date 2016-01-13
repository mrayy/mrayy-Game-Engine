

#include "stdafx.h"
#include "GUICommentsLayer.h"

#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "GUICommentDetailsPanel.h"
#include "AppData.h"

namespace mray
{
namespace GUI
{

	const GUID GUICommentsLayer::ElementType = "GUICommentsLayer";

	IMPLEMENT_RTTI(GUICommentsLayer, IGUIPanelElement);

	GUICommentsLayer::GUICommentsLayer(GUI::IGUIManager* m)
		:IGUIPanelElement(ElementType, m), GUIDockedElement(this)
	{

		GUI::GUIOverlay* o = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUICommentsLayerLayout.GUI");
		if (o)
		{
			o->CreateElements(m, this, this, this);
		}
		m_firstTime = true;
	}
	GUICommentsLayer::~GUICommentsLayer()
	{

	}
	void GUICommentsLayer::LoadXMLSettings(xml::XMLElement*e)
	{
		IGUIPanelElement::LoadXMLSettings(e);

		math::vector2d sz;
		sz = this->GetSize();
		sz.y = core::StringConverter::toInt(gAppData.GetValue("UI", "CommentHeight", core::StringConverter::toString(sz.y)));
		this->SetSize(sz);
	}

	void GUICommentsLayer::_AddComment(kmd::KMDComment* c)
	{
		GUICommentDetailsPanel* e = 0;
		if (m_graveYard.size()>0)
		{
			e=*m_graveYard.begin();
			m_graveYard.erase(m_graveYard.begin());
			e->SetVisible(true);
		}
		else
		{
			e = new GUICommentDetailsPanel(GetCreator());
			e->SetAnchorPosition(math::vector2d(0, 0));
			e->SetPosition(math::vector2d(0, 0));
			e->SetSize(math::vector2d(100, GetSize().y/3.0f));
			e->SetAnchorLeft(true);
			e->SetAnchorRight(true);
			e->SetAnchorTop(true);

			Background->AddElement(e);
		}
		e->SetComment(c);
		CommentInfo ifo;
		ifo.ui = e;
		ifo.alpha = 0;
		ifo.status = CommentInfo::EStart;
		m_comments.push_front(ifo);
	}

	void GUICommentsLayer::CommentsLoaded(const std::vector<kmd::KMDComment*>& comments)
	{
		if (m_toAdd.size() == 0)
			m_timeToAdd = 0;
		for (int i = 0; i < comments.size(); ++i)
		{
			if (m_firstTime)
				_AddComment(comments[i]);
			else
				m_toAdd.push(comments[i]);
		}
		m_firstTime = false;
	}
	void GUICommentsLayer::Draw(const math::rectf*vp)
	{
		math::vector2d sz = GetSize();
		sz.x = vp->getSize().x;
		SetSize(sz);
		GUIDockedElement::UpdateDocking();
		IGUIPanelElement::Draw(vp);
	}


	void GUICommentsLayer::Update(float dt)
	{
		if (m_toAdd.size()>0)
		{
			m_timeToAdd -= dt;
			if (m_timeToAdd < 0)
			{
				m_timeToAdd = 5;
				_AddComment(m_toAdd.front());
				m_toAdd.pop();
			}
		}


		std::list<CommentInfo>::iterator it = m_comments.begin();
		std::list<CommentInfo>::iterator it2 = it;

		float y = 0;
		for (; it != m_comments.end(); it = it2)
		{
			it2 = it;
			it2++;
			CommentInfo& c = *it;

			math::vector2d pos = c.ui->GetPosition();
			if (pos.y < y)
			{
				pos.y += 250 * dt;
			}
			else
			{
				pos.y = y;
			}
			c.ui->SetPosition(pos);

			y = pos.y + c.ui->GetSize().y;


			if (c.status == CommentInfo::EStart)
			{
				if (c.alpha < 0.5)
					c.alpha += dt;
				if (c.alpha >= 0.5)
				{
					c.alpha = 0.5;
					c.status = CommentInfo::EIdle;
				}
			}
			if (c.status == CommentInfo::EDone)
			{
				if (c.alpha >0)
					c.alpha -= dt;
				if (c.alpha <=0)
				{
					m_graveYard.push_back(c.ui);
					c.alpha = 0;
					c.ui->SetVisible(false);
					m_comments.erase(it);
				}
			}
			
			c.ui->SetAlpha(c.alpha);
		}

		IGUIPanelElement::Update(dt);
	}

IMPLEMENT_ELEMENT_FACTORY(GUICommentsLayer);

}
}

