

/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   16:01
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\EventBaseJobQueue.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	EventBaseJobQueue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __EventBaseJobQueue__
#define __EventBaseJobQueue__

#include "IJobQueue.h"

namespace mray
{
	namespace OS
	{
		class IThreadEvent;
	}

	class EventBasedJobQueueThreadFunc;
class EventBaseJobQueue:public IJobQueue
{
private:
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
	OS::IThreadEvent* m_newRequest;

	typedef std::list<LoadRequest*> RequeseList;
	RequeseList m_requests;
	RequeseList m_graveyard;
	friend class EventBasedJobQueueThreadFunc;

	void _addToGraveYard(RequeseList::iterator it);
	LoadRequest* _createRequest();
public:
	EventBaseJobQueue(uint id);
	virtual~EventBaseJobQueue();

	void SetPriority(OS::EThreadPriority p);
	OS::EThreadPriority GetPriority();

	void AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier=0);
	bool IsQueueDone();

	int GetOrdersCount();
};


}

#endif
