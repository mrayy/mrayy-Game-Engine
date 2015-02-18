
/********************************************************************
	created:	2010/03/19
	created:	19:3:2010   22:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ActorMemory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ActorMemory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ActorMemory___
#define ___ActorMemory___

#include "MemoryTicket.h"
#include "IAIComponent.h"
#include <map>

namespace mray{
namespace AI{

	typedef std::map<AIActor*,MemoryTicket*> MemoryTicketList;
class MRAY_AI_DLL ActorMemory:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	MemoryTicketList m_tickets;
	double m_fogetFactor;//how long this agent well keep track of an actor in his memory
	
	DECLARE_PROPERTY_TYPE(ForgetFactor,uint,MRAY_AI_DLL)
public:
	//rwProperty<ActorMemory,uint> *ForgetFactor;

	ActorMemory();
	virtual~ActorMemory();

	bool SetForgetFactor(const uint&v);
	const uint& GetForgetFactor();

	//add/modify vision information about an actor
	void AddVisionTicket(AIActor*actor,const VisionMemTick&ticket);

	//add/modify sound information about an actor
	void AddSoundTicket(AIActor*actor,const SoundMemTick&ticket);

	//get memory information about an actor
	MemoryTicket* GetMemoryTicket(AIActor*actor);

	//remove memory information about an actor
	void RemoveActorMemory(AIActor*actor);

	MemoryTicketList& GetMemoryTickets();

	void Update(float dt);

};

}
}


#endif //___ActorMemory___
