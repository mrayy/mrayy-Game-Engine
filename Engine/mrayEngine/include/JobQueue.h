 
#ifndef ___JobQueue___
#define ___JobQueue___

#include "GCPtr.h"
#include "IJobQueue.h"



namespace mray
{

	namespace OS
	{
		class ThreadBarrier;
		class IThread;
		class ICondition;
		class IMutex;
		class IThreadFunction;
		enum EThreadPriority ;
	}
	class JobQueueThreadFunc;


class MRAY_DLL JobQueue:public IJobQueue
{
protected:
	struct LoadRequest{
		LoadRequest():listner(0),barrier(0)
		{
		}
		virtual~LoadRequest(){
			order=0;
		}
		JobOrderPtr order;
		JobLoadCompleteDelegate listner;
		OS::ThreadBarrier*barrier;
	};

	OS::IThreadFunction* m_threadFunc;
	OS::IThread* m_thread;
	OS::IMutex* m_reqMutex;
	OS::IMutex* m_graveYardMutex;
	OS::ICondition* m_workCond;

	typedef std::list<LoadRequest*> RequeseList;
	RequeseList m_requests;
	RequeseList m_graveyard;
	bool m_terminated;
	friend class JobQueueThreadFunc;

	void _addToGraveYard(LoadRequest* r);
	LoadRequest* _createRequest();

public:

	JobQueue(uint id);
	virtual~JobQueue();
	bool IsTerminated(){return m_terminated;}

	void SetPriority(OS::EThreadPriority p);
	OS::EThreadPriority GetPriority();

	void AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier=0);
	bool RemoveRequest(JobOrder* order);
	bool IsQueueDone();

	int GetOrdersCount();
};

}



#endif


