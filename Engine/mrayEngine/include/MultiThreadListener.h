

/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   19:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MultiThreadListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MultiThreadListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	interface used for those who cares about threads , like Render Device
*********************************************************************/

#ifndef ___MultiThreadListener___
#define ___MultiThreadListener___


#include "GCPtr.h"

namespace mray{
namespace OS{

class MultiThreadListener
{
private:
protected:
public:
	MultiThreadListener(){}
	virtual~MultiThreadListener(){}

	virtual void preThreadStart()=0;
	virtual void postThreadStart()=0;

	virtual void registerThread()=0;
	virtual void unregisterThread()=0;
};

MakeSharedPtrType(MultiThreadListener);

}
}

#endif //___MultiThreadListener___
