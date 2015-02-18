
/********************************************************************
	created:	2008/12/18
	created:	18:12:2008   17:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IScript.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IScript
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IScript___
#define ___IScript___

#include "mString.h"
#include "common.h"
#include "IResource.h"
#include "IValue.h"
#include "CmdParams.h"
#include "IScriptBinder.h"
#include "IFunctionHandler.h"


namespace mray{
namespace OS{
	class IStream;
}
namespace script{

enum EScriptStateTag
{
	ESS_WAITFRAME,
	ESS_WAITTIME,
	ESS_RUNNING,
	ESS_NOTLOADED,
	ESS_DONE
};

class IScript:public IResource
{
public:

	virtual int SetScript(OS::IStream* file)=0;
	virtual int SetScript(const core::string& s)=0;

	virtual int ExecuteScript()=0;


};

MakeSharedPtrType(IScript);

}
}

#endif //___IScript___


