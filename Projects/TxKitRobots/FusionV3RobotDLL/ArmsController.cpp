
#include "stdafx.h"
#include "ArmsController.h"

namespace mray
{


	const byte CMD_ONOFF = 0x01;
	const byte CMD_JOINTANGLE_GET = 0x04; //http://wikilimbs.krkrpro.com/index.php?%E3%82%B3%E3%83%9E%E3%83%B3%E3%83%89%E4%B8%80%E8%A6%A7#r0111101
	const byte CMD_JOINTANGLE_SET = 0x06; //http://wikilimbs.krkrpro.com/index.php?%E3%82%B3%E3%83%9E%E3%83%B3%E3%83%89%E4%B8%80%E8%A6%A7#od230c2d
	const byte CMD_ALL_JOINTANGLE_SET = 0x07; //7parameters
	const byte CMD_ALL_JOINTANGLE_GET = 0x08;
	const byte CMD_ALL_HAND_SET = 0x11;
	const byte CMD_ALL_HAND_GET = 0x10;
	const byte CMD_ALL_ARMHAND_SET = 0x13;
	const byte CMD_ALL_TEMP = 0x0A;
	const byte CMD_ALL_JOINTTarget_SET = 0x13;
	const byte CMD_ALL_JOINTTarget_GET = 0x0C;
	const byte CMD_ALL_Battery_GET = 0x0E;


	bool Masks[7] = { true, true, true, true, true, true, true };

	ArmsController::ArmsController()
	{
		_state = EState::Wait;
		connected = false;
		m_serial = 0;

		ArmEnabled = false;

		_enableSending = false;
		_enableReading = false;
		_enableTemperature = true;

		TemperatureTime = 1000;
		_temperatureTime = 0;
		_timer = 0;
		timeMS = 3000;

	}
	ArmsController::~ArmsController()
	{
		Disconnect();
		delete m_serial;
	}


