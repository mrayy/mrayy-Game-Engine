
#include "stdafx.h"
#include "GUIElementRT_Manager.h"
#include "IGUIManager.h"
#include "IGUIElement.h"
#include "GUIElementRegion.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"

namespace mray
{	
namespace scene
{

GUIElementRT_Manager::GUIElementRT_Manager()
{
	m_mngr=0;
}

GUIElementRT_Manager::~GUIElementRT_Manager()
{
}

math::vector2d GUIElementRT_Manager::_GetElementSize()
{
	if(!m_mngr || !m_mngr->GetRootElement())
		return 0;
	return m_mngr->GetRootElement()->GetDefaultRegion()->GetRect().getSize();
}

bool GUIElementRT_Manager::_OnMouseEvent(MouseEvent* e)
{
	if(!m_mngr)
		return false;
	const math::rectf& rc=m_mngr->GetRootElement()->GetDefaultRegion()->GetRect();
	e->pos=rc.ULPoint + e->pos*rc.getSize();
	return m_mngr->OnEvent(e);
}
bool GUIElementRT_Manager::_OnKeyboardEvent(KeyboardEvent* e)
{
	if(!m_mngr)
		return false;
	return m_mngr->OnEvent(e);
}


void GUIElementRT_Manager::_DrawElement(const math::rectf* vp)
{
	if(!m_mngr)
		return;
	m_mngr->DrawAll(vp);
}
}
}
