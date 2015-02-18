
/********************************************************************
	created:	2009/05/30
	created:	30:5:2009   17:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ObjectFactoryManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ObjectFactoryManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ObjectFactoryManager___
#define ___ObjectFactoryManager___

#include "ObjectFactory.h"
#include "GenericLoggerSystem.h"


namespace mray{

	template <class T>
class ObjectFactoryManager
{
private:
protected:

	typedef std::map<core::string,ObjectFactory<T>*> FactoryMap;
	FactoryMap m_factories;

public:
	ObjectFactoryManager()
	{
	}
	virtual~ObjectFactoryManager()
	{
		ClearFactories();
	}

	void RegisterFactory(ObjectFactory<T>* factory)
	{
		m_factories[factory->GetType()]=factory;
	}
	void UnregisterFactory(const core::string&name)
	{
		FactoryMap::iterator it= m_factories.find(name);
		if(it==m_factories.end())return ;
		m_factories.erase(it);
	}
	ObjectFactory<T>* GetFactory(const core::string&name)
	{
		FactoryMap::iterator it= m_factories.find(name);
		if(it==m_factories.end())return 0;
		return it->second;
	}

	void ClearFactories()
	{
		FactoryMap::iterator it= m_factories.begin();
		for(;it!=m_factories.end();++it)
		{
			delete it->second;
		}
		m_factories.clear();
	}
	
	T* CreateObject(const core::string&name)
	{
		ObjectFactory<T>* f= GetFactory(name);
		if(!f)
		{
			gGenericLoggerSystem.log(mT("Factory with type: '")+name+mT("' Not Found"),ELL_WARNING,EVL_Normal);
			return 0;
		}
		return f->CreateObject();
	}
};

}


#endif //___ObjectFactoryManager___
