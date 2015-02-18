


#include "stdafx.h"
#include "GUITreeBox.h"
#include "IGUIManager.h"
#include "IGUITheme.h"


namespace mray
{
namespace GUI
{


IMPLEMENT_ELEMENT_FACTORY(GUITreeBox);


GUITreeBox::GUITreeBox(IGUIManager*mngr):IGUITreeBox(mngr)
{
	m_FontAttributes.hasShadow=false;
	m_FontAttributes.fontColor=video::DefaultColors::Black;
}

GUITreeBox::~GUITreeBox()
{
	ClearItems();
}

void GUITreeBox::fillProperties()
{

}

bool GUITreeBox::_OnKeyboardEvent(KeyboardEvent* evt)
{
	return false;
}

bool GUITreeBox::_OnMouseEvent(MouseEvent* evt)
{
	const math::rectf& rc=GetDefaultRegion()->GetClippedRect();
	if(rc.IsPointInside(evt->pos))
	{
		GUITreeBoxDrawInfo ifo;
		ifo.rc=GetDefaultRegion()->GetClippedRect();
		ifo.pos=ifo.rc.ULPoint;
		ifo.font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
		ifo.owner=this;
		ifo.fontAttrs=&m_FontAttributes;
		if(!ifo.font)
			ifo.font=gFontResourceManager.getDefaultFont();

		if(!GetCreator()->HasMouseOn(this))
		{
			GetCreator()->SetMouseOn(this);
		}
		if(evt->event==MET_LEFTDOWN)
		{
			//check which node was clicked
			for(int i=0;i<m_nodes.size();++i)
			{
				if(m_nodes[i]->OnMouseEvent(evt,ifo))
				{
					m_nodes[i]->SetExpanded(!m_nodes[i]->IsExpanded(),false);
					break;;
				}
			}
		}
		evt->setRecived();
		return true;
	}else
	{
		if(GetCreator()->HasMouseOn(this))
		{
			GetCreator()->SetMouseOn(0);
		}
	}
	return false;
}


GUITreeNode* GUITreeBox::AddItem(const core::string& text)
{
	GUITreeNode* node=new GUITreeNode();
	node->SetText(text);

	m_nodes.push_back(node);
	return node;
}

void GUITreeBox::RemoveItem(GUITreeNode* node)
{
	GUITreeNodeList::iterator it= m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		if(*it==node)
		{
			m_nodes.erase(it);
			break;
		}
	}
}

void GUITreeBox::ClearItems()
{
	for(int i=0;i<m_nodes.size();++i)
	{
		delete m_nodes[i];
	}
	m_nodes.clear();
}


void GUITreeBox::ExpandAll()
{
	for(int i=0;i<m_nodes.size();++i)
	{
		m_nodes[i]->SetExpanded(true,true);
	}
}

void GUITreeBox::CollapseAll()
{
	for(int i=0;i<m_nodes.size();++i)
	{
		m_nodes[i]->SetExpanded(true,false);
	}
}


GUITreeNodeList& GUITreeBox::GetNodes()
{
	return m_nodes;
}


void GUITreeBox::Draw(const math::rectf*vp)
{
	_UpdateRegion(vp);
	if(!IsVisible())return;
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();

	{
		math::rectf oldScissor=creator->GetDevice()->getScissorRect();
		creator->GetDevice()->setScissorRect(clipRect);
		if(skin)
		{
			skin->drawSizableBox(creator->GetRenderQueue(),clipRect,0,mT("Frame"));
			creator->GetRenderQueue()->Flush();
		}else
		{
			creator->GetDevice()->draw2DRectangle(clipRect,GetColor());
		}
		GUITreeBoxDrawInfo ifo;
		ifo.pos=clipRect.ULPoint;
		ifo.rc=clipRect;
		ifo.font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
		ifo.owner=this;
		ifo.fontAttrs=&m_FontAttributes;
		if(!ifo.font)
			ifo.font=gFontResourceManager.getDefaultFont();
		for(int i=0;i<m_nodes.size();++i)
		{
			m_nodes[i]->Draw(ifo);
			 if(ifo.pos.y>=clipRect.BRPoint.y)
				 break;
		}
		creator->GetDevice()->setScissorRect(oldScissor);

	}
}

void GUITreeBox::Update(float dt)
{
}



IGUIElement* GUITreeBox::Duplicate()
{
	return new GUITreeBox(GetCreator());
}

FontAttributes* GUITreeBox::GetFontAttributes()
{
	return &m_FontAttributes;
}


}
}

