
#ifndef OmniBaseController_h__
#define OmniBaseController_h__

#include "IBaseController.h"
#include "Tserial_event.h"


namespace mray
{
	
class OmniBaseController:public IBaseController
{
protected:
	Tserial_event *comROBOT;	// Serial Port
	bool connected;
	void sendCommand(const core::string& cmd);

	std::vector<float> m_sensors;
public:
	OmniBaseController();
	virtual ~OmniBaseController();

	Tserial_event *GetComEvent(){ return comROBOT; }


	virtual bool Connect(const core::string& port) ;
	virtual bool IsConnected() ;
	virtual void Disconnect() ;

	virtual void Drive(const math::vector2di& speed, int rotationSpeed);
	virtual void DriveStop();

	virtual void UpdateSensors();
	virtual int GetSensorCount();
	virtual float GetSensorValue(int s);
	virtual int GetBatteryLevel();


	void _onSerialData(int size, char *buffer);
};

}

#endif // OmniBaseController_h__


