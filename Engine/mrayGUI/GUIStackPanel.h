

/********************************************************************
	created:	2011/12/14
	created:	14:12:2011   13:57
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIStackPanel.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIStackPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIStackPanel__
#define __GUIStackPanel__

#include "CompileConfig.h"
#include "IGUIStackPanel.h"
#include "GUIElementFactoryDef.h"


namespace mray
{
namespace GUI
{
	class GUISliderbarComponent;
class MRAYGUI_API GUIStackPanel:public IGUIStackPanel
{
	DECLARE_RTTI;
private:
protected:
	GUISliderbarComponent* m_sliderComp;
	virtual void _RecalcPositions();
	math::vector2d m_acctualSize;
	bool _OnMouseEvent(MouseEvent* e);
	float m_startPos;
public:
	GUIStackPanel(IGUIManager*mngr);
	virtual~GUIStackPanel();

	virtual bool SetStackDirection(EStackDirection dir);
	virtual void SetScrollOffset(float offset);
	virtual float GetScrollOffset() { return m_startPos; }

	virtual bool SetUseScroll(bool scroll) ;
	virtual bool GetUseScroll()const ;

	//virtual const math::rectf& GetUnclippedRect();
	virtual void Draw(const math::rectf*vp);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
};
DECLARE_ELEMENT_FACTORY(GUIStackPanel);

}
}

#endif

