

/********************************************************************
	created:	2011/12/12
	created:	12:12:2011   10:35
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIPropEditBoxGridItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPropEditBoxGridItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIPropEditBoxGridItem__
#define __GUIPropEditBoxGridItem__

#include "IGUIPropGridItem.h"
#include "GUIEditBoxComponent.h"

namespace mray
{
namespace GUI
{

class GUIPropEditBoxGridItem:public IGUIPropGridItem,public GUIEditBoxComponent::IListener
{
private:
protected:
	GUIEditBoxComponent* m_component;
public:
	GUIPropEditBoxGridItem(IGUIPropertyGrid* grid);
	virtual~GUIPropEditBoxGridItem();

	virtual void OnTextChangeCallback(GUIEditBoxComponent*caller);
	virtual void SetProperty(IProperty* prop,CPropertieSet* object);

	virtual math::rectf Draw(const math::rectf& rc);

	virtual bool HandleMouseEvent(MouseEvent*e);
	virtual bool HandleKeyboardEvent(KeyboardEvent*e);

	virtual void SetActive(bool a){m_component->active=a;}
	virtual bool IsActive(){return m_component->active;}

};


}
}

#endif
