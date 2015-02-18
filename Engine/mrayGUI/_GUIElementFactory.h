
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

#include "IGUIElementCreator.h"
#include "ISingleton.h"

#include <map>
#include "IGUIElement.h"
 
namespace mray{
namespace GUI{

class MRAY_DLL GUIElementFactory:public ISingleton<GUIElementFactory>
{
private:
protected:
	typedef std::map<core::string,GCPtr<IGUIElementCreator>> ElementCreators;
	ElementCreators m_creators;
public:
	GUIElementFactory();
	virtual~GUIElementFactory();

	void RegisterFactory(GCPtr<IGUIElementCreator> c);
	void removeElementCreator(const core::string&type);

	IGUIElement* CreateElement(const core::string&type);
};

}
}


#endif //___GUIElementFactory___
