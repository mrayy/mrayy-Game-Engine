

/********************************************************************
	created:	2010/04/24
	created:	24:4:2010   23:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\SymbolMemory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	SymbolMemory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SymbolMemory_h__
#define SymbolMemory_h__

#include "CompileConfig.h"
#include "IAIComponent.h"
#include <IDelegate.h>

#include <mstring.h>
#include <map>
#include <vector>

namespace mray
{
namespace AI
{

struct MemorySymbol
{
private:
protected:
public:
	core::string name;
	bool value;

public:
	MemorySymbol():value(false)
	{}
	MemorySymbol(const core::string& n,bool v)
		:name(n),value(v)
	{
	}
	virtual~MemorySymbol()
	{
	}
};

typedef IDelegate1<bool,PVOID>* SymbolSensorFunc;
typedef IDelegate1<void,PVOID>* SymbolTriggerFunc;

class MRAY_AI_DLL SymbolMemory:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	typedef std::map<core::string,int> SymbolIDMap;
	typedef std::vector<MemorySymbol> SymbolsList;

	typedef std::map<int,SymbolSensorFunc> SensorsMap;
	typedef std::map<int,SymbolTriggerFunc> TriggersMap;

	typedef std::map<core::string,SymbolMemory*> NameMemoryMap;

	core::string m_name;

	SymbolsList m_symbols;
	SymbolIDMap m_idmap;
	SensorsMap m_sensors;
	TriggersMap m_triggers;
	NameMemoryMap m_attachedMemories;
public:
	SymbolMemory();
	virtual ~SymbolMemory();

	void SetName(const core::string &name);
	const core::string &GetName();

	void AttachSensor(const core::string&symbol,SymbolSensorFunc func);
	void AttachTrigger(const core::string&symbol,SymbolTriggerFunc func);

	void AddSymbol(const MemorySymbol&s);

	void SetSymbol(const core::string&name,bool v);
	bool GetSymbol(const core::string&name);
	int GetSymbolID(const core::string&name,SymbolMemory**outMem);

	void AttachMemory(SymbolMemory*rbs);
	void DeattachMemory(SymbolMemory*rbs);


	void CheckSensors();
	void ApplyTriggers();


};

}
}


#endif // SymbolMemory_h__
