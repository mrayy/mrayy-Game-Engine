

/********************************************************************
	created:	2012/06/28
	created:	28:6:2012   15:28
	filename: 	d:\Development\mrayEngine\Engine\mrayOptiTrack\OptiTrackClient.h
	file path:	d:\Development\mrayEngine\Engine\mrayOptiTrack
	file base:	OptiTrackClient
	file ext:	h
	author:		Written by Charith Fernando , modefied by Yamen Saraiji
	
	purpose:	
	This client connects to a NatNet server multicast Adaptor, receives data stream on a different core processing .dll and writes to the memory. 
	Through DMA you can read the data in the main file or anywhere in the application. The purpose is to illustrate using the NatNetClient class.

*********************************************************************/


#ifndef __OptiTrackClient__
#define __OptiTrackClient__

#include "ListenerContainer.h"
#include "OptiTrackRigidBody.h"

namespace mray
{
namespace animation
{
	class OptiTrackData;
	class OptiTrackClient;

class IOptiTrackClientListener
{
public:
	virtual void OnOptiTrackData(OptiTrackClient*client,const OptiTrackRigidBody& body){}
};

enum OptiConnectType
{
	Opti_Unicast,
	Opti_Multicast,
};

class OptiTrackClient:public ListenerContainer<IOptiTrackClientListener*>
{
private:
protected:
	DECLARE_FIRE_METHOD(OnOptiTrackData,(const OptiTrackRigidBody& body),(this,body));

	OptiTrackData* m_data;

	std::vector<OptiTrackRigidBody> m_rigidBodies;

	friend class OptiTrackData;

	int m_totalSamples;
	int m_samplesPerSecond;
	float m_lastTime;
	float m_timeAcc;

	void _PreHandleData();
public:
	OptiTrackClient();
	virtual~OptiTrackClient();


	bool Connect(OptiConnectType connection,const core::string& serverIP,const core::string& localIP,const core::string& multiCastInterface=mT("239.255.30.99"));
	void Disconnect();
	bool IsConnected();

	int GetSamplesPerSecond(){return m_samplesPerSecond;}

};


}
}

#endif
