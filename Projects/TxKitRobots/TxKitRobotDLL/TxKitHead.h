
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
	serial::Serial* m_gyroserial;


	ServoParameters m_parameters[3];
	bool m_paramsLoaded;
	float m_limits[3][2];

	float m_gyroRotation[3];
	math::quaternion m_gyroQuat;

	unsigned short m_lastValues[3];

	bool m_EEPROMset;

	bool _writeEEPROM();
	int _sendCommand(const uint8_t* cmd, int len, uint8_t* reply, int rlen, int waitTime=0);
	

public:
	TxKitHead();
	virtual ~TxKitHead();


	virtual bool Connect(const core::string& port, const core::string& gyroPort = "", bool autoSearch = true);
	virtual bool Connect(const core::string& port, bool autoSearchPort = true)
	{
		return Connect(port, "", autoSearchPort);
	}
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetRotation(const math::vector3d& rotation);
	virtual math::vector3d GetRotation() ;
	void _onSerialData(int size, char *buffer);

	void UpdateThreaded();
};

}


#endif
