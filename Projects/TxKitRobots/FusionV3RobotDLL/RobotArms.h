
#ifndef __RobotArms__
#define __RobotArms__

#include "serial/serial.h"
#include <windows.h>
#include "ArmsController.h"

namespace mray
{

class RobotArms 
{
public:
	enum TargetArm
	{
		Left,
		Right
	};
protected:


	ArmsController *m_leftArm;
	ArmsController *m_rightArm;

	bool m_leftEnabled;
	bool m_rightEnabled;
public:
	RobotArms();
	virtual ~RobotArms();

	virtual bool Connect(const core::string& lPort, const core::string& rPort);
	virtual bool IsConnected();
	virtual void Disconnect();

	void SetArmAngles(TargetArm arm, float *angles,int n);
	void SetHand(TargetArm, float* angles, int n);

	void Start(bool leftArm,bool rightArm,bool enableReadingAngles);
	void Stop(bool force=false);

	void Update(float dt);

	float GetBatteryLevel() { return 100; }

	ArmsController::JoinInfo* GetLeftArm() { return m_leftArm->GetArm(); }
	ArmsController::JoinInfo* GetRightArm() { return m_rightArm->GetArm(); }
	ArmsController::JoinInfo* GetLeftHand() { return m_leftArm->GetHand(); }
	ArmsController::JoinInfo* GetRightHand() { return m_rightArm->GetHand(); }
	float* GetHandSensor(TargetArm hand);

	ArmsController::EState GetStatus() {
		ArmsController::EState lState = m_leftArm->GetStatus();
		ArmsController::EState rState = m_rightArm->GetStatus();
		if (m_leftArm->IsEnabled() && m_rightArm->IsEnabled())
		{
			if (lState == rState)
				return lState;
			if (lState == ArmsController::Initializing || lState == ArmsController::Shutingdown)
				return lState;
			return rState;
		}if ( m_rightArm->IsEnabled())
		{
			return rState;
		}if (m_leftArm->IsEnabled())
		{
			return lState;
		}
		return ArmsController::Wait; 
	}
};

}


#endif
