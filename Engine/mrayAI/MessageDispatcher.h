
/********************************************************************
	created:	2010/03/27
	created:	27:3:2010   15:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\MessageDispatcher.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	MessageDispatcher
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MessageDispatcher___
#define ___MessageDispatcher___

#include "AITypeDef.h"
#include <IMessageDispatcher.h>

namespace mray{
namespace AI{


class MessageDispatcher:public IMessageDispatcher<AIActor*,uint>
{
private:
protected:
	typedef std::set<AIMessageTelegram> TelegramsList;

	TelegramsList m_telegrams;

	void SendTelegram(const AIMessageTelegram& msg);
public:
	MessageDispatcher();
	virtual~MessageDispatcher();
};

}
}


#endif //___MessageDispatcher___
