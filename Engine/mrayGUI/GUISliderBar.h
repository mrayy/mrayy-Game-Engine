

/********************************************************************
	created:	2012/01/26
	created:	26:1:2012   11:55
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUISliderBar.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUISliderBar
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUISliderBar__
#define __GUISliderBar__

#include "compileConfig.h"
#include "IGUISliderBar.h"

#include "GUIElementFactoryDef.h"
#include "DraggableItem.h"

namespace mray
{
namespace GUI
{

class MRAYGUI_API GUISliderBar:public IGUISliderBar
{
	DECLARE_RTTI
private:
protected:
	ESliderBarDirection m_direction;

	float m_value;
	float m_minValue;
	float m_maxValue;

	DraggableItem m_ticketMover;

	math::rectf m_ticketRect;

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);
public:
	GUISliderBar(IGUIManager* creator);
	virtual~GUISliderBar();

	bool SetDirection(ESliderBarDirection val);
	ESliderBarDirection GetDirection()const;

	bool SetValue(float val);
	float GetValue()const;

	bool SetMaxValue(float val);
	float GetMaxValue()const;

	bool SetMinValue(float val);
	float GetMinValue()const;

	virtual void Draw(const math::rectf*vp);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
};

DECLARE_ELEMENT_FACTORY(GUISliderBar);

}
}

#endif

