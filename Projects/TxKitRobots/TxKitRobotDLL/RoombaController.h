


#ifndef RoombaController_h__
#define RoombaController_h__

#include <queue>
#include <vector>
#include "ISerialPort.h"
#include "IBaseController.h"

namespace mray
{
	
class RoombaControllerImpl
{
protected:




public:
	enum Led
	{
		/// No led.
		LED_NONE = 0,
		/// Play led only.
		LED_PLAY = 2,
		/// Advance led only.
		LED_ADVANCE = 8,
		/// All leds.
		LED_ALL = LED_PLAY | LED_ADVANCE
	};

	enum SensorPacket
	{
		/// Groups packets 7 to 26.
		SENSOR_GROUP_0 = 0,
		/// Groups packets 7 to 16.
		SENSOR_GROUP_1 = 1,
		/// Groups packets 17 to 20.
		SENSOR_GROUP_2 = 2,
		/// Groups packets 21 to 26.
		SENSOR_GROUP_3 = 3,
		/// Groups packets 27 to 34.
		SENSOR_GROUP_4 = 4,
		/// Groups packets 35 to 42.
		SENSOR_GROUP_5 = 5,
		/// Groups packets 7 to 42.
		SENSOR_GROUP_6 = 6,
		SENSOR_GROUP_100 = 100,
		SENSOR_GROUP_101 = 101,
		SENSOR_GROUP_106 = 106,
		SENSOR_GROUP_107 = 107,
		/// Wheel and bumper states.
		SENSOR_BUMPS_WHEELS_DROPS = 7,
		/// Wall sensor state.
		SENSOR_WALL = 8,
		/// Left cliff sensor state.
		SENSOR_CLIFF_LEFT = 9,
		/// Front left cliff sensor state.
		SENSOR_CLIFF_FRONT_LEFT = 10,
		/// Front right cliff sensor state.
		SENSOR_CLIFF_FRONT_RIGHT = 11,
		/// Right cliff sensor state.
		SENSOR_CLIFF_RIGHT = 12,
		/// Virtual wall sensor state.
		SENSOR_VIRTUAL_WALL = 13,
		/// Overcurrent sensors states.
		SENSOR_OVERCURRENTS = 14,
		/// IR bytes received.
		SENSOR_IR = 17,
		/// Buttons states.
		SENSOR_BUTTONS = 18,
		/// Traveled distance since last read.
		SENSOR_DISTANCE = 19,
		/// Turned angle since last read.
		SENSOR_ANGLE = 20,
		/// Charging state.
		SENSOR_CHARGING_STATE = 21,
		/// Battery voltage.
		SENSOR_VOLTAGE = 22,
		/// Batty current.
		SENSOR_CURRENT = 23,
		/// Battery temperature.
		SENSOR_BATTERY_TEMPERATURE = 24,
		/// Battery charge in milliamp-hours (mAh).
		SENSOR_BATTERY_CHARGE = 25,
		/// Battery charge capacity in milliamp-hours (mAh).
		SENSOR_BATTERY_CAPACITY = 26,
		/// Wall's sensor signal strength.
		SENSOR_WALL_SIGNAL = 27,
		/// Left cliff signal strength.
		SENSOR_CLIFF_LEFT_SIGNAL = 28,
		/// Front left cliff signal strength.
		SENSOR_CLIFF_FRONT_LEFT_SIGNAL = 29,
		/// Front right cliff signal strength.
		SENSOR_CLIFF_FRONT_RIGHT_SIGNAL = 30,
		/// Right cliff signal strength.
		SENSOR_CLIFF_RIGHT_SIGNAL = 31,
		/// Cargo Bay digital input strength.
		SENSOR_CARGO_BAY_DIGITAL_INPUT = 32,
		/// Cargo Bay analog input strength.
		SENSOR_CARGO_BAY_ANALOG_SIGNAL = 33,
		/// Available charging sources.
		SENSOR_CHARGING_SOURCES_AVAILABLE = 34,
		/// Current Open Interface mode.
		SENSOR_OI_MODE = 35,
		/// Current selected song.
		SENSOR_SONG_NUMBER = 36,
		/// Indicates whether or not a song is being played.
		SENSOR_SONG_PLAYING = 37,
		/// List of streamed packets.
		SENSOR_NUMBER_STREAM_PACKETS = 38,
		/// Requested velocity.
		SENSOR_REQUESTED_VELOCITY = 39,
		/// Requested radius.
		SENSOR_REQUESTED_RADIUS = 40,
		/// Requested right velocity.
		SENSOR_REQUESTED_RIGHT_VELOCITY = 41,
		/// Requested left velocity.
		SENSOR_REQUESTED_LEFT_VELOCITY = 42,
		SENSOR_ENCODER_COUNTS_LEFT = 43,
		SENSOR_ENCODER_COUNTS_RIGHT = 44,
		SENSOR_LIGHT_BUMPER = 45,
		SENSOR_LIGHT_BUMP_LEFT = 46,
		SENSOR_LIGHT_BUMP_FRONT_LEFT = 47,
		SENSOR_LIGHT_BUMP_CENTER_LEFT = 48,
		SENSOR_LIGHT_BUMP_CENTER_RIGHT = 49,
		SENSOR_LIGHT_BUMP_FRONT_RIGHT = 50,
		SENSOR_LIGHT_BUMP_RIGHT = 51,
		SENSOR_IR_OPCODE_LEFT = 52,
		SENSOR_IR_OPCODE_RIGHT = 53,
		SENSOR_LEFT_MOTOR_CURRENT = 54,
		SENSOR_RIGHT_MOTOR_CURRENT = 55,
		SENSOR_MAIN_BRUSH_CURRENT = 56,
		SENSOR_SIDE_BRUSH_CURRENT = 57,
		SENSOR_STASIS = 58
	};
	enum ChargingState
	{
		CHARGING_STATE_NOT_CHARGING = 0,
		CHARGING_STATE_RECONDITIONING_CHARGING = 1,
		CHARGING_STATE_FULL_CHARGING = 2,
		CHARGING_STATE_TRICKLE_CHARGING = 3,
		CHARGING_STATE_WAITING = 4,
		CHARGING_STATE_CHARGING_FAULT_CONDITION = 5

	};
	/// \brief iRobot Create modes
	/// The mode defines which commands are available and
	/// how the robot behaves when some events happens
	/// like wheel drop or if a cliff is detected.
	enum Mode
	{
		/// \brief Start-up mode, no command can be sent except start.
		/// \see Create::sendStartCommand
		IROBOT_CREATE_OFF,
		/// Passive mode, actuators can not be controlled.
		IROBOT_CREATE_PASSIVE,
		/// \brief Safe mode, all commands are available.
		/// iRobot Create will stop if the wheels are dropped,
		/// if a cliff is detected or if the charger is plugged in
		/// and powered.
		IROBOT_CREATE_SAFE,
		/// \brief Full mode, all commands are available.
		/// No safety check is done in this mode. The robot
		/// may fall from cliffs and will keep running if the
		/// wheels are dropped.
		IROBOT_CREATE_FULL
	};
	typedef std::pair<unsigned char, unsigned char> note_t;
	/// \brief Define a song (a list of notes).
	/// \see note_t
	typedef std::vector<note_t> song_t;
	typedef std::vector<SensorPacket> sensorPackets_t;
	typedef std::vector<SensorPacket> queriedPackets_t;

public:
	RoombaControllerImpl();
	virtual ~RoombaControllerImpl();

