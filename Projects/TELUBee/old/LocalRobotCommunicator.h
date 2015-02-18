

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   15:01
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\LocalRobotCommunicator.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	LocalRobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _LocalRobotCommunicator_
#define _LocalRobotCommunicator_

#include "IRobotCommunicator.h"

namespace mray
{
namespace TBee
{
class LocalRobotCommunicatorData;
class LocalRobotCommunicator:public IRobotCommunicator
{
protected:
	LocalRobotCommunicatorData* m_data;

	void _SendUpdate();
public:
	LocalRobotCommunicator();
	virtual~LocalRobotCommunicator();

	virtual bool Connect(const core::string& ip,int port);
	virtual void Disconnect();
	virtual bool IsConnected();

	virtual void SetUserID(const core::string& userID);
	virtual void ConnectUser(bool c);
	virtual void ConnectRobot(bool c);

	virtual void Update(float dt);

	void LoadFromXml(xml::XMLElement* e);
};

}
}

#endif
