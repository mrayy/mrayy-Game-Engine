
#include "stdafx.h"
#include "RuleBasedSystem.h"



namespace mray
{
namespace AI
{

RuleBasedSystem::RuleBasedSystem(const core::string &name):
	m_name(name)
{
}
RuleBasedSystem::~RuleBasedSystem()
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


void RuleBasedSystem::SetName(const core::string &name)
{
	m_name=name;
}
const core::string & RuleBasedSystem::GetName()
{
	return m_name;
}


void RuleBasedSystem::AddSymbol(const RBSSymbol&s)
{
	SymbolIDMap::iterator it= m_idmap.find(s.name);
	if(it==m_idmap.end()){
		m_symbols.push_back(s);
		m_idmap.insert(SymbolIDMap::value_type(s.name,m_symbols.size()-1));
	}
}
void RuleBasedSystem::AddRule(const std::vector<RBSSymbol> &conditions,
			 const std::vector<RBSSymbol> &actions)
{
	RBSRule r;
	RuleBasedSystem*rbs;
	for(int i=0;i<conditions.size();++i){
		const RBSSymbol &s=conditions[i];
		rbs=0;
		int id=GetSymbolID(s.name,&rbs);
		if(id==-1 || rbs==0)
			return;
		r.conditions.push_back(RBSRule::RuleAtom(SymbolID(rbs,id),s.value));
	}
	for(int i=0;i<actions.size();++i){
		const RBSSymbol &s=actions[i];
		rbs=0;
		int id=GetSymbolID(s.name,&rbs);
		if(id==-1 || rbs==0)
			return;
		r.actions.push_back(RBSRule::RuleAtom(SymbolID(rbs,id),s.value));
	}
	m_rules.push_back(r);
}

void RuleBasedSystem::AttachSensor(const core::string&symbol,RBSSensorFunc func)
{
	SymbolIDMap::iterator it= m_idmap.find(symbol);
	if(it==m_idmap.end())
		return;
	m_sensors[it->second]=func;
}
void RuleBasedSystem::AttachTrigger(const core::string&symbol,RBSTriggerFunc func)
{
	SymbolIDMap::iterator it= m_idmap.find(symbol);
	if(it==m_idmap.end())
		return;
	m_triggers[it->second]=func;
}
void RuleBasedSystem::SetSymbol(const core::string&name,bool v)
{
	SymbolIDMap::iterator it= m_idmap.find(name);
	if(it==m_idmap.end()){
		RBSSymbol symb(name,v,ESV_Unset);
		m_symbols.push_back(symb);
		m_idmap.insert(SymbolIDMap::value_type(name,m_symbols.size()));
	}else{
		m_symbols[it->second].value=v;
	}
}
int RuleBasedSystem::GetSymbolID(const core::string&name,RuleBasedSystem**outRBS)
{
	if(outRBS)
		*outRBS=0;
	SymbolIDMap::iterator it= m_idmap.find(name);
	if(it==m_idmap.end()){
		//look for RBS name
		int i=name.find(':');
		if(i==-1)
			return -1;
		core::string rbsName=name.substr(0,i);
		RBSMap::iterator it= m_attachedRBS.find(rbsName);
		if(it==m_attachedRBS.end())
			return -1;
		RuleBasedSystem*rbs=it->second;
		core::string symName=name.substr(i+1,name.length()-i);
		return rbs->GetSymbolID(symName,outRBS);
	}
	if(outRBS)
		*outRBS=this;
	return it->second;
}
bool RuleBasedSystem::GetSymbol(const core::string&name)
{
	RuleBasedSystem*rbs=0;
	int id=GetSymbolID(name,&rbs);
	if(id==-1 || rbs==0)
		return false;
	return rbs->m_symbols[id].value;
}


void RuleBasedSystem::CheckSensors()
{
	SensorsMap::iterator it=m_sensors.begin();
	for(;it!=m_sensors.end();++it)
	{
		RBSSensorFunc f(it->second);
		m_symbols[it->first].value=(*f)(0);
	}
}
void RuleBasedSystem::ApplyTriggers()
{
	TriggersMap::iterator it=m_triggers.begin();
	for(;it!=m_triggers.end();++it)
	{
		if(m_symbols[it->first].value)
		{
			RBSTriggerFunc f(it->second);
			(*f)(0);
		}
	}
}


void RuleBasedSystem::ResetVariables()
{
	for(int i=0;i<m_symbols.size();++i){
		if(m_symbols[i].defaultValue!=ESV_Unset)
		{
			m_symbols[i].value=(m_symbols[i].defaultValue==ESV_True);
		}
	}
}
void RuleBasedSystem::CheckRules()
{
	for(int i=0;i<m_rules.size();++i){
		RBSRule&rule=m_rules[i];
		bool ok=true;
		for(int j=0;j<rule.conditions.size();++j)
		{
			RBSRule::RuleAtom &atom=rule.conditions[j];
			bool v=atom.id.rbs->m_symbols[atom.id.id].value;
			if(v!=atom.val)
			{
				ok=false;
				break;
			}
		}
		if(!ok)
			continue;
		ResetVariables();
		for(int j=0;j<rule.actions.size();++j)
		{
			RBSRule::RuleAtom &atom=rule.actions[j];
			atom.id.rbs->m_symbols[atom.id.id].value=atom.val;
		}
		return;
	}
	ResetVariables();
}


void RuleBasedSystem::Process()
{
	CheckSensors();
	CheckRules();
	ApplyTriggers();
}

void RuleBasedSystem::AttachRBS(RuleBasedSystem*rbs)
{
	m_attachedRBS[rbs->GetName()]=rbs;
}
void RuleBasedSystem::DeattachRBS(RuleBasedSystem*rbs)
{
	m_attachedRBS.erase(rbs->GetName());
}

}
}