	bool Connect(const core::string& port);
	bool IsConnected();
	void Disconnect();

	//Switch to passive mode.
	void SendStartCommand();

	//Switch to safe mode.
	void SendSafeCommand();

	//Switch to full mode.
	void SendFullModeCommand();

	Mode GetMode(){ return currentMode_; }

	virtual void Drive(const math::vector2di& speed, int rotationSpeed) ;
	virtual void DriveStop() ;

	//drive each wheel seperately
	void DriveWheels(int leftWheel, int rightWheel);

	///  Change led status.
	///  Switch on/off advance or play leds.
	///  c Power led color.
	///  i Power led light intensity.
	void SetLED(Led l,int c,int i);

	//sid Song id.
	void SetSong(unsigned char sid, const song_t& song);

	void PlaySong(uint sid);

	void RequestSensorPacket(SensorPacket type);
	
	void QueryListCommand(const sensorPackets_t& p);

	// t is in tenth of second
	void RobotWait(uchar t);

	void UpdateSensors();

	bool wheeldropCaster();
	bool wheeldropLeft();
	bool wheeldropRight();
	bool bumpLeft();
	bool bumpRight();

	/// Get wall sensor value.
	bool wall();

	bool cliffLeft();
	bool cliffFrontLeft();
	bool cliffFrontRight();
	bool cliffRight();
	bool deviceDetect();
	bool digitalInput3();
	bool digitalInput2();
	bool digitalInput1();
	bool digitalInput0();
	short analogSignal();
	bool homeBaseChargerAvailable();
	bool internalChargerAvailable();
	bool virtualWall();
	bool leftWheelOvercurrent();
	bool rightWheelOvercurrent();
	bool ld2Overcurrent();
	bool ld1Overcurrent();
	bool ld0Overcurrent();
	unsigned char ir();
	bool advanceButton();
	bool playButton();
	short distance();
	short angle();
	ChargingState chargingState();
	short batteryVoltage();
	short batteryCurrent();
	short batteryTemperature();
	short batteryCharge();
	short batteryCapacity();
	short wallSignal();
	short cliffLeftSignal();
	short cliffFrontLeftSignal();
	short cliffFrontRightSignal();
	short cliffRightSignal();
	unsigned char songNumber();
	bool songPlaying();
	unsigned char streamPackets();
	short requestedVelocity();
	short requestedRadius();
	short requestedLeftVelocity();
	short requestedRightVelocity();

