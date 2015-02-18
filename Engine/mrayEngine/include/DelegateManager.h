
#ifndef __DelegateManager__
#define __DelegateManager__
/********************************************************************
	created:	2009/07/18
	created:	18:7:2009   15:52
	filename: 	c:\MyFiles\Programming\mrayEngine_Prj\mrayEngine\include\DelegateManager.h
	file path:	c:\MyFiles\Programming\mrayEngine_Prj\mrayEngine\include
	file base:	DelegateManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#include "ISingleton.h"
#include "IDelegateContainer.h"
#include "GCPtr.h"



namespace mray{

	typedef std::map<core::string,GCPtr<ObjectDelegate>> DelegateMap;

class MRAY_DLL DelegateManager:public ISingleton<DelegateManager>
{
private:
protected:

	DelegateMap m_delegates;

public:
	DelegateManager();
	virtual~DelegateManager();

	void registerDelegate(const GCPtr<ObjectDelegate>& d);
	GCPtr<ObjectDelegate> getDelegate(const core::string&name);
	void removeDelegate(const core::string&name);

	void clear();
};

}

#endif
