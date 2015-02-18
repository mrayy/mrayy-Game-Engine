

#ifndef ___GCCollecter___
#define ___GCCollecter___




#include "IGCCollector.h"

#include "CMutexVar.h"
#include "GCInfo.h"

namespace mray{
	namespace OS
	{
		class IThread;
	}

	template<class T>
	class GCPtr;


typedef std::map<const void*,GCInfo*> TMemoryList;
typedef std::map<const void*,GCInfo*>::iterator TMemoryListIT;

typedef std::list<IGCCollector::IInfoTemplate*> TDeadMemoryList;
typedef std::list<IGCCollector::IInfoTemplate*>::iterator TDeadMemoryListIT;

class MRAY_DLL GCCollector:public IGCCollector
{
protected:
	OS::CMutexVar<TMemoryList> m_memList;
	OS::CMutexVar<TDeadMemoryList> m_deadList;
	OS::IThread* m_thread;

	bool m_multiThreaded;

	void startupMT();


	GCInfo* _getInfo(const void* addr);
	void _addInfo(GCInfo*info);

	virtual void inner_eraseAddress(const void*addr)
	{
		m_memList.lock();
		m_memList().erase(addr);
		m_memList.unlock();
	}
	virtual void inner_addToDeadList(IInfoTemplate*info)
	{
		m_deadList.lock();
		m_deadList().push_back(info);
		m_deadList.unlock();
	}
public:

	GCCollector();
	~GCCollector();
	static void shutdown();

	virtual bool isMultiThreaded()
	{
		return m_multiThreaded;
	}
	virtual void SetMultiThreaded(bool mt);

	virtual void SetThread(OS::IThread*t);
	virtual OS::IThread* GetThread();
	virtual void SetMutex(OS::IMutex*memMtx,OS::IMutex*deadMtx);


	bool collect();

	int activeSize();
	int deadSize();
};



}



#endif