	bool ArmsController::Connect(const core::string& port,bool left)
	{
		if (left)
		{
			MidPos = new float[7]{ 15, 42, 0, 90, 0, 0, 0 };
			ShutdownPos = new float[7]{ 0, 10, 0, 0, 0, 0, 0 };
			Signs = new bool[7]{ true, false, true, false, true, false, false }; //XX-X-X
		}
		else
		{
			MidPos = new float[7]{ 15, -42, 0, 90, 0, 0, 0 };
			ShutdownPos = new float[7]{ 0, -10, 0, 0, 0, 0, 0 };
			Signs = new bool[7]{ false, false, true, true, true, false, true }; //XX-X-X
		}
		Disconnect();

		m_serial = new serial::Serial("\\\\.\\" + port, 3000000, serial::Timeout::simpleTimeout(30), serial::eightbits, serial::parity_none);
		connected = m_serial->isOpen();
		if (!connected)
		{
			delete m_serial;
			m_serial = 0;
		}
		else
		{
			m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);
			gLogManager.log("Connected", ELL_INFO);
		}
		//m_serial->owner = this;
		return connected;
	}
	bool ArmsController::IsConnected()
	{
		return connected;
	}
	void ArmsController::Disconnect()
	{
		_Close();
	}


	short ArmsController::AngleToServo(float angle, bool reverse)
	{
		angle = NormalizeAngle(angle);
		angle = math::clamp(angle, -180.0f, 180.0f);
		if (reverse)
			angle = -angle;
		return (short)(angle * 100);
	}

	float ArmsController::ServoToAngle(short val, bool reverse)
	{
		float angle = val * 0.01f;

		if (reverse)
			angle = -angle;
		return angle;
	}

	void ArmsController::_updateHand()
	{
		cmd[0] = CMD_ALL_HAND_SET;

		
		for (int i = 0; i < 6; ++i)
			cmd[1 + i] = 180-(byte)(math::clamp<float>(_Hand[i].targetAngle, 30.0f, 150.0f));


		_sendCommand(cmd, 1 + 6);
	}
	void ArmsController::_UpdateJoints(ushort time, bool midPos, bool immediate, bool hand)
	{
		if (time >= 0)
			cmd[0] = CMD_ALL_JOINTTarget_SET;
		else
			cmd[0] = CMD_ALL_JOINTANGLE_SET;
// 		if (hand)
// 			cmd[0] = CMD_ALL_ARMHAND_SET;

		hand = false;

		byte bytes[24];
		int offset = 0;

		int _jointsCount = 7;
		for (int i = 0; i < _jointsCount; ++i)
		{
			float angle = 0;
			if (Masks[i])
			{
				if (midPos)
				{
					angle = MidPos[i];

				}
				else if (_state != EState::Shutdown)
					angle = _GetValue(i);
				else if (_state == EState::Shutdown)
				{
					angle = ShutdownPos[i];
				}
			}

			short servo = AngleToServo(angle, Signs[i]);
			memcpy(bytes, &servo, sizeof(servo));
			//bytes = BitConverter.GetBytes();
			cmd[1 + i * 2 + 0] = bytes[1];
			cmd[1 + i * 2 + 1] = bytes[0];
			offset += 2;
		}

		if (hand || time >= 0)
		{
			//bytes = BitConverter.GetBytes(time);
			memcpy(bytes, &time, sizeof(time));
			cmd[1 + offset + 0] = bytes[1];
			cmd[1 + offset + 1] = bytes[0];
			offset += 2;
		}
		if (hand)
		{

			for (int i = 0; i < 3; ++i)
				cmd[1 + offset + i] = (byte)(math::clamp<float>(_Hand[i].targetAngle, 25, 150.0f));
		}
		if (hand)
		{
			_sendCommand(cmd, 2 + _jointsCount * 2 + 3 + 2, immediate);
		}
		else if (time >= 0)
		{
			_sendCommand(cmd, 2 + _jointsCount * 2 + 2, immediate);
		}
		else
			_sendCommand(cmd, 2 + _jointsCount * 2, immediate);

	}



	void ArmsController::_readJoints()
	{

		cmd[0] = CMD_ALL_JOINTANGLE_GET;
		_sendCommand(cmd, 1);
		byte d[2];
		short val;
		int _jointsCount = 7;
		if (ReadData(data) > 0)
		{
			for (int i = 0; i < _jointsCount; ++i)
			{
				d[0] = data[2 * i + 1];
				d[1] = data[2 * i + 0];
				memcpy(&val, d, sizeof(val));
				//val=BitConverter.ToInt16(d, 0)
				float a = ServoToAngle(val,Signs[i]);
				_SetValue( i, a);
			}
		}
	}

	void ArmsController::_readTemperature()
	{

		cmd[0] = CMD_ALL_TEMP;
		_sendCommand(cmd, 1, true);
		byte d[2];
		short val;
		int _jointsCount = 7;
		if (ReadData(data) > 0)
		{
			for (int i = 0; i < _jointsCount; ++i)
			{
				d[0] = data[2 * i + 1];
				d[1] = data[2 * i + 0];
				memcpy(&val, d, sizeof(val));
				float a = (float)val*0.01f;
				_SetTemp( i, a);
			}
		}
	}
	void ArmsController::_readHand()
	{
		cmd[0] = CMD_ALL_HAND_GET;
		_sendCommand(cmd, 1);
		byte d[2];
		short val;
		if (ReadData(data) > 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				d[0] = data[2 * i + 1];
				d[1] = data[2 * i + 0];
				memcpy(&val, d, sizeof(val));
				_adcValues[i] = val;
			}
		}
	}


	void ArmsController::ProcessState()
	{
		int servoUpdate = 0;
		_timeToWait = 0;
		_readBattery();
		if (_enableTemperature && _temperatureTime >= TemperatureTime)
		{
			_temperatureTime = 0;
			_readTemperature();
		}
		switch (_state)
		{
		case EState::Wait:
			if (_enableSending)
			{
				_On();
				_timer = 0;
				_state = EState::Initialize;
			}
			break;
		case EState::Initialize:
			_state = EState::Initializing;
			if (ArmEnabled)
				_UpdateJoints(timeMS, true, true);
			_sleep(timeMS);
			_timer = 0;
			if (ArmEnabled)
				_UpdateJoints(timeMS);/**/
			break;
		case EState::Initializing:
			if (_timer > timeMS)
			{
				_timer = 0;
				_state = EState::Operate;
			}
			break;
		case EState::Operate:
			
			if (ArmEnabled) {
				_UpdateJoints(servoUpdate, false, true, true);
				//if (handTimer > 50)
				{
					handTimer = 0;
					_updateHand();
					_readHand();
				}
			}
			if (ArmEnabled)
				_timeToWait += servoUpdate;

			if (!_enableSending)
				_state = EState::Shutdown;
			break;
		case EState::Shutdown:
			_timer = 0;
			/**/if (ArmEnabled)
				_UpdateJoints(timeMS, true, true);
			_state = EState::Shutingdown;
			break;
		case EState::Shutingdown:
			if (_timer > timeMS)
			{
				if (_off)
					_Off();
				_state = EState::Wait;
			}
			break;
		}

		if (_enableReading)
		{
			if (ArmEnabled)
				_readJoints();
		}
	}
	void ArmsController::ProcessThread()
	{
		while (connected && m_serial->isOpen())
		{
			try
			{
				ProcessState();
				_timeToWait += 5;
				/**/
				if (_buffer.size() > 0) {

					m_serial->write(&_buffer[0], _buffer.size());
					_buffer.clear();
					_timeToWait = 10;
				}
				if (_state == EState::Wait)
					_timeToWait = 100;
				if (_timeToWait > 0)
					_sleep(_timeToWait);

				handTimer += _timeToWait;
				//   System.Threading._sleep(30);
			}
			catch (std::exception& e)
			{
				//Debug.LogWarning(e.Message);
			}
		}

	}
	DWORD ArmsController::timerThreadRobot(ArmsController *robot, LPVOID pdata) {
		robot->ProcessThread();
		return 0;
	}


	byte ArmsController::_readByte()
	{
		std::string v = m_serial->read(1);
		if (v.length() == 0)
			return 0;
		return v[0];

	}

	int ArmsController::ReadData(byte*data)
	{
		if (!m_serial || !m_serial->isOpen())
			return 0;


		int length = (byte)_readByte();
		int sum = length;
		for (int i = 0; i < length - 2; ++i)
		{
			data[i] = (byte)_readByte();
			sum += data[i];
		}
		sum = sum & 0xff;
		if (sum != _readByte())
			return 0;
		return length - 2;
	}
	bool ArmsController::_sendCommand(byte* cmd, int length, bool immediate)
	{
		if (!m_serial || !m_serial->isOpen())
			return false;

		m_serial->flushInput();
		m_serial->flushOutput();
		//byte[] data = new byte[length + 2];
		int dlen = length + 2;
		data[0] = dlen;
		int sum = data[0];
		for (int i = 0; i < length; ++i)
		{
			data[i + 1] = cmd[i];
			sum += data[i + 1];
		}
		data[dlen - 1] = (byte)(sum & 0xff);
		if (immediate)
		{
			m_serial->write(data, dlen);
			//_sleep(4);
		}
		else {
			for (int i = 0; i < dlen; ++i)
				_buffer.push_back(data[i]);
		}
		return true;

	}
	void ArmsController::_On()
	{
		cmd[0] = CMD_ONOFF;
		cmd[1] = (byte)0x1;
		_sendCommand(cmd, 2);
		_sleep(200);
	}
	void ArmsController::_Off()
	{
		for (int i = 0; i < 7; ++i)
		{
			ArmsController::JoinInfo&j1 = GetArm()[i];
			j1.temp = 0;
		}
		cmd[0] = CMD_ONOFF;
		cmd[1] = (byte)0x0;
		_sendCommand(cmd, 2);
		_sleep(200);
	}
	void ArmsController::SafeShutdown(int timeout)
	{
		if (!connected)
			return;
		connected = false;
		_enableSending = false;
		if (m_robotThread != 0)
		{
			WaitForSingleObject(m_robotThread, INFINITE);
		}

		TerminateThread(m_robotThread, 0);
		m_robotThread = 0;

		_state = EState::Shutdown;
		if (ArmEnabled)
			_UpdateJoints(timeMS, false, true);
	}
	void ArmsController::_Close()
	{
		_state = EState::Wait;

		_Off();
		if (m_serial != 0)
		{
			m_serial->close();
			delete m_serial;
			m_serial = 0;
		}

	}
	void ArmsController::_readBattery()
	{
		cmd[0] = CMD_ALL_Battery_GET;
		_sendCommand(cmd, 1);
		byte d[2];
		if (ReadData(data) > 0)
		{
			d[0] = data[1];
			d[1] = data[0];
//			memcpy(&BatteryLevel, d, 2);
		}
	}

	float ArmsController::_GetValue( int id)
	{
		return _Arm[id].targetAngle;
	}
	void ArmsController::_SetValue( int id, float val)
	{
		_Arm[id].currAngle = val;

	}
	void ArmsController::_SetTemp( int id, float val)
	{
		_Arm[id].temp = val;
	}

	void ArmsController::SetArmAngles( float *angles, int n)
	{
		for (int i = 0; i < n; ++i)
		{
			_Arm[i].SetValue(angles[i]);
		}
	}

	void ArmsController::SetHand( float* angles, int n)
	{
		for (int i = 0; i < n; ++i)
		{
			_Hand[i].SetValue(angles[i]);
		}
		if(n!=6)
			_Hand[5].SetValue(180 - _Hand[0].targetAngle);
	}
	void ArmsController::Start(bool enabled, bool enableReadingAngles)
	{
		_off = false;
		_enableSending = true;
		ArmEnabled = enabled;
		_enableReading = enableReadingAngles;
	}
	void ArmsController::Stop(bool force)
	{
		_enableSending = false;
		if (force)
		{
			_state = EState::Wait;
			_Off();
		}
	}

	void ArmsController::Update(float dt)
	{

		_timer += dt * 1000;
		_temperatureTime += dt * 1000;
	}

}



