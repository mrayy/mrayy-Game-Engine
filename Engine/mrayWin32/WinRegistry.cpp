#include "stdafx.h"
#include "WinRegistry.h"

#include <windows.h>

namespace mray{
namespace OS{

	const HKEY s_regKeys[]=
	{
		HKEY_CLASSES_ROOT,
		HKEY_CURRENT_USER,
		HKEY_LOCAL_MACHINE,
		HKEY_USERS,
		HKEY_CURRENT_CONFIG
	};

bool WinRegistry::getKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,core::string&value){

	mchar buf[256];
	DWORD size = sizeof( buf );
	HKEY regKey;
	LONG res;
	if ( (res=RegOpenKeyEx( s_regKeys[cat], path.c_str(), 0, KEY_QUERY_VALUE, &regKey )) == ERROR_SUCCESS )
	{

		if ( (res=RegQueryValueEx( regKey, key.c_str(), NULL, NULL, (BYTE*)buf, &size )) == ERROR_SUCCESS )
		{
			value =buf ;
		}

		RegCloseKey( regKey );
	}
	return res== ERROR_SUCCESS;
}
bool WinRegistry::setKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,const core::string&value){

	DWORD size = value.length()*sizeof(mchar);
	HKEY regKey;
	LONG res;
	res=RegCreateKeyEx( s_regKeys[cat], path.c_str(), 0,0,0, KEY_WRITE,0, &regKey ,0);
	switch(res){
		case ERROR_FILE_NOT_FOUND:
			break;
	}
	if(res==ERROR_SUCCESS)
	{
		res=RegSetValueEx( regKey, key.c_str(), NULL, REG_SZ, (const BYTE*)value.c_str(), size );
		RegCloseKey( regKey );
	}

	return res== ERROR_SUCCESS;
}
bool WinRegistry::isKeyExist(ERegisteryKey cat,const core::string& path,const core::string&key){

	LONG res;
	HKEY regKey;
	if ( (res=RegOpenKeyEx( s_regKeys[cat], path.c_str(), 0, KEY_QUERY_VALUE, &regKey )) == ERROR_SUCCESS )
	{

		RegCloseKey( regKey );
	}
	return res== ERROR_SUCCESS;
}

}
}

