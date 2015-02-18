

/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   20:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IHardwareBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IHardwareBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHardwareBuffer___
#define ___IHardwareBuffer___

#include "GCPtr.h"
#include "mTypes.h"
#include "macros.h"
//#include "EDeviceType.h"
#include "mraySystem.h"

namespace mray{
namespace video{

class MRAY_DLL IHardwareBuffer
{
public:

	enum EUsageType
	{
		EUT_Static=0x1,
		EUT_Dynamic=0x2,
		EUT_WriteOnly=0x4,
		EUT_StaticWriteOnly=0x5,
		EUT_DynamicWriteOnly=0x6,
		EUT_Discardable=0x8,
 		EUT_DynamicWriteOnlyDiscardable=0xE
	};
	enum ELockOptions
	{
		/** Normal mode,  allows read/write and contents are preserved. */
		ELO_Normal,
		/** Discards the entire buffer while locking; this allows optimisation to be 
		performed because synchronisation issues are relaxed. Only allowed on buffers 
		created with the EUT_Dynamic flag. 
		*/
		ELO_Discard,
		/** Lock the buffer for reading only. Not allowed in buffers which are created with EUT_WriteOnly. 
		Mandatory on static buffers, i.e. those created without the EUT_Dynamic flag. 
		*/ 
		ELO_ReadOnly,
		/** As HBL_NORMAL, except the application guarantees not to overwrite any 
		region of the buffer which has already been used in this frame, can allow
		some optimization on some APIs. */
		ELO_NoOverwrite

	};

protected:
	EUsageType m_usage;
	uint m_sizeInBytes;
	bool m_isLocked;

	GCPtr<IHardwareBuffer> m_virtualBuffer;
	bool m_useVirtualBuffer;
	bool m_VirtualBufferChanged;

	uint m_startLock;
	uint m_sizeLocked;

	uint m_id;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options)=0;
	virtual void inner_unlock()=0;
	virtual bool inner_resize(uint newSize){
		if(m_useVirtualBuffer){
			m_virtualBuffer->resize(newSize);
		}
		return true;
	}

	virtual void updateFromVirtual(){
		if(m_useVirtualBuffer && m_VirtualBufferChanged){
			const void*src=m_virtualBuffer->inner_lock(m_startLock,m_sizeLocked,ELO_ReadOnly);
			ELockOptions op;
			if(m_startLock==0 && m_sizeLocked==m_sizeInBytes)
				op=ELO_Discard;
			else
				op=ELO_Normal;
			void*dest=inner_lock(m_startLock,m_sizeInBytes,op);
			mraySystem::memCopy(dest,src,m_sizeInBytes);
			inner_unlock();

			m_virtualBuffer->inner_unlock();
			m_VirtualBufferChanged=false;
		}
	}
public:
	IHardwareBuffer(EUsageType usage,uint id,bool useVirtualBuffer){
		m_useVirtualBuffer=useVirtualBuffer;
		m_id=id;
		m_virtualBuffer=0;
		m_sizeInBytes=0;
		m_isLocked=0;
		m_usage=usage;
		if(m_useVirtualBuffer && m_usage==EUT_Dynamic){
			m_usage=EUT_DynamicWriteOnly;
		}else if(m_useVirtualBuffer && m_usage==EUT_Static){
			m_usage=EUT_StaticWriteOnly;
		}
	}
	virtual~IHardwareBuffer(){
	}
	virtual void*lock(uint offset,uint length,ELockOptions options){
		FATAL_ERROR(isLocked(),mT("The buffer is already locked, Cann't Lock it again!!"));
		if(length==0)length=m_sizeInBytes-offset;
		m_startLock=offset;
		m_sizeLocked=length;
		m_isLocked=true;
		if(m_useVirtualBuffer){
			if(options!=ELO_ReadOnly){
				m_VirtualBufferChanged=true;
			}
			return m_virtualBuffer->lock(offset,length,options);
		}else{
			return inner_lock(offset,length,options);
		}
	}
	inline uint getID(){return m_id;}

	virtual void unlock(){
		FATAL_ERROR(!isLocked(),mT("The buffer is not locked, Cann't unlock it!!"));
		m_isLocked=false;
		if(m_useVirtualBuffer && m_virtualBuffer->isLocked()){
			m_virtualBuffer->unlock();
			updateFromVirtual();
		}else{
			inner_unlock();
		}
	}
	bool isLocked()const{
		return m_isLocked;
	}

	void copyData(IHardwareBuffer*src,uint srcOffset,uint dstOffset,uint length){
		const void*vsrc=src->lock(srcOffset,length,ELO_ReadOnly);
		writeData(dstOffset,length,vsrc,false);
		src->unlock();
	}

	virtual uint readData(uint offset,uint length,void*dest)=0;
	virtual uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer=false)=0;

	uint getSizeInBytes()const{return m_sizeInBytes;}
	EUsageType getUsageType()const{return m_usage;}
	bool hasVirtualBuffer()const{return m_useVirtualBuffer;}

	virtual bool isInGPU()=0;

	bool resize(uint newSize){
		if(inner_resize(newSize)){
			m_sizeInBytes=newSize;
		}
		return true;
	}

	virtual const core::string& getDeviceType()=0;
};
MakeSharedPtrType(IHardwareBuffer)

}
}

#endif //___IHardwareBuffer___
