
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   22:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraFrameQueue.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraFrameQueue
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraFrameQueue___
#define ___TheoraFrameQueue___

#include "TheoraFrame.h"
#include <IMutex.h>
#include <GCPtr.h>

#include <list>

namespace mray{
namespace video{

	class TheoraVideo;

class TheoraFrameQueue
{
private:
protected:
	typedef std::list<GCPtr<TheoraFrame>> FramesList;
	FramesList m_frames;

	TheoraVideo* m_owner;
	uint m_backColor;

	GCPtr<OS::IMutex> m_mutex;
public:
	TheoraFrameQueue(TheoraVideo*v,int frames);
	virtual~TheoraFrameQueue();

	GCPtr<TheoraFrame> requestFirstAvaliableFrame();
	GCPtr<TheoraFrame> requestAnyFreeFrame();

	void setFillColor(uint c);
	uint getFillColor();
	uint getFramesCount();

	void pop();
	void clear();

	void setSize(int n);
	int getSize();

	int getInUseCount();
};

}
}


#endif //___TheoraFrameQueue___
