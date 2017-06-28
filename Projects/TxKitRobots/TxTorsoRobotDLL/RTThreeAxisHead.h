
#ifndef __RTThreeAxisHead__
#define __RTThreeAxisHead__

#include "IHeadController.h"
#include "SFP10X_COM.h"

namespace mray
{
	
class RTThreeAxisHead:public IHeadController
{
	const int ROBOTIS_ID = 0x07; //XM ID address 
	const int GOAL_POSITION = 0x74; //XM GOAL_POSITION address 4byte
	const int CURRENT_POSITION = 0x84; //XM current position address 4 byte
	const int INST_READ = 0x02; //XM command read 
	const int INST_WRITE = 0x03; //XM command write
	const int INST_REGWRITE = 0x03; //XM command write
	const int CENTER_POS = (360 / 2); //Slider position
	const int MODE_POS = 0x01; // position get mode
	const int MODE_ID = 0x02; //id get mode
	const int MODE_OP = 0x03; //operating mode get mode
	const int MODE_TORQUE = 0x04; //operating mode get mode

	const double DEGREE_RESOLUTION = 11.377;// degree resolution  4096/360 =11.377
	const int XM_TORQUE = 0x40; //motor torque on address address 64
	const int XM_LED = 0x41; //motor torque on address address 65
	const int XM_OPE_MODE = 0x0B; // operating mode address 11
	const int TORQUE_ON = 1;
	const int TORQUE_OFF = 0;
	const int LED_ON = 1;
	const int LED_OFF = 0;


	const int RP2_PGAIN = 80;
	const int RP2_DGAIN = 82;
	const int RP2_IGAIN = 84;


protected:
	SFPDevice sfp_device;

	bool connected;
	math::vector3d m_rotation;
	math::vector3d m_position;

	math::vector3di m_pid[6];

	byte _buffer[256];
	int _bufferPos;

	float t ;

	void _sendCommand(const std::string& cmd);

	int calc_checksum(int crc_accum, byte* data_blk_ptr, int data_blk_size);
	void _searchServos();
	void _turnServosOn(int id);
	void _turnServosOff(int id);
	void _turnProjector(bool on);
	void _setServoPos(int id, int pos);
	void _flush();
	void set_packet(int id, int com, byte* param, int size);

	bool judge_limit(double sr, double tr, double lim1, double lim2, double x, double y, double z);
	void check_length(double sr, double tr, double x, double y, double z, double lim1, double lim2, double ph, double th);
	double get_theta(double sr, double tr, double x, double y, double z, double lim1, double lim2, double ph);
public:
	RTThreeAxisHead();
	virtual ~RTThreeAxisHead();

	virtual bool Connect(const core::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetRotation(const math::vector3d& rotation);
	virtual void SetPosition(const math::vector3d& pos);
	virtual math::vector3d GetRotation();
	virtual math::vector3d GetPosition();
	void _onSerialData(int size, char *buffer);

	void SetPgain(int id, int val);
	void SetIgain(int id, int val);
	void SetDgain(int id, int val);
};

}


#endif
