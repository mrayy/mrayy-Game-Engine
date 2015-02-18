
#ifndef ___GUIEffector___
#define ___GUIEffector___


#include "CompileConfig.h"
#include "GCPtr.h"
#include "IObject.h"

namespace mray{
namespace GUI{

class GUIElement;

class MRAY_DLL GUIEffector:public IObject
{
protected:
	bool m_enabled;
	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUIEffector,bool> Enabled;

	GUIEffector();
	virtual ~GUIEffector();

	void setEnable(const bool& e){m_enabled=e;}
	const bool& isEnabled(){return m_enabled;}

	virtual void effect(GUIElement* elem,float dt)=0;

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

MakeSharedPtrType(GUIEffector)

}
}

#endif




