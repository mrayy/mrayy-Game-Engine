

/********************************************************************
	created:	2013/07/22
	created:	22:7:2013   14:07
	filename: 	C:\Development\mrayEngine\Plugins\engineScriptExport\LuaHelpers.h
	file path:	C:\Development\mrayEngine\Plugins\engineScriptExport
	file base:	LuaHelpers
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LuaHelpers__
#define __LuaHelpers__



#define LUA_CONST_START( ClassName,L ) { object g = globals(L); object table = g[ClassName]; 
#define LUA_CONST( ClassName,ClassType, name ) table[ClassName] = ClassType::name 
#define LUA_CONST_END } 

#endif
