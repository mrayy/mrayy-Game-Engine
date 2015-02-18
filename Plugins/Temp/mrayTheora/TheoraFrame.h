
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   22:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraFrame.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraFrame
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraFrame___
#define ___TheoraFrame___

#include <mTypes.h>

namespace mray{
namespace video{

	class TheoraVideo;
	class TheoraFrameHelper;

class TheoraFrame
{
private:
protected:
	TheoraVideo* m_owner;
	uchar* m_buffer;

	friend class TheoraFrameHelper;
public:
	float timeToDisplay;
	bool ready;
	bool inUse;
public:

	TheoraFrame(TheoraVideo* owner);
	virtual~TheoraFrame();

	//! do not call directly, this function is used to reset back color after video clip output mode change
	void fillBackColor(uint c);

	uchar* getBuffer();

	//data is : th_ycbr_buffer
	void decodeFrame(void* data);

};

}
}


#endif //___TheoraFrame___
