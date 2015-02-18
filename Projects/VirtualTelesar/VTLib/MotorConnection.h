

/********************************************************************
	created:	2012/11/23
	created:	23:11:2012   14:20
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\MotorConnection.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	MotorConnection
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _MotorConnection_
#define _MotorConnection_

#include <ListenerContainer.h>

namespace mray
{
namespace VT
{

	class MotorConnection;/*
	class IMotorConnectionListener
	{
	public:
		virtual void OnConnectionChanged(MotorConnection* c,bool state){}
		virtual void OnTargetJointChanged(MotorConnection* c,const core::string& target){}
		virtual void OnSourceJointChanged(MotorConnection* c,const core::string& src){}
	};*/
class MotorConnection//:public ListenerContainer<IMotorConnectionListener*>
{
protected:

	core::string	m_sourceJoint;
	core::string	m_targetJoint;
	bool				m_connected;

// 	DECLARE_FIRE_METHOD(OnConnectionChanged,(MotorConnection* c,bool state),(c,state));
// 	DECLARE_FIRE_METHOD(OnTargetJointChanged,(MotorConnection* c,const core::string& target),(c,target));
// 	DECLARE_FIRE_METHOD(OnSourceJointChanged,(MotorConnection* c,const core::string& target),(c,target));
public:
	typedef DelegateEvent2<MotorConnection*,bool> OnConnectionChangedEvent;
	typedef DelegateEvent2<MotorConnection*,const core::string&> OnTargetJointChangedEvent;
	typedef DelegateEvent2<MotorConnection*,const core::string&> OnSourceJointChangedEvent;

	OnConnectionChangedEvent OnConnectionChanged;
	OnTargetJointChangedEvent OnTargetJointChanged;
	OnSourceJointChangedEvent OnSourceJointChanged;
public:
	MotorConnection():m_connected(false)
	{
	}
	virtual~MotorConnection(){}

	void SetConencted(bool c)
	{
		m_connected=c;
		//FIRE_LISTENR_METHOD(OnConnectionChanged,(this,c));
		OnConnectionChanged,(this,c);
	}
	bool IsConnected()
	{
		return m_connected && m_sourceJoint!="" && m_targetJoint!="";
	}

	void SetSourceJoint(const core::string& joint)
	{
		m_sourceJoint=joint;
		//FIRE_LISTENR_METHOD(OnSourceJointChanged,(this,joint));
		OnSourceJointChanged,(this,joint);
	}
	const core::string&  GetSourceJoint()
	{
		return m_sourceJoint;
	}

	void SetTargetJoint(const core::string& joint)
	{
		m_targetJoint=joint;
		//FIRE_LISTENR_METHOD(OnTargetJointChanged,(this,joint));
		OnTargetJointChanged,(this,joint);
	}
	const core::string& GetTargetJoint()
	{
		return m_targetJoint;
	}
};


}
}

#endif
