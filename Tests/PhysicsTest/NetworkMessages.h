


/********************************************************************
	created:	2012/06/30
	created:	30:6:2012   15:32
	filename: 	d:\Development\mrayEngine\Tests\PhysicsTest\NetworkMessages.h
	file path:	d:\Development\mrayEngine\Tests\PhysicsTest
	file base:	NetworkMessages
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __NetworkMessages__
#define __NetworkMessages__

#include "BitConverter.h"
#include "StreamWriter.h"
#include "StreamReader.h"

namespace mray
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
	 enum CameraSource
	{
		Left=0,
		Right
	};
class NetworkMessage
{
private:
protected:
	NetworkMessageType m_type;
public:
	NetworkMessage(NetworkMessageType t):m_type(t)
	{}
	virtual~NetworkMessage(){}

	NetworkMessageType GetType()const{return m_type;}

	virtual void Serialize(OS::StreamWriter& wrt)const 
	{
	}
	virtual bool Deserialize(OS::StreamReader& rdr)
	{
		return true;
	}

	virtual bool CanSkip()
	{
		return true;
	}


	static void SerializePacket(const NetworkMessage* p, OS::StreamWriter& s);

	static NetworkMessage* DeserializePacket( OS::StreamReader& s);
};

class HeadChangeMessage : public NetworkMessage
{
protected:
	math::vector3d m_speed;
public:
	HeadChangeMessage();
	HeadChangeMessage(const math::vector3d& v);
	const math::vector3d& GetAngles()
	{
		return m_speed;
	}
	void SetAngles(const math::vector3d& v)
	{
		m_speed=v;
	}

	bool CanSkip()
	{
		return false;
	}
	virtual void Serialize(OS::StreamWriter& wrt)const ;
	virtual bool Deserialize(OS::StreamReader& rdr);
};


class SpeedChangeMessage : public NetworkMessage
{
protected:
	math::vector2d m_speed;
public:
	SpeedChangeMessage();
	SpeedChangeMessage(const math::vector2d& v);
	const math::vector2d& GetSpeed()
	{
		return m_speed;
	}
	void SetSpeed(const math::vector2d& v)
	{
		m_speed=v;
	}

	bool CanSkip()
	{
		return false;
	}
	virtual void Serialize(OS::StreamWriter& wrt)const ;
	virtual bool Deserialize(OS::StreamReader& rdr);
};

class VideoImageMessage : public NetworkMessage
{
protected:
	video::ImageInfo* m_img;
	CameraSource m_camSource;



	core::string m_encoder;
public:
	 VideoImageMessage();
	 ~VideoImageMessage();

	 CameraSource GetCamSource()
	 {
		return m_camSource;
	 }
	 video::ImageInfo* GetImage()
	 {
		 return m_img;
	 }
	 virtual void Serialize(OS::StreamWriter& wrt)const ;
	 virtual bool Deserialize(OS::StreamReader& rdr);
};


class StopMessage : public NetworkMessage
{
public:
	StopMessage()
		: NetworkMessage(NetworkMessageType::Stop)
	{
	}
	bool CanSkip()
	{
		return false;
	}
};


}

#endif
