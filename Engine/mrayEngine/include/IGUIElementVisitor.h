


/********************************************************************
	created:	2011/01/07
	created:	7:1:2011   17:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIElementVisitor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIElementVisitor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIElementVisitor_h__
#define IGUIElementVisitor_h__


namespace mray
{
namespace GUI
{
	class IGUIElement;
	class IGUIPanelElement;
	class IGUIComponent;

class IGUIElementVisitor
{
private:
protected:
public:
	IGUIElementVisitor(){}
	virtual ~IGUIElementVisitor(){}
	
	virtual void OnVisitComponent(IGUIComponent* comp)=0;
	virtual void OnVisitElement(IGUIElement* elem)=0;
	virtual void OnVisitPanel(IGUIPanelElement* elem)=0;

};

}
}


#endif // IGUIElementVisitor_h__
