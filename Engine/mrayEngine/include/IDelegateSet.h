
/********************************************************************
	created:	2009/06/02
	created:	2:6:2009   18:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDelegateSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDelegateSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDelegateSet___
#define ___IDelegateSet___

#include "IDelegate.h"


namespace mray{

	typedef std::list<IDelegateEvent*> DelegateList;
	typedef core::IteratorPair<DelegateList> DelegateListIterator; 

class IDelegateSet
{
private:
protected:
public:
	IDelegateSet(){}
	virtual~IDelegateSet(){}

	virtual void addDelegateEvent(IDelegateEvent*e)=0;
	virtual DelegateListIterator getDelegateEventList()=0;
	virtual IDelegateEvent* getDelegateEvent(const core::string&name)=0;

};

}


#endif //___IDelegateSet___
