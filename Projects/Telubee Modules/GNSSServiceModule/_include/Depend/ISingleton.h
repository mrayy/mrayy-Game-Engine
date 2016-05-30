

#ifndef ___ISingleton___
#define ___ISingleton___


namespace mray{

template <typename T>
class ISingleton
{
protected:
	static T*m_instance;
public:
	ISingleton(){

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
		return *m_instance;
	}
	static bool isExist(){
		return m_instance!=0;
	}
	static T*getInstancePtr(){

		return m_instance;
	}
};
template <typename T> T* ISingleton <T>::m_instance = 0;

}


#endif

