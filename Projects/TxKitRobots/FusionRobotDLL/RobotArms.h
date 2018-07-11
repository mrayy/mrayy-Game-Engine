
#ifndef __RobotArms__
#define __RobotArms__

#include "serial/serial.h"
#include <windows.h>

namespace mray
{

class RobotArms 
{
public:
	enum TargetArm
	{
		Left,
		Right
	};
	enum EState
	{
		Wait,
		Initialize,
		Initializing,
		Operate,
		Shutdown,
		Shutingdown
	};
	struct JoinInfo
	{
		JoinInfo()
		{
			targetAngle = 0;
			currAngle = 0;
			temp = 0;
		}
		float targetAngle;
		float currAngle;
		float temp;
	};
protected:




	JoinInfo _leftArm[7];
	JoinInfo _rightArm[7];
	JoinInfo _leftHand[5];
	JoinInfo _rightHand[5];

	EState _state ;

	int JointsCount;

	serial::Serial* m_serial;
	bool connected;
	HANDLE m_robotThread;

	std::list<byte> _buffer;

	byte _readByte();

	byte cmd[256];
	byte data[256];

	bool LArmEnabled ;
	bool RArmEnabled ;

	bool _enableSending ;
	bool _enableReading ;
	bool _enableTemperature;

	float TemperatureTime ;
	float _temperatureTime ;
	ushort timeMS;
	float _timer;

	bool _off;

	bool *RSigns;
	bool *LSigns;

	float *LMidPos;
	float *RMidPos;
	
	float *LShutdownPos;
	float *RShutdownPos;

	ushort BatteryLevel;

	static float NormalizeAngle(float v)
	{
		while (v > 180)
			v -= 360.0f;
		while (v < -180)
			v += 360.0f;
		return v;
	}

	float _GetValue(TargetArm arm, int id);
	void _SetValue(TargetArm arm, int id, float val);
	void _SetTemp(TargetArm arm, int id, float temp);


	int ReadData(byte*data);
	bool _sendCommand(byte* cmd, int length, bool immediate=true);


	void _On();
	void _Off();
	void _Close();


	short AngleToServo(float angle, bool reverse);
	float ServoToAngle(short val, bool reverse);
	void _UpdateJoints(TargetArm arm, ushort time, bool midPos = false);
	void _readJoints(TargetArm arm);
	void _readTemperature(TargetArm arm);
	void _readBattery();

	void _updateHand(TargetArm arm);

	void ProcessState();
	void ProcessThread();


	static DWORD timerThreadRobot(RobotArms *robot, LPVOID pdata);
public:
	RobotArms();
	virtual ~RobotArms();

	virtual bool Connect(const core::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	void SetArmAngles(TargetArm arm, float *angles,int n);
	void SetHand(TargetArm, float* angles, int n);

	void Start(bool leftArm,bool rightArm);
	void Stop(bool force=false);

	void Update(float dt);

	float GetBatteryLevel() { return BatteryLevel*0.001f; }

	JoinInfo* GetLeftArm() { return _leftArm; }
	JoinInfo* GetRightArm() { return _rightArm; }
	JoinInfo* GetLeftHand() { return _leftHand; }
	JoinInfo* GetRightHand() { return _rightHand; }

	EState GetStatus() { return _state; }
};

}


#endif
