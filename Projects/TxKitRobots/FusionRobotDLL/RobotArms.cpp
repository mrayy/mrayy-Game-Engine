
#include "stdafx.h"
#include "RobotArms.h"

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
const byte CMD_ALL_JOINTTarget_SET = 0x0D;
const byte CMD_ALL_JOINTTarget_GET = 0x0C;
const byte CMD_ALL_Battery_GET = 0x0E;


bool Masks[7]=  { true, true, true, true, true, true, true };

/*
void arms_SerialEventManager(Tserial_event * object, uint32 event) {
	char *buffer;
	int   size;
	Tserial_event *com;

	com = (Tserial_event *)object;
	if (com != 0) {
		switch (event) {
		case  SERIAL_CONNECTED:
			//printf("Com Port Connected! \n");
			break;
		case  SERIAL_DATA_ARRIVAL:
			size = com->getDataInSize();
			buffer = com->getDataInBuffer();
			((RobotArms*)com->owner)->_onSerialData(size, buffer);
			com->dataHasBeenRead();
			break;
		}
	}


}*/


RobotArms::RobotArms()
{
	connected = false;
	m_serial = 0;
	_state = EState::Wait;

	JointsCount = 7;

	LArmEnabled = false;
	RArmEnabled = false;

	_enableSending = false;
	_enableReading = false;
	_enableTemperature = true;

	TemperatureTime = 1000;
	_temperatureTime = 0;
	_timer = 0;
	timeMS = 3000;

	if (JointsCount == 6)
	{
		// V1 parameters
		LMidPos = new float[7]{ -15, -42, 0, -50, 42, 0, 0 };
		RMidPos = new float[7]{ 15, 42, 0, -50, -42, 0, 0 };

		LShutdownPos = new float[7]{ 0, -80, 0, -75, 52, 0, 0 };
		RShutdownPos = new float[7]{ 0, 80, 0, -75, -52, 0, 0 };

		RSigns = new bool[7] { false, true, true, false, true, false, true }; //XX-X-X
		LSigns = new bool[7] { false, false, false, false, false, true, true };
	}
	if (JointsCount == 7)
	{
		LMidPos = new float[7] { 15, 42, 0, 90, 0, 0, 0 };
		RMidPos = new float[7] { 15, -42, 0, 90, 0, 0, 0 };

		LShutdownPos = new float[7] { 0, 10, 0, 0, 0, 0, 0 };
		RShutdownPos = new float[7] { 0, -10, 0, 0, 0, 0, 0 };

		LSigns = new bool[7] { true, false, true, false, true, false, true };
		RSigns = new bool[7] { false, false, true, true, true, false, false }; //XX-X-X
	}
}
RobotArms::~RobotArms()
{
	Disconnect();
	delete m_serial;
}


bool RobotArms::Connect(const core::string& port)
{
	Disconnect();

	m_serial = new serial::Serial(port, 1500000, serial::Timeout::simpleTimeout(30), serial::eightbits, serial::parity_none);
	connected = m_serial->isOpen();
	if (!connected)
	{
		//	printf("Failed to connect robot\n");
		delete m_serial;
		m_serial = 0;
	}
	else
	{
		m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);
		gLogManager.log("Connected", ELL_INFO);
		//m_serial->setRxSize(15);
		//_sendCommand("#ea");//enable angle logging

		//read EEPROM
		//Note: for some reason the servo IDs are changed when calling write EEPROM function.
		// This function is disabled for the time being
		//_writeEEPROM();
	}
	//m_serial->owner = this;
	return connected;
}
bool RobotArms::IsConnected()
{
	return connected;
}
void RobotArms::Disconnect()
{
	_Close();
}


short RobotArms::AngleToServo(float angle, bool reverse)
{
	angle = NormalizeAngle(angle);
	angle = math::clamp(angle, -180.0f, 180.0f);
	if (reverse)
		angle = -angle;
	return (short)(angle * 100);
}

float RobotArms::ServoToAngle(short val, bool reverse)
{
	float angle = val * 0.01f;

	if (reverse)
		angle = -angle;
	return angle;
}

