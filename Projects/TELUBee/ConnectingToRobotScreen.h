
/********************************************************************
	created:	2013/02/07
	created:	7:2:2013   17:11
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\ConnectingToRobotScreen.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	ConnectingToRobotScreen
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ConnectingToRobotScreen__
#define __ConnectingToRobotScreen__

#include "IRenderingState.h"
#include "GUIConnectingToRobot.h"
#include "IUDPClient.h"

class UDPClient;


namespace mray
{
namespace TBee
{
#define ToRobotVision_Code 0x300
#define BackToMap_Code 0x301
	class IRobotCommunicator;
	class DVISwitcherController;
class ConnectingToRobotScreen:public IRenderingState
{
protected:
	enum EStatus
	{
		EInitializing,
		EConnectingToRobot,
		EConnected,
		EEstablished
	}m_status;
	GUI::IGUIManager* m_guiManager;
	GUIConnectingToRobot* m_connectingPnl;
	GUI::IGUIPanelElement* m_guiroot;
	IRobotCommunicator* m_roboComm;
	DVISwitcherController* m_dviSwitcher;
	float m_time;
	float m_targetTime;

	void _Connect();
	void _Disconnect();
public:
	ConnectingToRobotScreen();
	virtual~ConnectingToRobotScreen();

	virtual void InitState();

	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);
	virtual void Update(float dt);
};

}
}


#endif

