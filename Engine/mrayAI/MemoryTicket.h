
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   16:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\MemoryTicket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	MemoryTicket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	holds 
*********************************************************************/

#ifndef ___MemoryTicket___
#define ___MemoryTicket___

#include "VisionMemTick.h"
#include "SoundMemTick.h"

namespace mray{
namespace AI{

	class AIActor;

class MemoryTicket
{
private:
protected:
public:
	MemoryTicket(AIActor* a):actor(a),visionTick(0),soundTick(0),importance(1),time(0)
	{
	}
	virtual~MemoryTicket()
	{
		if(visionTick)
			delete visionTick;
		if(soundTick)
			delete soundTick;
	}

	double time;	//last "updated" time for this memory ticket
	void UpdateTime(uint t){
		time=time>t? time:t ;
	}

	AIActor* actor;
	VisionMemTick *visionTick;
	SoundMemTick *soundTick;

	float importance;	//how much this mem ticket is important to keep it or not (factor variable)
						//importance >1,this is an important ticket
						//importance <1,this is not important ticket

};

}
}

#endif //___MemoryTicket___