	unsigned short lightBumper();
	unsigned short lightBumpLeft();
	unsigned short lightBumpFrontLeft();
	unsigned short lightBumpCenterLeft();
	unsigned short lightBumpCenterRight();
	unsigned short lightBumpFrontRight();
	unsigned short lightBumpRight();

protected:

	void OnSerialPortDataArrived(OS::ISerialPort* sp);
	uchar* _ParseSensorPacket(SensorPacket p, uchar* data);
	void _ProcessPacket(uchar* p);
	uchar* _GetPacketData(int size);

	OS::ISerialPort* m_serialPort;
	queriedPackets_t queriedSensors_;

	/// List of sensors currently streamed.
	sensorPackets_t streamedSensors_;

	Mode currentMode_;
	struct DataPacket
	{
		uchar* data;
		int size;
	};
	std::vector<DataPacket> toProcess_;
	OS::IMutex* m_toProcessMutex;

	int m_leftWheel;
	int m_rightWheel;

	/// Wheeldrop caster
	bool wheeldropCaster_;
	/// Wheeldrop left
	bool wheeldropLeft_;
	/// Wheeldrop right
	bool wheeldropRight_;
	/// Bump left
	bool bumpLeft_;
	/// Bump right
	bool bumpRight_;
	/// Wall sensor.
	bool wall_;
	/// Left cliff sensor.
	bool cliffLeft_;
	/// Front left cliff sensor.
	bool cliffFrontLeft_;
	/// Front right cliff sensor.
	bool cliffFrontRight_;
	/// Right cliff sensor.
	bool cliffRight_;
	/// Device detect pin state.
	bool deviceDetect_;
	/// Digital input 3 pin state.
	bool digitalInput3_;
	/// Digital input 2 pin state.
	bool digitalInput2_;
	/// Digital input 1 pin state.
	bool digitalInput1_;
	/// Digital input 0 pin state.
	bool digitalInput0_;
	/// Analog signal pin state.
	short analogSignal_;
	/// Is the home base charger available?
	bool homeBaseChargerAvailable_;
	/// Is the internal charger available?
	bool internalChargerAvailable_;
	/// Virtual wall sensor.
	bool virtualWall_;
	/// Left wheel overcurrent (true = overcurrent).
	bool leftWheelOvercurrent_;
	/// Right wheel overcurrent (true = overcurrent).
	bool rightWheelOvercurrent_;
	/// LD2 overcurrent (true = overcurrent).
	bool ld2Overcurrent_;
	/// LD1 overcurrent (true = overcurrent).
	bool ld1Overcurrent_;
	/// LD0 overcurrent (true = overcurrent).
	bool ld0Overcurrent_;
	/// Ir sensor.
	unsigned char ir_;
	/// Advance button state (true = pushed).
	bool advanceButton_;
	/// Play button (true = pushed).
	bool playButton_;
	/// Distance sensor.
	short distance_;
	/// Angle sensor.
	short angle_;
	/// Current charging state.
	ChargingState chargingState_;
	/// Battery voltage.
	short batteryVoltage_;
	/// Battery current.
	short batteryCurrent_;
	/// Battery temperature.
	short batteryTemperature_;
	/// Battery charge.
	short batteryCharge_;
	/// Battery capacity.
	short batteryCapacity_;
	/// Distance from wall.
	short wallSignal_;
	/// Distance from cliff (left).
	short cliffLeftSignal_;
	/// Distance from cliff (front left).
	short cliffFrontLeftSignal_;
	/// Distance from cliff (front right).
	short cliffFrontRightSignal_;
	/// Distance from cliff (right).
	short cliffRightSignal_;
	/// Current song number.
	unsigned char songNumber_;
	/// Is a song currently played?
	bool songPlaying_;
	/// Number of streamed packets.
	unsigned char streamPackets_;
	/// Last requested velocity (drive command).
	short requestedVelocity_;
	/// Last requested radius (drive command).
	short requestedRadius_;
	/// Last requested left velocity (drive direct command).
	short requestedLeftVelocity_;
	/// Last requested right velocity (drive direct command).
	short requestedRightVelocity_;

