#ifndef __CompareCondition__
#define __CompareCondition__

#include "ICondition.h"
#include "IInput.h"
#include "ECompareType.h"

namespace mray{

class CompareCondition:public ICondition
{
public:
	ECompareType m_compare;
public:
	CompareCondition(const core::string&name,ECompareType compare):
	  ICondition(name),m_compare(compare)
	  {
	  }
	  virtual~CompareCondition(){
	  }

	ECompareType getType(){return m_compare;}

	virtual bool checkCondition()=0;
};

}

#endif