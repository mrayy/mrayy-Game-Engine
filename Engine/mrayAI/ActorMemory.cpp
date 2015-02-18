#include "stdafx.h"

#include "ActorMemory.h"
#include "ITimer.h"
#include "Engine.h"
#include "AIComponentTypes.h"


namespace mray{
namespace AI{

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(ForgetFactor,ActorMemory,uint,mT("ForgetFactor"),EPBT_Basic,mT(""),10000);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(ForgetFactor,ActorMemory,uint,SetForgetFactor,GetForgetFactor,core::StringConverter::toString,core::StringConverter::toUInt,false)


	ActorMemory::ActorMemory():
	m_fogetFactor(PropertyTypeForgetFactor::instance.GetDefaultValue())
{
	CPropertieDictionary*dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeForgetFactor::instance);
	}
}
ActorMemory::~ActorMemory()
{
	MemoryTicketList::iterator it= m_tickets.begin();
	for (;it!= m_tickets.end();++it)
	{
		delete it->second;
	}
}

bool ActorMemory::SetForgetFactor(const uint&v)
{
	m_fogetFactor=v;
	return true;
}
const uint& ActorMemory::GetForgetFactor()
{
	return m_fogetFactor;
}
void ActorMemory::AddVisionTicket(AIActor*actor,const VisionMemTick&ticket)
{
	MemoryTicketList::iterator it= m_tickets.find(actor);
	MemoryTicket*mem=0;
	if(it==m_tickets.end())
	{
		mem=new MemoryTicket(actor);
		m_tickets[actor]=mem;
	}else
		mem=it->second;
	if(!mem->visionTick)
		mem->visionTick=new VisionMemTick(ticket);
	else{
		mem->visionTick->time=ticket.time;
		mem->visionTick->position=ticket.position;
		mem->visionTick->isVisible=ticket.isVisible;
		mem->visionTick->direction=ticket.direction;
	}
	mem->UpdateTime(ticket.time);
}
void ActorMemory::AddSoundTicket(AIActor*actor,const SoundMemTick&ticket)
{
	MemoryTicketList::iterator it= m_tickets.find(actor);
	MemoryTicket*mem=0;
	if(it==m_tickets.end())
	{
		mem=new MemoryTicket(actor);
		m_tickets[actor]=mem;
	}else
		mem=it->second;
	if(!mem->soundTick)
		mem->soundTick=new SoundMemTick(ticket);
	else{
		mem->soundTick->time=ticket.time;
		mem->soundTick->position=ticket.position;
	}
	mem->UpdateTime(ticket.time);
}

MemoryTicket* ActorMemory::GetMemoryTicket(AIActor*actor)
{
	return m_tickets[actor];
}

void ActorMemory::RemoveActorMemory(AIActor*actor)
{

	MemoryTicketList::iterator it= m_tickets.find(actor);
	if(it!=m_tickets.end())
	{
		delete it->second;
		m_tickets.erase(it);
	}
}

MemoryTicketList& ActorMemory::GetMemoryTickets()
{
	return m_tickets;
}


void ActorMemory::Update(float dt)
{
	double time=gEngine.getTimer()->getSeconds();
	MemoryTicketList::iterator it= m_tickets.begin();
	while (it!= m_tickets.end())
	{
		MemoryTicket*tick=it->second;
		bool remove=false;
		if(tick->importance<=0)
			remove=true;
		else if(((time-tick->time)/tick->importance) > m_fogetFactor)
				remove=true;
		if(remove)
		{
			delete tick;
			it=m_tickets.erase(it);
		}else
			++it;
	}
}


}
}
