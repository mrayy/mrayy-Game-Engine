 

#include "stdafx.h"
#include "LuaScriptManager.h"
#include "LuaScript.h"

#include <TraceManager.h>
#include <ILogManager.h>
#include <IStream.h>
#include <EngineScriptExporter.h>
#include "LuaHelpers.h"

namespace mray{
namespace script{


	const core::string& LuaScriptManager::FlagError(const core::string &info)
	{
		return LuaHelpers::FlagError(m_luaState,info);
	}
LuaScriptManager::LuaScriptManager()
{
	traceFunction(Script);

	m_luaState = luaL_newstate();//lua_open();
	luabind::open(m_luaState);
	luaL_openlibs(m_luaState);
	luaopen_math(m_luaState);
	EngineScriptExporter::openLibs(m_luaState);

}
LuaScriptManager::~LuaScriptManager(){
	traceFunction(Script);
	lua_close(m_luaState);
}

IScript* LuaScriptManager::createScriptInternal(){
	LuaScript* s=new LuaScript(this);
	return s;
}

lua_State* LuaScriptManager::GetLuaState(){
	return m_luaState;
}



void LuaScriptManager::ExecuteFile(OS::IStream* file){
	traceFunction(Script);
	file->seek(0,OS::ESeek_End);
	int len=file->getPos()/sizeof(char);
	file->seek(0,OS::ESeek_Set);

	char *scriptTxt=new char[len+1];

	file->read(scriptTxt,len);
	scriptTxt[len]=0;

	int status = luaL_loadbuffer(m_luaState, scriptTxt, strlen(scriptTxt), "Console");

	if(status!=0){
		FlagError(mT("Compile Error:"));
	}else{
		status = lua_pcall(m_luaState, 0,0, 0);
		if(status){
			FlagError(mT("Compile time error"));
		}
	}

	delete [] scriptTxt;
}

bool LuaScriptManager::ExectuteScript(const core::string&cmd){
	
	int status = luaL_dostring(m_luaState,cmd.c_str());
	if(status!=0){
		FlagError(mT("Compile time error"));
		return false;
	}
	return true;
}

}
}