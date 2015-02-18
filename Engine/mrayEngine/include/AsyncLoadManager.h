


/********************************************************************
	created:	2011/10/19
	created:	19:10:2011   15:05
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\AsyncLoadManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	AsyncLoadManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __AsyncLoadManager__
#define __AsyncLoadManager__

#include "ISingleton.h"
#include "JobOrder.h"


namespace mray
{
	namespace OS
	{
		class IMutex;
	}

	enum EAsyncLoadPriority
	{
		EALP_High=0,
		EALP_Medium=5,
		EALP_Low=9,
		EALP_Count
	};

	class IAsyncLoadRequest;
	class AsyncLoadManagerJobOrder;

class MRAY_DLL AsyncLoadManager:public ISingleton<AsyncLoadManager>
{
public:
	typedef std::list<IAsyncLoadRequest*> RequestQueue;

	class AsyncLoadManagerAccessor
	{
	protected:
		AsyncLoadManager* m_manager;
		OS::IMutex* m_mutex;
		friend class AsyncLoadManager;

	public:
		AsyncLoadManagerAccessor();
		~AsyncLoadManagerAccessor();

		void AddRequest(IAsyncLoadRequest*request,EAsyncLoadPriority priority);
		IAsyncLoadRequest* PopRequest();
		uint GetQueueSize(EAsyncLoadPriority q);
	};

private:
protected:
	RequestQueue m_requestQueues[EALP_Count];
	AsyncLoadManagerAccessor m_accessor;

	friend class AsyncLoadManagerAccessor;
	friend class AsyncLoadManagerJobOrder;

	JobOrderPtr m_loadingJob;

	bool m_jobInUse;

	JobLoadCompleteDelegate m_loadCompleted;

	AsyncLoadManagerAccessor* GetAccessor();

	void LoadingComplete(JobOrder*,bool);
public:
	AsyncLoadManager();
	virtual~AsyncLoadManager();


	void AddRequest(IAsyncLoadRequest*request,EAsyncLoadPriority priority);

	uint GetRequestQueueSize(EAsyncLoadPriority priority);
};


}

#endif

