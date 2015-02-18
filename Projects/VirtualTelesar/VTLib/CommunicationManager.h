
/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   23:12
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\CommunicationManager.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	CommunicationManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CommunicationManager___
#define ___CommunicationManager___

#include "ISingleton.h"
#include "RobotCommunicator.h"

namespace mray
{
	namespace OS
	{
		class IThread;
		class IThreadFunction;
	}
namespace VT
{
	class ICommunicatorComponent;
	class CommunicationManagerThreadFunction;

class CommunicationManager:public ISingleton<CommunicationManager>,public IRobotCommunicatorListener
{

public:

	typedef std::list<ICommunicatorComponent*> CommunicatorList;
	typedef std::map<core::string,ICommunicationLayer*> CommunicationLayerMap;
	typedef std::map<core::string,ICommunicatorComponent*> CommunicatorMap;

protected:

	CommunicatorList m_communicatorsList;
	CommunicatorMap m_communicators;
	CommunicationLayerMap m_commLayers;

	std::list<RobotCommunicator*> m_robotCommunicators;

	OS::IThread* m_commThread;
	OS::IThreadFunction* m_commThreadFunc;


	friend class CommunicationManagerThreadFunction;

	void _OnUpdateTick(float dt);

public:
	CommunicationManager();
	virtual~CommunicationManager();

	void AddCommunicationLayer(const core::string& name, ICommunicationLayer* layer);
	ICommunicationLayer* GetCommunicationLayer(const core::string& name);
	void RemoveCommunicationLayer(const core::string& name);
	
	void AddCommunicator(ICommunicatorComponent* c);
	ICommunicatorComponent* GetCommunicator(const core::string& name);
	void RemoveCommunicator(ICommunicatorComponent* c);
	void ClearCommunicators();
	const CommunicatorList& GetCommunicators();

	void AddRobotCommunication(RobotCommunicator*l);
	void RemoveRobotCommunication(RobotCommunicator*l);
	void ClearRobotCommunications();
	const std::list<RobotCommunicator*>& GetRobotCommunications(){return m_robotCommunicators;}

	virtual void OnDataReceived(RobotCommunicator*c,const core::string& robotName,const core::string& target,const CommValueList& values);
	
};

}
}

#endif