

/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   15:57
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IJobQueue.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IJobQueue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IJobQueue__
#define __IJobQueue__

#include "JobOrder.h"

namespace mray
{
	namespace OS
	{
		class ThreadBarrier;
		class IThreadFunction;
		enum EThreadPriority ;
	}

class IJobQueue
{
private:
protected:
	uint m_id;
public:
	IJobQueue(uint id):m_id(id){}
	virtual~IJobQueue(){}

	void SetID(uint id){m_id=id;}
	uint GetID(){return m_id;}

	virtual void SetPriority(OS::EThreadPriority p)=0;
	virtual OS::EThreadPriority GetPriority()=0;

	virtual void AddRequest(const JobOrderPtr& order,JobLoadCompleteDelegate listner,OS::ThreadBarrier*barrier=0)=0;
	virtual bool RemoveRequest(JobOrder* order)=0;
	virtual bool IsQueueDone()=0;

	virtual int GetOrdersCount()=0;
};


}

#endif
