 

#include "stdafx.h"
#include "mrayLUA.h"
#include "LuaScriptManager.h"

namespace mray
{
namespace script
{
	IScriptManager* CreateLuaScriptManager()
	{
		return new LuaScriptManager();
	}
}

}

