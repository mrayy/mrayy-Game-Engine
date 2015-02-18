

/********************************************************************
	created:	2008/12/18
	created:	18:12:2008   18:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IScriptManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IScriptManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this class is the base class for script languages
*********************************************************************/

#ifndef ___IScriptManager___
#define ___IScriptManager___

#include "IScript.h"
#include "ISingleton.h"
#include "IFunctionHandler.h"

namespace mray{
namespace script{

	typedef int (*ScriptFunction)(IFunctionHandler*h);

class MRAY_DLL IScriptManager:public ISingleton<IScriptManager>
{
	typedef std::list<IScriptPtr> ScriptsList;
	ScriptsList m_scripts;

	uint m_lastID;

	virtual IScript* createScriptInternal()=0;
public :
	IScriptManager();
	virtual~IScriptManager();

	virtual GCPtr<IScript> createScript(const core::string&name=mT(""));
	virtual GCPtr<IScript> createScript(const core::string&name,OS::IStream* file);

	virtual void ExecuteFile(OS::IStream* file)=0;

	virtual bool ExectuteScript(const core::string& cmd)=0;



	virtual void addScript(const IScriptPtr& s);
	virtual IScriptPtr getScript(const core::string&name);
	virtual void removeScript(const core::string&name);

	//update scripts
	virtual void Update(float dt);


};

}
}


#endif //___IScriptManager___

