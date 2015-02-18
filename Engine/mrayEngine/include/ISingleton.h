

#ifndef ___ISingleton___
#define ___ISingleton___

#include "macros.h"

namespace mray{

template <typename T>
class ISingleton
{
protected:
	static T*m_instance;
public:
	ISingleton(){
		FATAL_ERROR(m_instance!=0,mT("Duplicated Singleton!"));

#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(ISingleton <T>*)(T*)1;
		m_instance = (T*)((int)this + offset);
#else
		m_instance = static_cast< T* >( this );
#endif
	}
	virtual~ISingleton(){
		m_instance=0;
	}
	static T&getInstance(){
		FATAL_ERROR(m_instance==0,mT("Instance object not created!"));		return *m_instance;
	}
	static bool isExist(){
		return m_instance!=0;
	}
	static T*getInstancePtr(){
		FATAL_ERROR(m_instance==0,mT("Instance object not created!"));
		return m_instance;
	}
};
template <typename T> T* ISingleton <T>::m_instance = 0;

}


#endif

