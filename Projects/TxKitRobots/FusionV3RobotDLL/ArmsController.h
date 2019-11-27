
#ifndef __ArmsController__
#define __ArmsController__

#include "serial/serial.h"
#include <windows.h>
#include "movingAverage.h"
#include "PID.h"
#include "ITimer.h"

namespace mray
{

class ArmsController
{
public:
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
		JoinInfo() :_samples(1)
		{
			targetAngle = 0;
			currAngle = 0;
			temp = 0;
		}

		MovAvg _samples;
		float targetAngle;
		float currAngle;
		float temp;
		float PIDVal;

		PID _pid;

		void SetValue(float v)
		{
			targetAngle = _samples.getNext(v);
		}
		void internalUpdate(double dt)
		{
			PIDVal=_pid.calculate(dt, targetAngle, currAngle);
		}

		float GetValue()
		{
			return targetAngle;// currAngle + PIDVal;//PIDVal;//
		}
	};
protected:


	EState _state;


	JoinInfo _Arm[7];
	JoinInfo _Hand[6];

	float _adcValues[4];

	serial::Serial* m_serial;
	bool connected;
	HANDLE m_robotThread;

	std::vector<byte> _buffer;

	short _lastServos[7];

	byte _readByte();

	byte cmd[256];
	byte data[256];

	bool ArmEnabled;

	bool _enableSending;
	bool _enableReading;
	bool _enableTemperature;

	float TemperatureTime;
	float _temperatureTime;
	ushort timeMS;
	float _timer;
	ushort handTimer;

	float _timeToWait;

	bool _off;

	bool *Signs;

	float *MidPos;

	float *ShutdownPos;

	int _version;

	double _lastTime;

	OS::ITimer* _ticker;

	static float NormalizeAngle(float v)
	{
		while (v > 180)
			v -= 360.0f;
		while (v < -180)
			v += 360.0f;
		return v;
	}

	float _GetValue( int id);
	void _SetValue(int id, float val);
	void _SetTemp(int id, float temp);


	int ReadData(byte*data);
	bool _sendCommand(byte* cmd, int length, bool immediate = true);


	void _On();
	void _Off();
	void _Close();


	short AngleToServo(float angle, bool reverse);
	float ServoToAngle(short val, bool reverse);
	void _updateHand();
	void _UpdateJoints(ushort time, bool midPos = false, bool immediate = false, bool hand = false);
	void _readJoints();
	void _readTemperature();
	void _readHand();
	void _readBattery();

	void ProcessState();
	void ProcessThread();


	static DWORD timerThreadRobot(ArmsController *robot, LPVOID pdata);
public:
	ArmsController(const core::string& pidFile);
	virtual ~ArmsController();

	virtual bool Connect(const core::string& port,bool left);
	virtual bool IsConnected();
	void SafeShutdown(int timeout);
	virtual void Disconnect();
	bool IsEnabled() {
		return ArmEnabled;
	}

	void SetArmAngles(float *angles, int n);
	void SetHand(float* angles, int n);

	void Start(bool enabled, bool enableReadingAngles);
	void Stop(bool force = false);

	void Update(float dt);

	JoinInfo* GetArm() { return _Arm; }
	JoinInfo* GetHand() { return _Hand; }
	float* GetHandSensor() { return _adcValues; }

	EState GetStatus() { return _state; }
};

}


#endif
