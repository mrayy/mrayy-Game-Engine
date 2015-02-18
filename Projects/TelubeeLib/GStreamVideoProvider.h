

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   21:27
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\GStreamVideoProvider.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	GStreamVideoProvider
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GStreamVideoProvider_
#define _GStreamVideoProvider_

#include "IVideoProvider.h"
namespace mray
{
	namespace video
	{
		class GstVideoPlayer;
	}
namespace TBee
{

class GStreamVideoProvider:public IVideoProvider
{
protected:
	video::GstVideoPlayer* m_playBack;
	float m_startTime;
public:
	GStreamVideoProvider();
	virtual~GStreamVideoProvider();

	void LoadMovie(const core::string& path);
	void LoadSideBySideMovie(const core::string& left, const core::string& right);
	virtual void ConnectToCameras(const core::string& ip, int videoPort, int audioPort, int localPort);
	virtual void Open2Streams( int port1,int port2);
	virtual void Disconnect();
	virtual bool IsConnected();

	void SetFrameSize(int w, int h);

	virtual bool HasNewImage(int id);
	virtual const video::ImageInfo* GetImage(int id);
	math::rectf	GetTexRect(int id);

	virtual void Update(float dt);

	int GetRecvBytesPerSecond();
};


}
}

#endif
