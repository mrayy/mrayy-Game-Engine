
#include "stdafx.h"
#include "EventBaseJobQueue.h"
#include "IOSystem.h"
#include "IThreadManager.h"
#include "ILogManager.h"
#include "ThreadBarrier.h"
#include "IThread.h"
#include "ICondition.h"
#include "IMutex.h"
#include "MutexLocks.h"
#include "IThreadEvent.h"


namespace mray
{

class EventBasedJobQueueThreadFunc:public OS::IThreadFunction
{
protected:
	EventBaseJobQueue* m_resQueue;
public:
	EventBasedJobQueueThreadFunc(EventBaseJobQueue*o){
		m_resQueue=o;
	}
	virtual~EventBasedJobQueueThreadFunc(){
	}
	virtual void setup(){
	}
	virtual void execute(OS::IThread*caller,void*arg){
		EventBaseJobQueue::RequeseList::iterator it;
		EventBaseJobQueue::LoadRequest r;
		OS::IMutex* mutex= m_resQueue->m_reqMutex;
		while(caller->isActive()){
			mutex->lock();
			if(m_resQueue->IsQueueDone()){
				//caller->suspend();
				if(m_resQueue->m_newRequest->Wait(0)!=OS::ETEvent_Object0)
				{
					mutex->unlock();
					continue;
				}
			}
			it=m_resQueue->m_requests.begin();
			if(it!=m_resQueue->m_requests.end()){
				r=*(*it);
				m_resQueue->_addToGraveYard(it);
			}
			mutex->unlock();

			bool res= r.order->ExecuteJob();
			if(r.listner)
				(*r.listner)(r.order,res);
			if(r.barrier)
				r.barrier->Signal();
		}
	}
};

EventBaseJobQueue::EventBaseJobQueue(uint id):IJobQueue(id)
{
	m_reqMutex=OS::IThreadManager::getInstance().createMutex();
	m_threadFunc=new EventBasedJobQueueThreadFunc(this);
	m_thread=OS::IThreadManager::getInstance().createThread(m_threadFunc);
	m_newRequest=OS::IThreadManager::getInstance().createEvent();
	m_thread->start(0);/**/
}
EventBaseJobQueue::~EventBaseJobQueue(){
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	m_thread=0;
	delete m_threadFunc;
	delete m_reqMutex;
	delete m_newRequest;
	m_requests.clear();
}



void EventBaseJobQueue::SetPriority(OS::EThreadPriority p)
{
	m_thread->setThreadPriority(p);
}

OS::EThreadPriority EventBaseJobQueue::GetPriority()
{
	return m_thread->getThreadPriority();
}

void EventBaseJobQueue::_addToGraveYard(EventBaseJobQueue::RequeseList::iterator it)
{
	LoadRequest*r=*it;
	m_requests.erase(it);

	r->order=0;
	r->listner=0;
	m_graveyard.push_back(r);
}
EventBaseJobQueue::LoadRequest* EventBaseJobQueue::_createRequest()
{
	if(m_graveyard.begin()==m_graveyard.end())
		return new LoadRequest;
	else
	{
		LoadRequest*r=*m_graveyard.begin();
		m_graveyard.erase(m_graveyard.begin());
		return r;
	}
}

void EventBaseJobQueue::AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier)
{
	LoadRequest* r=_createRequest();
	r->listner=listner;
	r->order=order;
	r->barrier=barrier;
	//	if(r->barrier)r->barrier->Lock();
	m_reqMutex->lock();
	m_requests.push_back(r);
	m_reqMutex->unlock();
	m_newRequest->SetEvent();
}

bool EventBaseJobQueue::IsQueueDone()
{
	OS::ScopedLock autoLock(m_reqMutex);
	return m_requests.size()==0;
}
int EventBaseJobQueue::GetOrdersCount()
{
	OS::ScopedLock autoLock(m_reqMutex);
	return m_requests.size();
}


}



