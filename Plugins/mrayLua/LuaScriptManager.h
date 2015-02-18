
/********************************************************************
	created:	2008/12/18
	created:	18:12:2008   19:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayLua\LuaScriptManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayLua
	file base:	LuaScriptManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LuaScriptManager___
#define ___LuaScriptManager___

#include <IScriptManager.h>
#include "compileConfig.h"
#include "CmdParams.h"


#include "luabind/luabind.hpp"
#include "luabind/function.hpp"


namespace mray{
namespace script{


class MRAY_LUA_DLL LuaScriptManager:public IScriptManager
{
protected:
	core::string m_lastErrorString;
	lua_State*  m_luaState;


protected:
	virtual IScript* createScriptInternal();

	Param::EParamType getParamType(lua_State*s,int idx);


public:

	LuaScriptManager();
	virtual~LuaScriptManager();

	lua_State* GetLuaState();

	virtual void ExecuteFile(OS::IStream* file);

	virtual bool ExectuteScript(const core::string& cmd);

	template<class T>
	void BindObject(const core::string&name,T*o)
	{
		luabind::globals(m_luaState)[name.c_str()]=o;
	}



	const core::string& FlagError(const core::string &info);
	const core::string& FlagError(lua_State*s,const core::string &info);
};


}
}


#endif //___LuaScriptManager___
