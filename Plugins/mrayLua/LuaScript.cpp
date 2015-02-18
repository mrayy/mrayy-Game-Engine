
#include "stdafx.h"
#include "LuaScript.h"
#include "LuaScriptManager.h"

#include <GenericValues.h>
#include <LogManager.h>
#include <TraceManager.h>

#include <IStream.h>

#include    <stdio.h>
#include    <string.h>
#include    <assert.h>
#include "LuaHelpers.h"


namespace mray{
namespace script{

LuaScript::LuaScript(LuaScriptManager* manager)
{
	traceFunction(Script);
	m_manager=manager;
	lua_State *state=manager->GetLuaState();

	// create a thread/state for this object
	m_threadState = lua_newthread(state);
	if(m_threadState)
	{
	//	lua_pushlightuserdata(state,m_threadState);
//		lua_settable(state,LUA_GLOBALSINDEX);
	}
}

LuaScript::~LuaScript()
{
	traceFunction(Script);
	m_manager->removeScript(this->getResourceName());
}

uint LuaScript::calcSizeInternal(){
	return sizeof(LuaScript);
}


int LuaScript::SetScript(OS::IStream* file){
	traceFunction(Script);
	if(!file)
		return -1;
	file->seek(0,OS::ESeek_End);
	int len=file->getPos()/sizeof(char);
	file->seek(0,OS::ESeek_Set);

	char *scriptTxt=new char[len+1];

	file->read(scriptTxt,len);
	scriptTxt[len]=0;
	int res=SetScript(scriptTxt);
	delete [] scriptTxt;
	return res;
}

int LuaScript::SetScript(const core::string& s){
	traceFunction(Script);
	int         status;

	FATAL_ERROR(m_threadState==0,mT("Lua Thread Not Created!"));

	//status = luaL_dostring(m_threadState, s.c_str());
	status = luaL_loadbuffer(m_threadState, s.c_str(), s.length(), "Console");
	if(status!=0){
		LuaHelpers::FlagError(m_threadState,mT("Compile Error:"));
		return -1;
	}
	return 0;
}


int LuaScript::ExecuteScript()
{
	traceFunction(Script);
	int         status;
	status = lua_pcall(m_threadState,0,0, 0);
	//1-> yield , that's ok..
	if (status>1)
	{
		LuaHelpers::FlagError(m_threadState,mT("Runtime Error:"));
		return -1;
	}
	return 0;
}


}
}

