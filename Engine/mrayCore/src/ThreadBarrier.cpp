#include "stdafx.h"

#include "ThreadBarrier.h"
#include "IThreadManager.h"
#include "IMutex.h"
#include "ICondition.h"
#include "MutexLocks.h"


namespace mray
{
namespace OS
{


ThreadBarrier::ThreadBarrier()
{
	m_phase=0;
	m_mutex=IThreadManager::getInstance().createMutex();
	m_condition=IThreadManager::getInstance().createCondition();

	m_locks=0;
	m_maxLocks=0;
}
ThreadBarrier::~ThreadBarrier()
{
	delete m_mutex;
	delete m_condition;
}

void ThreadBarrier::SetTargetCount(uint count)
{
	int locks=0;
	
	{
		ScopedLock locker(m_mutex);
		locks=m_locks;
	}
	if(locks>0)Release();
	m_maxLocks=count;
	m_locks=0;
}
void ThreadBarrier::Block(uint count)
{
	if(count>0)
		m_maxLocks=count;

	int phase=0;
	ScopedLock locker(m_mutex);
	phase=m_phase;
	++m_locks;
	if(m_locks==m_maxLocks)
	{
		m_locks=0;
		m_phase=1-m_phase;
		m_condition->broadcast();
	}else
	{
		while(m_phase==phase)
		{
			m_condition->wait(m_mutex);
		}
	}
}
void ThreadBarrier::Block()
{
	Block(m_maxLocks);
}

void ThreadBarrier::Signal()
{
	int phase=0;
	ScopedLock locker(m_mutex);
	phase=m_phase;
	++m_locks;
	if(m_locks==m_maxLocks)
	{
		m_locks=0;
		m_phase=1-m_phase;
		m_condition->broadcast();
	}
}

void ThreadBarrier::Release()
{
	int my_phase=0;
	ScopedLock locker(m_mutex);
	my_phase = m_phase;

	m_locks=0;
	m_phase=1-my_phase;
	m_condition->broadcast();
}
uint ThreadBarrier::GetBlockedThreads()
{
	int cnt=0;
	ScopedLock locker(m_mutex);
	cnt= m_locks;
	return cnt;
}

}
}