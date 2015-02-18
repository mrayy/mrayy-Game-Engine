


/********************************************************************
	created:	2011/12/29
	created:	29:12:2011   12:39
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\DraggableItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	DraggableItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#include "CompileConfig.h"

#ifndef __DraggableItem__
#define __DraggableItem__


namespace mray
{
namespace GUI
{

class MRAYGUI_API DraggableItem
{
private:
protected:
	math::vector2d m_startPos;
	math::vector2d m_startMousePos;
	math::vector2d m_lastMousePos;
	bool m_dragging;
	math::Point2d<bool> m_disableAxis;
public:
	DraggableItem();
	virtual~DraggableItem();

	void Enable(const math::vector2d& currPos,const math::vector2d& mousePos);
	void Disable();
	bool IsEnabled();
	const math::vector2d& UndoDrag();
	math::vector2d OnMouseMove(const math::vector2d&pos);

};

}
}

#endif

