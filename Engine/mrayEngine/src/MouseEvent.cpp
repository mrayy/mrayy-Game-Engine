
#include "stdafx.h"
#include "MouseEvent.h"
#include "RenderWindow.h"

namespace mray
{

scene::ViewPort* MouseEvent::GetViewPort()
{
	if(!m_ownerWindow)
		return 0;
	return m_ownerWindow->GetViewportFromPoint(pos.x,pos.y,false);
}

}