#include "stdafx.h"

#include "MessageDispatcher.h"
#include "ITimer.h"
#include "AIActor.h"

namespace mray{
namespace AI{

MessageDispatcher::MessageDispatcher()
{
}
MessageDispatcher::~MessageDispatcher()
{
}

void MessageDispatcher::SendTelegram(const AIMessageTelegram& msg)
{
	//TODO: process broadcast case and send to group case
	if(!msg.getTarget()==EMTT_Single)return;

	msg.receiver->OnMessageTelegram(msg);

}
}
}