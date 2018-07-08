
#ifndef __THREEAXISHEAD__
#define __THREEAXISHEAD__

#include "IHeadController.h"
#include "serial/serial.h"

namespace mray
{
	
class ThreeAxisHead:public IHeadController
{
protected:

	serial::Serial* m_serial;
	bool connected;
	math::vector3d m_rotation;

	std::string _buffer;

	void _sendCommand(const std::string& cmd);
public:
	ThreeAxisHead();
	virtual ~ThreeAxisHead();

	serial::Serial *GetComEvent(){ return m_serial; }

	virtual bool Connect(const core::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetRotation(const math::vector3d& rotation);
	virtual math::vector3d GetRotation() ;
	void _onSerialData(int size, char *buffer);
};

}


#endif
