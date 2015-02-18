
/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   22:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICondition.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICondition
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICondition___
#define ___ICondition___

#include "mTypes.h"
#include "GCPtr.h"

namespace mray{
namespace OS{

	class IMutex;

class ICondition
{
private:
protected:
public:
	ICondition(){}
	virtual~ICondition(){}

	virtual int wait(IMutex*m)=0;
	virtual int wait(IMutex*m,uint ms)=0;

	//  Signal a single thread to wake if it's waiting.
	virtual int signal()=0;

	// Wake all threads waiting on this condition.
	virtual int broadcast()=0;

};

MakeSharedPtrType(ICondition)

}
}


#endif //___ICondition___
