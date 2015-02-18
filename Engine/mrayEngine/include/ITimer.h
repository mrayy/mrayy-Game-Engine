
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   16:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ITimer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ITimer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ITimer___
#define ___ITimer___

#include "ISingleton.h"
#include "mTypes.h"

namespace mray{
namespace OS{

class MRAY_DLL ITimer
{
public:
	ITimer(){}
	virtual~ITimer(){}

	virtual void initTimer()=0;

	//! get ms value
	virtual ulong getMilliseconds(void)=0;

	virtual ulong getMicroseconds() = 0;
	virtual double getSeconds() = 0;
};

}
}

#endif //___ITimer___

