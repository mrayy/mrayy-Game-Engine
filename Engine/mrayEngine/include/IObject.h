

/********************************************************************
	created:	2009/02/24
	created:	24:2:2009   23:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IObject___
#define ___IObject___

#include "GCPtr.h"
#include "CPropertieSet.h"
#include "XMLElement.h"
#include "RTTI.h"
#include "IDelegateContainer.h"

namespace mray{

class MRAY_DLL IObject:public CPropertieSet
{
	DECLARE_RTTI
public:

	IObject();
	virtual~IObject();

	virtual core::string toString()const;
	virtual void parse(const core::string&str);

	virtual void fillProperties();

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);


	virtual void OnVisit(IPropertyVistor* visitor);
};


}


#endif //___IObject___
