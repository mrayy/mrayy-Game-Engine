/********************************************************************
	created:	2013/03/08
	created:	8:3:2013   2:22
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GUISpringTouchElement.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GUISpringTouchElement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GUISpringTouchElement_
#define _GUISpringTouchElement_

#include "IGUIElement.h"

namespace mray
{
	namespace VT
	{
		class SpringTouchSensorComponentImpl;
	}
namespace GUI
{

class GUISpringTouchElement:public IGUIElement
{
	DECLARE_RTTI;
protected:
	VT::SpringTouchSensorComponentImpl* m_elem;
public:
	static const GUID ElementType;
public:
	GUISpringTouchElement(IGUIManager* creator);
	virtual~GUISpringTouchElement();

	void SetElement(VT::SpringTouchSensorComponentImpl* c);
	VT::SpringTouchSensorComponentImpl* GetElement();
	virtual void Draw(video::IRenderArea* vp);
};


}
}

#endif