
#ifndef ___GCPtr___
#define ___GCPtr___

#include "IGCCollector.h"
#include "GCInfo.h"
namespace mray{


template <class T>
class GCPtr
{
	T*	 m_addr;
	//bool m_isArray;
	GCInfo *m_info;
	template <class B>
	friend class GCPtr;
public:
	GCPtr(T*addr=0)
	{
		m_info=IGCCollector::GetInfo(addr);
		if(!m_info){
			m_info=new GCInfo(addr);
			IGCCollector::AddInfo(m_info);
		}else{
			m_info->addRef();
		}
		m_addr=addr;
		//m_isArray=isArray;


	}

	explicit GCPtr(const GCPtr<T> &o)
	{
		m_info=o.m_info;
		m_info->addRef();

		m_addr=o.m_addr;
		//m_isArray=o.m_isArray;

	}/**/
	template <class B>
	GCPtr(const GCPtr<B> &o)
	{
		B*addr=o.pointer();
		T*d=dynamic_cast<T*>(addr);
		m_info=o.m_info;
		m_info->addRef();

		m_addr=d;
		//m_isArray=o.m_isArray;

	}

	~GCPtr(){
		subRef();
	}

	inline bool isNull()const{
		return m_addr==NULL;
	}

	inline T*pointer()const {
		return m_addr;
	}

	T*operator = (T*addr){
		subRef();

		m_info=IGCCollector::GetInfo(addr);

		if(!m_info){
			m_info=new GCInfo(addr);
			IGCCollector::AddInfo(m_info);
		}else{
			m_info->addRef();
		}
		m_addr=addr;
		return m_addr;
	}

	inline bool operator==(const GCPtr<T>&o)const{
		return m_addr==o.m_addr;
	}
	inline bool operator!=(const GCPtr<T>&o)const{
		return m_addr!=o.m_addr;
	}

	GCPtr<T>& operator = (const GCPtr<T>& o){
		subRef();

		m_info= o.m_info;
		m_info->addRef();
		m_addr=o.m_addr;
		//m_isArray=o.m_isArray;
		return *this;
	}

	template <class B>
	GCPtr<T>& operator = (const GCPtr<B>& o){
		subRef();
		T*addr=dynamic_cast<T*>(o.m_addr);
		m_info= o.m_info;
		m_info->addRef();
		m_addr=addr;
		//m_isArray=o.m_isArray;
		return *this;
	}
	void dispose(){
		subRef(true);
		m_info=0;
		m_addr=0;
	}

	inline T& operator*()
	{
		return *m_addr;
	}
	inline T& operator*()const
	{
		return *m_addr;
	}
	inline T* operator->()const
	{
		return m_addr;
	}

	inline T& operator[](int index)
	{
		return m_addr[index];
	}

	inline T& operator[](int index)const
	{
		return m_addr[index];
	}
	inline operator T*(){return m_addr;}
	inline operator T*()const{return m_addr;}

	void subRef(bool imediateRemove=false){
		if(m_addr && m_info->getRefCount()<=1){
			/*
			if(m_isArray)
				delete [] m_addr;
			else delete m_addr;*/
			m_addr=0;
		}
		if(m_info)
			m_info->subRef<T>(imediateRemove);
	}
	int getRefCount()const
	{
		if(m_info)
			return m_info->getRefCount();
		return 0;
	}

	static const GCPtr<T> Null;
};

template <class T>
const GCPtr<T> GCPtr<T>::Null=0;


#define MakeSharedPtrType(type) class type;typedef GCPtr<type> type##Ptr;typedef const GCPtr<type>& type##CRef;

}



#endif



