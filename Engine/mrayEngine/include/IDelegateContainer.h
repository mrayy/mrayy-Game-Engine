

/********************************************************************
	created:	2012/07/08
	created:	8:7:2012   15:44
	filename: 	c:\Development\mrayEngine\Engine\mrayEngine\include\IDelegateContainer.h
	file path:	c:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IDelegateContainer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IDelegateContainer__
#define __IDelegateContainer__

#include "IDelegate.h"

namespace mray
{
	class IObject;
	typedef IObject* IObjectPtr;
	typedef IDelegate2<void,IObjectPtr,mray::PVOID> ObjectDelegate;
	#define DECLARE_OBJECT_DELEGATE(Name) void Name(IObjectPtr p1,mray::PVOID p2)
	#define ADD_DELEGATE(T,Name) AddDelegate(newClassDelegate2(mT(#Name),this,&T::Name))

class IDelegateContainer
{
protected:
	typedef std::map<core::string,ObjectDelegate*> DelegateMap;
	DelegateMap m_delegates;
public:
	IDelegateContainer(){}
	virtual~IDelegateContainer()
	{
		DelegateMap::iterator it=m_delegates.begin();
		for (;it!=m_delegates.end();++it)
		{
			delete it->second;
		}
		m_delegates.clear();
	}
	void AddDelegate(ObjectDelegate* d)
	{
		m_delegates[d->getName()]=d;
	}

	void CallDelegate(const core::string& name,IObject* p1,PVOID p2)
	{
		DelegateMap::iterator it= m_delegates.find(name);
		if(it==m_delegates.end())
			return;
		(*(*it).second)(p1,p2);
	}
};

}

#endif