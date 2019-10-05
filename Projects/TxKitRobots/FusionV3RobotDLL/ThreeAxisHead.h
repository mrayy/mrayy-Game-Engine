
#ifndef __THREEAXISHEAD__
#define __THREEAXISHEAD__

#include "IHeadController.h"
#include "serial/serial.h"
#include <windows.h>
namespace mray
{
	
class ThreeAxisHead:public IHeadController
{
protected:

	serial::Serial* m_serial;
	bool connected;
	HANDLE m_robotThread;
	math::vector3d m_rotation;
	math::vector3d m_sentRotation;

	bool m_enableAngleLog;
	std::string _buffer;
	std::string _rcvbuffer;

	bool m_laserEnabled;

	void _sendCommand(const std::string& cmd);
	void _sendRotation();
public:
	ThreeAxisHead();
	virtual ~ThreeAxisHead();

	serial::Serial *GetComEvent(){ return m_serial; }

	virtual bool Connect(const core::string& port)
	{
		return Connect(port, false);
	}
	virtual bool Connect(const core::string& port,bool laserEnabled) {
		return Connect(port, false, laserEnabled);
	}
	virtual bool Connect(const core::string& port, bool enableAngleLog, bool laserEnabled);
	virtual bool IsConnected();
	virtual void Disconnect();

	void SetLaser(int value);
	void SetPanDamping(float value);

	virtual void SetRotation(const math::vector3d& rotation);
	virtual math::vector3d GetRotation() ;
	void _onSerialData(int size, char *buffer);

	void CheckSerial();
	void _ProcessThread();
};

}


#endif
