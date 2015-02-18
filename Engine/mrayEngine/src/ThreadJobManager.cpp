#include "stdafx.h"


#include "ThreadJobManager.h"

#include "IJobPiece.h"
#include "JobOrderPieceCollection.h"
#include "IThreadManager.h"
#include "IMutex.h"
#include "JobPool.h"
#include "ThreadBarrier.h"
#include "ITimer.h"
#include "Engine.h"

namespace mray
{

	class TimedJob:public JobOrderPieceCollection
	{
		float m_startTime;
		float m_endTime;
		OS::ThreadBarrier* m_barrier;
	public:
		TimedJob(OS::ThreadBarrier*b,OS::IMutex*m):JobOrderPieceCollection(m), m_endTime(0),m_startTime(0),m_barrier(b)
		{
		}

		float GetTime(){return m_endTime-m_startTime;}
		void Reset(){m_startTime=m_endTime=0;}

		virtual bool ExecuteJob()
		{
			m_startTime = gEngine.getTimer()->getSeconds();
			JobOrderPieceCollection::ExecuteJob();
			m_endTime = gEngine.getTimer()->getSeconds();

			//printf("Signal-");
			m_barrier->Signal();

			return true;
		}
	};

ThreadJobManager::ThreadJobManager()
:m_currStack(0),m_isMT(false)
{
	m_barrier=new OS::ThreadBarrier();
	m_usedThreadsCount=0;
	m_jobCompleted=0;

//	m_jobCompleted=newClassDelegate2<ThreadJobManager,void,JobOrder*,bool>("",this,&ThreadJobManager::OnJobCompleted);
	int cnt=(EDefaultJobQ_Generic3-EDefaultJobQ_Generic0)+1;
	for (int i=0;i<cnt;++i)
	{
		m_jobs.push_back(new TimedJob(m_barrier,OS::IThreadManager::getInstance().createMutex()));
	}
}
ThreadJobManager::~ThreadJobManager()
{
	Reset();
	m_jobs.clear();
	delete m_jobCompleted;
	delete m_barrier;
}
void ThreadJobManager::OnJobCompleted(JobOrder*const &o,const bool&s)
{
	//m_barrier->Signal();
}

void ThreadJobManager::SetMultiThreaded(bool mt)
{
	m_isMT=mt;
}
bool ThreadJobManager::IsMultiThreaded()
{
	return m_isMT;
}

void ThreadJobManager::Reset()
{
	for (int i=0;i<m_jobs.size();++i)
	{
		m_jobs[i]->ClearPieces();
		JobPool::getInstance().RemoveRequest(m_jobs[i]);
	}
	m_currStack=0;
	m_usedThreadsCount=0;
}

void ThreadJobManager::AddPiece(IJobPiece* p)
{
	m_jobs[m_currStack]->AddPiece(p);
	m_currStack=(m_currStack+1)%m_jobs.size();
}


void ThreadJobManager::BeginProcessing()
{
	if(m_isMT){
		m_usedThreadsCount=0;
		int totalPieces=0;
		//count threads 
		for (int i=0;i<m_jobs.size();++i)
		{
			totalPieces+=m_jobs[i]->GetPiecesCount();
			if(m_jobs[i]->GetPiecesCount()>0)
			{
				++m_usedThreadsCount;
			}
		}

//		printf("Start[%d]-",totalPieces);
		m_barrier->SetTargetCount(m_usedThreadsCount+1);

		for (int i=0;i<m_jobs.size();++i)
		{
			if(m_jobs[i]->GetPiecesCount()>0)
			{
				JobPool::getInstance().AddRequest(m_jobs[i],0);
			}
		}
	}else
	{
		for (int i=0;i<m_jobs.size();++i)
		{
			if(m_jobs[i]->GetPiecesCount()>0)
			{
				m_jobs[i]->ExecuteJob();
				m_jobs[i]->SignalJobsDone();
			}
		}
	}

}

void ThreadJobManager::EndProcessing()
{
	if(!m_isMT)
		return;

	m_barrier->Block(m_usedThreadsCount+1);
//	printf("Done\n");
//	float t=0;
	for (int i=0;i<m_jobs.size();++i)
	{
		TimedJob* j=(TimedJob*)m_jobs[i].pointer();
		j->SignalJobsDone();
		//t+=j->GetTime();
		j->Reset();
	}
	//printf("Exec Time=%f\n",t);
}

}

