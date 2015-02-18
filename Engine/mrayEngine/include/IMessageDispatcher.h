
/********************************************************************
	created:	2010/03/28
	created:	28:3:2010   20:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IMessageDispatcher.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IMessageDispatcher
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IMessageDispatcher___
#define ___IMessageDispatcher___

#include "MessageTelegram.h"
#include "Engine.h"
#include "ITimer.h"
#include <set>

namespace mray{


template <class Entity,class MSG>
class IMessageDispatcher
{
private:
	typedef MessageTelegram<Entity,MSG> TMessageTelegram;
	typedef std::set<TMessageTelegram> TelegramsList;

	TelegramsList m_telegrams;
protected:
	virtual void SendTelegram(const TMessageTelegram& msg)=0;
public:
	IMessageDispatcher(){
	}
	virtual~IMessageDispatcher(){
		m_telegrams.clear();
	}

	void DispatchMessage(Entity src,Entity dst,uint group,MSG message,double delay,void*data=0)
	{
		TMessageTelegram msgTele(message,src,dst,group,0,data);

		if(delay<=0)
		{
			SendTelegram(msgTele);
		}else{
			double currentTime=gEngine.getTimer()->getSeconds();
			msgTele.dispatchTime=currentTime+delay;
			m_telegrams.insert(msgTele);
		}
	}

	void ProcessMessages()
	{
		double currentTime=gEngine.getTimer()->getSeconds();

		while (!m_telegrams.empty())
		{
			const TMessageTelegram& msg=*m_telegrams.begin();
			if(msg.dispatchTime>currentTime)
				break;
			SendTelegram(msg);
			m_telegrams.erase(m_telegrams.begin());
		}
	}
};

}


#endif //___IMessageDispatcher___
