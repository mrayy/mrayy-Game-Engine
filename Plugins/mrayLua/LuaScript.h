

/********************************************************************
	created:	2008/12/18
	created:	18:12:2008   19:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayLua\LuaScript.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayLua
	file base:	LuaScript
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LuaScript___
#define ___LuaScript___


#include <IScript.h>
#include "compileConfig.h"

typedef struct lua_State lua_State;

namespace mray{
namespace script{

	class LuaScriptManager;

class MRAY_LUA_DLL LuaScript:public IScript
{
protected:
     
	lua_State*					m_threadState;
	LuaScriptManager*		m_manager;

	virtual uint calcSizeInternal();

public:

	LuaScript(LuaScriptManager* manager);
	virtual ~LuaScript();

	lua_State* GetLuaState(){return m_threadState;}

	virtual int SetScript(OS::IStream* file);
	virtual int SetScript(const core::string& s);

	virtual int ExecuteScript();


};

}
}


#endif //___LuaScript___

