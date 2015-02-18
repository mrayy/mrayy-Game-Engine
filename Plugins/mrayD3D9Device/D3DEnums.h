


/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   15:00
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DEnums.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DEnums
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DEnums__
#define __D3DEnums__

#include "IHardwareBuffer.h"
#include "IHardwareIndexBuffer.h"

namespace mray
{
namespace video
{
class D3DEnums
{
public:

	static DWORD Map(IHardwareBuffer::EUsageType usage);
	static D3DFORMAT Map(IHardwareIndexBuffer::EIndexType usage);
	static DWORD Map(IHardwareBuffer::ELockOptions options,IHardwareBuffer::EUsageType usage);

};
}
}

#endif
