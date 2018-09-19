
#ifndef __RobotArms__
#define __RobotArms__

#include "serial/serial.h"
#include <windows.h>
#include "movingAverage.h"

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
		JoinInfo():_samples(10)
		{
			targetAngle = 0;
			currAngle = 0;
			temp = 0;
		}

		MovAvg _samples;
		float targetAngle;
		float currAngle;
		float temp;

		void SetValue(float v)
		{
			targetAngle=_samples.getNext(v);
		}
	};
protected:




	JoinInfo _leftArm[7];
	JoinInfo _rightArm[7];
	JoinInfo _leftHand[5];
	JoinInfo _rightHand[5];

	float _adcValues[2][2];

	EState _state ;

	int JointsCount;

	serial::Serial* m_serial;
	bool connected;
	HANDLE m_robotThread;

	std::vector<byte> _buffer;

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

	float _timeToWait;

	bool _off;

	bool *RSigns;
	bool *LSigns;

	float *LMidPos;
	float *RMidPos;
	
	float *LShutdownPos;
	float *RShutdownPos;

	int _version;

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
	void _updateHand(TargetArm arm);
	void _UpdateJoints(TargetArm arm, ushort time, bool midPos = false,bool immediate=false, bool hand=false);
	void _readJoints(TargetArm arm);
	void _readTemperature(TargetArm arm);
	void _readHand(TargetArm arm);
	void _readBattery();

	void ProcessState();
	void ProcessThread();


	static DWORD timerThreadRobot(RobotArms *robot, LPVOID pdata);
public:
	RobotArms();
	virtual ~RobotArms();

	virtual bool Connect(const core::string& port,int version);
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
	float* GetHandSensor(TargetArm hand) { return _adcValues[(hand == TargetArm::Right) ? 0 : 1]; }

	EState GetStatus() { return _state; }
};

}


#endif
