#include "stdafx.h"

#include "TheoraManager.h"
#include "JobPool.h"
#include <IThreadManager.h>
#include "TheoraVideo.h"
#include "CFPS.h"


namespace mray{
namespace video{

	class TheoraManagerBGWorker:public OS::IThreadFunction
	{
		TheoraManager* m_owner;
		bool m_closed;
	public:
		TheoraManagerBGWorker(TheoraManager*o)
		{
			m_closed=false;
			m_owner=o;
		}
		virtual void setup()
		{
		}
		bool closed(){return m_closed;}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			m_closed=false;
			while(caller->isActive())
			{
				m_owner->_Update();
				caller->sleep(10);
			}
			m_closed=true;
		}
	};

TheoraManager::TheoraManager(int workers){

	m_soundManager=0;
	m_workerMutex=OS::IThreadManager::getInstance().createMutex(); 
	m_theoraCommon=MRAY_new TheoraCommon();

	m_workerFunc=MRAY_new TheoraManagerBGWorker(this);

	m_backgroundThread=OS::IThreadManager::getInstance().createThread(m_workerFunc);
	m_backgroundThread->start(0);
}
TheoraManager::~TheoraManager()
{
	OS::IThreadManager::getInstance().killThread(m_backgroundThread);
	while(!m_workerFunc->closed())
		OS::IThreadManager::getInstance().sleep(10);

	delete m_workerFunc;

}


IVideoSourcePtr TheoraManager::CreateVideo()
{
	GCPtr<TheoraVideo> ret=new TheoraVideo(5);
	m_workerMutex->lock();
	m_videos.push_back(ret);
	m_workerMutex->unlock();
	return ret;
}
void TheoraManager::RemoveVideo(IVideoSource* v)
{
	VideoList::iterator it;
	m_workerMutex->lock();
	for(it=m_videos.begin();it!=m_videos.end();++it)
	{
		if((*it).pointer()==v)
		{
			m_videos.erase(it);
			break;
		}
	}
	m_workerMutex->unlock();
}

IVideoSource* TheoraManager::requestJob()
{

	IVideoSource*v=0;
	uint i=0;
	VideoList::iterator it;
	m_workerMutex->lock();
	if(!m_videos.size())
		v=0;
	else{
		int p,lp=-1;
		for(it=m_videos.begin();it!=m_videos.end();++it){
			TheoraVideo* vdo=(TheoraVideo*)(*it).pointer();
			p=vdo->getPriority();
			if(p>lp && !vdo->m_assignedWorker){
				lp=p;
				v=vdo;
			}
		}
	}
	

	m_workerMutex->unlock();
	return v;
}

TheoraWorker* TheoraManager::RequestWorker()
{
	for(int i=0;i<m_workersPool.size();++i)
	{
		TheoraWorker* worker=m_workersPool[i];
		if(!worker->GetVideo())
		{
			return worker;
		}
	}
	TheoraWorker* w=new TheoraWorker();
	m_workersPool.push_back(w);
	return w;
}
void TheoraManager::_Update()
{
	float dt=gFPS.dt();
	IVideoSource*v=0;
	uint i=0;
	VideoList::iterator it;
		int p,lp=-1;
	for(it=m_videos.begin();it!=m_videos.end();++it){
		TheoraVideo* vdo=(TheoraVideo*)(*it).pointer();
		if(vdo)
		{
			if(vdo->m_seekPos>=0)
				vdo->doSeek();
			vdo->decodeNextFrame();
			vdo->decodedAudioCheck();

		}
	}

/*
	TheoraVideo* j=(TheoraVideo*)requestJob();
	if(j)
	{
		TheoraWorker* w=RequestWorker();
		w->SetVideo(j);
		j->m_assignedWorker=w;
		w->ExecuteJob();
		//JobPool::getInstance().AddRequest(w,0);
	}
*/
}

}
}

