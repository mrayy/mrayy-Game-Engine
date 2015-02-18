

/********************************************************************
	created:	2013/07/22
	created:	22:7:2013   12:48
	filename: 	C:\Development\mrayEngine\Plugins\mrayLua\LuaHelpers.h
	file path:	C:\Development\mrayEngine\Plugins\mrayLua
	file base:	LuaHelpers
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LuaHelpers__
#define __LuaHelpers__

#include "compileconfig.h"


namespace mray
{
namespace script
{

class MRAY_LUA_DLL LuaHelpers
{
protected:
public:
	static core::string FlagError(lua_State*  s,const core::string &info);
};

}
}


#endif
