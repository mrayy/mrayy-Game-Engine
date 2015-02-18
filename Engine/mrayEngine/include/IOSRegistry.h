/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   11:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IOSRegistry.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IOSRegistry
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IOSRegistry___
#define ___IOSRegistry___

#include "ISingleton.h"
#include "mstring.h"

namespace mray{
namespace OS{

	enum ERegisteryKey
	{
		ERK_Classes_Root,
		ERK_Current_User,
		ERK_Local_Machine,
		ERK_Users,
		ERK_Current_Config
	};

class MRAY_DLL IOSRegistry:public ISingleton<IOSRegistry>
{
public:
	virtual bool getKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,core::string&value)=0;
	virtual bool setKeyValue(ERegisteryKey cat,const core::string& path,const core::string&key,const core::string&value)=0;
	virtual bool isKeyExist(ERegisteryKey cat,const core::string& path,const core::string&key)=0;
};

}

}


#endif //___IOSRegistry___
