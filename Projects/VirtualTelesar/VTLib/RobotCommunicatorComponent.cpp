


#include "stdafx.h"
#include "RobotCommunicatorComponent.h"
#include "CommunicatorScheme.h"
#include "CommunicationManager.h"
#include "CommunicationData.h"
#include "IControllableComponent.h"
#include "SoundListenerComponent.h"
#include "CameraComponent.h"

namespace mray
{
namespace VT
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(UseKinematic,RobotCommunicatorComponent,bool,mT("UseKinematic"),EPBT_Basic,mT(""),true);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(UseKinematic,RobotCommunicatorComponent,bool,SetUseKinematics,GetUseKinematics,core::StringConverter::toString,core::StringConverter::toBool,false);

RobotCommunicatorComponent::RobotCommunicatorComponent(game::GameEntityManager*mngr)
{
	m_cameraComp=0;
	m_listenerComp=0;
	m_controlSource=EControl_Kinematic;
	m_scheme=new CommunicatorScheme();
	m_communicator = new RobotCommunicator();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeUseKinematic::instance);
	}
}
RobotCommunicatorComponent::~RobotCommunicatorComponent()
{
	delete m_scheme;

	VT::CommunicationManager::getInstance().RemoveCommunicator(this);
	VT::CommunicationManager::getInstance().RemoveRobotCommunication(m_communicator);

	delete m_communicator;
}

bool RobotCommunicatorComponent::InitComponent()
{
	if(!game::IGameComponent::InitComponent())
		return false;


	m_scheme->SetName(GetName());
	//find all components which are an IControllableJointComponent type
	const std::list<IObjectComponent*>& lst=m_ownerComponent->GetComponentsList();
	std::list<IObjectComponent*>::const_iterator it=lst.begin();
	for(;it!=lst.end();++it)
	{
		if(dynamic_cast<IControllableComponent*>(*it)!=0)
		{
			IControllableComponent* c=dynamic_cast<IControllableComponent*>(*it);
			IVTComponent* comp=dynamic_cast<IVTComponent*>(*it);
			CommunicatorScheme::CommunicatorItem*ci=m_scheme->AddItem(comp->GetName(),ECDT_Float);
			m_controllableMap[GUID(comp->GetName()).ID()]=c;
		}else if(dynamic_cast<IFeedbackControl*>(*it)!=0)
		{
			IFeedbackControl* c=dynamic_cast<IFeedbackControl*>(*it);
			IVTComponent* comp=dynamic_cast<IVTComponent*>(*it);
			m_feedbackMap[GUID(comp->GetName()).ID()]=c;
			c->AddListener(this);
		} else if(!m_listenerComp && dynamic_cast<SoundListenerComponent*>(*it)!=0)
		{
			m_listenerComp=dynamic_cast<SoundListenerComponent*>(*it);
		}else if(!m_cameraComp && dynamic_cast<CameraComponent*>(*it)!=0)
		{
			m_cameraComp=dynamic_cast<CameraComponent*>(*it);
		}
	}

	CommunicationManager::getInstance().AddCommunicator(this);
	CommunicationManager::getInstance().AddRobotCommunication(m_communicator);
	ICommunicationLayer* layer= CommunicationManager::getInstance().GetCommunicationLayer(m_name);
	if (layer)
		m_communicator->SetCommunicatorLayer(layer);


	{
		const std::map<uint, VT::IControllableComponent*>& controllables = GetControllables();
		std::map<uint, VT::IControllableComponent*>::const_iterator it = controllables.begin();
		for (; it != controllables.end(); ++it)
		{
			m_motorConnections.push_back(m_communicator->AddConnection(it->second->GetControllableName(), it->second->GetControllableName()));
		}
	}

	return true;
}

bool RobotCommunicatorComponent::SetEnabled(bool enabled)
{

	if (enabled && !m_enabled)
	{

		CommunicationManager::getInstance().AddCommunicator(this);
		CommunicationManager::getInstance().AddRobotCommunication(m_communicator);
	}
	if (!enabled && m_enabled)
	{

		CommunicationManager::getInstance().RemoveCommunicator(this);
		CommunicationManager::getInstance().RemoveRobotCommunication(m_communicator);
	}

	return ICommunicatorComponent::SetEnabled(enabled);
}

void RobotCommunicatorComponent::ReceiveData(const core::string& target,const CommValueList& values)
{
	if(!m_enabled)
		return;


	uint targetID=GUID(target).ID();
	std::map<uint,IControllableComponent*>::iterator cit= m_controllableMap.find(targetID);
	if(cit==m_controllableMap.end())
		return;
	cit->second->SetControlValue(values,m_controlSource);
}

void RobotCommunicatorComponent::SetActive()
{
	if(m_listenerComp)
	{
		m_listenerComp->SetActive();
	}
}

void RobotCommunicatorComponent::OnFeedbackValue(IFeedbackControl*p,IFeedbackResult* r)
{

}

}
}
