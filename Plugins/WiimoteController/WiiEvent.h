

/********************************************************************
	created:	2011/01/28
	created:	28:1:2011   23:03
	filename: 	i:\Programing\GameEngine\mrayEngine\Controllers\WiimoteController\WiiEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\Controllers\WiimoteController
	file base:	WiiEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WiiEvent_h__
#define WiiEvent_h__

#include "Event.h"

namespace mray
{
	enum EWiiEventType
	{
		EWiiEvent_Unkown,

		EWiiEvent_Connect,
		EWiiEvent_Disconnect,

		EWiiEvent_ReadData,

		EWiiEvent_ButtonChanged,
		EWiiEvent_AccelChanged,
		EWiiEvent_OrintationChanged,
		EWiiEvent_IRChanged,
		
		EWiiEvent_NunchukInserted,

		EWiiEvent_BalanceConnected,
		EWiiEvent_BalanceWeightChanged,

		EWiiEvent_BatteryChanged,
		EWiiEvent_BatteryDrained,


	};
	namespace controllers
	{
		class CWiimote;
	}

class WiiEvent:public Event
{
private:
protected:
public:

	static const GUID EventID;

	WiiEvent();
	virtual ~WiiEvent();

	EWiiEventType event;

	controllers::CWiimote* mote;
	
};

}
#endif // WiiEvent_h__
