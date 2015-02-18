#include "stdafx.h"

#include "SymbolMemory.h"

#include "AIComponentTypes.h"
namespace mray
{
namespace AI
{

SymbolMemory::SymbolMemory()
{
}
SymbolMemory::~SymbolMemory()
{
	{

		SensorsMap::iterator it=m_sensors.begin();
		for(;it!=m_sensors.end();++it)
		{
			delete it->second;
		}
	}
	{
		TriggersMap::iterator it=m_triggers.begin();
		for(;it!=m_triggers.end();++it)
		{
			delete it->second;
		}
	}
}

void SymbolMemory::SetName(const core::string &name)
{
	m_name=name;
}
const core::string &SymbolMemory::GetName()
{
	return m_name;
}

void SymbolMemory::AttachSensor(const core::string&symbol,SymbolSensorFunc func)
{
	SymbolIDMap::iterator it= m_idmap.find(symbol);
	if(it==m_idmap.end())
		return;
	m_sensors[it->second]=func;
}
void SymbolMemory::AttachTrigger(const core::string&symbol,SymbolTriggerFunc func)
{
	SymbolIDMap::iterator it= m_idmap.find(symbol);
	if(it==m_idmap.end())
		return;
	m_triggers[it->second]=func;
}

void SymbolMemory::AddSymbol(const MemorySymbol&s)
{
	SymbolIDMap::iterator it= m_idmap.find(s.name);
	if(it==m_idmap.end()){
		m_symbols.push_back(s);
		m_idmap.insert(SymbolIDMap::value_type(s.name,m_symbols.size()-1));
	}
}

void SymbolMemory::SetSymbol(const core::string&name,bool v)
{
	SymbolIDMap::iterator it= m_idmap.find(name);
	if(it==m_idmap.end()){
		MemorySymbol symb(name,v);
		m_symbols.push_back(symb);
		m_idmap.insert(SymbolIDMap::value_type(name,m_symbols.size()));
	}else{
		m_symbols[it->second].value=v;
	}
}
bool SymbolMemory::GetSymbol(const core::string&name)
{
	SymbolMemory*mem=0;
	int id=GetSymbolID(name,&mem);
	if(id==-1 || mem==0)
		return false;
	return mem->m_symbols[id].value;
}
int SymbolMemory::GetSymbolID(const core::string&name,SymbolMemory**outMem)
{
	if(outMem)
		*outMem=0;
	SymbolIDMap::iterator it= m_idmap.find(name);
	if(it==m_idmap.end()){
		//look for mem name
		int i=name.find(':');
		if(i==-1)
			return -1;
		core::string memName=name.substr(0,i);
		NameMemoryMap::iterator it= m_attachedMemories.find(memName);
		if(it==m_attachedMemories.end())
			return -1;
		SymbolMemory*mem=it->second;
		core::string symName=name.substr(i+1,name.length()-i);
		return mem->GetSymbolID(symName,outMem);
	}
	if(outMem)
		*outMem=this;
	return it->second;
}

void SymbolMemory::AttachMemory(SymbolMemory*mem)
{
	m_attachedMemories[mem->GetName()]=mem;
}
void SymbolMemory::DeattachMemory(SymbolMemory*mem)
{
	m_attachedMemories.erase(mem->GetName());
}


void SymbolMemory::CheckSensors()
{
	SensorsMap::iterator it=m_sensors.begin();
	for(;it!=m_sensors.end();++it)
	{
		SymbolSensorFunc f(it->second);
		m_symbols[it->first].value=(*f)(0);
	}
}
void SymbolMemory::ApplyTriggers()
{
	TriggersMap::iterator it=m_triggers.begin();
	for(;it!=m_triggers.end();++it)
	{
		if(m_symbols[it->first].value)
		{
			SymbolTriggerFunc f(it->second);
			(*f)(0);
		}
	}
}



}
}
