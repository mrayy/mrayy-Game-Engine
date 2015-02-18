
#ifndef ___IAttribute___
#define ___IAttribute___

#include "mString.h"
#include "EGenericTypes.h"


namespace mray
{


class MRAY_DLL IAttribute
{
protected:
	EGenericType m_type;
	core::string m_typeStr;
	core::string m_name;
	core::string m_info;
	
public:

	IAttribute(const core::string&type,const core::string&name,const core::string&info);
	
	void setName(const core::string&name);
	const core::string&getName();

	core::string& getNameStr();

	EGenericType getType();
	const core::string& getTypeStr();

	void setInfo(const core::string&info);
	const  core::string&getInfo();

	virtual void setValue(void*value)=0;
	virtual void*getValue()=0;

	virtual void parseString(const core::string&format)=0;

	virtual const core::string&toString()const=0;

	virtual void*getData()=0;

};

}


#endif