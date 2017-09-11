


/********************************************************************
	created:	2011/01/02
	created:	2:1:2011   22:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIElement.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIElement
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIElement_h__
#define IGUIElement_h__

#include "compileconfig.h"
#include "IObject.h"
#include "GUID.h"
#include "TypedProperty.h"

namespace mray
{
	class Event;
	class MouseEvent;
	class KeyboardEvent;
	class IEventReciver;
	namespace video
	{
		class IRenderArea;
	}
namespace GUI
{

	class IGUIPanelElement;
	class IGUIManager;
	class IGUIElement;
	class IGUIElementVisitor;
	class FontAttributes;
	class GUIElementRegion;


	enum  EHorizontalAlignment
	{
		EHA_Left,
		EHA_Center,
		EHA_Right
	};
	enum  EVerticalAlignment
	{
		EVA_Top,
		EVA_Center,
		EVA_Bottom
	};

	enum  EElementDock
	{
		EED_None,
		EED_Fill,
		EED_Left,
		EED_Right,
		EED_Top,
		EED_Bottom
	};

	enum  EElementAnchor
	{
		EEA_Top,
		EEA_Bottom,
		EEA_Left,
		EEA_Right,
		EEA_Count
	};

	class GUIElementEvent
	{
	public:
		GUIElementEvent(IGUIElement* e):element(e)
		{
		}
		IGUIElement* element;
	};

class MRAY_DLL IGUIElement:public IObject
{
	DECLARE_RTTI
public:

private:
protected:

	//  data members
	core::string			m_name;		// name of the element
	const GUID				*m_type;		// GUID represents it's type
	uint					m_id;		// ID of the element

	IGUIManager*			m_creator;	// GUI manager which created this element

	//IGUIPanelElement*		m_parent;	// parent panel for the element
	const GUIElementRegion*		m_attachedRegion;

	EHorizontalAlignment	m_horizontalAlign;	// horizontal alignment for the element
	EVerticalAlignment		m_verticalAlign;	// vertical alignment for the element

	EElementDock			m_docking;		// Docking settings for the element

	bool					m_anchor[EElementAnchor::EEA_Count];

	bool					m_visible;	// Visibility flag for the element
	bool					m_enabled;	// enable flag

	IEventReciver*			m_eventReciver; // attached event receiver with the element

	GUIElementRegion*		m_defaultRegion;
	//m_unclippedRect;	//Unclipped rect of the element
	//math::rectf				m_clippedRect;		//Clipped by parent rect of the element

	math::vector2d			m_position;		//position of the element from top left
	math::vector2d			m_anchorPosition;		//position of the anchor at the bottom right
	math::vector2d			m_size;			//size of the element

	math::vector2d			m_derivedPosition;	// derived position (which is in the screen space)
	math::vector2d			m_derivedSize;	// derived size (which is in the screen space)

	core::UTFString			m_text;			// text of this element

	video::SColor			m_color;		//Element's color
	float					m_derivedAlphaValue;	//Derived alpha
	bool					m_deriveAlpha;	//Should the element derive alpha from parent?
protected:
	// control flags
	bool m_unclippedRectDirt;
	bool m_clippedRectDirt;
	bool m_derivedPosDirt;
	bool m_derivedSizeDirt;
	bool m_locked;

	virtual void fillProperties();

	void _UpdateDocking(const math::rectf*vp);
	void _UpdateAlignment(const math::rectf*vp);
	void _UpdateAnchor(const math::rectf*vp);

public:


	ObjectEvent OnMouseEnter;
	ObjectEvent OnMouseLeave;
	ObjectEvent OnMouseMove;
	ObjectEvent OnMouseDown;
	ObjectEvent OnMouseUp;

	ObjectEvent OnReceiveFocus;
	ObjectEvent OnLostFocus;


	DECLARE_PROPERTY_TYPE(Locked,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ID,uint,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Visible,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Enabled,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Position, math::vector2d, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AnchorPosition, math::vector2d, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Size,math::vector2d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(HorizontalAlignment,EHorizontalAlignment,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(VerticalAlignment,EVerticalAlignment,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Docking,EElementDock,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Text,core::UTFString,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Color,video::SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Alpha,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(DeriveAlpha,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AnchorLeft, bool, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AnchorRight, bool, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AnchorTop, bool, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AnchorBottom, bool, MRAY_DLL);

	virtual bool _UpdateRegion(const math::rectf*vp);
	virtual void _UpdateDerivedAlpha();
public:
	IGUIElement(const GUID& type,IGUIManager* creator);
	virtual ~IGUIElement();

	void SetCreator(IGUIManager* mngr);
	IGUIManager* GetCreator();


	const bool& IsLocked();
	virtual bool SetLocked(const bool& locked);

	bool SetID(const uint&id);
	const uint& GetID();
	
	bool SetName(const core::string& name);
	const core::string&  GetName() const;

	const GUID& GetType()const;

	virtual bool SetText(const core::UTFString&t);
	virtual const core::UTFString& GetText()const ;

	virtual bool SetVisible(bool v);
	bool IsVisible(bool parentSearch)const;
	const bool& IsVisible()const;

	virtual bool SetColor(const video::SColor& c){m_color.Set(c.R,c.G,c.B,m_color.A);return true;}
	const video::SColor& GetColor()const{return m_color;}

	virtual bool SetAlpha(float c);
	float GetAlpha()const{return m_color.A;}

	// Get the active Alpha value
	float GetDerivedAlpha(){return m_deriveAlpha?m_derivedAlphaValue:m_color.A;}

	virtual bool SetDeriveAlpha(bool v){m_deriveAlpha=v;return true;}
	bool GetDeriveAlpha()const{return m_deriveAlpha;}

	bool Show();
	bool Hide();

	virtual bool SetEnabled(bool e);
	bool IsEnabled(bool parentSearch)const;
	const bool& IsEnabled()const;

	bool Enable();
	bool Disable();

	// move the element to the topmost of it's siblings
	bool MoveToTop();
	// move the element to the bottom of it's siblings
	bool MoveToBottom();

	//returns true if the point (x,y) is within the elements region
	virtual bool IsPointInside(const math::vector2d& pos);

	virtual bool Move(const math::vector2d& dpos);
	virtual bool SetPosition(const math::vector2d& pos);
	virtual const math::vector2d& GetPosition()const;
	virtual const math::vector2d& GetDerivedPosition();


	virtual bool SetAnchorPosition(const math::vector2d& pos);
	virtual const math::vector2d& GetAnchorPosition()const;

	virtual bool SetSize(const math::vector2d& sz);
	virtual const math::vector2d& GetSize()const;
	virtual const math::vector2d& GetDerivedSize();

	//virtual const math::rectf& GetUnclippedRect();
	//virtual const math::rectf& GetClippedRect();

	virtual const GUIElementRegion* GetDefaultRegion();
	virtual const GUIElementRegion* GetDefaultRegion()const;

	virtual void AttachToRegion(GUIElementRegion* p);
	IGUIPanelElement* GetParent()const;

	virtual bool SetAnchor(EElementAnchor anchor, bool set);
	virtual bool GetAnchor(EElementAnchor anchor)const ;

	bool SetAnchorLeft(bool s){ return SetAnchor(EEA_Left, s); }
	bool GetAnchorLeft()const{ return GetAnchor(EEA_Left); }
	bool SetAnchorRight(bool s){ return SetAnchor(EEA_Right, s); }
	bool GetAnchorRight()const{ return GetAnchor(EEA_Right); }
	bool SetAnchorTop(bool s){ return SetAnchor(EEA_Top, s); }
	bool GetAnchorTop()const{ return GetAnchor(EEA_Top); }
	bool SetAnchorBottom(bool s){ return SetAnchor(EEA_Bottom, s); }
	bool GetAnchorBottom()const{ return GetAnchor(EEA_Bottom); }

	virtual bool SetVerticalAlignment(EVerticalAlignment e);
	const EVerticalAlignment& GetVerticalAlignment()const;

	virtual bool SetHorizontalAlignment(EHorizontalAlignment e);
	const EHorizontalAlignment& GetHorizontalAlignment()const;

	virtual bool SetDocking(EElementDock d);
	const EElementDock& GetDocking()const;

	//return true 'only' if it was "IGUIPanelElement"
	virtual bool IsPanelElement(){return false;}

	virtual bool OnEvent(Event* e);

	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);
	virtual void PostDraw(const math::rectf*vp){}

	virtual void LoadXMLSettings(xml::XMLElement*e);

	virtual void OnVisit(IGUIElementVisitor*v);

	virtual FontAttributes* GetFontAttributes(){return 0;}

	virtual IGUIElement* Duplicate();
	virtual void CopyProperties(const IGUIElement*o);

	virtual void _RectsDirt();


protected:
	//internal methods
	virtual bool _OnMouseEvent(MouseEvent*evt);
	virtual bool _OnKeyboardEvent(KeyboardEvent*evt);

	virtual void OnMoved(const GUIElementEvent& e);
	virtual void OnResized(const GUIElementEvent& e);
	virtual void OnChangeVisible(const GUIElementEvent& e);
	virtual void OnChangeEnable(const GUIElementEvent& e);
	virtual void OnChangeParent(const GUIElementEvent& e);
	virtual void OnSetCreator(const GUIElementEvent& e);

	friend class IGUIPanelElement;

protected:
	virtual void _notifyMouseEnter();
	virtual void _notifyMouseLeave();

	virtual void _notifyReceiveFocus();
	virtual void _notifyLostFocus();

	friend class IGUIManager;

};

MakeSharedPtrType(IGUIElement);

}
}


#endif // IGUIElement_h__
