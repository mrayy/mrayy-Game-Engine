

#include "stdafx.h"
#include "NetworkMessage.h"
#include "StreamWriter.h"
#include "StreamReader.h"
#include "BitConverter.h"
#include "TextureResourceManager.h"
#include "TexLoader.h"


namespace mray
{
namespace TBee
{

void NetworkMessage::SerializePacket(NetworkMessage p, OS::StreamWriter* s)
{
#define DATA_LEN 4
	byte data[DATA_LEN];
	int len=core::BitConverter::GetBytes((int)p.msgType,data,DATA_LEN);
	s->write(data, len);
	p.Serialize(s);
}

NetworkMessage* NetworkMessage::DeserializePacket( OS::StreamReader* s)
{
#define DATA_LEN 128
	byte data[DATA_LEN];
	s->read(data, 4);
	NetworkMessageType v=(NetworkMessageType)core::BitConverter::ToUInt32(data);
	NetworkMessage *ret = 0;
	switch (v)
	{
	case NetworkMessageType::SpeedChange:
	case NetworkMessageType::HeadChange:
	case NetworkMessageType::Stop:
		break;
	case NetworkMessageType::VideoImage:
		ret = new VideoImageMessage();
		break;
	}
	if (ret != 0)
		ret->Deserialize(s);
	return ret;
}

VideoImageMessage::~VideoImageMessage()
{
	delete m_bmp;
}

	void VideoImageMessage::Serialize(OS::StreamWriter* s)
	{
#define DATA_LEN 4
		byte b[DATA_LEN];
// 		b = BitConverter.GetBytes((int)m_camSource);
// 		s.Write(b, 0, b.Length);
		//ImageEncoder.Encode(m_bmp, s, m_encoder);
	}
	void VideoImageMessage::Deserialize(OS::StreamReader*s)
	{
		m_bmp=new video::ImageInfo();
#define DATA_LEN 32
		byte data[DATA_LEN];
		s->read(data,  4);
		m_camSource = (ENetCameraSource)core::BitConverter::ToInt32(data);

		s->read(data,  4);//read length
 		int length=core::BitConverter::ToUInt32(data);
// 		s->read(data,  16);//read Hash
		loaders::TexLoader* l= gTextureResourceManager.GetLoaders();
		loaders::ITexLoader* loader= l->getLoader("jpg");
		loader->load(s->getStream(),m_bmp,video::ETT_2DTex);
		//m_bmp = new Bitmap(ImageEncoder.Decode(s));
	}

}
}