	unsigned short encoderCountsLeft_;
	unsigned short encoderCountsRight_;
	unsigned char lightBumper_;
	unsigned short lightBumpLeft_;
	unsigned short lightBumpFrontLeft_;
	unsigned short lightBumpCenterLeft_;
	unsigned short lightBumpCenterRight_;
	unsigned short lightBumpFrontRight_;
	unsigned short lightBumpRight_;
	unsigned char irOpcodeLeft_;
	unsigned char irOpcodeRight_;
	short leftMotorCurrent_;
	short rightMotorCurrent_;
	short mainBrushCurrent_;
	short sideBrushCurrent_;
	bool stasis_;

	/// \brief Enumerate available opcodes.
	/// Opcode are the basics instructions that the protocol
	/// support. This class wraps them to avoid using them directly.
	/// The only case where you have to use them is when you want
	/// to send an Open Interface script.
	enum Opcode
	{
		/// Start command.
		OPCODE_START = 128,
		/// Baud command.
		OPCODE_BAUD = 129,
		/// Control command (equivalent to safe).
		OPCODE_CONTROL = 130,
		/// Safe command.
		OPCODE_SAFE = 131,
		/// Full command.
		OPCODE_FULL = 132,
		/// Spot demo command.
		OPCODE_SPOT = 134,
		/// Cover demo command.
		OPCODE_COVER = 135,
		/// Demo command.
		OPCODE_DEMO = 136,
		/// Driver command.
		OPCODE_DRIVE = 137,
		/// Low side drivers command.
		OPCODE_LOW_SIDE_DRIVERS = 138,
		/// Leds command.
		OPCODE_LEDS = 139,
		/// Song command.
		OPCODE_SONG = 140,
		/// Play song command.
		OPCODE_PLAY = 141,
		/// Sensors command.
		OPCODE_SENSORS = 142,
		/// Cover and dock demo command.
		OPCODE_COVER_AND_DOCK = 143,
		/// Pwm low side drivers command.
		OPCODE_PWM_LOW_SIDE_DRIVERS = 144,
		/// Driver direct command.
		OPCODE_DRIVE_DIRECT = 145,
		/// Driver direct PWM command.
		OPCODE_DRIVE_DIRECT_PWM = 146,
		/// Digital ouputs command.
		OPCODE_DIGITAL_OUTPUTS = 147,
		/// Stream command.
		OPCODE_STREAM = 148,
		/// Query list command.
		OPCODE_QUERY_LIST = 149,
		/// Pause/resume stream command.
		OPCODE_PAUSE_RESUME_STREAM = 150,
		/// Send IR command.
		OPCODE_SEND_IR = 151,
		/// Script command.
		OPCODE_SCRIPT = 152,
		/// Play script command.
		OPCODE_PLAY_SCRIPT = 153,
		/// Show script command.
		OPCODE_SHOW_SCRIPT = 154,
		/// Wait time command.
		OPCODE_WAIT_TIME = 155,
		/// Wait distance command.
		OPCODE_WAIT_DISTANCE = 156,
		/// Wait angle command.
		OPCODE_WAIT_ANGLE = 157,
		/// Wait event command.
		OPCODE_WAIT_EVENT = 158
	};

public:

	/// Define the green color for power led.
	static const unsigned char LED_COLOR_GREEN = 0;
	/// Define the red color for power led.
	static const unsigned char LED_COLOR_RED = 255;

	/// Define minimum intensity for power led (off).
	static const unsigned char LED_INTENSITY_OFF = 0;
	/// Define full intensity for power led.
	static const unsigned char LED_INTENSITY_FULL = 255;

	/// Define minimum velocity for low side drivers.
	static const unsigned char LOW_SIDE_VELOCITY_MIN = 0;

	/// Define maximum velocity for low side drivers.
	static const unsigned char LOW_SIDE_VELOCITY_MAX = 128;

	/// Define minimum velocity for robot wheels motors.
	static const int VELOCITY_MIN = -500;
	/// Define maximum velocity for robot wheels motors.
	static const int VELOCITY_MAX = 500;

