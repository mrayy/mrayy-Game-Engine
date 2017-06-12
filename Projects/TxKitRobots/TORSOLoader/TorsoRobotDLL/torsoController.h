#ifndef __torsoController__
#define __torsoController__

#include <string>
#include "IRobotController.h"
#include "vectors.h"
#include "RobotCapabilities.h"

class torsoControllerImpl;
class torsoController :public IRobotController
{
protected:

	char cCurrentPath[256];
	double targetRotMat[16];
	torsoControllerImpl* m_impl;
	float robotX, robotY, robotZ;
	int robot_vx, robot_vy, robot_rot;
	float pan, tilt, roll;
	bool baseConnected;

	bool m_connectFlag;
	//bool m_isConnected;

	enum EState
	{
		EInitlize,
		EIdle
	};

	EState m_state;


	ERobotControllerStatus robotState;
	mray::TBee::RobotCapabilities m_caps;

	float m_headPos[3];

	float m_offset[3];

	bool m_calibrated;

	IRobotStatusProvider* m_robotStatusProvider;

	static DWORD WINAPI timerThreadHead(torsoController *robot, LPVOID pdata);
	static DWORD WINAPI timerThreadBase(torsoController *robot, LPVOID pdata);

	std::string ScanePorts();

	int FirstMoving(void);
	int FinishMoving(void);
	int mainRoutine(int CalibSelect);
	int debugRoutine(void);

	void SetZeroPos();

	void _setupCaps();

	void _processData();

public:
	torsoController();
	virtual~torsoController();

	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider);
	void SetListener(ITelubeeRobotListener* l);
	int initRobot(bool debug);
	void ConnectRobot();
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);


	virtual void ShutdownRobot() {}
	virtual bool GetJointValues(std::vector<float>& values){ values.clear(); return false; }
	virtual void ManualControlRobot() {};


	static void ConvertToMatrix(const Quaternion& q, const float* pos, double* mat);


	void _innerProcessRobot();

	ERobotControllerStatus GetRobotStatus();
	//void ShutdownRobot();
	//bool GetJointValues(std::vector<float>& values);
	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return &m_caps; }

};



#endif
