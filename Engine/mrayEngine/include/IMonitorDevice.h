

/********************************************************************
	created:	2012/09/26
	created:	26:9:2012   17:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IMonitorDevice.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IMonitorDevice
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IMonitorDevice___
#define ___IMonitorDevice___

#include "mstring.h"

namespace mray
{
namespace video
{

class IMonitorDevice
{
protected:

public:
	IMonitorDevice(){}
	virtual~IMonitorDevice(){}
	

	virtual int GetIndex()=0;
	virtual const core::string& GetName()const=0;
	virtual const math::vector2di& GetSize()const=0;
	virtual const math::vector2di& GetStartPosition()const=0;
	virtual const math::recti& GetRect()const=0;

	virtual bool Refresh()=0;
	virtual bool ChangeSettings(int width,int height,int colorDepth,int freq)=0;
	virtual void RestoreSettings()=0;


	
};

}
}

#endif

