

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   14:09
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\CommunicatorScheme.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	CommunicatorScheme
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CommunicatorScheme___
#define ___CommunicatorScheme___

#include "VTCommon.h"

namespace mray
{
namespace VT
{

class CommunicatorScheme
{
public:
	struct CommunicatorItem
	{
		core::string name;
		uint id;
		ECommunicationDataType dataType;
	};
	typedef std::vector<CommunicatorItem*> ItemsList;
protected:

	core::string m_name;

	typedef std::map<core::string,uint> NameItemMap;
	typedef std::map<uint,uint> IDItemMap;

	NameItemMap m_nameMap;
	IDItemMap m_idMap;
	ItemsList m_componentsList;
	uint m_lastID;


	std::map<core::string,CommunicatorScheme*> m_subSchemes;

public:
	CommunicatorScheme();
	virtual~CommunicatorScheme();

	void SetName(const core::string& name){m_name=name;}
	const core::string& GetName(){return m_name;}
	
	CommunicatorItem* AddItem(const core::string &name,ECommunicationDataType dt);
	CommunicatorItem* GetItemByName(const core::string &name);
	CommunicatorItem* GetItemByID(uint id);
	void ClearItems();

	const ItemsList& GetComponents()const{return m_componentsList;}

	void AddSubScheme(CommunicatorScheme* s);
	CommunicatorScheme* GetScheme(const core::string& name);

};

}
}

#endif

