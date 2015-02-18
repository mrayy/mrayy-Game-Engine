

#include "stdafx.h"
#include "GeomDepthRect.h"
#include "GZipCompress.h"



namespace mray
{
namespace TBee
{

GeomDepthRect::GeomDepthRect()
{
}
GeomDepthRect::~GeomDepthRect()
{
}

void GeomDepthRect::SetFrame(DepthFrame* src, math::rectf& rc)
{
	m_rect = src->GetCroppedFrame(rc, &m_frame);
}

int GeomDepthRect::WriteToStream(OS::IStream*stream)
{
	int len = 0;
	len+=stream->write(&m_rect,sizeof(m_rect));
	int w = m_rect.getWidth();
	int h = m_rect.getHeight();
	ushort* data= m_frame.GetRawData();

	
	OS::CMemoryStream src("", (uchar*)data, w*h*sizeof(ushort), false);
	Compress::GZipCompress c;
	int ret=c.compress(&src, stream);
	len += ret;
	//len += stream->write(data, w*h*sizeof(ushort));
	return len;
}

int GeomDepthRect::ReadFromStream(OS::IStream*stream)
{
	int len = 0;
	len+=stream->read(&m_rect, sizeof(m_rect));
	Compress::GZipCompress c;

	m_frame.CreateRawData(m_rect.getWidth(), m_rect.getHeight());
	OS::CMemoryStream dst("", (uchar*)m_frame.GetRawData(), m_rect.getWidth()*m_rect.getHeight()*sizeof(ushort), false);
	len+=c.decompress(stream, &dst);
	//len += stream->read(m_frame.GetRawData(), m_rect.getWidth()*m_rect.getHeight()*sizeof(ushort));
	return len;
}


}
}
