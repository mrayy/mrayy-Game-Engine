
#include "stdafx.h"
#include "OptiTrackClient.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "ITimer.h"
#include "Engine.h"

#include <optiTrack\NatNetTypes.h>
#include <optiTrack\NatNetClient.h>

#include <quaternion.h>

namespace mray
{
namespace animation
{

	class OptiTrackData
	{
	public:
		OptiTrackData(OptiTrackClient* r)
		{
			theClient=0;
			receiver=r;
		}
		NatNetClient* theClient;

		OptiTrackClient* receiver;

		static void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);			// receives data from the server
		void Uninitialize()
		{
			if(theClient)
			{
				theClient->Uninitialize();
				delete theClient;
				theClient=0;
			}
		}

		int Initialize(int iConnectionType,const core::string& serverIP,const core::string& localIP,const core::string& multiCastInterface)
		{
			Uninitialize();

			theClient = new NatNetClient(iConnectionType);
			gLogManager.log("NatNet Sample Client for TELESAR-V ",ELL_INFO);

			// Set callback handlers
			theClient->SetVerbosityLevel(Verbosity_None);
			theClient->SetMulticastAddress((char*)multiCastInterface.c_str());
			theClient->SetDataCallback( DataHandler, this );	// this function will receive data from the server

			core::stringc locIP,servIP;
			core::string_to_char(localIP,locIP);
			core::string_to_char(serverIP,servIP);
			// Init Client and connect to NatNet server
			int retCode = theClient->Initialize(&locIP[0], &servIP[0]);
			if (retCode != ErrorCode_OK){
				gLogManager.log("OptiTrackClient() - Unable to connect to server.  Error code: "+core::StringConverter::toString(retCode),ELL_WARNING);
				return ErrorCode_Internal;
			}
			else{
				sServerDescription ServerDescription;
				memset(&ServerDescription, 0, sizeof(ServerDescription));
				theClient->GetServerDescription(&ServerDescription);
				if(!ServerDescription.HostPresent) {
					gLogManager.log("OptiTrackClient() - Unable to connect to server. Host not present..",ELL_WARNING);
					return 1;
				}
			}
			return ErrorCode_OK;
		}
	};

