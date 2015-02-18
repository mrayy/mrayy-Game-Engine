
#include "stdafx.h"
#include "CommunicationManager.h"
#include "ICommunicatorComponent.h"
#include "CommunicationData.h"
#include "IThreadManager.h"


namespace mray
{
namespace VT
{

	class CommunicationManagerThreadFunction:public OS::IThreadFunction
	{
	protected:
		CommunicationManager* m_manager;
	public:
		CommunicationManagerThreadFunction(CommunicationManager*m){m_manager=m;}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			float t1 = gEngine.getTimer()->getSeconds();
			float dt=0;
			while(caller->isActive())
			{
				OS::IThreadManager::getInstance().sleep(10);
				float t2 = gEngine.getTimer()->getSeconds();
				dt=t2-t1;
				t1=t2;
				m_manager->_OnUpdateTick(dt);
			}
		}
	};

CommunicationManager::CommunicationManager()
{
	m_commThreadFunc=new CommunicationManagerThreadFunction(this);
	m_commThread=OS::IThreadManager::getInstance().createThread(m_commThreadFunc);
	//m_commThread->start(0);
}
CommunicationManager::~CommunicationManager()
{
	ClearCommunicators();
}


void CommunicationManager::AddCommunicationLayer(const core::string& name, ICommunicationLayer* layer)
{
	m_commLayers[name] = layer;
}
ICommunicationLayer* CommunicationManager::GetCommunicationLayer(const core::string& name)
{
	CommunicationLayerMap::iterator it = m_commLayers.find(name);
	if (it != m_commLayers.end())
		return it->second;
	return 0;
}

void CommunicationManager::RemoveCommunicationLayer(const core::string& name)
{
	m_commLayers.erase(name);
}

void CommunicationManager::_OnUpdateTick(float dt)
{
}


void CommunicationManager::AddCommunicator(ICommunicatorComponent* c)
{
	if(!c)
		return;
	m_communicatorsList.push_back(c);
	m_communicators[c->GetName()]=c;
}
ICommunicatorComponent* CommunicationManager::GetCommunicator(const core::string& name)
{
	CommunicatorMap::iterator it=m_communicators.find(name);
	if(it!=m_communicators.end())
		return it->second;
	return 0;
}
void CommunicationManager::RemoveCommunicator(ICommunicatorComponent* c)
{
	if(!c)
		return;
	CommunicatorMap::iterator it=m_communicators.find(c->GetName());
	if(it!=m_communicators.end())
		m_communicators.erase(it);
	m_communicatorsList.remove(c);;
}
void CommunicationManager::ClearCommunicators()
{
	m_communicators.clear();
	m_communicatorsList.clear();
}
const CommunicationManager::CommunicatorList& CommunicationManager::GetCommunicators()
{
	return m_communicatorsList;
}

void CommunicationManager::AddRobotCommunication(RobotCommunicator*l)
{
	if(l)
	{
		l->AddListener(this);
		m_robotCommunicators.push_back(l);
	}
}
void CommunicationManager::RemoveRobotCommunication(RobotCommunicator*l)
{
	if(l)
	{
		l->RemoveListener(this);
		m_robotCommunicators.remove(l);
	}
}
void CommunicationManager::ClearRobotCommunications()
{
	std::list<RobotCommunicator*>::iterator it= m_robotCommunicators.begin();
	for (;it!=m_robotCommunicators.end();++it)
	{
		(*it)->RemoveListener(this);
	}
	m_robotCommunicators.clear();
}

//void CommunicationManager::OnDataArrived(ICommunicationLayer* layer,const CommunicationData *d)
void CommunicationManager::OnDataReceived(RobotCommunicator*c,const core::string& robotName,const core::string& target,const CommValueList& values)
{
	ICommunicatorComponent *comp=GetCommunicator(robotName);
	if(comp)
	{
		comp->ReceiveData(target,values);
	}
}


}
}


