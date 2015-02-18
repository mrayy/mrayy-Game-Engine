

/********************************************************************
	created:	2013/03/01
	created:	1:3:2013   12:21
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\BaseDataReceiver.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	BaseDataReceiver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __BaseDataReceiver__
#define __BaseDataReceiver__


#include "ListenerContainer.h"

namespace mray
{
	namespace video
	{
		class ImageInfo;
	}
namespace TBee
{
	class BaseDataReceiver;

	enum ECameraSource
	{
		ECamera_Left,
		ECamera_Right
	};

	class IBaseDataReceiverListener
	{
	public:
		virtual void OnVideoImage(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src){}
		virtual void OnUdpPort(BaseDataReceiver*r,int port){}
		virtual void OnUdpStop(BaseDataReceiver*r){}
	};

class BaseDataReceiver:public ListenerContainer<IBaseDataReceiverListener*>
{
protected:
	bool m_closed;
	DECLARE_FIRE_METHOD(OnVideoImage,(BaseDataReceiver*r,video::ImageInfo* image,ECameraSource src),(r,image,src));
	DECLARE_FIRE_METHOD(OnUdpPort,(BaseDataReceiver*r,int port),(r,port));
	DECLARE_FIRE_METHOD(OnUdpStop,(BaseDataReceiver*r),(r));
public:
	BaseDataReceiver(){m_closed=false;}
	virtual~BaseDataReceiver(){Close();}

	virtual void Start()
	{
	}

	virtual void Close()
	{
		m_closed = true;
	}
};

}
}


#endif
