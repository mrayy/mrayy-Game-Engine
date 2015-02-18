


/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   21:01
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\UDPVideoProvider.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	UDPVideoProvider
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _UDPVideoProvider_
#define _UDPVideoProvider_

#include "IVideoProvider.h"
#include "IVideoGrabber.h"
#include "BaseDataReceiver.h"

namespace mray
{
namespace TBee
{
	class BaseDataReceiver;

class UDPVideoProvider:public IVideoProvider,public IBaseDataReceiverListener
{
protected:
	BaseDataReceiver* m_dataReceiver[2];
	bool m_newimage[2];
	video::ImageInfo m_image[2];

	//video::IVideoGrabber* m_grabbers[2];

	void OnVideoImage(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src);
public:
	UDPVideoProvider();
	virtual~UDPVideoProvider();

	virtual void ConnectToCameras(const core::string& ip,int port1,int port2);
	virtual void Disconnect();
	virtual bool IsConnected();

	virtual bool HasNewImage(int id);
	virtual const video::ImageInfo* GetImage(int id);
	math::rectf	GetTexRect(int id);

	virtual void Update(float dt);
};


}
}

#endif