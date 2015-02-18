

#include "stdafx.h"
#include "CommunicatorScheme.h"


namespace mray
{
namespace VT
{


CommunicatorScheme::CommunicatorScheme()
{
	m_lastID=0;
}
CommunicatorScheme::~CommunicatorScheme()
{
	ClearItems();
}

CommunicatorScheme::CommunicatorItem* CommunicatorScheme::AddItem(const core::string &name,ECommunicationDataType dt)
{
	CommunicatorItem* c=new CommunicatorItem();
	c->name=name;
	c->dataType=dt;
	c->id=++m_lastID;
	m_componentsList.push_back(c);
	m_idMap[c->id]=m_componentsList.size()-1;
	m_nameMap[c->name]=m_componentsList.size()-1;
	return c;

}
CommunicatorScheme::CommunicatorItem* CommunicatorScheme::GetItemByName(const core::string &name)
{
	NameItemMap::iterator it= m_nameMap.find(name);
	if(it==m_nameMap.end())
		return 0;
	return m_componentsList[it->second];
}
CommunicatorScheme::CommunicatorItem* CommunicatorScheme::GetItemByID(uint id)
{
	IDItemMap::iterator it= m_idMap.find(id);
	if(it==m_idMap.end())
		return 0;
	return m_componentsList[it->second];
}
void CommunicatorScheme::ClearItems()
{
	m_lastID=0;
	m_idMap.clear();
	m_nameMap.clear();
	ItemsList::iterator it= m_componentsList.begin();
	for(;it!=m_componentsList.end();++it)
	{
		delete *it;
	}
	m_componentsList.clear();
}

void CommunicatorScheme::AddSubScheme(CommunicatorScheme* s)
{
	m_subSchemes[s->GetName()]=s;
}
CommunicatorScheme* CommunicatorScheme::GetScheme(const core::string& name)
{
	std::map<core::string,CommunicatorScheme*>::iterator it= m_subSchemes.find(name);
	if(it==m_subSchemes.end())
		return 0;
	return it->second;
	
}

}
}



