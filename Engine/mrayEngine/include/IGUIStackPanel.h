


/********************************************************************
	created:	2011/12/14
	created:	14:12:2011   13:54
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIStackPanel.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIStackPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIStackPanel__
#define __IGUIStackPanel__

#include "IGUIPanelElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIStackPanel:public IGUIPanelElement
{
public:
	enum EStackDirection
	{
		EHorizontal,
		EVertical
	};
private:
protected:
	EStackDirection m_direction;
	float m_offset;
public:
	DECLARE_PROPERTY_TYPE(Direction,EStackDirection,MRAY_DLL)
	DECLARE_PROPERTY_TYPE(Offset,float,MRAY_DLL)
public:
	static const GUID ElementType;

	IGUIStackPanel(IGUIManager* creator);
	virtual~IGUIStackPanel();

	virtual bool SetOffset(float offset)
	{
		m_offset=offset;
		return true;
	}
	float GetOffset()
	{
		return m_offset;
	}

	virtual bool SetUseScroll(bool scroll) = 0;
	virtual bool GetUseScroll()const = 0;

	virtual void SetScrollOffset(float offset) = 0;
	virtual float GetScrollOffset() = 0;

	virtual bool SetStackDirection(EStackDirection dir)
	{
		m_direction=dir;
		return true;
	}
	EStackDirection GetStackDirection()
	{
		return m_direction;
	}

};

}
}

#endif