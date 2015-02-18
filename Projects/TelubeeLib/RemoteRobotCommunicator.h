


/********************************************************************
created:	2013/03/12
created:	12:3:2013   15:01
filename: 	D:\Development\mrayEngine\Projects\TELUBee\RemoteRobotCommunicator.h
file path:	D:\Development\mrayEngine\Projects\TELUBee
file base:	RemoteRobotCommunicator
file ext:	h
author:		MHD Yamen Saraiji

purpose:
*********************************************************************/
#ifndef _RemoteRobotCommunicator_
#define _RemoteRobotCommunicator_

#include "IRobotCommunicator.h"

namespace mray
{
namespace TBee
{
class RemoteRobotCommunicatorData;
class RemoteRobotCommunicator :public IRobotCommunicator
{
protected:
	RemoteRobotCommunicatorData* m_data;

	void _SendUpdate();
public:
	RemoteRobotCommunicator();
	virtual~RemoteRobotCommunicator();


	virtual bool Connect(const core::string& ip, int port);
	virtual void Disconnect();
	virtual bool IsConnected();

	virtual void SetUserID(const core::string& userID);
	virtual void ConnectUser(bool c);
	virtual void ConnectRobot(bool c);

	virtual void Update(float dt);

	void SetData(const core::string &key, const core::string &value,bool statusData);
	void RemoveData(const core::string &key);
	void ClearData(bool statusValues);

	void LoadFromXml(xml::XMLElement* e);
};

}
}

#endif
