

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   23:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIElementContainer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIElementContainer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIElementContainer___
#define ___GUIElementContainer___


#include "Point2d.h"
#include "GCPtr.h"
#include "Event.h"
#include "GUIElement.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIElementContainer
{
protected:
	std::list<GUIElementPtr > m_Children;
	GUIElement* m_activeElement;
public:

	GUIElementContainer();
	virtual~GUIElementContainer();


	void removeChild(GUIElement* elem);
	virtual void addChild(GUIElement* elem);

	GUIElement* getElementFromPoint(math::Point2df pos);

	GUIElement* getElementById(int id,bool searchChildren=0);

	bool isElementChild(GUIElement*  elem);

	GUIElement* getNextChild(GUIElement* elem);

	void removeAll();

	bool bringToFront(GUIElement* elem);
	bool dropToBack(GUIElement* elem);

	void setActiveElement(GUIElement* elem);

	std::list<GUIElementPtr > *getChildren();

	void onEvent(const EventPtr& event);
};

}
}


#endif //___GUIElementContainer___
