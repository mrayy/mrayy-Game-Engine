

#include "stdafx.h"
#include "RobotCommunicator.h"
#include "MotorConnection.h"


namespace mray
{
namespace VT
{

RobotCommunicator::RobotCommunicator()
{
	m_communicator=0;
}
RobotCommunicator::~RobotCommunicator()
{
	ClearConnectors();
}

void RobotCommunicator::SetCommunicatorLayer(ICommunicationLayer* c)
{
	if(m_communicator)
		m_communicator->RemoveListener(this);
	m_communicator=c;
	if(m_communicator)
		m_communicator->AddListener(this);
}

void RobotCommunicator::OnDataArrived(ICommunicationLayer* layer,const CommunicationData*d)
{
	MotorConnectionMap::iterator it= m_motorConnectors.begin();
	MotorConnectionMap::iterator end= m_motorConnectors.end();
	for (;it!=end;++it)
	{
		MotorConnection* conn=it->second;
		if(!conn->IsConnected() )
			continue;
		const std::list<CommunicationData::DataPair>& lst=d->GetData();
		std::list<CommunicationData::DataPair>::const_iterator dit=lst.begin();
		std::list<CommunicationData::DataPair>::const_iterator dend=lst.end();
		for (;dit!=dend;++dit)
		{
			if( conn->GetSourceJoint()==dit->name)
			{
				FIRE_LISTENR_METHOD(OnDataReceived,(this,d->GetTargetName(),conn->GetTargetJoint(),dit->values));
			}
		}
	}
}

void RobotCommunicator::OnClosed(ICommunicationLayer* l)
{

}
void RobotCommunicator::OnSchemeChanged(VT::ICommunicationLayer*, const std::vector<core::string>& names)
{
}
MotorConnection* RobotCommunicator::AddConnection(const core::string &src,const core::string &target,bool connected)
{
	MotorConnection* connection=new MotorConnection();
	connection->SetSourceJoint(src);
	connection->SetTargetJoint(target);
	connection->SetConencted(connected);

	m_motorConnectors[mray::GUID(target).ID()]=connection;
	return connection;
}

void RobotCommunicator::RemoveConnection(const core::string& target)
{
	MotorConnectionMap::iterator it= m_motorConnectors.find(mray::GUID(target).ID());
	if(it==m_motorConnectors.end())
		return;
	delete it->second;
	m_motorConnectors.erase(it);
}

void RobotCommunicator::ClearConnectors()
{
	MotorConnectionMap::iterator it= m_motorConnectors.begin();
	for (;it!=m_motorConnectors.end();++it)
	{
		delete it->second;
	}
	m_motorConnectors.clear();
}


}
}