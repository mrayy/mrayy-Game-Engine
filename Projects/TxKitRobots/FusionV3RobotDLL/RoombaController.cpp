

#include "stdafx.h"
#include "RoombaController.h"
#include "IOSystem.h"

#include "IThreadManager.h"
#include "IRobotController.h"
#include <windows.h>

namespace mray
{

	static const int s_PacketGroupSize[]=
	{
		26,
		10,
		6,
		10,
		14,
		12,
		52,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		1,
		2,
		2,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		2,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		1,
		2,
		2,
		2,
		2,
		1,
	};
	static const int s_PacketGroupSize100[] =
	{
		80,
		28,
		0,
		0,
		0,
		0,
		12,
		9,
	};

	static void SendByte(OS::ISerialPort* s, uchar b)
	{
		if (!s->IsOpen())
			return;
		try
		{
			s->Write(&b, sizeof(b));
		}
		catch (std::exception* e)
		{
		}
	}

	/// Push a 16 bits integer (high byte first) in a stream.
	static void
		sendInt16(OS::ISerialPort* s, short i)
	{
		if (!s->IsOpen())
			return;
		const unsigned char v[] = { (i & 0xFFFF) >> 8, i & 0xFFFF };
		try
		{
			s->Write(v, sizeof(v));
		}
		catch (std::exception* e)
		{
		}
	}

	#define MAKE_ONE(v1, v2) ((v1 << 8) + v2)

