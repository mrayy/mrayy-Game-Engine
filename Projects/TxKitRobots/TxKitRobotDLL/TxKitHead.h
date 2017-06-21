
#ifndef __TxKitHead__
#define __TxKitHead__

#include "IHeadController.h"
#include "serial/serial.h"

namespace mray
{
	
class TxKitHead:public IHeadController
{
protected:

	struct ServoParameters
	{
		int PGain ;
		int Deadband ;
		int Damping ;
		int Response ;
		int Speed ;
	};

	static const ServoParameters DEFAULT_PARAMETERS[3];
	static const float DEFAULT_LIMITS[3][2];

	bool connected;
	float m_rotation[3];
	serial::Serial* m_serial;

	ServoParameters m_parameters[3];
	float m_limits[3][2];

	unsigned short m_lastValues[3];

	bool m_EEPROMset;

	bool _writeEEPROM();
	int _sendCommand(const uint8_t* cmd, int len, uint8_t* reply, int rlen, int waitTime=0);
	

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
