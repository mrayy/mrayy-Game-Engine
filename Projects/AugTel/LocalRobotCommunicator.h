

/********************************************************************
	created:	2014/02/25
	created:	25:2:2014   16:36
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\LocalRobotCommunicator.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	LocalRobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LocalRobotCommunicator__
#define __LocalRobotCommunicator__


#include "IRobotCommunicator.h"

namespace mray
{
namespace TBee
{

	class LocalRobotCommunicatorImpl;
class LocalRobotCommunicator:public IRobotCommunicator
{
protected:
	LocalRobotCommunicatorImpl *m_impl;
public:
	LocalRobotCommunicator();
	virtual~LocalRobotCommunicator();


	virtual bool Connect(const core::string& ip, int port) ;
	virtual void Disconnect() ;
	virtual bool IsConnected() ;

	virtual void SetUserID(const core::string& userID) ;
	virtual void ConnectUser(bool c) ;
	virtual void ConnectRobot(bool c) ;

	virtual void Update(float dt);

	virtual core::string GetData(const core::string& key) { return ""; }


	void SetData(const core::string &key, const core::string &value, bool statusData);
	void RemoveData(const core::string &key);
	void ClearData(bool statusValues);

	virtual void LoadFromXml(xml::XMLElement* e) ;
};

}
}

#endif
