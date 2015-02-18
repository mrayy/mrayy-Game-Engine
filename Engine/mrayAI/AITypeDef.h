
/********************************************************************
	created:	2010/03/27
	created:	27:3:2010   19:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AITypeDef.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AITypeDef
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AITypeDef___
#define ___AITypeDef___

#include <MessageTelegramReciver.h>
#include <MessageTelegram.h>

namespace mray{
namespace AI{


	class AIActor;

	typedef MessageTelegramReciver<AIActor*,uint> AIMessageReciver;
	typedef MessageTelegram<AIActor*,uint> AIMessageTelegram;

}
}

#endif //___AITypeDef___
