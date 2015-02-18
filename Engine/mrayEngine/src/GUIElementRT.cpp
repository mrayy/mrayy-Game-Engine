

#include "stdafx.h"
#include "GUIElementRT.h"
#include "IGUIElement.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "GUIElementRegion.h"
#include "GUIElementRegion.h"
#include "IRenderArea.h"

namespace mray
{
namespace scene
{




GUIElementRT::GUIElementRT()
{
	m_element=0;
}

GUIElementRT::~GUIElementRT()
{
}

math::vector2d GUIElementRT::_GetElementSize()
{
	if(!m_element)
		return 0;
	return m_element->GetDefaultRegion()->GetRect().getSize();
}

bool GUIElementRT::_OnMouseEvent(MouseEvent* e)
{
	if(!m_element)
		return false;
	const math::rectf& rc=m_element->GetDefaultRegion()->GetRect();
	e->pos=rc.ULPoint + e->pos*rc.getSize();
	return m_element->OnEvent(e);
}
bool GUIElementRT::_OnKeyboardEvent(KeyboardEvent* e)
{
	if(!m_element)
		return false;
	return m_element->OnEvent(e);
}
void GUIElementRT::SetElement(GUI::IGUIElement* elem)
{
	m_element=elem;
}

GUI::IGUIElement* GUIElementRT::GetElement()
{
	return m_element;
}


void GUIElementRT::_DrawElement(const math::rectf* vp)
{
	if(!m_element)
		return;
	m_element->Draw(vp);
}

}	
}
