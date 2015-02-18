
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   21:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraWorker.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraWorker
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraWorker___
#define ___TheoraWorker___

#include <JobOrder.h>

namespace mray{
namespace video{

	class TheoraVideo;

class TheoraWorker:public JobOrder
{
private:
protected:
	TheoraVideo* m_video;
public:
	TheoraWorker();
	virtual~TheoraWorker();
	
	void SetVideo(TheoraVideo*v)
	{
		m_video=v;
	}
	TheoraVideo*GetVideo()
	{
		return m_video;
	}

	virtual bool ExecuteJob();
};

}
}

#endif //___TheoraWorker___
