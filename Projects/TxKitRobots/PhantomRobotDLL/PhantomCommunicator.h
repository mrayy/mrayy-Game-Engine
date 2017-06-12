



/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\PhantomRobotDLL\PhantomCommunicator
	file base:	PhantomCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhantomCommunicator_h__
#define PhantomCommunicator_h__

#include "IBaseController.h"

namespace mray
{
	class PhantomCommunicatorImpl;
class PhantomCommunicator :public IBaseController
{
protected:
	mray::PhantomCommunicatorImpl* m_impl;
public:

	PhantomCommunicator();
	virtual~PhantomCommunicator();

	virtual bool Connect(const core::string& port) ;
	virtual bool IsConnected();
	virtual void Disconnect();

	virtual void Start();
	virtual void Stop();
	virtual bool IsStarted();

	virtual void Drive(const math::vector2di& speed, int rotationSpeed) ;
	virtual void DriveStop() ;

	virtual void UpdateSensors(){}


	virtual int GetSensorCount() { return 0; }
	virtual float GetSensorValue(int s) { return 0; };
	virtual int GetBatteryLevel() { return 100; };
};
}

#endif // PhantomCommunicator_h__

