

/********************************************************************
	created:	2014/02/28
	created:	28:2:2014   4:58
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\FlyingTelubeeRobotCommunicator.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	FlyingTelubeeRobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FlyingTelubeeRobotCommunicator__
#define __FlyingTelubeeRobotCommunicator__


#include "IRobotCommunicator.h"

namespace mray
{
namespace TBee
{

	class FlyingTelubeeRobotCommunicatorData;
class FlyingTelubeeRobotCommunicator :public IRobotCommunicator
{
protected:
	FlyingTelubeeRobotCommunicatorData* m_data;

public:
	FlyingTelubeeRobotCommunicator();
	virtual~FlyingTelubeeRobotCommunicator();


	virtual bool Connect(const core::string& ip, int port);
	virtual void Disconnect();
	virtual bool IsConnected();

	virtual void SetUserID(const core::string& userID);
	virtual void ConnectUser(bool c);
	virtual void ConnectRobot(bool c);

	virtual void Update(float dt);

	void SetData(const core::string &key, const core::string &value, bool statusData);
	void RemoveData(const core::string &key);
	void ClearData(bool statusValues);

	void LoadFromXml(xml::XMLElement* e);
};

}
}


#endif
