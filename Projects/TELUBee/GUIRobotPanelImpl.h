

/********************************************************************
	created:	2013/02/07
	created:	7:2:2013   17:55
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GUIRobotPanelImpl.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GUIRobotPanelImpl
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIRobotPanelImpl__
#define __GUIRobotPanelImpl__


#include "GUIRobotPanel.h"
#include "TBRobotInfo.h"

namespace mray
{
namespace GUI
{

class GUIRobotPanelImpl:public GUIRobotPanel
{
protected:
	TBee::TBRobotInfo m_ifo;
	float m_alpha;
	bool m_visible;

public:
	GUIRobotPanelImpl(){m_alpha=0;m_visible=false;}
	virtual~GUIRobotPanelImpl(){}


	void Update(float dt);
	void SetRobotInfo(const TBee::TBRobotInfo& ifo);
	void Show();
	void Hide();
};

}
}

#endif
