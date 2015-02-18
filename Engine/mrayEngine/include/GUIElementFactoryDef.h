

/********************************************************************
	created:	2011/01/12
	created:	12:1:2011   20:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGUI\GUIElementFactoryDef.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGUI
	file base:	GUIElementFactoryDef
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIElementFactoryDef_h__
#define GUIElementFactoryDef_h__

#include "GUID.h"

namespace mray
{
namespace GUI
{

	class IGUIManager;
	class IGUIElement;

	class IGUIElementFactory
	{
	public:
		IGUIElementFactory()
		{
		}
		virtual IGUIElement* CreateElement(IGUIManager*creator)const=0;
		virtual const core::string& GetElementType()const=0;
	};

#define DECLARE_ELEMENT_FACTORY(elem)\
	class ElementFactory_##elem:public IGUIElementFactory\
	{\
	public:\
		static const ElementFactory_##elem instance;\
		static const core::string m_ElementType;\
		ElementFactory_##elem();\
		IGUIElement* CreateElement(IGUIManager*creator)const;\
		const core::string& GetElementType()const{return m_ElementType;}\
	};
#define IMPLEMENT_ELEMENT_FACTORY(elem)\
	const ElementFactory_##elem ElementFactory_##elem::instance;\
	const core::string ElementFactory_##elem::m_ElementType=mT(#elem);\
	ElementFactory_##elem::ElementFactory_##elem(){}\
	IGUIElement* ElementFactory_##elem::CreateElement(IGUIManager*creator)const\
		{\
		return new elem(creator);\
		}

}
}
#endif // GUIElementFactoryDef_h__

