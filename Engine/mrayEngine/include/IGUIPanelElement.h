


/********************************************************************
	created:	2011/01/03
	created:	3:1:2011   23:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIPanelElement.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIPanelElement
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIPanelElement_h__
#define IGUIPanelElement_h__

#include "IGUIElement.h"


namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIPanelElement:public IGUIElement
{
public:
	typedef std::list<IGUIElementPtr> ElementList;

private:
protected:
	ElementList m_elements;
public:
	IGUIPanelElement(const GUID& type,IGUIManager* creator);
	virtual ~IGUIPanelElement();

	virtual void _UpdateDerivedAlpha();

	virtual bool IsPanelElement();

	virtual void AddElement(IGUIElementCRef elem,GUIElementRegion* r);
	virtual void AddElement(IGUIElementCRef elem);
	virtual void RemoveElement(IGUIElement* elem);
	virtual IGUIElementCRef GetElementByID(uint id)const;
	virtual void ClearElements();
	virtual bool IsChild(IGUIElement*elem,bool recursive);

	bool OnEvent(Event* e);
	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);

	virtual const ElementList& GetElements(){return m_elements;}

	virtual bool MoveElementToTop(IGUIElement* elem);
	virtual bool MoveElementToBottom(IGUIElement* elem);

	virtual void OnVisit(IGUIElementVisitor*v);

	virtual void _RectsDirt();

	virtual void LoadXMLSettings(xml::XMLElement*e);

protected:
	virtual bool _OnMouseEvent(MouseEvent*evt);
	virtual bool _OnKeyboardEvent(KeyboardEvent*evt);

	virtual void OnMoved(const GUIElementEvent& e);
	virtual void OnResized(const GUIElementEvent& e);
	virtual void OnChangeVisible(const GUIElementEvent& e);
	virtual void OnChangeEnable(const GUIElementEvent& e);
	virtual void OnChangeParent(const GUIElementEvent& e);
	virtual void OnSetCreator(const GUIElementEvent& e);
};

}
}

#endif // IGUIPanelElement_h__
