

/********************************************************************
	created:	2013/03/01
	created:	1:3:2013   12:34
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\NetworkMessage.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	NetworkMessage
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __NetworkMessage__
#define __NetworkMessage__

#include "BitConverter.h"


namespace mray
{
	namespace OS
	{
		class StreamWriter;
		class StreamReader;
	}
namespace TBee
{

	enum NetworkMessageType
	{
		HeadChange=1,
		SpeedChange,
		VideoImage,
		Stop,

		UdpPort
	};
	enum UDPMessages
	{
		Connect = 100,
		Disconnect = 115
	};

	enum ENetCameraSource
	{
		ENCamera_Left,
		ENCamera_Right
	};

class NetworkMessage
{
protected:

	virtual void Serialize(OS::StreamWriter* s)
	{
	}
	virtual void Deserialize(OS::StreamReader* s)
	{
	}
public:


	NetworkMessageType msgType;

	 NetworkMessage()
	{
	}
	NetworkMessage(NetworkMessageType t)
	{
		msgType = t;
	}

	virtual bool CanSkip()
	{
		return true;
	}


	static void SerializePacket(NetworkMessage p, OS::StreamWriter* s);
	static NetworkMessage* DeserializePacket( OS::StreamReader* s);
};


class VideoImageMessage : public NetworkMessage
{
protected:
	void Serialize(OS::StreamWriter* s);
	void Deserialize(OS::StreamReader* s);
public:
	video::ImageInfo* m_bmp;
	ENetCameraSource m_camSource;

	core::string m_encoder;
	VideoImageMessage(): NetworkMessage(NetworkMessageType::VideoImage)
	{
		m_bmp=0;
	}
	VideoImageMessage(video::ImageInfo* s, const core::string& encoder,ENetCameraSource src)
		: NetworkMessage(NetworkMessageType::VideoImage)
	{
		m_camSource = src;
		m_bmp = s;
		m_encoder = encoder;
	}
	~VideoImageMessage();
};


}
}



#endif
