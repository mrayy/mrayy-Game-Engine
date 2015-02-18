
/********************************************************************
	created:	2009/07/05
	created:	5:7:2009   0:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ScriptRegister.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ScriptRegister
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ScriptRegister___
#define ___ScriptRegister___

#include "IFunctionHandler.h"

#include "IScriptManager.h"

namespace mray{
namespace script{

template <class T>
class ScriptRegister
{
public:
	typedef int (T::*TFunctionPtr)(IFunctionHandler*h);
	typedef struct { const char *name; TFunctionPtr mfunc; } RegType;

private:
protected:
	static std::vector<TFunctionPtr> m_registerFunctions;
	static int m_id;
public:


	ScriptRegister(){
	}
	virtual~ScriptRegister(){
	}

	static void Register(IScriptManager*m)
	{
		m_id=m->beginRegType(T::className);
		for(RegType *p=T::methods;(*p).name;p++){
			RegisterFunction(p->name,p->mfunc);
		}
		m->endRegType();
	}

	static void RegisterFunction(const char*fname,TFunctionPtr func){
		int fid=m_registerFunctions.size();
		m_registerFunctions.push_back(func);
		IScriptManager::getInstance().regTypedFunction(m_id,thunkFunction,fname,fid);
	}

	void pushThis(){
		IScriptManager::getInstance().pushObject(this,T::className);
	}

private:

	static int thunkFunction(IFunctionHandler*h){
		T*pThis=(T*)h->getAttachedObject(T::className);
		if(!pThis)
			return 0;
		int fid=h->getAttachedFunctionID();
		if(fid==-1)
			return 0;
		TFunctionPtr f=m_registerFunctions[fid];
		return (pThis->*f)(h);

	}
};
#define DECLARE_SCRIPT_CLASS(T)\
	mray::std::vector<mray::script::ScriptRegister<T>::TFunctionPtr> mray::script::ScriptRegister<T>::m_registerFunctions;\
	int mray::script::ScriptRegister<T>::m_id=0;

}
}


#endif //___ScriptRegister___
