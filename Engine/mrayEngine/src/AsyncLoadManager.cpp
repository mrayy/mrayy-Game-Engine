
#include "stdafx.h"
#include "AsyncLoadManager.h"
#include "IAsyncLoadRequest.h"
#include "IThreadManager.h"
#include "MutexLocks.h"
#include "JobPool.h"

namespace mray
{


	class AsyncLoadManagerJobOrder:public JobOrder
	{
	protected:
		AsyncLoadManager* m_manager;
	public:
		AsyncLoadManagerJobOrder(AsyncLoadManager* mngr):m_manager(mngr)
		{
		}
		bool ExecuteJob()
		{
			IAsyncLoadRequest*req= 0;
			do 
			{
				req=m_manager->GetAccessor()->PopRequest();
				if(!req)
					break;
				bool res=req->StartLoading();
				if(!res)
					req->OnFailedLoading();

			} while (true);
			return true;
		}

	};

//////////////////////////////////////////////////////////////////////////

AsyncLoadManager::AsyncLoadManagerAccessor::AsyncLoadManagerAccessor()
:m_manager(0),m_mutex(0)
{
}
AsyncLoadManager::AsyncLoadManagerAccessor::~AsyncLoadManagerAccessor()
{
	delete m_mutex;
}

void AsyncLoadManager::AsyncLoadManagerAccessor::AddRequest(IAsyncLoadRequest*request,EAsyncLoadPriority priority)
{
	OS::ScopedLock lock(m_mutex);
	m_manager->m_requestQueues[priority].push_back(request);
}
IAsyncLoadRequest* AsyncLoadManager::AsyncLoadManagerAccessor::PopRequest()
{
	OS::ScopedLock lock(m_mutex);
	for(int i=0;i<EALP_Count;++i)
	{
		if(!m_manager->m_requestQueues[i].empty())
		{
			IAsyncLoadRequest* tmp=m_manager->m_requestQueues[i].front();
			m_manager->m_requestQueues[i].pop_front();
			return tmp;
		}
	}
	return 0;
}
uint AsyncLoadManager::AsyncLoadManagerAccessor::GetQueueSize(EAsyncLoadPriority q)
{
	OS::ScopedLock lock(m_mutex);
	return m_manager->m_requestQueues[q].size();
}

//////////////////////////////////////////////////////////////////////////

AsyncLoadManager::AsyncLoadManager()
{
	m_jobInUse=false;
	m_accessor.m_manager=this;
	m_accessor.m_mutex=OS::IThreadManager::getInstance().createMutex();

	m_loadingJob=new AsyncLoadManagerJobOrder(this);

	m_loadCompleted=newClassDelegate2<AsyncLoadManager,void,JobOrder*,bool>(mT(""),this,&AsyncLoadManager::LoadingComplete);
}
AsyncLoadManager::~AsyncLoadManager()
{
	for(int i=0;i<EALP_Count;++i)
	{
		RequestQueue::iterator it=m_requestQueues[i].begin();
		for (;it!=m_requestQueues[i].end();++it)
		{
			(*it)->OnFailedLoading();
			//delete *it;
		}
		m_requestQueues[i].clear();
	}
	delete m_loadCompleted;
}
AsyncLoadManager::AsyncLoadManagerAccessor* AsyncLoadManager::GetAccessor()
{
	return &m_accessor;
}
void AsyncLoadManager::AddRequest(IAsyncLoadRequest*request,EAsyncLoadPriority priority)
{
	m_accessor.AddRequest(request,priority);
	if(!m_jobInUse)
	{
		m_jobInUse=true;
		JobPool::getInstance().AddRequest(m_loadingJob,m_loadCompleted);
	}
}

uint AsyncLoadManager::GetRequestQueueSize(EAsyncLoadPriority priority)
{
	return m_accessor.GetQueueSize(priority);
}

void AsyncLoadManager::LoadingComplete(JobOrder*,bool)
{
	m_jobInUse=false;
}

}

