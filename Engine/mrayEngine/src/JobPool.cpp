#include "stdafx.h"


#include "JobPool.h"
#include "JobQueue.h"
#include "IThread.h"
#include "EventBaseJobQueue.h"


namespace mray
{

JobPool::JobPool()
{
}
JobPool::~JobPool()
{
	ClearWorkers();
}

void JobPool::SetupDefaultPools()
{
	//Add the default Queue
	for(int i=0;i<2/*EDefaultJobQ_Count*/;++i)
		AddJobQueue(new JobQueue(i));
}
void JobPool::AddJobQueue(IJobQueue*q)
{
	//q->SetPriority(OS::ETP_Highest);
	if(m_jobs[q->GetID()]!=0)
		delete m_jobs[q->GetID()];
	m_jobs[q->GetID()]=(q);
}

void JobPool::ClearWorkers()
{
	JobsMap jtmp=m_jobs;
	m_jobs.clear();
	JobsMap::iterator it=jtmp.begin();
	for(;it!=jtmp.end();++it)
	{
	//	JobsList& jobs=it->second;
	//	for(int i=0;i<jobs.size();++i)
			delete it->second;
	}
	m_jobs.clear();
}/*

int JobPool::GetJobQueuesCount()
{
	return m_jobs.size();
}

JobQueue* JobPool::GetJobQueue(int i)
{
	if(i>=m_jobs.size())return 0;
	return m_jobs[i];
}*/

IJobQueue* JobPool::FindBestQueue()
{
	IJobQueue* targetQ=0;
	//find best queue to insert the order into it
	int bestCnt=99999;
	JobsMap::iterator it= m_jobs.begin();
	for (;it!=m_jobs.end();++it)
	{
		IJobQueue*q=it->second;
		int c=q->GetOrdersCount();
		if(!targetQ || c<bestCnt)
		{
			bestCnt=c;
			targetQ=q;
		}
	}
	return targetQ;
}

bool JobPool::AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier)
{

	int targetQueue=order->GetTargetQueue();
	IJobQueue*targetQ=0;
	if(targetQueue==-1)
	{
		targetQ=FindBestQueue();
	}else
	{
		JobsMap::iterator it= m_jobs.find(targetQueue);
		if(it==m_jobs.end())
			targetQ=FindBestQueue();
		else targetQ=it->second;
	}
	if(!targetQ)
		return false;

	targetQ->AddRequest(order,listner,barrier);

	return true;
}
void JobPool::RemoveRequest(JobOrder*order)
{
	JobsMap::iterator it= m_jobs.begin();
	for (;it!=m_jobs.end();++it)
	{
		IJobQueue*q=it->second;
		if(q->RemoveRequest(order))
			return ;
	}
}


}