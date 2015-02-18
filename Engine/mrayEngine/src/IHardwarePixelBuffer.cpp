#include "stdafx.h"


#include "IHardwarePixelBuffer.h"
#include "PixelUtil.h"



namespace mray{
namespace video{


IHardwarePixelBuffer::IHardwarePixelBuffer(uint width,uint height,uint depth,ETextureType texType,
										   EPixelFormat format,EUsageType usage,uint id,bool useVirtualBuffer)
										   :IHardwareBuffer(usage,id,useVirtualBuffer),
										   m_width(width),m_height(height),m_depth(depth),
										   m_format(format),m_texType(texType)
{
	m_rowPitch=m_width;
	m_slicePitch=m_width*m_height;

	m_sizeInBytes=m_width*m_height*m_depth*PixelUtil::getPixelDescription(m_format).elemSizeB;
}

IHardwarePixelBuffer::~IHardwarePixelBuffer(){
}

void*IHardwarePixelBuffer::inner_lock(uint offset,uint length,ELockOptions options){
	FATAL_ERROR(isLocked(),mT("The buffer is already locked, Cann't Lock it again!!"));
	FATAL_ERROR(offset!=0 || length!=m_sizeInBytes,mT("You cann't lock sub region using this call!"));
	math::box3d b(0,0,0,m_width,m_height,m_depth);
	const LockedPixelBox& lockedBox=lock(b,options);
	return lockedBox.data;
}

const LockedPixelBox& IHardwarePixelBuffer::lock(const math::box3d&lockBox,ELockOptions options){
	if(m_useVirtualBuffer)
	{
		if (options!=ELO_ReadOnly)
		{
			m_VirtualBufferChanged=true;
		}
		m_lockedBox=((IHardwarePixelBuffer*)m_virtualBuffer.pointer())->lock(lockBox,options);
	}else
	{
		m_lockedBox=inner_lock(lockBox,options);
		m_isLocked=true;
	}
	return m_lockedBox;
}
void* IHardwarePixelBuffer::lock(uint offset,uint length,ELockOptions options){
	FATAL_ERROR(true,mT("Not Implemented"));
	return 0;
}

uint IHardwarePixelBuffer::readData(uint offset,uint length,void*dest){
	FATAL_ERROR(true,mT("Not Implemented"));
	return 0;
}
uint IHardwarePixelBuffer::writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer){
	FATAL_ERROR(true,mT("Not Implemented"));
	return 0;
}



}
}
