


#ifndef ___CAttribute___
#define ___CAttribute___

#include "IAttribute.h"
#include "StringConverter.h"



namespace mray{

template <class T>
class MRAY_DLL CAttribute:public IAttribute{
protected:
	T m_value;
public:
	CAttribute(const core::string&name,const core::string&type,const core::string&info,T value):
	  IAttribute(type,name,info){
		  m_value=value;
	  }
	CAttribute(const core::string&name,const core::string&type,T value):
	  IAttribute(type,name,mT("")){
		  m_value=value;
	}

	bool operator<(const CAttribute&other)const {
		return m_name<other.m_name;
	}
	

	virtual void setValue(void*value){
		  m_value=*(T*)value;
	}
	virtual void*getValue(){
		return (void*)&m_value;
	}

	virtual void parseString(const core::string&format){
		core::StringConverter::parse(format,m_value);
	}

	virtual const mchar*toString()const{
		return core::StringConverter::toString(m_value);
	}

	virtual void*getData(){
		return (void*)&m_value;
	}
};


}



#endif




