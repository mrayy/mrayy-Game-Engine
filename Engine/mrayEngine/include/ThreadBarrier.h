


/********************************************************************
	created:	2010/12/10
	created:	10:12:2010   14:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ThreadBarrier.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ThreadBarrier
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ThreadBarrier_h__
#define ThreadBarrier_h__

#include "compileConfig.h"
#include "mTypes.h"

namespace mray
{
namespace OS
{
	class IMutex;
	class ICondition;

class MRAY_DLL ThreadBarrier
{
private:
protected:
	IMutex* m_mutex;
	ICondition* m_condition;
	uint m_locks;
	uint m_maxLocks;
	int m_phase;
public:
	ThreadBarrier();
	virtual ~ThreadBarrier();

	void SetTargetCount(uint count);
	void Block(uint count);//count is the number of threads to wait for
	void Block();
	void Signal();//signal for one thread
	void Release();
	uint GetBlockedThreads();
};

}
}


#endif // ThreadBarrier_h__