void RobotArms::_updateHand(TargetArm arm)
{
	cmd[0] = CMD_ALL_HAND_SET;
	cmd[1] = (arm == TargetArm::Right) ? (byte)0x01 : (byte)0x02;

	if (arm == TargetArm::Left)
	{
		for (int i = 0; i < 3; ++i)
			cmd[2 + i] = (byte)(math::clamp<float>(_leftHand[i].targetAngle,25, 150.0f));
	}
	else
	{
		for (int i = 0; i < 3; ++i)
			cmd[2 + i] = (byte)(math::clamp<float>(_rightHand[i].targetAngle, 25, 150.0f));

	}

	_sendCommand(cmd, 2 + 3);
}
void RobotArms::_UpdateJoints(TargetArm arm, ushort time, bool midPos, bool immediate,bool hand)
{
	if (time >= 0)
		cmd[0] = CMD_ALL_JOINTTarget_SET;
	else
		cmd[0] = CMD_ALL_JOINTANGLE_SET;
	if (hand)
		cmd[0] = CMD_ALL_ARMHAND_SET;

	cmd[1] = (arm == TargetArm::Right) ? (byte)0x01 : (byte)0x02;

	byte bytes[24];
	int offset = 0;
	for (int i = 0; i < JointsCount; ++i)
	{
		float angle = 0;
		if (Masks[i])
		{
			if (midPos)
			{
				angle = (arm == TargetArm::Right) ? RMidPos[i] : LMidPos[i];

			}
			else if ( _state != EState::Shutdown)
				angle = _GetValue(arm, i);
			else if ( _state == EState::Shutdown)
			{
				angle = (arm == TargetArm::Right) ? RShutdownPos[i] : LShutdownPos[i];
			}
		}

		short servo=AngleToServo(angle, (arm == TargetArm::Right) ? RSigns[i] : LSigns[i]);
		memcpy(bytes, &servo, sizeof(servo));
		//bytes = BitConverter.GetBytes();
		cmd[2 + i * 2 + 0] = bytes[1];
		cmd[2 + i * 2 + 1] = bytes[0];
		offset += 2;
	}

	if (hand || time>=0)
	{
		//bytes = BitConverter.GetBytes(time);
		memcpy(bytes, &time, sizeof(time));
		cmd[2 + offset + 0] = bytes[1];
		cmd[2 + offset + 1] = bytes[0];
		offset += 2;
	}
	if (hand)
	{

		if (arm == TargetArm::Left)
		{
			for (int i = 0; i < 3; ++i)
				cmd[2+offset + i] = (byte)(math::clamp<float>(_leftHand[i].targetAngle, 25, 150.0f));
		}
		else
		{
			for (int i = 0; i < 3; ++i)
				cmd[2 + offset + i] = (byte)(math::clamp<float>(_rightHand[i].targetAngle, 25, 150.0f));

		}
	}
	if (hand)
	{

		_sendCommand(cmd, 2 + JointsCount * 2 + 3 + 2, immediate);
	}
	else if (time >= 0)
	{
		_sendCommand(cmd, 2 + JointsCount * 2 + 2, immediate);
	}
	else
		_sendCommand(cmd, 2 + JointsCount * 2, immediate);

}



void RobotArms::_readJoints(TargetArm arm)
{

	cmd[0] = CMD_ALL_JOINTANGLE_GET;
	cmd[1] = (arm == TargetArm::Right) ? (byte)0x01 : (byte)0x02;
	_sendCommand(cmd, 2);
	byte d[2];
	short val;
	if (ReadData(data) > 0)
	{
		for (int i = 0; i < JointsCount; ++i)
		{
			d[0] = data[2 * i + 1];
			d[1] = data[2 * i + 0];
			memcpy(&val, d, sizeof(val));
			//val=BitConverter.ToInt16(d, 0)
			float a = ServoToAngle(val, (arm == TargetArm::Right) ? RSigns[i] : LSigns[i]);
			_SetValue(arm, i, a);
		}
	}
}

void RobotArms::_readTemperature(TargetArm arm)
{

	cmd[0] = CMD_ALL_TEMP;
	cmd[1] = (arm == TargetArm::Right) ? (byte)0x01 : (byte)0x02;
	_sendCommand(cmd, 2,true);
	byte d [2];
	short val;
	if (ReadData(data) > 0)
	{
		for (int i = 0; i < JointsCount; ++i)
		{
			d[0] = data[2 * i + 1];
			d[1] = data[2 * i + 0];
			memcpy(&val, d, sizeof(val));
			float a = (float)val*0.01f;
			_SetTemp(arm, i, a);
		}
	}
}
void RobotArms::_readHand(TargetArm arm)
{
	cmd[0] = CMD_ALL_HAND_GET;
	cmd[1] = (arm == TargetArm::Right) ? (byte)0x01 : (byte)0x02;
	_sendCommand(cmd, 2);
	byte d[2];
	short val;
	if (ReadData(data) > 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			d[0] = data[2 * i + 1];
			d[1] = data[2 * i + 0];
			memcpy(&val, d, sizeof(val));
			_adcValues[(arm == TargetArm::Right) ? 0 : 1][i] = val;
		}
	}
}


