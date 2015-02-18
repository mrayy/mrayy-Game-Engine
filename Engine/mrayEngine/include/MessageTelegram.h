
/********************************************************************
	created:	2009/07/17
	created:	17:7:2009   12:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MessageTelegram.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MessageTelegram
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	general purpose message telegram class
*********************************************************************/

#ifndef ___MessageTelegram___
#define ___MessageTelegram___

#include <math.h>
#include "mTypes.h"
#include "CompileConfig.h"

namespace mray{

enum EMessageTelegramTarget
{
	EMTT_Single,
	EMTT_Group,
	EMTT_Broadcast
};

const double MessageTelegramDelayEpsilon=0.25;
template <class Entity,class MSG>
class MessageTelegram
{
private:
protected:
public:

	Entity sender;	//who send the msg
	Entity receiver;	//who should receive the msg

	uint group;		//group ID

	MSG message;	//msg ID

	double dispatchTime;	//when to the dispatch the msg,it can be delayed for certain time

	void* extraData;		//pointer to additional data that the msg might have

	MessageTelegram():sender(0),receiver(0),message(0),dispatchTime(0),group(0),extraData(0)
	{
	}
	MessageTelegram(MSG msg,const Entity& sdr,const Entity& rcv,uint grp,double time,void*ud=0):
		sender(sdr),receiver(rcv),message(msg),dispatchTime(time),group(grp),extraData(ud)
	{
	}
	virtual~MessageTelegram(){
	}

	EMessageTelegramTarget getTarget()const{
		if(group!=0)
			return EMTT_Group;
		if(receiver!=0)
			return EMTT_Single;
		return EMTT_Broadcast;
	}

	inline bool operator ==(const MessageTelegram&t)const
	{
		if( message==t.message && group==t.group &&
			sender==t.sender && receiver==t.receiver && 
			fabs(dispatchTime-t.dispatchTime)<MessageTelegramDelayEpsilon)
			return true;
		return false;
	}

	inline bool operator <(const MessageTelegram&t)const
	{
		return dispatchTime-t.dispatchTime<-MessageTelegramDelayEpsilon;
	}


	template <class T>
	inline T CastData()
	{
		return *(T*)extraData;
	}
};

}


#endif //___MessageTelegram___
