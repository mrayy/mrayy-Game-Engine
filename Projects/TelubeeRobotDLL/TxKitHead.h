
#ifndef __TxKitHead__
#define __TxKitHead__

#include "IHeadController.h"
#include "serial/serial.h"

namespace mray
{
	
class TxKitHead:public IHeadController
{
protected:

	bool connected;
	math::vector3d m_rotation;
	serial::Serial* m_serial;

	void _sendCommand(const char* cmd,int len);
public:
	TxKitHead();
	virtual ~TxKitHead();


	virtual bool Connect(const core::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetRotation(const math::vector3d& rotation);
	virtual math::vector3d GetRotation() ;
	void _onSerialData(int size, char *buffer);
};

}


#endif
