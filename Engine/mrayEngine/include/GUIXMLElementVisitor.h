


/********************************************************************
	created:	2011/09/22
	created:	22:9:2011   14:55
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\GUIXMLElementVisitor.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIXMLElementVisitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIXMLElementVisitor__
#define __GUIXMLElementVisitor__

#include "CompileConfig.h"
#include "IGUIElementVisitor.h"
#include "ObjectSerializer.h"


namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace GUI
{

class MRAY_DLL GUIXMLElementVisitor:public IGUIElementVisitor
{
private:
protected:
	xml::XMLElement* m_currElement;
	ObjectSerializer m_serializer;

	xml::XMLElement* WriteNode(IGUIElement* elem);
public:
	GUIXMLElementVisitor();
	virtual~GUIXMLElementVisitor();

	xml::XMLElement* Visit(IGUIElement*e,const core::string&name);

	virtual void OnVisitComponent(IGUIComponent*comp);
	virtual void OnVisitElement(IGUIElement* elem);
	virtual void OnVisitPanel(IGUIPanelElement* elem);

};


}
}

#endif
