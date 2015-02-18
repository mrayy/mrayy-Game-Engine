

#include "stdafx.h"
#include "CarController.h"
#include "CommunicationData.h"
#include "CMemoryStream.h"

#include "INetwork.h"
#include "StreamWriter.h"
#include "UDPCommunicationLayer.h"

namespace mray
{

CarController::CarController()
{
	m_mode=EIdle;
	m_time=0;
	m_udpClient=0;
}

CarController::~CarController()
{
	delete m_udpClient;
}

void CarController::Init()
{
	m_udpClient=network::INetwork::getInstance().createUDPClient();
	m_udpClient->Open();
	m_udpClient->Connect(network::NetAddress("192.168.10.149",1234));

	byte dataBuffer[1024];
	OS::CMemoryStream stream("",dataBuffer,sizeof(dataBuffer),false);
	OS::StreamWriter wrtr(&stream);


	wrtr.binWriteInt(VT::ESchemeMessage);
	wrtr.binWriteInt(6);
	wrtr.binWriteString("JointNeck1");
	wrtr.binWriteString("JointNeck2");
	wrtr.binWriteString("JointNeck3");
	wrtr.binWriteString("JointFrontWheel");
	wrtr.binWriteString("JointBLWheel");
	wrtr.binWriteString("JointBRWheel");


	network::NetAddress addr(network::EAdressType::EAT_Localhost,1234);
	network::UDPClientError e= m_udpClient->SendTo(&addr,(char*)dataBuffer,stream.length());
}


void CarController::OnBodyDataReceived(const math::vector3d &pos,const math::quaternion& rot)
{
	math::vector3d newP=pos;
	newP.y=0;
	math::vector3d angles;
	rot.toEulerAngles(angles);
	if(m_mode==ECalibrating)
	{
		m_calibratedPosition.AddSample(newP);
		m_calibratedOrintation.AddSample(angles);
	}else if(m_mode==EControlling)
	{
		math::vector3d dp=newP-m_calibratedPosition.Mean();
		math::vector3d da=angles-m_calibratedOrintation.Mean();

		float dpLen=dp.Length();
		if(dpLen>0)
		{
			float l=math::clamp<float>(dpLen,0,m_calibratedPosition.Variance().Length());
			dp*=l/dpLen;
		}
		m_speedVector.x=dp.x;
		m_speedVector.y=dp.z;
		m_headRotation=da;


		VT::CommunicationData data;
		data.SetTargetName("TelesarV");
		data.AddData("JointNeck1",&m_headRotation.y,1);
		data.AddData("JointNeck2",&m_headRotation.x,1);
		data.AddData("JointNeck3",&m_headRotation.z,1);

		byte dataBuffer[1024];
		OS::CMemoryStream stream("",dataBuffer,sizeof(dataBuffer),false);
		OS::StreamWriter wrtr(&stream);
		wrtr.binWriteInt(VT::EDataMessage);
		VT::CommunicationDataStreamer::WriteData(data,&stream);

		network::NetAddress addr(network::EAdressType::EAT_Localhost,1234);
		network::UDPClientError e= m_udpClient->SendTo(&addr,(char*)dataBuffer,stream.length());
		if(e!=network::UDP_SOCKET_ERROR_NONE)
		{
			printf("%s\n",network::IUDPClient::TranslateErrorMessage(e).c_str());
		}
	}
}


void CarController::Update(float dt)
{
	if(m_mode==ECalibrating)
	{
		m_time+=dt;
		if(m_time>=m_calibrationTime)
		{
			m_mode=EControlling;
			m_calibratedPosition.Calculate();
			m_calibratedOrintation.Calculate();
		}
	}
}

}

