


/********************************************************************
	created:	2011/01/07
	created:	7:1:2011   17:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIElementSelectorVisitor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIElementSelectorVisitor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIElementSelectorVisitor_h__
#define GUIElementSelectorVisitor_h__

#include "CompileConfig.h"
#include "IGUIElementVisitor.h"




namespace mray
{
namespace GUI
{

class MRAY_DLL GUIElementSelectorVisitor:public IGUIElementVisitor
{
private:
protected:
	IGUIElement* m_result;
	math::vector2d m_pos;
public:
	GUIElementSelectorVisitor();
	virtual ~GUIElementSelectorVisitor();

	IGUIElement* Visit(IGUIElement*e,const math::vector2d& pos);

	virtual void OnVisitComponent(IGUIComponent*comp);
	virtual void OnVisitElement(IGUIElement* elem);
	virtual void OnVisitPanel(IGUIPanelElement* elem);
};

}
}


#endif // GUIElementSelectorVisitor_h__
