
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   17:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDllManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDllManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDllManager___
#define ___IDllManager___

#include "ISingleton.h"
#include "GCPtr.h"
#include "IDynamicLibrary.h"

namespace mray{
namespace OS{


class MRAY_DLL IDllManager:public ISingleton<IDllManager>
{
public:

	IDllManager(){};
	virtual~IDllManager(){};

	virtual void ClearLibraries()=0;

	virtual IDynamicLibraryPtr getLibrary(const  core::string& name)=0;
	virtual void loadLibrariesFromDir(const  core::string& path)=0;
};

}
}


#endif //___IDllManager___
