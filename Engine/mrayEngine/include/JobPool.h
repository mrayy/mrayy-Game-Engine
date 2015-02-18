


/********************************************************************
	created:	2010/12/13
	created:	13:12:2010   21:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\JobPool.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	JobPool
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef JobPool_h__
#define JobPool_h__

#include "IJobQueue.h"

#include "ISingleton.h"




namespace mray
{
	enum EDefaultJobQueues
	{
		EDefaultJobQ_Default,
		EDefaultJobQ_Draw,
		EDefaultJobQ_Scene,
		EDefaultJobQ_SkeletalAnimation,
		EDefaultJobQ_MeshAnimation,
		EDefaultJobQ_Loading,

		EDefaultJobQ_Generic0,
		EDefaultJobQ_Generic1,
		EDefaultJobQ_Generic2,
		EDefaultJobQ_Generic3,

		EDefaultJobQ_Count
	};

class MRAY_DLL JobPool:public ISingleton<JobPool>
{
public :
private:
protected:
	//typedef std::vector<JobQueue*> JobsList;
	typedef std::map<uint,IJobQueue*> JobsMap;
	JobsMap m_jobs;
	
public:
	JobPool();
	virtual ~JobPool();

	void SetupDefaultPools();

	void AddJobQueue(IJobQueue*q);

	void ClearWorkers();

	IJobQueue* FindBestQueue();
/*
	int GetJobQueuesCount();

	JobQueue* GetJobQueue(int i);
*/
	bool AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier=0);
	void RemoveRequest(JobOrder*order);
};

}
#endif // JobPool_h__
