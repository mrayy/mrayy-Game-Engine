
#include "stdafx.h"
#include "GUICheckBoxComponent.h"
#include "IGUIManager.h"
#include "IGUIElement.h"
#include "IGUITheme.h"


namespace mray
{
namespace GUI
{

void GUICheckBoxComponent::Draw()
{

	static const math::vector2d s_checkBoxSize(24,24);

	IGUIManager* creator=owner->GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();

	if(!skin)
		return;
	int state=0;
	if(!m_checked)
	{
		if(m_state==EC_Normal)
			state=0;
		else state=1;
	}else
	{
		if(m_state==EC_Normal)
			state=4;
		else state=5;
	}
	math::rectf rc=this->rc;


	float y=rc.ULPoint.y+(rc.getHeight()-s_checkBoxSize.y)*0.5;

	skin->drawBox(creator->GetRenderQueue(),math::rectf(rc.ULPoint.x,y,rc.ULPoint.x+s_checkBoxSize.x,y+s_checkBoxSize.y),mT("CheckBox"),state,video::DefaultColors::White,&rc);
	rc.ULPoint.x+=s_checkBoxSize.x;

	if(!rc.isValid())
		return;

	creator->GetRenderQueue()->AddQuad(0,rc,math::rectf(0,0,1,1),video::DefaultColors::White);
	FontAttributes*attrs= owner->GetFontAttributes();
	if(attrs)
	{
		IFont* f= gFontResourceManager.getFontByName(attrs->fontName);
		if(f)
		{
			f->print(rc,attrs,0,text,creator->GetRenderQueue());
		}
	}
}

bool GUICheckBoxComponent::OnKeyboardEvent(KeyboardEvent* e)
{
	IGUIManager* creator=owner->GetCreator();
	if(e->key==KEY_SPACE && e->press)
	{
		ChangeState();
		e->setRecived();
		return true;
	}
	return false;
}
bool GUICheckBoxComponent::OnMouseEvent(MouseEvent* e)
{
	IGUIManager* creator=owner->GetCreator();
	if(!rc.IsPointInside(e->pos))
	{
		if(m_state!=EC_Normal)
		{
			creator->SetMouseOn(0);
			m_state=EC_Normal;
		}
		return false;
	}

	// set the creator's mouse on element to be this
	creator->SetMouseOn(owner);
	m_state=EC_MouseOver;

	GUIEvent evt;
	switch (e->event)
	{
	case MET_LEFTDOWN:
		m_state=EC_MouseDown;
		creator->SetFocus(owner);
		//OnMouseDown(this,&e->pos);
		ChangeState();
		break;
	case MET_LEFTUP:
		/*OnMouseUp(this,&e->pos);
		if(m_eventReciver)
		{
			evt.Caller=this;
			evt.event=GET_Mouse_UP;
			m_eventReciver->onEvent(&evt);
		}*/
		break;
	case MET_MOVED:
		/*OnMouseMove(this,&e->pos);
		if(m_eventReciver)
		{
			evt.Caller=this;
			evt.event=GET_Mouse_MOVE;
			m_eventReciver->onEvent(&evt);
		}*/
		break;
	}
	e->setRecived();

	return true;
}

void GUICheckBoxComponent::LostFocus()
{
}
void GUICheckBoxComponent::LostMouseOn()
{
}

}
}
