
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   21:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraManager___
#define ___TheoraManager___

#include "IVideoSource.h"
#include "TheoraWorker.h"
#include "TheoraCommon.h"
//#include "TheoraVideo.h"

#include <GCPtr.h>
#include <IThread.h>
#include <ISingleton.h>

#include <list>

namespace mray{
namespace sound{
	class ISoundManager;
}
namespace video{

	class TheoraManagerBGWorker;
class TheoraManager:public ISingleton<TheoraManager>
{
private:
protected:

	friend class TheoraWorker;

	sound::ISoundManager* m_soundManager;

	typedef std::list<IVideoSourcePtr> VideoList;
	VideoList m_videos;

	GCPtr<TheoraCommon> m_theoraCommon;
	std::vector<GCPtr<TheoraWorker>> m_workersPool;

	GCPtr<OS::IThread> m_backgroundThread;
	TheoraManagerBGWorker* m_workerFunc;

	GCPtr<OS::IMutex> m_workerMutex;
protected:
	void initWorkers(int cnt);
	IVideoSource* requestJob();

	TheoraWorker* RequestWorker();
public:
	TheoraManager(int workersCount=1);
	virtual~TheoraManager();

	IVideoSourcePtr CreateVideo();
	void RemoveVideo(IVideoSource* v);

	void setSoundManager(sound::ISoundManager*m){m_soundManager=m;}
	sound::ISoundManager* getSoundManager(){return m_soundManager;}

	void _Update();
};

}
}


#endif //___TheoraManager___
