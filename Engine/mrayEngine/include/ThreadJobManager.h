


/********************************************************************
	created:	2011/02/06
	created:	6:2:2011   20:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ThreadJobManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ThreadJobManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ThreadJobManager_h__
#define ThreadJobManager_h__


#include "JobOrderPieceCollection.h"
#include "GCPtr.h"

namespace mray
{
	class IJobPiece;
	namespace OS
	{
		class ThreadBarrier;
	}

class MRAY_DLL ThreadJobManager
{
private:
protected:
	std::vector<GCPtr<JobOrderPieceCollection>> m_jobs;
	int m_currStack;
	int m_usedThreadsCount;

	bool m_isMT;

	OS::ThreadBarrier* m_barrier;
	JobLoadCompleteDelegate m_jobCompleted;
	void OnJobCompleted(JobOrder*const &o,const bool&s);
public:
	ThreadJobManager();
	virtual ~ThreadJobManager();
	
	void SetMultiThreaded(bool mt);
	bool IsMultiThreaded();

	void Reset();

	void AddPiece(IJobPiece* p);

	void BeginProcessing();
	void EndProcessing();
};

}


#endif // ThreadJobManager_h__
