

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   18:49
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\RobotCommunicatorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	RobotCommunicatorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___RobotCommunicatorComponent___
#define ___RobotCommunicatorComponent___

#include "ICommunicatorComponent.h"
#include "IFeedbackControl.h"
#include "IControllableComponent.h"

namespace mray
{
namespace VT
{
	class CommunicationData;
	class SoundListenerComponent;
	class CameraComponent;
	class MotorConnection;
	class RobotCommunicator;

class RobotCommunicatorComponent:public ICommunicatorComponent,public IFeedbackControlListener
{
	DECLARE_RTTI;
protected:

	CommunicatorScheme* m_scheme;
	std::map<uint,IControllableComponent*> m_controllableMap;
	std::map<uint,IFeedbackControl*> m_feedbackMap;

	SoundListenerComponent* m_listenerComp;
	CameraComponent* m_cameraComp;

	EControlSource m_controlSource;

	VT::RobotCommunicator* m_communicator;

	std::vector<MotorConnection*> m_motorConnections;
public:
	DECLARE_PROPERTY_TYPE(UseKinematic,bool,);

public:
	RobotCommunicatorComponent(game::GameEntityManager*mngr);
	virtual~RobotCommunicatorComponent();

	CommunicatorScheme* GetScheme(){return m_scheme;}

	const std::vector<MotorConnection*>& GetMotorConnections(){ return m_motorConnections; }
	RobotCommunicator* GetRobotCommunicator(){ return m_communicator; }

	virtual bool SetEnabled(bool enabled);

	bool SetUseKinematics(bool k){m_controlSource=k?EControl_Kinematic:EControl_Realtime;return true;}
	bool GetUseKinematics(){return m_controlSource==EControl_Kinematic;}
	
	virtual bool InitComponent();

	virtual void ReceiveData(const core::string& target,const CommValueList& values);
	
	virtual void OnFeedbackValue(IFeedbackControl*p,IFeedbackResult* r);

	CameraComponent* GetCameraComponent(){return m_cameraComp;}

	void SetActive();


	const std::map<uint,IControllableComponent*>& GetControllables(){return m_controllableMap;}
	const std::map<uint,IFeedbackControl*>& GetFeedbacks(){return m_feedbackMap;}


};

DECLARE_GAMECOMPONENT_FACTORY(RobotCommunicatorComponent);
}
}

#endif
