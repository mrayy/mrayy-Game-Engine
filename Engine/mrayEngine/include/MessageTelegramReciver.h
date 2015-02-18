
/********************************************************************
	created:	2010/03/27
	created:	27:3:2010   19:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MessageTelegramReciver.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MessageTelegramReciver
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MessageTelegramReciver___
#define ___MessageTelegramReciver___

#include "MessageTelegram.h"

namespace mray{

template <class Entity,class MSG>
class MessageTelegramReciver
{
private:
	typedef MessageTelegram<Entity,MSG> TMessageTelegram;
protected:
public:

	MessageTelegramReciver(){}
	virtual~MessageTelegramReciver(){}

	virtual bool OnMessageTelegram(const TMessageTelegram&msg)=0;
};

}


#endif //___MessageTelegramReciver___

