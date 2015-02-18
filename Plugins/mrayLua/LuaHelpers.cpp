

#include "stdafx.h"
#include "LuaHelpers.h"
#include "ILogManager.h"
#include <sstream>

namespace mray
{
namespace script
{


core::string LuaHelpers::FlagError(lua_State*  luaState,const core::string &info)
{
	luabind::object error_msg(luabind::from_stack(luaState, -1));
	std::stringstream ss; 

	ss << error_msg ;

	/*
	msg = lua_tostring(luaState, -1);
	if (msg == NULL){
		return "";
		//msg = "(error with no message)";
	}
	lua_pop(luaState, 1);

	*/
	gLogManager.log(info,ss.str().c_str(),ELL_WARNING);

	return ss.str().c_str();
}


}
}


