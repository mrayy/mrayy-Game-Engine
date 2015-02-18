#include "stdafx.h"

#include "GLHardwarePixelBuffer.h"
#include "PixelUtil.h"
#include "GLDev.h"


namespace mray{
namespace video{


GLHardwarePixelBuffer::GLHardwarePixelBuffer(uint width,uint height,uint depth,ETextureType texType,EPixelFormat format,
											 EUsageType usage,uint id)
											 :IHardwarePixelBuffer(width,height,depth,texType,format,
											 usage,id,false),
											 m_buffer(width,height,depth,format,0),
											 m_internalFormat(GL_NONE)
{

}


GLHardwarePixelBuffer::~GLHardwarePixelBuffer()
{
	delete [] (uchar*)m_buffer.data;
}

const core::string& GLHardwarePixelBuffer::getDeviceType()
{
	return GLDev::m_deviceType;
}

void GLHardwarePixelBuffer::Resize(uint width,uint height,uint depth)
{
	m_buffer=LockedPixelBox(width,height,depth,m_format,0);

	m_width=width;
	m_height=height;
	m_depth=depth;

	m_rowPitch=m_width;
	m_slicePitch=m_width*m_height;

	m_sizeInBytes=m_width*m_height*m_depth*PixelUtil::getPixelDescription(m_format).elemSizeB;
}

void GLHardwarePixelBuffer::allocBuffer(){
	if(m_buffer.data)
		return;
	m_buffer.data=new uchar[m_sizeInBytes];
}
void GLHardwarePixelBuffer::deallocBuffer(){
	if(m_usage & EUT_Static){
		delete [] (uchar*)m_buffer.data;
		m_buffer.data=0;
	}
}

LockedPixelBox GLHardwarePixelBuffer::inner_lock(const math::box3d&lockBox,ELockOptions options)
{
	allocBuffer();
	if(options != ELO_Discard && (m_usage & EUT_WriteOnly)==0){
		download(m_buffer);
	}
	m_buffer.format = m_format;
	m_currentLO=options;
	return m_buffer.getSubBox(lockBox);
}


void GLHardwarePixelBuffer::inner_unlock()
{
	if (m_currentLO != ELO_ReadOnly)
	{
		// From buffer to card, only upload if was locked for writing
		upload(m_lockedBox);
	}

	deallocBuffer();
}


void GLHardwarePixelBuffer::blitFromMemory(const LockedPixelBox&b){
// 	if( b.box.getWidth()==m_width &&
// 		b.box.getHeight()==m_height &&
// 		b.box.getDepth()==m_depth)
	{
		upload(b);
		deallocBuffer();
	}
}

void GLHardwarePixelBuffer::download(const LockedPixelBox&b)
{
	FATAL_ERROR(true,mT("cann't download() from GLHardwarePixelBuffer"));
}
void GLHardwarePixelBuffer::upload(const LockedPixelBox&b)
{
	FATAL_ERROR(true,mT("cann't upload() from GLHardwarePixelBuffer"));
}

void GLHardwarePixelBuffer::bindToFrameBuffer(GLenum attachment,uint zoffset){
	FATAL_ERROR(true,mT("cann't bindToFrameBuffer() from GLHardwarePixelBuffer"));
}

}
}
