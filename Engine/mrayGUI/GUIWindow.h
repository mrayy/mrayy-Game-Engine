

#ifndef ___GUIWINDOW___
#define ___GUIWINDOW___


#include "CompileConfig.h"
#include "IGUIPanelElement.h"
#include "SColor.h"
#include "IFont.h"

#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"
#include "DraggableItem.h"
#include "TextureUnit.h"

namespace mray
{
	class KeyboardEvent;
	class MouseEvent;
	namespace video
	{
		class TextureUnit;
	}
namespace GUI
{

class MRAYGUI_API GUIWindow:public IGUIPanelElement
{
	DECLARE_RTTI
protected:
	 
	enum ESelectedIcon
	{
		ESI_None,
		ESI_Close,
		ESI_Collapse
	};
	enum EMouseState
	{
		EMS_None,
		EMS_Hover,
		EMS_Down
	};
	ESelectedIcon m_selectedIcon;
	EMouseState m_mouseState;

	FontAttributes m_FontAttributes;

	bool m_isMovable;
	float m_titleHeight;
	bool m_canClose;

	DraggableItem m_windowDrag;

	//math::rectf m_frameRect;
	math::rectf m_innerRect;
	//math::rectf m_titleRect;

	GUIElementRegion* m_frameRegion;
	GUIElementRegion* m_titleRegion;

	video::TextureUnitPtr m_titleTexture;

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);

	virtual void fillProperties();

	void _GetRects(const math::rectf& rc,math::rectf& title,math::rectf& frame);
	virtual bool _UpdateRegion(const math::rectf*vp);
public:
	static const GUID ElementType;

// 	DECLARE_PROPERTY_TYPE(IsMovable,bool,MRAYGUI_API);
// 	DECLARE_PROPERTY_TYPE(CanClose,bool,MRAYGUI_API);
// 	DECLARE_PROPERTY_TYPE(TitleThickness,float,MRAYGUI_API);
public:

	GUIWindow(IGUIManager*mngr);
	GUIWindow(const GUID& type,IGUIManager*mngr);
	virtual~GUIWindow();

	//virtual const math::rectf& GetUnclippedRect();
	virtual void Draw(const math::rectf*vp);

	virtual void AddElement(IGUIElement* e);

	void SetTitleTexture(video::ITextureCRef  tex);
	video::TextureUnit* GetTitleTexture();

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

public:

public:

	const float& GUIWindow::getTitleHeight() { return m_titleHeight; }
	void GUIWindow::setTitleHeight(const float&val) { m_titleHeight = val; }

	const bool &GUIWindow::getIsMovable()  { return m_isMovable; }
	void GUIWindow::setIsMovable(const bool& val) { m_isMovable = val; }

	const bool &GUIWindow::getCanClose()  { return m_canClose; }
	void GUIWindow::setCanClose(const bool& val) { m_canClose = val; }


};

DECLARE_ELEMENT_FACTORY(GUIWindow);

}
}

#endif







