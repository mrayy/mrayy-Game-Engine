

#include "stdafx.h"
#include "GStreamVideoProvider.h"

#include "GstVideoPlayer.h"
#include "Engine.h"


namespace mray
{
namespace TBee
{

GStreamVideoProvider::GStreamVideoProvider()
{
	m_playBack = 0;
	//m_playBack = new video::GstVideoPlayer();
	//m_playBack->SetFrameSize(1280, 800);
	//m_playBack->Connect("192.168.10.170",170,5000);

}
GStreamVideoProvider::~GStreamVideoProvider()
{
	delete m_playBack;
}
void GStreamVideoProvider::SetFrameSize(int w, int h)
{
	m_playBack->SetFrameSize(w,h);

}

void GStreamVideoProvider::LoadSideBySideMovie(const core::string& left, const core::string& right)
{

	m_playBack->loadSideBySideMovie(left, right);
	m_playBack->play();
	m_startTime = gEngine.getTimer()->getSeconds();
}
void GStreamVideoProvider::LoadMovie(const core::string& path)
{
	m_playBack->loadMovie(path);
	m_playBack->play();
	m_startTime = gEngine.getTimer()->getSeconds();

}

void GStreamVideoProvider::ConnectToCameras(const core::string& ip, int  videoPort, int audioPort, int localPort)
{
	m_playBack->Connect(ip,videoPort,audioPort,localPort);
	m_playBack->play();
	m_startTime = gEngine.getTimer()->getSeconds();
}
void GStreamVideoProvider::Open2Streams(int port1, int port2)
{
	m_playBack->Open2Streams(port1,port2);
	m_playBack->play();
	m_startTime = gEngine.getTimer()->getSeconds();
}
void GStreamVideoProvider::Disconnect()
{
	m_playBack->close();
}
bool GStreamVideoProvider::IsConnected()
{
	return m_playBack->isLoaded();
}

bool GStreamVideoProvider::HasNewImage(int id)
{
	return m_playBack->HasNewFrame();
}
const video::ImageInfo* GStreamVideoProvider::GetImage(int id)
{
	return m_playBack->GetLastFrame();
}
math::rectf	GStreamVideoProvider::GetTexRect(int id)
{
	static const math::rectf trcs[]=
	{
		math::rectf(0,0,1,1),
		math::rectf(0,0,1,1)
	};
	return trcs[id];
}

void GStreamVideoProvider::Update(float dt)
{
	m_playBack->GrabFrame();
}

int GStreamVideoProvider::GetRecvBytesPerSecond()
{
	int bytes = m_playBack->RecevedBytes();
	return bytes * 1000.0f/ (gEngine.getTimer()->getSeconds() - m_startTime);
}


}
}

