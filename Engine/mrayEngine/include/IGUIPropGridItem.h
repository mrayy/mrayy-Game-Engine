


/********************************************************************
	created:	2011/12/11
	created:	11:12:2011   21:13
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIGridItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIGridItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIGridItem__
#define __IGUIGridItem__


namespace mray
{
	class MouseEvent;
	class KeyboardEvent;
	class CPropertieSet;
	class IProperty;	

namespace GUI
{
	class IGUIPropertyGrid;


class IGUIPropGridItem
{
private:
protected:
	CPropertieSet*	m_object;
	IProperty*		m_property;

	IGUIPropertyGrid* m_ownerGrid;
public:
	IGUIPropGridItem(IGUIPropertyGrid* grid):m_ownerGrid(grid){}
	virtual~IGUIPropGridItem(){}

	CPropertieSet*	GetObject(){return m_object;}
	IProperty*		GetProperty(){return m_property;}
	virtual void SetProperty(IProperty* prop,CPropertieSet* object)
	{
		m_object=object;
		m_property=prop;
	}

	virtual void SetActive(bool a)=0;
	virtual bool IsActive()=0;

	virtual math::rectf Draw(const math::rectf& rc)=0;

	virtual bool HandleMouseEvent(MouseEvent*e)=0;
	virtual bool HandleKeyboardEvent(KeyboardEvent*e)=0;
};

}
}

#endif
