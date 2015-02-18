


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   10:32
	filename: 	d:\Development\mrayEngine\Engine\mraySerialization\SerializationManager.h
	file path:	d:\Development\mrayEngine\Engine\mraySerialization
	file base:	SerializationManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SerializationManager__
#define __SerializationManager__

#include <map>

namespace mray
{
namespace serialize
{
	class IClassSerialize;

class SerializationManager
{
private:
protected:
	typedef std::map<core::string,IClassSerialize*> SerializationMap;
	SerializationMap m_classes;

public:
	SerializationManager();
	virtual~SerializationManager();

	void AddClass(IClassSerialize*c);
	IClassSerialize* GetClass(const core::string&name);

	xml::XMLElement* Serialize(void*obj,const core::string&type);
};


}
}

#endif
