
#ifndef __TxKitBraccio__
#define __TxKitBraccio__

#include "IHeadController.h"
#include "serial/serial.h"

namespace mray
{

class TxKitBraccio
{
protected:

	const float BASE_HGT = 0.078f;
	const float HUMERUS = 0.124f;
	const float ULNA = 0.124f;
	const float GRIPPER = 0.058f;
	/* pre-calculations */
	float hum_sq;
	float uln_sq;

	float _joints[6];
	bool connected;
	serial::Serial* m_serial;

	math::vector3d _position;
	math::vector3d _rotation;

	void _calcIK(float x, float y, float z);
	void _sendCommand();

public:
	TxKitBraccio();
	virtual ~TxKitBraccio();


	virtual bool Connect(const core::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetPosition(const math::vector3d& position);
	virtual void SetRotation(const math::vector3d& rotation);
	void UpdateThreaded();
};

}


#endif
