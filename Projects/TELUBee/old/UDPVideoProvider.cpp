
#include "stdafx.h"
#include "UDPVideoProvider.h"
#include "UDPDataReceiver.h"
#include "UDPVideoGrabber.h"

namespace mray
{
namespace TBee
{

UDPVideoProvider::UDPVideoProvider()
{
	m_dataReceiver[0]=new UDPDataReceiver();
	m_dataReceiver[1]=new UDPDataReceiver();

	m_dataReceiver[0]->AddListener(this);
	m_dataReceiver[1]->AddListener(this);

	m_newimage[0]=m_newimage[1]=0;
// 
// 	m_grabbers[0]=new UDPVideoGrabber(m_dataReceiver[0],ECamera_Right);
// 	m_grabbers[1]=new UDPVideoGrabber(m_dataReceiver[1],ECamera_Left);
}

UDPVideoProvider::~UDPVideoProvider()
{
// 	delete m_grabbers[0];
// 	delete m_grabbers[1];

	delete m_dataReceiver[0];
	delete m_dataReceiver[1];
}

void UDPVideoProvider::OnVideoImage(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src)
{
	int idx=0;
	if(src==ECamera_Right)
		idx=0;
	else idx=1;
	m_newimage[idx]=true;
	m_image[idx].setData(image->imageData,image->Size,image->format);
}

void UDPVideoProvider::ConnectToCameras(const core::string& ip,int port1,int port2)
{
	((UDPDataReceiver*)m_dataReceiver[0])->Connect(ip,port1);
	((UDPDataReceiver*)m_dataReceiver[1])->Connect(ip,port2);

	m_dataReceiver[0]->Start();
	m_dataReceiver[1]->Start();
}

void UDPVideoProvider::Disconnect()
{
	m_dataReceiver[0]->Close();
	m_dataReceiver[1]->Close();
}

bool UDPVideoProvider::IsConnected()
{
	return ((UDPDataReceiver*)m_dataReceiver[0])->IsConnected() || ((UDPDataReceiver*)m_dataReceiver[1])->IsConnected();
}


bool UDPVideoProvider::HasNewImage(int id)
{
	if(id<0 || id>=2)
		return 0;
	return m_newimage[id];
}
const video::ImageInfo* UDPVideoProvider::GetImage(int id)
{
	if(id<0 || id>=2)
		return 0;
	return &m_image[id];
}
math::rectf	UDPVideoProvider::GetTexRect(int id)
{
	return math::rectf(0,0,1,1);
}

void UDPVideoProvider::Update(float dt)
{
}


}
}