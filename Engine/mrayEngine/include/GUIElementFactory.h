
/********************************************************************
	created:	2009/04/10
	created:	10:4:2009   10:32
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIElementFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIElementFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIElementFactory___
#define ___GUIElementFactory___

#include "GUIElementFactoryDef.h"
#include "ISingleton.h"

#include <map>


namespace mray{
namespace GUI{

class MRAY_DLL GUIElementFactory:public ISingleton<GUIElementFactory>
{
private:
protected:
	typedef std::map<core::string,const IGUIElementFactory*> ElementCreators;
	ElementCreators m_creators;
public:
	GUIElementFactory();
	virtual~GUIElementFactory();

	void RegisterFactory(const IGUIElementFactory* c);
	void RemoveElementCreator(const core::string&type);

	IGUIElement* CreateElement(const core::string&type,IGUIManager* creator);
};
#define REGISTER_GUIElement_FACTORY(elem) mray::GUI::GUIElementFactory::getInstance().RegisterFactory(&ElementFactory_##elem::instance)
}
}


#endif //___GUIElementFactory___
