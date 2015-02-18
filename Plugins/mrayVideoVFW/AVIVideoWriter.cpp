

#include "stdafx.h"
#include "AVIVideoWriter.h"
#include "ITexture.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vfw.h>


namespace mray
{
namespace video
{

	DWORD g_CodecType[]=
	{
		mmioFOURCC('W','M','V','2'),
		mmioFOURCC('W','M','V','3'),
		mmioFOURCC('D','I','V','X')
	};


	struct AVIVideoWriterData
	{
		PAVIFILE file;
		PAVISTREAM stream;
		PAVISTREAM streamCompressed;
	};

AVIVideoWriter::AVIVideoWriter()
{
	m_codec=DIVX;
	m_data=new AVIVideoWriterData();
	//m_buffer=0;
	m_data->file=0;
	m_data->stream=0;
	m_data->streamCompressed=0;
	m_position=0;
	m_rate=30;
	m_quality=50;
}
AVIVideoWriter::~AVIVideoWriter()
{
	Close();
	delete m_data;
}


void AVIVideoWriter::Close()
{
	if(m_data->streamCompressed)
		AVIStreamClose(m_data->streamCompressed);
	if(m_data->stream)
		AVIStreamClose(m_data->stream);
	if(m_data->file)
		AVIFileClose(m_data->file);

	//delete [] m_buffer;
//	m_buffer=0;

	m_data->file=0;
	m_data->stream=0;
	m_data->streamCompressed=0;

	m_sampleWritten=0;
	m_bytesWritten=0;
}
bool AVIVideoWriter::Open(const core::string& path,const math::vector2d &size)
{
	Close();

	if(!SUCCEEDED(AVIFileOpen(&m_data->file,path.c_str(),OF_CREATE | OF_WRITE,0)))
		return false;

	m_stride=size.x*3;
	if(m_stride % 4 !=0)
		m_stride+=4-m_stride % 4;

	m_size=size;
	AVISTREAMINFO info;
	info.fccType=streamtypeVIDEO;
	info.fccHandler=g_CodecType[m_codec];
	info.dwScale=1;
	info.dwRate=m_rate;
	info.dwSuggestedBufferSize=m_stride*size.y;


	if(!SUCCEEDED(AVIFileCreateStream(m_data->file,&m_data->stream,&info)))
	{
		AVIFileClose(m_data->file);
		m_data->streamCompressed=0;
		m_data->stream=0;
		m_data->file=0;
		return false;
	}

	AVICOMPRESSOPTIONS options;
	options.fccHandler=g_CodecType[m_codec];
	options.dwQuality=m_quality;

	if(!SUCCEEDED(AVIMakeCompressedStream(&m_data->streamCompressed,m_data->stream,&options,0)))
	{
		AVIStreamClose(m_data->stream);
		AVIFileClose(m_data->file);
		m_data->streamCompressed=0;
		m_data->stream=0;
		m_data->file=0;
		return false;
	}

	BITMAPINFOHEADER header;
	header.biSize=sizeof(BITMAPINFOHEADER);
	header.biWidth=m_size.x;
	header.biHeight=m_size.y;
	header.biPlanes=1;
	header.biBitCount=24;
	header.biSizeImage=0;
	header.biCompression=0;

	if(!AVIStreamSetFormat(m_data->streamCompressed,0,&header,sizeof(header)))
	{
		AVIStreamClose(m_data->streamCompressed);
		AVIStreamClose(m_data->stream);
		AVIFileClose(m_data->file);

		m_data->streamCompressed=0;
		m_data->stream=0;
		m_data->file=0;
		return false;
	}

	//m_buffer=new uchar[m_stride*size.y];
	m_position=0;

	return true;
}

bool AVIVideoWriter::AddFrame(video::ITexture* tex)
{
	if(!m_data->streamCompressed)
		return false;
	IHardwarePixelBuffer* surf=tex->getSurface(0);
	if(!surf)
		return false;
	if(surf->getWidth()!=m_size.x || surf->getHeight()!=m_size.y || surf->getFormat()!=video::EPixel_R8G8B8)
		return false;

	math::box3d box(0,0,0,m_size.x,m_size.y,1);


	const video::LockedPixelBox& pbox=surf->lock(box,video::IHardwareBuffer::ELO_ReadOnly);

	LONG sW=0,bW=0;

	if(!SUCCEEDED(AVIStreamWrite(m_data->streamCompressed,m_position,1,pbox.data,m_stride*m_size.y,0,&sW,&bW)))
	{
		return false;
	}
	surf->unlock();
	++m_position;
	return true;
}


}
}