void RobotArms::ProcessState()
{
	_timeToWait = 0;
	int updateTime = 8;
	_readBattery();
	if (_enableTemperature && _temperatureTime >= TemperatureTime)
	{
		_temperatureTime = 0;
		_readTemperature(TargetArm::Left);
		_readTemperature(TargetArm::Right);
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
		if (LArmEnabled)
			_UpdateJoints(TargetArm::Left, timeMS, true,true);
		if (RArmEnabled)
			_UpdateJoints(TargetArm::Right, timeMS, true, true);
		_sleep(timeMS);
		_timer = 0;
		if (LArmEnabled)
			_UpdateJoints(TargetArm::Left, timeMS);
		if (RArmEnabled)
			_UpdateJoints(TargetArm::Right, timeMS);
		break;
	case EState::Initializing:
		if (_timer > timeMS)
		{
			_timer = 0;
			_state = EState::Operate;
		}
		break;
	case EState::Operate:
		if (LArmEnabled)
		{
			_UpdateJoints(TargetArm::Left, 18,false,true,true);
			//_updateHand(TargetArm::Left);
			//_readHand(TargetArm::Left);
		}
		if (RArmEnabled) {
			_UpdateJoints(TargetArm::Right, 18, false, true, true);
			//_updateHand(TargetArm::Right);
			//_readHand(TargetArm::Right);
		}
		if (LArmEnabled || RArmEnabled)
			_timeToWait += updateTime;

		if (!_enableSending)
			_state = EState::Shutdown;
		break;
	case EState::Shutdown:
		_timer = 0;
		if (LArmEnabled)
			_UpdateJoints(TargetArm::Left, timeMS, true, true);
		if (RArmEnabled)
			_UpdateJoints(TargetArm::Right, timeMS, true, true);
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
		if (LArmEnabled)
			_readJoints(TargetArm::Left);
		if (RArmEnabled)
			_readJoints(TargetArm::Right);
	}
}
void RobotArms::ProcessThread()
{
	while (connected && m_serial->isOpen() )
	{
		try
		{
			ProcessState();

			if (_buffer.size() > 0) {

				m_serial->write(&_buffer[0], _buffer.size());
				_buffer.clear();
				_timeToWait = 10;
			}
			if (_state == EState::Wait)
				_timeToWait = 100;
			_sleep( _timeToWait);
			//   System.Threading._sleep(30);
		}
		catch (std::exception& e)
		{
			//Debug.LogWarning(e.Message);
		}
	}

}
DWORD RobotArms::timerThreadRobot(RobotArms *robot, LPVOID pdata) {
	robot->ProcessThread();
	return 0;
}


byte RobotArms::_readByte()
{
	std::string v=m_serial->read(1);
	if (v.length() == 0)
		return 0;
	return v[0];

}

int RobotArms::ReadData(byte*data)
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
bool RobotArms::_sendCommand(byte* cmd, int length, bool immediate )
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
		_sleep(4);
	}
	else {
		for (int i = 0; i < dlen; ++i)
			_buffer.push_back(data[i]);
	}
	return true;

}
void RobotArms::_On()
{
	cmd[0] = CMD_ONOFF;
	cmd[1] = (byte)0x1;
	_sendCommand(cmd, 2);
	_sleep(200);
}
void RobotArms::_Off()
{
	cmd[0] = CMD_ONOFF;
	cmd[1] = (byte)0x0;
	_sendCommand(cmd, 2);
	_sleep(200);
}
void RobotArms::_Close()
{
	if (!connected)
		return;
	_enableSending = false;
	connected = false;
	if (m_robotThread != 0)
	{
		WaitForSingleObject(m_robotThread, INFINITE);
	}

	TerminateThread(m_robotThread,0);
	m_robotThread = 0;

	_state = EState::Shutdown;
	if (LArmEnabled)
		_UpdateJoints(TargetArm::Left, timeMS, false, true);
	if (RArmEnabled)
		_UpdateJoints(TargetArm::Right, timeMS, false, true);
	_sleep(timeMS);
	_state = EState::Wait;

	_Off();
	if (m_serial != 0 )
	{
		m_serial->close();
		delete m_serial;
		m_serial = 0;
	}

}
void RobotArms::_readBattery()
{
	cmd[0] = CMD_ALL_Battery_GET;
	_sendCommand(cmd, 1);
	byte d[2];
	if (ReadData(data) > 0)
	{
		d[0] = data[1];
		d[1] = data[0];
		memcpy(&BatteryLevel, d, 2);
	}
}

float RobotArms::_GetValue(TargetArm arm, int id)
{
	if (arm == Left)
		return _leftArm[id].targetAngle;
	else
		return _rightArm[id].targetAngle;
}
void RobotArms::_SetValue(TargetArm arm, int id, float val)
{
	if (arm == Left)
		_leftArm[id].currAngle=val;
	else
		_rightArm[id].currAngle=val;

}
void RobotArms::_SetTemp(TargetArm arm, int id, float val)
{
	if (arm == Left)
		_leftArm[id].temp = val;
	else
		_rightArm[id].temp = val;
}

void RobotArms::SetArmAngles(TargetArm arm, float *angles, int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (arm == Left)
			_leftArm[i].SetValue(angles[i]);
		else
			_rightArm[i].SetValue(angles[i]);
	}
}

void RobotArms::SetHand(TargetArm arm, float* angles, int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (arm == Left)
			_leftHand[i].SetValue(angles[i]);
		else
			_rightHand[i].SetValue(angles[i]);
	}
}
void RobotArms::Start(bool leftArm, bool rightArm)
{
	_off = false;
	_enableSending = true;
	LArmEnabled = leftArm;
	RArmEnabled = rightArm;
}
void RobotArms::Stop(bool force )
{
	_enableSending = false;
	if (force)
	{
		_state = EState::Wait;
		_Off();
	}
}

void RobotArms::Update(float dt)
{

	_timer += dt * 1000;
	_temperatureTime += dt * 1000;
}

}



