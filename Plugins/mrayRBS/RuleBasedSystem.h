
/********************************************************************
	created:	2010/04/02
	created:	2:4:2010   13:59
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayRBS\RuleBasedSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayRBS
	file base:	RuleBasedSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RuleBasedSystem___
#define ___RuleBasedSystem___

#include <mstring.h>
#include <IDelegate.h>
#include <map>
#include <vector>

namespace mray
{
namespace AI
{

enum ERBSSymbolValue
{
	ESV_False,
	ESV_True,
	ESV_Unset
};

struct RBSSymbol
{
private:
protected:
public:

	core::string name;
	bool value;
	ERBSSymbolValue defaultValue;

public:
	RBSSymbol(const core::string& n,bool v,ERBSSymbolValue def)
		:name(n),defaultValue(def),value(v)
	{
	}
	RBSSymbol()
		:defaultValue(ESV_Unset),value(false)
	{
	}
	virtual~RBSSymbol()
	{
	}

};
typedef IDelegate1<bool,PVOID>* RBSSensorFunc;
typedef IDelegate1<void,PVOID>* RBSTriggerFunc;

class RuleBasedSystem
{
private:
protected:

	struct SymbolID
	{
		SymbolID():rbs(0),id(0)
		{}
		SymbolID(RuleBasedSystem*r,int i):rbs(r),id(i)
		{}
		RuleBasedSystem*rbs;
		int id;
	};
	struct RBSRule
	{
		struct RuleAtom
		{
			RuleAtom():val(false)
			{
			}
			RuleAtom(SymbolID i,bool v):id(i),val(v)
			{
			}
			SymbolID id;
			bool val;
		};
		std::vector<RuleAtom> conditions;
		std::vector<RuleAtom> actions;
	};


	typedef std::map<core::string,int> SymbolIDMap;
	typedef std::vector<RBSSymbol> SymbolsMap;
	typedef std::map<int,RBSSensorFunc> SensorsMap;
	typedef std::map<int,RBSTriggerFunc> TriggersMap;
	typedef std::vector<RBSRule> RulesList;
	typedef std::map<core::string,RuleBasedSystem*> RBSMap;


	core::string m_name;

	SymbolsMap m_symbols;
	SymbolIDMap m_idmap;
	SensorsMap m_sensors;
	TriggersMap m_triggers;
	RulesList m_rules;
	RBSMap m_attachedRBS;	//used as shared memory,or as external symbols source

	void CheckSensors();
	void ApplyTriggers();
	void ResetVariables();
	void CheckRules();

public:
	RuleBasedSystem(const core::string &name);
	virtual~RuleBasedSystem();

	void SetName(const core::string &name);
	const core::string &GetName();

	void AttachSensor(const core::string&symbol,RBSSensorFunc func);
	void AttachTrigger(const core::string&symbol,RBSTriggerFunc func);

	void AddRule(const std::vector<RBSSymbol> &conditions,
		const std::vector<RBSSymbol> &actions);
	void AddSymbol(const RBSSymbol&s);

	void SetSymbol(const core::string&name,bool v);
	bool GetSymbol(const core::string&name);
	int GetSymbolID(const core::string&name,RuleBasedSystem**outRBS);

	void AttachRBS(RuleBasedSystem*rbs);
	void DeattachRBS(RuleBasedSystem*rbs);

	void Process();

};

}
}

#endif //___RuleBasedSystem___
