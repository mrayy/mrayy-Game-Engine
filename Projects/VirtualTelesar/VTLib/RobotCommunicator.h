

/********************************************************************
	created:	2012/11/23
	created:	23:11:2012   14:35
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\RobotCommunicator.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	RobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _RobotCommunicator_
#define _RobotCommunicator_

#include "ICommunicationLayer.h"
#include "CommunicationData.h"

namespace mray
{
namespace VT
{
class MotorConnection;
class RobotCommunicator;

	class IRobotCommunicatorListener
	{
		public:
			virtual void OnDataReceived(RobotCommunicator*c,const core::string& robotName,const core::string& target,const CommValueList& values){}
	};
	typedef std::map<uint,MotorConnection*> MotorConnectionMap;


	//////////////////////////////////////////////////////////////////////////
	// Robot Communicator handles telesar data which arrives via ICommunicationLayer and delivers it to the assigned listeners
class RobotCommunicator:public ICommunicationLayerListener,public ListenerContainer<IRobotCommunicatorListener*>
{
protected:
	MotorConnectionMap m_motorConnectors;

	ICommunicationLayer* m_communicator;

	DECLARE_FIRE_METHOD(OnDataReceived,(RobotCommunicator*c,const core::string& robotName,const const core::string& target,const CommValueList& values),(c,robotName,target,values));

public:
	RobotCommunicator();
	virtual~RobotCommunicator();

	void SetCommunicatorLayer(ICommunicationLayer* c);

	virtual void OnSchemeChanged(VT::ICommunicationLayer*, const std::vector<core::string>& names);
	virtual void OnDataArrived(ICommunicationLayer* layer,const CommunicationData*d);
	virtual void OnClosed(ICommunicationLayer* l);

	const MotorConnectionMap& GetMotorConnections(){return m_motorConnectors;}
	MotorConnection* AddConnection(const core::string &src,const core::string &target,bool connected=true);
	void RemoveConnection(const core::string& target);
	void ClearConnectors();
};


}
}

#endif
