

/********************************************************************
	created:	2009/06/19
	created:	19:6:2009   14:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EventTimer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EventTimer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EventTimer___
#define ___EventTimer___

#include "IDelegate.h"

namespace mray{

class MRAY_DLL EventTimer
{
private:
protected:
	bool m_enabled;
	float m_interval;

	float m_timeToFire;
public:
	typedef EventTimer* PTimer;

	DelegateEvent1<PTimer> onTick;

	EventTimer();
	virtual~EventTimer();

	void removeTimer();

	void setEnabled(const bool&v);
	const bool& getEnabled(){return m_enabled;}

	void setInterval(const float&v);
	const float& getInterval(){return m_interval;}

	void update(float dt);
};


}


#endif //___EventTimer___