	/// Define minimum radius turn of the robot.
	static const int RADIUS_MIN = -2000;
	/// Define maximum radius turn of the robot.
	static const int RADIUS_MAX = 2000;

	/// Define minimum song id.
	static const unsigned char SONG_MIN = 0;
	/// Define maximum song id.
	static const unsigned char SONG_MAX = 15;

	/// Define song maximum size.
	static const unsigned char SONG_MAX_SIZE = 16;

	/// Define the value for a rest node (no sound).
	static const unsigned char NO_NOTE = 30;
	/// Define the minimum note (G).
	static const unsigned char NOTE_MIN = 31;
	/// Define the maximum note (G).
	static const unsigned char NOTE_MAX = 127;

	/// Define maximum script size.
	static const unsigned char SCRIPT_MAX_SIZE = 100;

	/// Stream header ``magic value''.
	static const unsigned char STREAM_HEADER = 19;


	/// Bit used to retrieve the wheeldrop caster status.
	static const unsigned char SENSOR_BIT_WHEELDROP_CASTER = 4;
	/// Bit used to retrieve the wheeldrop caster status.
	static const unsigned char SENSOR_BIT_WHEELDROP_LEFT = 3;
	/// Bit used to retrieve the wheeldrop caster status.
	static const unsigned char SENSOR_BIT_WHEELDROP_RIGHT = 2;
	/// Bit used to retrieve the bump left status.
	static const unsigned char SENSOR_BIT_BUMP_LEFT = 1;
	/// Bit used to retrieve the bump right status.
	static const unsigned char SENSOR_BIT_BUMP_RIGHT = 0;

	/// Bit used to retrieve the left wheel overcurrent status.
	static const unsigned char SENSOR_BIT_LEFTWHEELOVERCURRENT = 4;
	/// Bit used to retrieve the right wheel overcurrent status.
	static const unsigned char SENSOR_BIT_RIGHTWHEELOVERCURRENT = 3;
	/// Bit used to retrieve the LD2 overcurrent status.
	static const unsigned char SENSOR_BIT_LD2OVERCURRENT = 2;
	/// Bit used to retrieve the LD1 overcurrent status.
	static const unsigned char SENSOR_BIT_LD1OVERCURRENT = 1;
	/// Bit used to retrieve the LD0 overcurrent status.
	static const unsigned char SENSOR_BIT_LD0OVERCURRENT = 0;

	/// Bit used to retrieve the advance button status.
	static const unsigned char SENSOR_BIT_ADVANCEBUTTON = 2;
	/// Bit used to retrieve the play button status.
	static const unsigned char SENSOR_BIT_PLAYBUTTON = 0;

	/// Bit used to retrieve the device detect pin status.
	static const unsigned char SENSOR_BIT_DEVICEDETECT = 4;
	/// Bit used to retrieve the digital input 3 pin status.
	static const unsigned char SENSOR_BIT_DIGITALINPUT3 = 3;
	/// Bit used to retrieve the digital input 2 pin status.
	static const unsigned char SENSOR_BIT_DIGITALINPUT2 = 2;
	/// Bit used to retrieve the digital input 1 pin status.
	static const unsigned char SENSOR_BIT_DIGITALINPUT1 = 1;
	/// Bit used to retrieve the digital input 0 pin status.
	static const unsigned char SENSOR_BIT_DIGITALINPUT0 = 0;

	/// Bit used to retrieve whether the home base charger is available or not.
	static const unsigned char SENSOR_BIT_HOMEBASECHARGERAVAILABLE = 1;
	/// Bit used to retrieve whether the internal charger is available or not.
	static const unsigned char SENSOR_BIT_INTERNALCHARGERAVAILABLE = 0;

};

class RoombaController :public IBaseController
{
	RoombaControllerImpl* m_impl;
public:
	RoombaController()
	{
		m_impl = new RoombaControllerImpl();
	}
	virtual ~RoombaController()
	{
		delete m_impl;
	}
	virtual bool Connect(const core::string& port) ;
	virtual bool IsConnected() ;
	virtual void Disconnect();
	virtual void Start();
	virtual void Stop();
	virtual bool IsStarted();

	virtual void Drive(const math::vector2di& speed, int rotationSpeed) ;
	virtual void DriveStop();

	virtual void UpdateSensors();

	virtual int GetSensorCount();
	virtual float GetSensorValue(int s);
	virtual int GetBatteryLevel();

	virtual std::string ExecCommand(const core::string& cmd, const core::string& args);
};
}

#endif // RoombaController_h__
