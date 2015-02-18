
/********************************************************************
	created:	2009/04/10
	created:	10:4:2009   18:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIOverlay.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIOverlay
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIOverlay___
#define ___GUIOverlay___

#include "CompileConfig.h"

#include "mstring.h"


namespace mray
{
	class IDelegateContainer;
namespace xml
{
	class XMLTree;
	class XMLElement;
	class IXMLNode;
}
namespace GUI
{

	class IGUIElement;
	class IGUIManager;
	class IGUIPanelElement;
	class IGUISchemeBase;

typedef std::list<IGUIElement*> ElementList;
class MRAY_DLL GUIOverlay
{
private:
protected:
	typedef std::list<xml::XMLElement*> xmlElementList;

	//GUIEffector* createEffector(xml::XMLElement*elem);
	IGUIElement* _CreateElement(xml::XMLElement* elem,IGUIManager* manager);

	xmlElementList m_elements;
	core::string m_name;

	xml::IXMLNode* CreateNode(xml::IXMLNode*node);
	xml::IXMLNode* DuplicateElement(xml::IXMLNode* elem);

	void TraverseElements(IGUIElement* elems, IGUISchemeBase* scheme,IDelegateContainer* eventListener);

public:
	GUIOverlay();
	virtual~GUIOverlay();

	void SetName(const core::string&name);
	const core::string& GetName();

	void AddElement(xml::XMLElement* elem);
	const std::list<xml::XMLElement*>& GetElements(){return m_elements;}

	ElementList CreateElements(IGUIManager* manager,IGUIPanelElement* parent,IDelegateContainer* eventListener,IGUISchemeBase* scheme);

};

}
}


#endif //___GUIOverlay___
