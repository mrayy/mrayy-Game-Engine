

/********************************************************************
	created:	2011/01/11
	created:	11:1:2011   22:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIComponent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIComponent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIComponent_h__
#define IGUIComponent_h__


namespace mray
{
	class KeyboardEvent;
	class MouseEvent;
namespace GUI
{

	class IGUIElement;
	class IGUIComponentContext;

class IGUIComponent
{
private:
protected:
public:

	IGUIElement* owner;
	math::rectf rc;
	core::UTFString text;
	bool enabled;
	bool active;

	IGUIComponent():owner(0),enabled(true),active(false)
	{}
	virtual ~IGUIComponent(){}

	virtual void Draw()=0;

	virtual bool OnKeyboardEvent(KeyboardEvent* e)=0;
	virtual bool OnMouseEvent(MouseEvent* e)=0;

	virtual void LostFocus(){}
};

}
}

#endif // IGUIComponent_h__
