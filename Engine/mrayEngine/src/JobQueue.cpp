#include "stdafx.h"

#include "JobQueue.h"

#include "IOSystem.h"
#include "IThreadManager.h"
#include "ILogManager.h"
#include "ThreadBarrier.h"
#include "IThread.h"
#include "ICondition.h"
#include "IMutex.h"
#include "MutexLocks.h"
 


namespace mray
{

class JobQueueThreadFunc:public OS::IThreadFunction
{
protected:
	JobQueue* m_resQueue;
	JobQueue::RequeseList m_requests;
public:
	JobQueueThreadFunc(JobQueue*o){
		m_resQueue=o;
	}
	virtual~JobQueueThreadFunc(){
	}
	virtual void setup(){
	}
	virtual void execute(OS::IThread*caller,void*arg){
		JobQueue::RequeseList::iterator it;
		JobQueue::LoadRequest *r;
		OS::IMutex* mutex= m_resQueue->m_reqMutex;
		while(caller->isActive()){
			mutex->lock();
			if(m_resQueue->IsQueueDone()){
				//caller->suspend();
				if(m_resQueue->m_workCond->wait(mutex)==-1)
				{
					break;
				}
				if(m_resQueue->IsTerminated())
					break;
			}
			m_requests=m_resQueue->m_requests;
			m_resQueue->m_requests.clear();
			mutex->unlock();

			it=m_requests.begin();
			for(;it!=m_requests.end();++it)
			{
				r=(*it);
				bool res= r->order->ExecuteJob();
				if(r->listner)
					(*r->listner)(r->order,res);
				if(r->barrier)
					r->barrier->Signal();
			}

			m_resQueue->m_graveYardMutex->lock();
			it=m_requests.begin();
			for(;it!=m_requests.end();++it)
			{
				m_resQueue->_addToGraveYard(*it);
			}
			m_requests.clear();
			m_resQueue->m_graveYardMutex->unlock();
		}
	}
};

JobQueue::JobQueue(uint id):IJobQueue(id)
{
	m_terminated=false;
	m_reqMutex=OS::IThreadManager::getInstance().createMutex();
	m_graveYardMutex=OS::IThreadManager::getInstance().createMutex();
	m_threadFunc=new JobQueueThreadFunc(this);
	m_thread=OS::IThreadManager::getInstance().createThread(m_threadFunc);
	m_workCond=OS::IThreadManager::getInstance().createCondition();
	m_thread->start(0);/**/
}
JobQueue::~JobQueue(){
	m_terminated=true;
	m_workCond->signal();
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	m_thread=0;
	delete m_threadFunc;
	delete m_reqMutex;
	delete m_graveYardMutex;
	delete m_workCond;
	m_requests.clear();
	RequeseList::iterator it= m_graveyard.begin();
	for (;it!=m_graveyard.end();++it)
	{
		delete *it;
	}
}



void JobQueue::SetPriority(OS::EThreadPriority p)
{
	m_thread->setThreadPriority(p);
}

OS::EThreadPriority JobQueue::GetPriority()
{
	return m_thread->getThreadPriority();
}

void JobQueue::_addToGraveYard(LoadRequest* r)
{
	r->order=0;
	r->listner=0;
	m_graveyard.push_back(r);
}
JobQueue::LoadRequest* JobQueue::_createRequest()
{
	OS::ScopedLock lock(m_graveYardMutex);
	if(m_graveyard.begin()==m_graveyard.end())
		return new LoadRequest;
	else
	{
		LoadRequest*r=*m_graveyard.begin();
		m_graveyard.erase(m_graveyard.begin());
		return r;
	}
}

void JobQueue::AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier)
{
	LoadRequest* r=_createRequest();
	r->listner=listner;
	r->order=order;
	r->barrier=barrier;
//	if(r->barrier)r->barrier->Lock();
	m_reqMutex->lock();
		m_requests.push_back(r);
	m_reqMutex->unlock();
	m_workCond->signal();
}

bool JobQueue::RemoveRequest(JobOrder* order)
{
	bool ret=false;
	bool signal = false;
	m_reqMutex->lock();
	if (m_requests.size() > 0)
	{
		signal = true;
		RequeseList::iterator it = m_requests.begin();
		for (; it != m_requests.end(); ++it)
		{
			if ((*it)->order.pointer() == order)
			{
				m_requests.erase(it);
				ret = true;
				break;
			}
		}
	}
	m_reqMutex->unlock();
	if (signal)
		m_workCond->signal();
	return true;
}

bool JobQueue::IsQueueDone()
{
	OS::ScopedLock autoLock(m_reqMutex);
	return m_requests.size()==0;
}
int JobQueue::GetOrdersCount()
{
	OS::ScopedLock autoLock(m_reqMutex);
	return m_requests.size();
}


}