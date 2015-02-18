
#ifndef __ICondition__
#define __ICondition__


#include <mstring.h>

namespace mray{


class ICondition
{
protected:
	core::string m_name;
public:
	ICondition(const core::string&name):m_name(name){}
	virtual~ICondition(){}

	virtual core::string&getName(){return m_name;}

	virtual bool checkCondition()=0;
};


}

#endif
