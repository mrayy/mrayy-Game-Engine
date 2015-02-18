
/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   22:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayWin32\WinCondition.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayWin32
	file base:	WinCondition
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___WinCondition___
#define ___WinCondition___

#include <ICondition.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace mray{
namespace OS{

class WinCondition:public ICondition
{
private:
protected:
	long m_waiters;
	HANDLE m_sema;
	HANDLE m_waitersDone;
	int m_wasBroadcast;

	int cooperativeWait(HANDLE h,uint ms);
public:
	WinCondition();
	virtual~WinCondition();

	virtual int wait(IMutex*m);
	virtual int wait(IMutex*m,uint ms);

	//  Signal a single thread to wake if it's waiting.
	virtual int signal();

	// Wake all threads waiting on this condition.
	virtual int broadcast();
};

}
}


#endif //___WinCondition___
