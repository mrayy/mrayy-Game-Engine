

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   14:39
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\IRobotCommunicator.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	IRobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _IRobotCommunicator_
#define _IRobotCommunicator_

#include "mstring.h"

namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace TBee
{


class IRobotCommunicator
{
protected:
public:
	IRobotCommunicator(){}
	virtual~IRobotCommunicator(){}

	virtual bool Connect(const core::string& ip,int port)=0;
	virtual void Disconnect()=0;
	virtual bool IsConnected()=0;

	virtual void SetUserID(const core::string& userID)=0;
	virtual void ConnectUser(bool c)=0;
	virtual void ConnectRobot(bool c)=0;

	virtual void Update(float dt)=0;

	virtual core::string GetData(const core::string& key)=0;

	virtual void SetData(const core::string &key, const core::string &value, bool statusData) = 0;
	virtual void RemoveData(const core::string &key) = 0;
	virtual void ClearData(bool statusValues)=0;

	virtual void LoadFromXml(xml::XMLElement* e)=0;
};


}
}

#endif