	void __cdecl OptiTrackData::DataHandler(sFrameOfMocapData* data, void* pUserData)
	{
		OptiTrackData* pClient = (OptiTrackData*) pUserData;

		OptiTrackClient* owner=pClient->receiver;

		owner->_PreHandleData();

		//Console::locate(0,20);
		//printf("Latency : %3.2lf(ms) \n", dDuration);

		// Rigid Bodies data to Rotation Matrix
		// Convert a quaternion to euler angles (Y, Z, X)
		// Y = Heading  (Yaw), Z = Attitude (Pitch), X = Bank     (Roll)
		// conventions:
		//	input and output units are both in radians
		//  euler order is YZX
		//  euler angles are about global axes
		//  euler + angle is right-handed

		//getting available RigidBody count
		//trackables[0].rigidBodies = data->nRigidBodies; 

		if(owner->m_rigidBodies.size()!=data->nRigidBodies)
		{
			owner->m_rigidBodies.resize(data->nRigidBodies);
			for(int i=0; i < owner->m_rigidBodies.size(); i++)
			{
				owner->m_rigidBodies[i].SetID(data->RigidBodies[i].ID);
			}
		}

		math::vector3d pos;
		math::quaternion ori;

		for(int i=0; i < data->nRigidBodies; i++){

			//check if the body is visible or not 
			bool ok = false;
			for (int j = 0; j < data->RigidBodies[i].nMarkers; ++j)
			{
				if (data->RigidBodies[i].Markers[j][0] != 0)
				{
					ok = true;
					break;;
				}
			}
			if (!ok)
				continue;
			pos.x = -data->RigidBodies[i].y;
			pos.y = data->RigidBodies[i].z;
			pos.z = data->RigidBodies[i].x;
			/*
			// Same ground plane with FST
			ori.x = data->RigidBodies[i].qx;
			ori.y = data->RigidBodies[i].qy;
			ori.z = data->RigidBodies[i].qz;
			ori.w = data->RigidBodies[i].qw;

			ori.Normalize();


			math::vector3d angles;
			ori.toEulerAngles(angles);
			angles.x=-angles.x;
			angles.z=-angles.z;
			ori.fromEulerAngles(angles.x,angles.y,angles.z);

			printf("%s\n",core::StringConverter::toString(angles).c_str());*/
			
			/**/
			// Same ground plane with FST
			double sqx = data->RigidBodies[i].qx;
			double sqy = data->RigidBodies[i].qy;
			double sqz = data->RigidBodies[i].qz;
			double sqw = data->RigidBodies[i].qw;
#if 1
			sqx = data->RigidBodies[i].qx*data->RigidBodies[i].qx;
			sqy = data->RigidBodies[i].qy*data->RigidBodies[i].qy;
			sqz = data->RigidBodies[i].qz*data->RigidBodies[i].qz;
			sqw = data->RigidBodies[i].qw*data->RigidBodies[i].qw;
			// invs (inverse square length) is only required if quaternion is not already normalised
			double invs = 1 / sqrt(sqx + sqy + sqz + sqw);

			// Tracking Tools, Original Convension
			float rotMatrix[16];
			rotMatrix[0] = 2.0 * (sqw + sqx - 0.5) *invs;
			rotMatrix[4] = 2.0 * (sqw + sqy - 0.5) *invs;
			rotMatrix[8] = 2.0 * (sqw + sqz - 0.5) *invs;

			double tmp1 = data->RigidBodies[i].qx*data->RigidBodies[i].qy;
			double tmp2 = data->RigidBodies[i].qz*data->RigidBodies[i].qw;
			rotMatrix[3] = 2.0 * (tmp1 + tmp2)*invs;
			rotMatrix[1] = 2.0 * (tmp1 - tmp2)*invs;

			tmp1 = data->RigidBodies[i].qx*data->RigidBodies[i].qz;
			tmp2 = data->RigidBodies[i].qw*data->RigidBodies[i].qy;
			rotMatrix[6] = 2.0 * (tmp1 - tmp2)*invs;
			rotMatrix[2] = 2.0 * (tmp1 + tmp2)*invs;

			tmp1 = data->RigidBodies[i].qy*data->RigidBodies[i].qz;
			tmp2 = data->RigidBodies[i].qw*data->RigidBodies[i].qx;
			rotMatrix[7] = 2.0 * (tmp1 + tmp2)*invs;
			rotMatrix[5] = 2.0 * (tmp1 - tmp2)*invs; 
			
			float x = math::toDeg(-atan2(rotMatrix[7], rotMatrix[8]));	// x
			float y = math::toDeg(-asin(rotMatrix[6]));					// y
			float z = math::toDeg(-atan2(rotMatrix[3], rotMatrix[0]));		// z

			// Euler calculations
			/*float z = math::toDeg(atan2(rotMatrix[7], rotMatrix[8]));	// x
			float y = math::toDeg(-asin(rotMatrix[6]));					// y 
			float x = math::toDeg(atan2(rotMatrix[3], rotMatrix[0]));		// z*/
			ori.fromEulerAngles(x,y,z);
			//ori.fromMatrix(rotMatrix);
#else
			ori = math::quaternion(sqw, sqx, sqy, sqz);
// 			math::vector3d angles;
// 			ori.toEulerAngles(angles);
// 			angles.x = -angles.x;
// 			angles.z = -angles.z;
	//		ori.fromEulerAngles(angles.x, angles.y, angles.z);
#endif

			owner->m_rigidBodies[i].SetName(data->MocapData[i].szName);
			owner->m_rigidBodies[i].SetPosition(pos);
			owner->m_rigidBodies[i].SetOrintation(ori);
			owner->__FIRE_OnOptiTrackData(owner->m_rigidBodies[i]);
		}
	}

OptiTrackClient::OptiTrackClient()
{
	m_data=new OptiTrackData(this);
	m_lastTime=0;
	m_samplesPerSecond=0;
	m_timeAcc=0;
	m_totalSamples=0;
}
OptiTrackClient::~OptiTrackClient()
{
	Disconnect();
	delete m_data;
}


void OptiTrackClient::_PreHandleData()
{

	// same system latency test

	float fThisTick = (float)gEngine.getTimer()->getSeconds();
	//	float fDiff = fThisTick - data->fLatency;
	//	double dDuration = fDiff;
	m_totalSamples++;

	m_timeAcc+=fThisTick-m_lastTime;
	if(m_timeAcc>=1000)
	{
		m_timeAcc-=1000;
		m_samplesPerSecond=m_totalSamples;
		m_totalSamples=0;
	}
	m_lastTime=fThisTick;

}
bool OptiTrackClient::Connect(OptiConnectType connection,const core::string& serverIP,const core::string& localIP,const core::string& multiCastInterface)
{
	int iResult;
	iResult=m_data->Initialize(connection==Opti_Unicast? ConnectionType_Unicast: ConnectionType_Multicast,serverIP,localIP,multiCastInterface);
	m_lastTime = gEngine.getTimer()->getSeconds();;
	m_samplesPerSecond=0;
	m_timeAcc=0;
	m_totalSamples=0;

	return iResult==ErrorCode_OK;
}
bool OptiTrackClient::IsConnected()
{
	return m_data->theClient!=0;;
}

void OptiTrackClient::Disconnect()
{
	m_data->Uninitialize();
}

}
}