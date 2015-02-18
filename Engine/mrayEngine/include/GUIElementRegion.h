


/********************************************************************
	created:	2011/12/18
	created:	18:12:2011   9:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\GUIElementRegion.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIElementRegion
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIElementRegion__
#define __GUIElementRegion__


namespace mray
{
namespace GUI
{
	class IGUIElement;

class GUIElementRegion
{
private:
protected:
	GUI::IGUIElement *m_owner;
	math::rectf m_rect;
	math::rectf m_clippedRect;
public:
	GUIElementRegion(GUI::IGUIElement* o):m_owner(o)
	{}
	virtual~GUIElementRegion()
	{}
	const math::rectf& GetRect()const{return m_rect;}
	math::rectf& GetRect(){return m_rect;}
	void SetRect(const math::rectf& rc){m_rect=rc;m_clippedRect=rc;}

	const math::rectf& GetClippedRect()const{return m_clippedRect;}
	void SetClipRect(const math::rectf& rc)
	{
		//m_clippedRect=m_rect;
		m_clippedRect.clipRect(rc);
	}

	GUI::IGUIElement* GetOwner()const{return m_owner;}
};


}
}

#endif
