

/********************************************************************
	created:	2013/12/04
	created:	4:12:2013   20:23
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\CRobotConnector.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	CRobotConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CRobotConnector__
#define __CRobotConnector__




namespace mray
{
	namespace TBee
	{
		class IRobotCommunicator;
		class IHeadController;
		class IInputController;

class CRobotConnector
{
protected:
	core::string m_robotIP;
	TBee::IRobotCommunicator* m_communicator;
	bool m_connected;
	bool m_status;

	IHeadController* m_headController;
	IInputController* m_robotController;
	math::vector2d m_speed;
	math::quaternion m_headRotation;
	math::vector3d m_headPosition;
	float m_rotation;
	int m_videoPort;
	int m_audioPort;
	bool m_rtcp;
	int m_commPort;
public:
	CRobotConnector();
	~CRobotConnector();

	void SetHeadController(IHeadController* c){ m_headController = c; }
	IHeadController* GetHeadController(){ return m_headController; }

	void SetRobotController(IInputController* c){ m_robotController = c; }
	IInputController* GetRobotController(){ return m_robotController; }

	void SetCommunicator(IRobotCommunicator* comm){ m_communicator = comm; }

	bool IsRobotConnected();
	void ConnectRobot();
	void ConnectRobotIP(const core::string& ip, int videport, int audioPort, int commPort, bool rtcp);
	void DisconnectRobot();
	void StartUpdate();
	void EndUpdate();
	void LoadXML(xml::XMLElement* e);

	void SetData(const core::string& key, const core::string& val, bool status);
	void RemoveData(const core::string& key);

	const math::vector2d& GetSpeed(){ return m_speed; }
	const math::quaternion& GetHeadRotation(){ return m_headRotation; }
	const math::vector3d& GetHeadPosition(){ return m_headPosition; }
	float GetRotation(){ return m_rotation; }

	void HandleController();
	void UpdateStatus();

};
	}

}


#endif
