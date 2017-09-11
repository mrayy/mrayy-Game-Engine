

/********************************************************************
	created:	2014/02/22
	created:	22:2:2014   18:02
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\GStreamerCore.h
	file path:	C:\Development\mrayEngine\Plugins\mrayGStreamer
	file base:	GStreamerCore
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GStreamerCore__
#define __GStreamerCore__

#include "IThread.h"
#include "ITimer.h"


namespace mray
{
namespace video
{

class GStreamerCore
{
protected:
	static GStreamerCore* m_instance;
	static uint m_refCount;

	OS::IThread* m_mainLoopThread;
	OS::IThreadFunction* m_threadFunc;

	OS::ITimer *m_timer;

	GStreamerCore();


	void _Init();

	void _StartLoop();
	void _StopLoop();
public:

	virtual~GStreamerCore();

	OS::ITimer* GetTimer(){ return m_timer; }

	static void Ref();
	static void Unref();
	static uint RefCount(){ return m_refCount; }

	static GStreamerCore* Instance();
};

#define gGStreamerCore mray::video::GStreamerCore::Instance()

}
}


#endif
