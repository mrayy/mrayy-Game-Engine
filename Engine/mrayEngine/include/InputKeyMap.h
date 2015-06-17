
#ifndef __INPUTKEYMAP__
#define __INPUTKEYMAP__

#include "compileConfig.h"
#include "KEYCode.h"

namespace mray
{
	class KeyboardEvent;

namespace controllers
{
	
class MRAY_DLL InputKeyMap
{
public:
	struct MRAY_DLL CommandInfo
	{
		uint cmd;
		EKEY_CODE code;
		bool ctrl;
		bool shift;

		core::string info;

		core::string ToString(bool withInfo)const;
	};
	typedef std::vector<CommandInfo> CommandList;
protected:

	CommandList m_commands;
public:
	InputKeyMap();
	virtual ~InputKeyMap();

	//return true if no conflicts, and cmd not 0
	bool RegisterKey(uint cmd, EKEY_CODE key, bool ctrl, bool shift, const core::string& info);
	bool UpdateKey(uint cmd, EKEY_CODE key, bool ctrl, bool shift, const core::string& info);
	void RemoveKey(uint cmd);
	int GetCommandIndex(uint cmd);
	uint GetCommand(const KeyboardEvent* e); // return 0 if key undefined

	void ClearKeys();

	const CommandList& GetCommands(){ return m_commands; }
};

}
}


#endif

