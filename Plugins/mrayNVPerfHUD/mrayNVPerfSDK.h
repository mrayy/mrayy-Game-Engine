

/********************************************************************
	created:	2012/02/20
	created:	20:2:2012   9:40
	filename: 	d:\Development\mrayEngine\Engine\mrayNVPrefHUD\mrayNVPerfSDK.h
	file path:	d:\Development\mrayEngine\Engine\mrayNVPrefHUD
	file base:	mrayNVPerfSDK
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __mrayNVPerfSDK__
#define __mrayNVPerfSDK__

#include "RenderWindow.h"

namespace mray
{
namespace video
{

enum EPerfHUDCounterName
{
	EGPUIdle,
	EPixelShaderBusy,
	EVertexShaderBusy,
	EGPUBusy,

	ECountersCount
};

class mrayNVPerfSDK;

class INVPerfSDKListener
{
public:
	virtual void OnCounterAdd(mrayNVPerfSDK* perf,EPerfHUDCounterName counter){};
	virtual void OnCounterRemoved(mrayNVPerfSDK* perf,EPerfHUDCounterName counter){};
	virtual void OnCounterClear(mrayNVPerfSDK* perf){}

	virtual void OnNewSample(mrayNVPerfSDK* perf){}
};

class mrayNVPerfSDK:public IRenderWindowListener,ListenerContainer<INVPerfSDKListener>
{
public:

private:
protected:
	static const int m_maxBufferEntryCount=10;
	bool m_isInited;

	int m_currentBufferEntry;
	uint m_counterIndex[ECountersCount];
	uint m_counterValues[ECountersCount][m_maxBufferEntryCount];

	DECLARE_FIRE_METHOD(OnCounterAdd,(mrayNVPerfSDK* perf,EPerfHUDCounterName counter),(perf,counter));
	DECLARE_FIRE_METHOD(OnCounterRemoved,(mrayNVPerfSDK* perf,EPerfHUDCounterName counter),(perf,counter));
	DECLARE_FIRE_METHOD(OnCounterClear,(mrayNVPerfSDK* perf),(perf));
	DECLARE_FIRE_METHOD(OnNewSample,(mrayNVPerfSDK* perf),(perf));
public:
	mrayNVPerfSDK();
	virtual~mrayNVPerfSDK();

	bool Init();
	void Shutdown();

	bool AddCounter(EPerfHUDCounterName name);
	bool DeleteCounter(EPerfHUDCounterName name);
	void ClearCounters();

	bool HasCounter(EPerfHUDCounterName name);

	void Sample();

	float GetCounterValue(EPerfHUDCounterName name);

	virtual void WindowSwapBuffer(RenderWindow* wnd);
};


}
}

#endif
