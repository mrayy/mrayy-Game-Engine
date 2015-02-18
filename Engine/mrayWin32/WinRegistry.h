
/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   11:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\WinRegistry.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	WinRegistry
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___WinRegistry___
#define ___WinRegistry___

#include <IOSRegistry.h>
#include "CompileConfig.h"

namespace mray{
namespace OS{

class MRAY_Win32_DLL WinRegistry:public IOSRegistry
{
public:
	virtual~WinRegistry(){}
	bool getKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,core::string&value);
	bool setKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,const core::string&value);
	bool isKeyExist(ERegisteryKey cat,const core::string& path,const core::string&key);
};

}
}

#endif //___WinRegistry___
