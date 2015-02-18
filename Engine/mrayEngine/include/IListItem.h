

/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   11:56
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\IListItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	IListItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IListItem__
#define __IListItem__

#include "UTFString.h"

namespace mray
{
namespace GUI
{

	class IGUIElement;
	class IFont;
	class IGUIRenderer;

class IListItem
{
private:
protected:
public:
	void* userData;
	IListItem(void*ud=0):userData(ud){}
	virtual~IListItem(){}

	virtual core::UTFString toString()const=0;

	virtual void Draw(const math::rectf& rc, IGUIElement* lstBox, IFont* font, IGUIRenderer* renderer) = 0;

	//virtual float GetHeight(IGUIElement* lstBox) = 0;
};

class MRAY_DLL StringListItem:public IListItem
{
	core::UTFString m_text;
public:
	StringListItem(const core::UTFString& txt,void* ud=0):IListItem(ud),m_text(txt)
	{}
	virtual~StringListItem()
	{}

	virtual core::UTFString toString()const
	{
		return m_text;
	}
	virtual void Draw(const math::rectf& rc, IGUIElement* lstBox, IFont* font, IGUIRenderer* renderer);
	//virtual float GetHeight(IGUIElement* lstBox);
};

typedef std::vector<IListItem*> ItemList;


}
}

#endif
