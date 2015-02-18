

/********************************************************************
	created:	2011/12/12
	created:	12:12:2011   12:23
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIPropComboListGridItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPropComboListGridItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIPropComboListGridItem__
#define __GUIPropComboListGridItem__

#include "IGUIPropGridItem.h"
#include "GUIComboListComponent.h"

namespace mray
{
namespace GUI
{

class GUIPropComboListGridItem:public IGUIPropGridItem,public GUIComboListComponent::IListener
{
private:
protected:
	GUIComboListComponent* m_component;
public:
	GUIPropComboListGridItem(IGUIPropertyGrid* grid);
	virtual~GUIPropComboListGridItem();

	virtual void OnChangeSelection(GUIListBoxComponent*caller);

	virtual void SetProperty(IProperty* prop,CPropertieSet* object);

	void OnValueChanged(GUISliderbarComponent*caller){}

	virtual math::rectf Draw(const math::rectf& rc);

	virtual bool HandleMouseEvent(MouseEvent*e);
	virtual bool HandleKeyboardEvent(KeyboardEvent*e);

	virtual void SetActive(bool a){m_component->active=a;}
	virtual bool IsActive(){return m_component->active;}
};

}
}

#endif