	/// Receive a 16 bits integer (high byte first) in a stream.
	static short
		receiveInt16(OS::ISerialPort* s)
	{
		if (!s->IsOpen())
			return 0;
		unsigned char v[2];
		try
		{
			s->Read(v, sizeof(v));
		}
		catch (std::exception* e)
		{
		}
		return MAKE_ONE(v[1] , v[0]);
	}

#define INITIALIZE_SENSORS()                    \
  streamedSensors_ (),                          \
    wheeldropCaster_ (),                        \
    wheeldropLeft_ (),                          \
    wheeldropRight_ (),                         \
    bumpLeft_ (),                               \
    bumpRight_ (),                              \
    wall_ (),                                   \
    cliffLeft_ (),                              \
    cliffFrontLeft_ (),                         \
    cliffFrontRight_ (),                        \
    cliffRight_ (),                             \
    deviceDetect_ (),                           \
    digitalInput3_ (),                          \
    digitalInput2_ (),                          \
    digitalInput1_ (),                          \
    digitalInput0_ (),                          \
    analogSignal_ (),                           \
    homeBaseChargerAvailable_ (),               \
    internalChargerAvailable_ (),               \
    virtualWall_ (),                            \
    leftWheelOvercurrent_ (),                   \
    rightWheelOvercurrent_ (),                  \
    ld2Overcurrent_ (),                         \
    ld1Overcurrent_ (),                         \
    ld0Overcurrent_ (),                         \
    ir_ (),                                     \
    advanceButton_ (),                          \
    playButton_ (),                             \
    distance_ (),                               \
    angle_ (),                                  \
    chargingState_ (),                          \
    batteryVoltage_ (),                         \
    batteryCurrent_ (),                         \
    batteryTemperature_ (),                     \
    batteryCharge_ (),                          \
    batteryCapacity_ (),                        \
    wallSignal_ (),                             \
    cliffLeftSignal_ (),                        \
    cliffFrontLeftSignal_ (),                   \
    cliffFrontRightSignal_ (),                  \
    cliffRightSignal_ (),                       \
    songNumber_ (),                             \
    songPlaying_ (),                            \
    streamPackets_ (),                          \
    requestedVelocity_ (),                      \
    requestedRadius_ (),                        \
    requestedLeftVelocity_ (),                  \
    requestedRightVelocity_ ()
RoombaControllerImpl::RoombaControllerImpl():
INITIALIZE_SENSORS(), currentMode_(IROBOT_CREATE_OFF)
{
	m_leftWheel = m_rightWheel = 0;
	m_serialPort = OS::IOSystem::getInstance().GetSerialPortService()->CreateSerialPort();

	m_serialPort->OnDataArrived += CreateDataArrivedDelegate(RoombaControllerImpl, this, OnSerialPortDataArrived);

	m_toProcessMutex = OS::IThreadManager::getInstance().createMutex();

}
RoombaControllerImpl::~RoombaControllerImpl()
{
	Disconnect();
	delete m_serialPort;
	delete m_toProcessMutex;
}
uchar* RoombaControllerImpl::_GetPacketData(int size)
{
	int avail = 0;
	std::vector<DataPacket> tmp;
	m_toProcessMutex->lock();
	for (int i = 0; i < toProcess_.size(); ++i)
	{
		tmp.push_back(toProcess_[i]);
		avail += toProcess_[i].size;
		if (avail >= size)
			break;
	}
	if (avail < size)
	{
		m_toProcessMutex->unlock();
		return 0; // no enough data
	}

	uchar* data = new uchar[size];
	avail = 0;
	int idx = 0;
	DataPacket rPacket;
	rPacket.size = 0;
	rPacket.data = 0;
	for (int i = 0; i < tmp.size(); ++i)
	{
		int remain = size - avail;
		avail += tmp[i].size;
		int len = min(tmp[i].size,remain);
		memcpy(data + idx, tmp[i].data, len);
		idx += len;
		if (avail >= size)
		{
			//we are done
			if (tmp[i].size > len)
			{
				//we need to push back the remaining data
				rPacket.size = tmp[i].size - len;
				rPacket.data = new uchar[rPacket.size];
				memcpy(rPacket.data, tmp[i].data + len, rPacket.size);
			}

			break;
		}
	}
	for (int i = 0; i < tmp.size(); ++i)
	{
		toProcess_.erase(toProcess_.begin());
		delete [] tmp[i].data;
	}
	if (rPacket.size>0)
		toProcess_.insert(toProcess_.begin(), rPacket);
	m_toProcessMutex->unlock();

	return data;

}
uchar* RoombaControllerImpl::_ParseSensorPacket(SensorPacket p, uchar* data)
{
	uchar* ptr = data;
#define CASE_PARSE_GROUP(group,start,end)case group:{for(int i=start;i<=end;++i){ptr=_ParseSensorPacket((SensorPacket)i,ptr);}break;}
#define CASE_PARSE_SINGLE(S,val)case S:{val=*ptr++;break;}
#define CASE_PARSE_SINGLE_TYPED(S,val,Type)case S:{val=static_cast<Type>(*ptr++);break;}
#define CASE_PARSE_DOUBLE(S,val)case S:{uchar v1,v2;v2=*ptr++;v1=*ptr++; val=MAKE_ONE(v2,v1); break;}

#define TEST(v,b)(v&(1<<b))
	switch (p)
	{
		CASE_PARSE_GROUP(SENSOR_GROUP_0, 7, 26)
		CASE_PARSE_GROUP(SENSOR_GROUP_1, 7, 16)
		CASE_PARSE_GROUP(SENSOR_GROUP_2, 17, 20)
		CASE_PARSE_GROUP(SENSOR_GROUP_3, 21, 26)
		CASE_PARSE_GROUP(SENSOR_GROUP_4, 27, 34)
		CASE_PARSE_GROUP(SENSOR_GROUP_5, 35, 42)
		CASE_PARSE_GROUP(SENSOR_GROUP_6, 7, 42)
		CASE_PARSE_GROUP(SENSOR_GROUP_100, 7, 58)
		CASE_PARSE_GROUP(SENSOR_GROUP_101, 43, 58)
		CASE_PARSE_GROUP(SENSOR_GROUP_106, 46, 51)
		CASE_PARSE_GROUP(SENSOR_GROUP_107, 54, 58)

	case SENSOR_BUMPS_WHEELS_DROPS:
		{
			uchar v = *ptr++;
			wheeldropCaster_ = TEST(v ,SENSOR_BIT_WHEELDROP_CASTER);
			wheeldropLeft_ = TEST(v, SENSOR_BIT_WHEELDROP_LEFT);
			wheeldropRight_ = TEST(v, SENSOR_BIT_WHEELDROP_RIGHT);
			bumpLeft_ = TEST(v, SENSOR_BIT_BUMP_LEFT);
			bumpRight_ = TEST(v, SENSOR_BIT_BUMP_RIGHT);
		}break;

	CASE_PARSE_SINGLE(SENSOR_WALL, wall_);
	CASE_PARSE_SINGLE(SENSOR_CLIFF_LEFT, cliffLeft_);
	CASE_PARSE_SINGLE(SENSOR_CLIFF_FRONT_LEFT, cliffFrontLeft_);
	CASE_PARSE_SINGLE(SENSOR_CLIFF_FRONT_RIGHT, cliffFrontRight_);
	CASE_PARSE_SINGLE(SENSOR_CLIFF_RIGHT, cliffRight_);
	CASE_PARSE_SINGLE(SENSOR_VIRTUAL_WALL, virtualWall_);

	case SENSOR_OVERCURRENTS:
	{
		uchar v = *ptr++;
		leftWheelOvercurrent_ = TEST(v , SENSOR_BIT_LEFTWHEELOVERCURRENT);
		rightWheelOvercurrent_ = TEST(v , SENSOR_BIT_RIGHTWHEELOVERCURRENT);
		ld2Overcurrent_ = TEST(v , SENSOR_BIT_LD2OVERCURRENT);
		ld1Overcurrent_ = TEST(v , SENSOR_BIT_LD1OVERCURRENT);
		ld0Overcurrent_ = TEST(v , SENSOR_BIT_LD0OVERCURRENT);
		break;
	}

	case 15:
	case 16:
	{
		uchar v = *ptr++;
		break;
	}
	CASE_PARSE_SINGLE(SENSOR_IR, ir_);

	case SENSOR_BUTTONS:
	{
		uchar v = *ptr++;
		advanceButton_ = TEST(v , SENSOR_BIT_ADVANCEBUTTON);
		playButton_ = TEST(v , SENSOR_BIT_PLAYBUTTON);
		break;
	}

	CASE_PARSE_DOUBLE(SENSOR_DISTANCE, distance_);
	CASE_PARSE_DOUBLE(SENSOR_ANGLE, angle_);

	CASE_PARSE_SINGLE_TYPED(SENSOR_CHARGING_STATE, chargingState_,ChargingState);
	

	CASE_PARSE_DOUBLE(SENSOR_VOLTAGE, batteryVoltage_);
	CASE_PARSE_DOUBLE(SENSOR_CURRENT, batteryCurrent_);
	CASE_PARSE_SINGLE(SENSOR_BATTERY_TEMPERATURE, batteryTemperature_);
	CASE_PARSE_DOUBLE(SENSOR_BATTERY_CHARGE, batteryCharge_);
	CASE_PARSE_DOUBLE(SENSOR_BATTERY_CAPACITY, batteryCapacity_);
	CASE_PARSE_DOUBLE(SENSOR_WALL_SIGNAL, wallSignal_);
	CASE_PARSE_DOUBLE(SENSOR_CLIFF_LEFT_SIGNAL, cliffLeftSignal_);
	CASE_PARSE_DOUBLE(SENSOR_CLIFF_FRONT_LEFT_SIGNAL, cliffFrontLeftSignal_);
	CASE_PARSE_DOUBLE(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL, cliffFrontRightSignal_);
	CASE_PARSE_DOUBLE(SENSOR_CLIFF_RIGHT_SIGNAL, cliffRightSignal_);


	case SENSOR_CARGO_BAY_DIGITAL_INPUT:
	{
		uchar v = *ptr++;
		deviceDetect_ = TEST(v , SENSOR_BIT_DEVICEDETECT);
		digitalInput3_ = TEST(v, SENSOR_BIT_DIGITALINPUT3);
		digitalInput2_ = TEST(v, SENSOR_BIT_DIGITALINPUT2);
		digitalInput1_ = TEST(v, SENSOR_BIT_DIGITALINPUT1);
		digitalInput0_ = TEST(v, SENSOR_BIT_DIGITALINPUT0);
		break;
	}

		CASE_PARSE_DOUBLE(SENSOR_CARGO_BAY_ANALOG_SIGNAL, analogSignal_);

	case SENSOR_CHARGING_SOURCES_AVAILABLE:
	{
		uchar v = *ptr++;
		homeBaseChargerAvailable_ =
			TEST(v , SENSOR_BIT_HOMEBASECHARGERAVAILABLE);
		internalChargerAvailable_ =
			TEST(v , SENSOR_BIT_INTERNALCHARGERAVAILABLE);
		break;
	}

		CASE_PARSE_SINGLE_TYPED(SENSOR_OI_MODE, currentMode_, Mode);

		CASE_PARSE_SINGLE(SENSOR_SONG_NUMBER, songNumber_);
		CASE_PARSE_SINGLE(SENSOR_SONG_PLAYING, songPlaying_);
		CASE_PARSE_SINGLE(SENSOR_NUMBER_STREAM_PACKETS, streamPackets_);

		CASE_PARSE_DOUBLE(SENSOR_REQUESTED_VELOCITY, requestedVelocity_);
		CASE_PARSE_DOUBLE(SENSOR_REQUESTED_RADIUS, requestedRadius_);
		CASE_PARSE_DOUBLE(SENSOR_REQUESTED_LEFT_VELOCITY, requestedLeftVelocity_);
		CASE_PARSE_DOUBLE(SENSOR_REQUESTED_RIGHT_VELOCITY, requestedRightVelocity_);

		CASE_PARSE_DOUBLE(SENSOR_ENCODER_COUNTS_LEFT, encoderCountsLeft_);
		CASE_PARSE_DOUBLE(SENSOR_ENCODER_COUNTS_RIGHT, encoderCountsRight_);

		CASE_PARSE_SINGLE(SENSOR_LIGHT_BUMPER, lightBumper_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_LEFT, lightBumpLeft_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_FRONT_LEFT, lightBumpFrontLeft_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_CENTER_LEFT, lightBumpCenterLeft_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_CENTER_RIGHT, lightBumpCenterRight_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_FRONT_RIGHT, lightBumpFrontRight_);
		CASE_PARSE_DOUBLE(SENSOR_LIGHT_BUMP_RIGHT, lightBumpRight_);

		CASE_PARSE_SINGLE(SENSOR_IR_OPCODE_LEFT, irOpcodeLeft_);
		CASE_PARSE_SINGLE(SENSOR_IR_OPCODE_RIGHT, irOpcodeRight_);

		CASE_PARSE_DOUBLE(SENSOR_LEFT_MOTOR_CURRENT, leftMotorCurrent_);
		CASE_PARSE_DOUBLE(SENSOR_RIGHT_MOTOR_CURRENT, rightMotorCurrent_);
		CASE_PARSE_DOUBLE(SENSOR_MAIN_BRUSH_CURRENT, mainBrushCurrent_);
		CASE_PARSE_DOUBLE(SENSOR_SIDE_BRUSH_CURRENT, sideBrushCurrent_);

	case SENSOR_STASIS:
	{
		uchar v = *ptr++;
		stasis_ =
			TEST(v, 0);
		break;
	}
	default:
		break;
	}
	return ptr;
}

void RoombaControllerImpl::_ProcessPacket(uchar* p)
{

	if (queriedSensors_.size() != 0)
	{
		SensorPacket q = queriedSensors_[0];
		queriedSensors_.erase(queriedSensors_.begin());
		_ParseSensorPacket(q, p);
	}
}


void RoombaControllerImpl::OnSerialPortDataArrived(OS::ISerialPort* sp)
{
	int count = m_serialPort->AvailableData();
	if (!count)
		return;

	uchar* data = new uchar[count];
	int n=m_serialPort->Read(data, count);
	DataPacket p;
	p.size = n;
	p.data = data;


	m_toProcessMutex->lock();
	toProcess_.push_back(p);
	m_toProcessMutex->unlock();

}

bool RoombaControllerImpl::Connect(const core::string& port)
{
	Disconnect();
	try
	{
		m_serialPort->OpenByName(port, OS::ISerialPort::EBaud115200);
	}
	catch (std::exception* e)
	{
		return false;
	}
	SendStartCommand();
	Sleep(100);	
	if (IsConnected())
	{
		SendFullModeCommand();

	}
	return IsConnected();
}
bool RoombaControllerImpl::IsConnected()
{
	return m_serialPort->IsOpen();
}
void RoombaControllerImpl::Disconnect()
{
	if (IsConnected())
		SendStartCommand();//switch to passive mode before disconnecting
	m_serialPort->Close();

	queriedSensors_.clear();
	for (int i = 0; i < toProcess_.size(); ++i)
		delete [] toProcess_[i].data;
	toProcess_.clear();
}

//Switch to passive mode.
void RoombaControllerImpl::SendStartCommand()
{

// 	if (currentMode_ != IROBOT_CREATE_OFF)
// 		return;
	const unsigned char op = OPCODE_START;
	SendByte(m_serialPort, op);
	currentMode_ = IROBOT_CREATE_PASSIVE;
}

//Switch to safe mode.
void RoombaControllerImpl::SendSafeCommand()
{
	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;
	const unsigned char op = OPCODE_SAFE;
	SendByte(m_serialPort, op);
	currentMode_ = IROBOT_CREATE_SAFE;

}

//Switch to full mode.
void RoombaControllerImpl::SendFullModeCommand()
{
	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;
	const unsigned char op = OPCODE_FULL;
	SendByte(m_serialPort, op);
	currentMode_ = IROBOT_CREATE_FULL;

}


void RoombaControllerImpl::Drive(const math::vector2di& speed, int radius)
{

	int velocity = speed.x;
	if (currentMode_ < IROBOT_CREATE_SAFE)
		return;
	velocity = math::clamp(velocity, VELOCITY_MIN, VELOCITY_MAX);
	radius = math::clamp(radius, RADIUS_MIN, RADIUS_MAX);

	const unsigned char op = OPCODE_DRIVE;

	SendByte(m_serialPort, op);
	sendInt16(m_serialPort, velocity);
	sendInt16(m_serialPort, radius);
}
void RoombaControllerImpl::DriveStop()
{
	Drive(0, 0);
}
//drive each wheel seperately
void RoombaControllerImpl::DriveWheels(int leftWheel, int rightWheel)
{
	if (currentMode_ < IROBOT_CREATE_SAFE)
		return;
	if (leftWheel == m_leftWheel && rightWheel == m_rightWheel)
		return;
	m_leftWheel = leftWheel;
	m_rightWheel = rightWheel;
	leftWheel = math::clamp(leftWheel, VELOCITY_MIN, VELOCITY_MAX);
	rightWheel = math::clamp(rightWheel, VELOCITY_MIN, VELOCITY_MAX);

	const unsigned char op = OPCODE_DRIVE_DIRECT;
	SendByte(m_serialPort, op);
	sendInt16(m_serialPort, rightWheel);
	sendInt16(m_serialPort, leftWheel);

}

///  Change led status.
///  Switch on/off advance or play leds.
///  c Power led color.
///  i Power led light intensity.
void RoombaControllerImpl::SetLED(Led l, int c, int i)
{
	if (currentMode_ < IROBOT_CREATE_SAFE)
		return;
	if (l < LED_NONE || l > LED_ALL)
		return;

	const unsigned char b = l;
	const unsigned char op[] = { OPCODE_LEDS, b, c, i };
	m_serialPort->Write(op, sizeof(op));
}

//sid Song id.
void RoombaControllerImpl::SetSong(unsigned char sid, const song_t& song)
{
	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;
	if (sid > SONG_MAX)
		return;
	if (song.size() > SONG_MAX_SIZE)
		return;

	std::stringstream ss;
	for (song_t::const_iterator it = song.begin(); it != song.end(); ++it)
	{
		if (it->first < NO_NOTE || it->first > NOTE_MAX)
			return;
		ss << it->first << it->second;
	}
	const unsigned char size = song.size();
	const unsigned char op[] = { OPCODE_SONG, sid, size };
	m_serialPort->Write(op, sizeof(op));
	m_serialPort->Write(ss.str().c_str(), size);

}

void RoombaControllerImpl::PlaySong(uint v)
{
	if (currentMode_ < IROBOT_CREATE_SAFE)
		return;
	if (v > SONG_MAX)
		return;

	const unsigned char op[] = { OPCODE_PLAY, v };
	m_serialPort->Write(op, sizeof(op));

}

void RoombaControllerImpl::RequestSensorPacket(SensorPacket packet)
{
	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;
// 	if (packet < SENSOR_GROUP_0
// 		|| packet > SENSOR_REQUESTED_LEFT_VELOCITY)
// 		return;

	for (int i = 0; i < queriedSensors_.size(); ++i)
	{
	//	if (queriedSensors_[i] == packet)
	//		return;
	}

	const unsigned char p = packet;
	const unsigned char op[] = { OPCODE_SENSORS, p };
	m_serialPort->Write(op, sizeof(op));
	queriedSensors_.push_back(packet);

}


void RoombaControllerImpl::QueryListCommand(const sensorPackets_t& packets)
{

	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;
	if (packets.size() > 255)
		return;
	std::stringstream ss;
	for (sensorPackets_t::const_iterator it = packets.begin();
		it != packets.end(); ++it)
	{
		if (*it < SENSOR_GROUP_0
			|| *it > SENSOR_REQUESTED_LEFT_VELOCITY)
			continue;
// 		bool flag = false;
// 		for (int i = 0; i < queriedSensors_.size(); ++i)if (queriedSensors_[i] == *it){ flag = true; break; }
// 		if (flag)continue;
// 		++total;
		const unsigned char p = *it;
		ss << p;
		queriedSensors_.push_back(*it);
	}
	const unsigned char size = packets.size();
	const unsigned char op[] = { (OPCODE_QUERY_LIST), size };
	m_serialPort->Write(op, sizeof(op));
	m_serialPort->Write(ss.str().c_str(), size);


}

// t is in tenth of second
void RoombaControllerImpl::RobotWait(uchar t)
{

	if (currentMode_ < IROBOT_CREATE_PASSIVE)
		return;

	const unsigned char op[] = { OPCODE_WAIT_TIME, t };
	m_serialPort->Write(op, sizeof(op));
}

void RoombaControllerImpl::UpdateSensors()
{
	while (true)
	{
		if (queriedSensors_.empty())
			break;
		SensorPacket q = queriedSensors_[0];
		int packetSize = 0;
		if (q >= SENSOR_GROUP_100 && q <= SENSOR_GROUP_107)
			packetSize = s_PacketGroupSize100[q - SENSOR_GROUP_100];
		else
		{
			packetSize= s_PacketGroupSize[q];
		}
		uchar* p = _GetPacketData(packetSize);
		if (!p)
			break;
		_ParseSensorPacket(q, p);
		queriedSensors_.erase(queriedSensors_.begin());
		delete[] p;
	}

	RequestSensorPacket(SENSOR_GROUP_0);
	RequestSensorPacket(SENSOR_GROUP_106);
}

#define MK_SENSOR_GETTER(TYPE, NAME)            \
  TYPE                                          \
  RoombaControllerImpl::NAME ()                               \
  {                                             \
    return NAME##_;                             \
  }

MK_SENSOR_GETTER(bool, wheeldropCaster)
MK_SENSOR_GETTER(bool, wheeldropLeft)
MK_SENSOR_GETTER(bool, wheeldropRight)
MK_SENSOR_GETTER(bool, bumpLeft)
MK_SENSOR_GETTER(bool, bumpRight)
MK_SENSOR_GETTER(bool, wall)
MK_SENSOR_GETTER(bool, cliffLeft)
MK_SENSOR_GETTER(bool, cliffFrontLeft)
MK_SENSOR_GETTER(bool, cliffFrontRight)
MK_SENSOR_GETTER(bool, cliffRight)
MK_SENSOR_GETTER(bool, deviceDetect)
MK_SENSOR_GETTER(bool, digitalInput3)
MK_SENSOR_GETTER(bool, digitalInput2)
MK_SENSOR_GETTER(bool, digitalInput1)
MK_SENSOR_GETTER(bool, digitalInput0)
MK_SENSOR_GETTER(short, analogSignal)
MK_SENSOR_GETTER(bool, homeBaseChargerAvailable)
MK_SENSOR_GETTER(bool, internalChargerAvailable)
MK_SENSOR_GETTER(bool, virtualWall)
MK_SENSOR_GETTER(bool, leftWheelOvercurrent)
MK_SENSOR_GETTER(bool, rightWheelOvercurrent)
MK_SENSOR_GETTER(bool, ld2Overcurrent)
MK_SENSOR_GETTER(bool, ld1Overcurrent)
MK_SENSOR_GETTER(bool, ld0Overcurrent)
MK_SENSOR_GETTER(unsigned char, ir)
MK_SENSOR_GETTER(bool, advanceButton)
MK_SENSOR_GETTER(bool, playButton)
MK_SENSOR_GETTER(short, distance)
MK_SENSOR_GETTER(short, angle)
MK_SENSOR_GETTER(RoombaControllerImpl::ChargingState, chargingState)
MK_SENSOR_GETTER(short, batteryVoltage)
MK_SENSOR_GETTER(short, batteryCurrent)
MK_SENSOR_GETTER(short, batteryTemperature)
MK_SENSOR_GETTER(short, batteryCharge)
MK_SENSOR_GETTER(short, batteryCapacity)
MK_SENSOR_GETTER(short, wallSignal)
MK_SENSOR_GETTER(short, cliffLeftSignal)
MK_SENSOR_GETTER(short, cliffFrontLeftSignal)
MK_SENSOR_GETTER(short, cliffFrontRightSignal)
MK_SENSOR_GETTER(short, cliffRightSignal)
MK_SENSOR_GETTER(unsigned char, songNumber)
MK_SENSOR_GETTER(bool, songPlaying)
MK_SENSOR_GETTER(unsigned char, streamPackets)
MK_SENSOR_GETTER(short, requestedVelocity)
MK_SENSOR_GETTER(short, requestedRadius)
MK_SENSOR_GETTER(short, requestedLeftVelocity)
MK_SENSOR_GETTER(short, requestedRightVelocity)

MK_SENSOR_GETTER(unsigned short, lightBumper);
MK_SENSOR_GETTER(unsigned short, lightBumpLeft)
MK_SENSOR_GETTER(unsigned short, lightBumpFrontLeft)
MK_SENSOR_GETTER(unsigned short, lightBumpCenterLeft)
MK_SENSOR_GETTER(unsigned short, lightBumpCenterRight)
MK_SENSOR_GETTER(unsigned short, lightBumpFrontRight)
MK_SENSOR_GETTER(unsigned short, lightBumpRight)


//////////////////////////////////////////////////////////////////////////



bool RoombaController::Connect(const core::string& port)
{
	return m_impl->Connect(port);
}
bool RoombaController::IsConnected()
{
	return m_impl->IsConnected();
}
void RoombaController::Disconnect()
{
	 m_impl->Disconnect();

}
bool RoombaController::IsStarted()
{
	return m_impl->GetMode() == RoombaControllerImpl::IROBOT_CREATE_FULL;
}

void RoombaController::Start()
{
	m_impl->SendFullModeCommand();
}
void RoombaController::Stop()
{
	m_impl->SendStartCommand();
}
void RoombaController::Drive(const math::vector2di& speed, int rotationSpeed)
{
	int l = 0;
	int r = 0;

	bool bl = m_impl->bumpLeft();
	bool br = m_impl->bumpRight();

	const math::vector2d frontVec = math::vector2d(1, 0);

	float angle = 180;
	float step = 160.0f / 5.0f;
	angle -= 10.0f;//starting angle
	math::vector2d lightVec[6];
	unsigned short irLight[6] = {
		m_impl->lightBumpLeft(),
		m_impl->lightBumpFrontLeft(),
		m_impl->lightBumpCenterLeft(),
		m_impl->lightBumpCenterRight(),
		m_impl->lightBumpFrontRight(),
		m_impl->lightBumpRight()
	};
	for (int i = 0; i < 6; ++i)
	{
		lightVec[i].x = math::sind(angle);
		lightVec[i].y = math::cosd(angle);
		angle -= step;
	}



	math::vector2d antiVec ;
	math::vector2d antiVec2;
	float totalW = 0;
	for (int i = 0; i < 6; ++i)
	{
		float w = (float)irLight[i] / 4095.0f;
		antiVec2 += lightVec[i] * w;
		totalW += sqrtf(w)*lightVec[i].x;
	}
	//if (totalW > 0.1)
	//	antiVec2 /= totalW;

	float minW = 0.4;
	float maxW = 0.8;
	if (totalW>minW)
	{
		if (totalW < maxW)
		{
			antiVec2 = frontVec*(totalW + 1-maxW);
		}
		else
		{
			antiVec2 = frontVec;
		}
	}
//	antiVec.Normalize();
	
	antiVec = antiVec2;
	if (bl || br)
		antiVec = frontVec;
	math::vector2d s = speed ;
	antiVec = antiVec*s.Length();

	s -= antiVec;

	l = r = s.x;
	l -= rotationSpeed;
	r += rotationSpeed;

// 	l /= 2;
// 	r /= 2;
	 m_impl->DriveWheels(l,r);

}
void RoombaController::DriveStop()
{
	m_impl->DriveStop();
}


void RoombaController::UpdateSensors()
{
	m_impl->UpdateSensors();
}

int RoombaController::GetSensorCount(){ 
	return 2; 
}
float RoombaController::GetSensorValue(int s){
	switch (s)
	{
	case 0:
		return m_impl->bumpLeft();
	case 1:
		return m_impl->bumpRight();
	case 2:
		return m_impl->lightBumpLeft();
	case 3:
		return m_impl->lightBumpFrontLeft();
	case 4:
		return m_impl->lightBumpCenterLeft();
	case 5:
		return m_impl->lightBumpCenterRight();
	case 6:
		return m_impl->lightBumpFrontRight();
	case 7:
		return m_impl->lightBumpRight();
	}
	return 0; 
}
int RoombaController::GetBatteryLevel(){ 

	short cap = m_impl->batteryCapacity();
	short charge= m_impl->batteryCharge();
	if (cap == 0)
		return 1;
	return 100 * (float)charge / (float)cap;
}


std::string RoombaController::ExecCommand(const core::string& cmd, const core::string& args)
{
	if (cmd == IRobotController::CMD_GetBatteryCharge)
	{
		switch (m_impl->chargingState())
		{
		case RoombaControllerImpl::CHARGING_STATE_NOT_CHARGING:
			return "NotCharging";
		case RoombaControllerImpl::CHARGING_STATE_RECONDITIONING_CHARGING:
			return "Recondition";
		case RoombaControllerImpl::CHARGING_STATE_FULL_CHARGING:
			return "FullCharging";
		case RoombaControllerImpl::CHARGING_STATE_TRICKLE_CHARGING:
			return "TrickleCharging";
		case RoombaControllerImpl::CHARGING_STATE_WAITING:
			return "Waiting";
		case RoombaControllerImpl::CHARGING_STATE_CHARGING_FAULT_CONDITION:
			return "Fault";
		}
	}
	return "";
}

}





