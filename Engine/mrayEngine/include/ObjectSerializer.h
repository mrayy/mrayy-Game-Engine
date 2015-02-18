


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   12:36
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ObjectSerializer.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ObjectSerializer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ObjectSerializer__
#define __ObjectSerializer__

#include "compileConfig.h"
#include "IPropertyVistor.h"
#include "mstring.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
class IObject;

class MRAY_DLL ObjectSerializer:public IPropertyVistor
{
private:
protected:
	xml::XMLElement* m_currElement;
	CPropertieSet* m_currObject;

	xml::XMLElement* WriteElement(const core::string&name);
public:
	ObjectSerializer();
	virtual~ObjectSerializer();

	xml::XMLElement* Serialize(IObject* object);

	void Visit(IObject*prop);
	void Visit(IProperty*prop);
	void Visit(CPropertieSet*prop);
	void Visit(CPropertieDictionary*prop);
};


}

#endif