
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   17:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDynamicLibrary.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDynamicLibrary
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDynamicLibrary___
#define ___IDynamicLibrary___

#include "compileConfig.h"
#include "mString.h"

#include "IDynamicLibrary.h"
#include "GCPtr.h"

namespace mray{

namespace OS{


class MRAY_DLL IDynamicLibrary
{
public:
	IDynamicLibrary(){};
	virtual~IDynamicLibrary(){}

	virtual const  core::string&getLibName()const=0;

	virtual void* getSymbolName(const char*name)const=0;

};

MakeSharedPtrType(IDynamicLibrary);

}

}


#endif //___IDynamicLibrary___
