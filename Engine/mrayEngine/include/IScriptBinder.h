
/********************************************************************
	created:	2009/07/03
	created:	3:7:2009   19:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IScriptBinder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IScriptBinder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IScriptBinder___
#define ___IScriptBinder___

#include "CompileConfig.h"

namespace mray{
namespace script{

class MRAY_DLL IScriptBinder
{
private:
protected:
public:

	virtual void pushObject(void*o,const char*type)=0;
	virtual void* checkType(int idx,const char*type)=0;
};

}
}

#endif //___IScriptBinder___
