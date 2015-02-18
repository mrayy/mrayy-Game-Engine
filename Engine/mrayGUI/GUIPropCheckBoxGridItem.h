



/********************************************************************
	created:	2011/12/12
	created:	12:12:2011   14:51
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIPropCheckBoxGridItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPropCheckBoxGridItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIPropCheckBoxGridItem__
#define __GUIPropCheckBoxGridItem__

#include "IGUIPropGridItem.h"
#include "GUICheckBoxComponent.h"

namespace mray
{
namespace GUI
{

class GUIPropCheckBoxGridItem:public IGUIPropGridItem,public GUICheckBoxComponent::IListener
{
private:
protected:
	GUICheckBoxComponent* m_component;
public:
	GUIPropCheckBoxGridItem(IGUIPropertyGrid* grid);
	virtual~GUIPropCheckBoxGridItem();

	virtual void OnChangeCheck(GUICheckBoxComponent*caller);

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
