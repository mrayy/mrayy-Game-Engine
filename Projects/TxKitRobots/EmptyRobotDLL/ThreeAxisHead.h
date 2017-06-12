
#ifndef __THREEAXISHEAD__
#define __THREEAXISHEAD__

#include "IHeadController.h"
#include "Tserial_event.h"

namespace mray
{
	
class ThreeAxisHead:public IHeadController
{
protected:

	Tserial_event *comROBOT;	// Serial Port
	bool connected;
	math::vector3d m_rotation;

	void _sendCommand(const std::string& cmd);
public:
	ThreeAxisHead();
	virtual ~ThreeAxisHead();

	Tserial_event *GetComEvent(){ return comROBOT; }

	virtual bool Connect(const std::string& port);
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void SetRotation(const math::vector3d& rotation);
	virtual math::vector3d GetRotation() ;
	void _onSerialData(int size, char *buffer);
};

}


#endif
