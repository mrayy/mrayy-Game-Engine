
/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\NullMutex.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	NullMutex
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NullMutex___
#define ___NullMutex___

#include "IMutex.h"

namespace mray{
namespace OS{

class NullMutex:public IMutex
{
public:
	virtual void lock(){}
	virtual void unlock(){}
};

}
}

#endif //___NullMutex___
