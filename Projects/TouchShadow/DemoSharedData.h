

/********************************************************************
	created:	2013/09/08
	created:	8:9:2013   1:16
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\DemoSharedData.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	DemoSharedData
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DemoSharedData__
#define __DemoSharedData__




namespace mray
{

	class UserFrame;
	class DeviceController;
	class InteractionDemo;
	class TSApplication;
	class AudioInterfaceManager;
	class ChannelManager;
	class EffectManager;

class DemoSharedData
{
protected:
	static DemoSharedData* s_Instance;
public:

	static DemoSharedData* Instance(){return s_Instance;}

	DemoSharedData():ActiveUserFrame(0),DeviceController(0),ActiveDemo(0)
	{
	}
	virtual~DemoSharedData(){}

public:
	UserFrame* ActiveUserFrame;
	DeviceController* DeviceController;
	InteractionDemo* ActiveDemo;
	AudioInterfaceManager* AudioManager;
	ChannelManager* ChannelManager;
	EffectManager* EffectManager;

	TSApplication* App;
};

}


#endif
