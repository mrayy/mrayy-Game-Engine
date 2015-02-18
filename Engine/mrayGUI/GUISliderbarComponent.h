


/********************************************************************
	created:	2011/12/15
	created:	15:12:2011   14:22
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUISliderbarComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUISliderbarComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUISliderbarComponent__
#define __GUISliderbarComponent__

#include "CompileConfig.h"
#include "IGUIComponent.h"

namespace mray
{
namespace GUI
{

class MRAYGUI_API GUISliderbarComponent:public IGUIComponent
{
public:
	class IListener
	{
	public:
		virtual void OnValueChanged(GUISliderbarComponent*caller)=0;
	};

	enum EDirection
	{
		EHorizontal,
		EVertical
	};

private:
protected:
	static const int g_scrollSize;
	float m_lastMousePos;
	bool m_drawScroll;
	bool m_scrollOn;
	int m_upButState;
	int m_botButState;
	float m_startPos;

	void GetScrollBarRects(const math::rectf& innerRect,
		math::rectf &topArrow,math::rectf &botArrow,math::rectf &scrollerBG,math::rectf &scrollerBar);

	void _changeValue(float amount);

public:

	float m_value;
	float m_minVal;
	float m_maxVal;

	float itemsCount;
	float pageSize;

	void DrawScroll(bool d){ m_drawScroll = d; }
	bool IsDrawScroll(){ return m_drawScroll ; }

	void SetValue(float v);
	void SetMinValue(float v);
	void SetMaxValue(float v);

	float GetMaxValue(){ return m_maxVal; }
	float GetMinValue(){ return m_minVal; }

	void SetPercentValue(float v){m_value=v*(m_maxVal-m_minVal)+m_minVal;}
	float GetPercentValue(){return (m_value-m_minVal)/(m_maxVal-m_minVal);}

	IListener* listener;
	EDirection m_direction;

	GUISliderbarComponent();
	virtual~GUISliderbarComponent();

	virtual void Draw();

	bool OnKeyboardEvent(KeyboardEvent* e);
	bool OnMouseEvent(MouseEvent* e);

	void LostFocus();
	void LostMouseOn();
};


}
}

#endif
