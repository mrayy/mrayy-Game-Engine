
/********************************************************************
	created:	2014/01/18
	created:	18:1:2014   21:34
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\IGstAppListener.h
	file path:	C:\Development\mrayEngine\Plugins\mrayGStreamer
	file base:	IGstAppListener
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ofGstAppSink__
#define __ofGstAppSink__


namespace mray
{
namespace video
{

class IGstAppListener
{
public:
	virtual ~IGstAppListener(){}
	virtual int OnBuffer(void * buffer){ return 0; }

	virtual void OnEOS(){}

	// pings when enough data has arrived to be able to get sink properties
	virtual void OnStreamPrepared(){};
};


}
}


#endif
