
#include "stdafx.h"
#include "NetworkMessages.h"

#include "BitConverter.h"
#include "TextureResourceManager.h"
#include "TexLoader.h"

namespace mray
{


	void NetworkMessage::SerializePacket(const NetworkMessage* p, OS::StreamWriter& s)
	{
		s.binWriteInt(p->GetType());
		byte data[10];
		int len=core::BitConverter::GetBytes((int)p->GetType(),data,sizeof(data));
		s.write(data,len);
		p->Serialize(s);
	}

	NetworkMessage* NetworkMessage::DeserializePacket( OS::StreamReader& s)
	{
		byte d[4];
		s.read(d, 4);
		NetworkMessageType v=(NetworkMessageType)core::BitConverter::ToUInt32(d);
		NetworkMessage* ret = 0;
		switch (v)
		{
		case NetworkMessageType::SpeedChange:
			ret = new SpeedChangeMessage();
			break;
		case NetworkMessageType::HeadChange:
			ret = new HeadChangeMessage();
			break;
		case NetworkMessageType::Stop:
			ret = new StopMessage();
			break;
		case NetworkMessageType::VideoImage:
			ret = new VideoImageMessage();
			break;
		}
		if (ret != 0)
			if(!ret->Deserialize(s))
			{
				delete ret;
				ret=0;
			}
		return ret;
	}

	HeadChangeMessage::HeadChangeMessage()
		: NetworkMessage(NetworkMessageType::HeadChange)
	{
	}
	HeadChangeMessage::HeadChangeMessage(const math::vector3d& v)
		: NetworkMessage(NetworkMessageType::HeadChange)
	{
		m_speed=v;
	}

	void HeadChangeMessage::Serialize(OS::StreamWriter& s)const
	{
		byte b[10];
		int len = core::BitConverter::GetBytes(m_speed.x,b,sizeof(b));
		s.write(b, len);
		 len = core::BitConverter::GetBytes(m_speed.y,b,sizeof(b));
		s.write(b, len);
		 len = core::BitConverter::GetBytes(m_speed.z,b,sizeof(b));
		s.write(b, len);
	}

	bool HeadChangeMessage::Deserialize(OS::StreamReader& s)
	{
		byte data[4];
		s.read(data,  4);
		m_speed.x = core::BitConverter::ToFloat(data);
		s.read(data, 4);
		m_speed.y = core::BitConverter::ToFloat(data);
		s.read(data, 4);
		m_speed.z = core::BitConverter::ToFloat(data);
		return true;
	}


	SpeedChangeMessage ::SpeedChangeMessage()
		: NetworkMessage(NetworkMessageType::SpeedChange)
	{
	}
	SpeedChangeMessage::SpeedChangeMessage(const math::vector2d& v)
		: NetworkMessage(NetworkMessageType::SpeedChange)
	{
		m_speed=v;
	}

	void SpeedChangeMessage::Serialize(OS::StreamWriter& s)const
	{
		byte b[10];
		int len = core::BitConverter::GetBytes(m_speed.x,b,sizeof(b));
		s.write(b, len);
		 len = core::BitConverter::GetBytes(m_speed.y,b,sizeof(b));
		s.write(b, len);
	}

	bool SpeedChangeMessage::Deserialize(OS::StreamReader& s)
	{
		byte data[4];
		s.read(data,  4);
		m_speed.x = core::BitConverter::ToFloat(data);
		s.read(data, 4);
		m_speed.y = core::BitConverter::ToFloat(data);
		return true;
	}

	VideoImageMessage::VideoImageMessage()
		: NetworkMessage(NetworkMessageType::VideoImage)
	{
		m_img=new video::ImageInfo();
	}
	VideoImageMessage::~VideoImageMessage()
	{
		delete m_img;
	}

	void VideoImageMessage::Serialize(OS::StreamWriter& s)const
	{
		byte b[10];
		int len = core::BitConverter::GetBytes((int)m_camSource,b,10);
		s.write(b, len);
		//ImageEncoder.Encode(m_bmp, s, m_encoder);
	}
	bool VideoImageMessage::Deserialize(OS::StreamReader& s)
	{
		byte data[4];
		s.read(data,  4);
		m_camSource = (CameraSource)core::BitConverter::ToInt32(data);
		s.read(data,  4);
		int len=core::BitConverter::ToInt32(data);
		OS::CMemoryStream stream;
		byte* buff=new byte[len];
		s.read(buff,  len);
		stream.setData(buff,len);
		stream.setDeleteAtEnd(true);
		loaders::ITexLoader* l=gTextureResourceManager.GetLoaders()->getLoader("jpg");
		return l->load(&stream,m_img,video::ETT_2DTex);
		//m_bmp = new Bitmap(ImageEncoder.Decode(s));
	}



}

