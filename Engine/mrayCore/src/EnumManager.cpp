#include "stdafx.h"

#include "EnumManager.h"


namespace mray
{



bool EnumInfo::ContainsValue(const core::string& v)const
{
	EnumNameValMap::const_iterator it=NameVal.find(v);
	if(it==NameVal.end())
		return false;
	return true;
}
bool EnumInfo::ContainsValue(int v)const
{
	EnumValNameMap::const_iterator it=ValName.find(v);
	if(it==ValName.end())
		return false;
	return true;
}

const core::string& EnumInfo::ToString(int v)const
{
	EnumValNameMap::const_iterator it=ValName.find(v);
	if(it!=ValName.end())
		return it->second;
	return core::string::Empty;
}
int EnumInfo::Parse(const core::string& v)const
{
	EnumNameValMap::const_iterator it=NameVal.find(v);
	if(it!=NameVal.end())
		return it->second;
	return -1;
}

EnumManager::EnumManager():m_dummyString(mT("")){
	m_currInfo=0;
}
EnumManager::~EnumManager(){
	clear();
}
void EnumManager::clear(){
	EnumInfoMap::iterator p=m_enumInfo.begin();
	EnumInfoMap::iterator end=m_enumInfo.end();
	for(;p!=end;++p)
	{
		delete (*p).second;
	}
}

void EnumManager::startEnum(const core::string&name){
	m_currInfo=new EnumInfo();
	m_currInfo->name=name;
	m_enumInfo[name]=m_currInfo;
}
void EnumManager::addValue(const core::string&name,int val){
	if(!m_currInfo)
		return;
	m_currInfo->NameVal[name]=val;
	m_currInfo->ValName[val]=name;
}
void EnumManager::endEnum(){
	m_currInfo=0;
}

void EnumManager::removeEnum(const core::string&e){
	EnumInfoMap::iterator it= m_enumInfo.find(e);
	if(it==m_enumInfo.end())
		return;
	delete it->second;
	m_enumInfo.erase(it);
}

int EnumManager::getValue(const core::string&e,const core::string&v)
{
	EnumInfoMap::iterator it= m_enumInfo.find(e);
	if(it==m_enumInfo.end())
		return -1;
	return it->second->Parse(v);
}
const core::string& EnumManager::getName(const core::string&e,int v){
	EnumInfoMap::iterator it= m_enumInfo.find(e);
	if(it==m_enumInfo.end())
		return core::string::Empty;
	return it->second->ToString(v);
}
const EnumInfo*  EnumManager::getEnumInfo(const core::string&e){
	EnumInfoMap::iterator it= m_enumInfo.find(e);
	if(it==m_enumInfo.end())
		return 0;
	return it->second;
}

}
