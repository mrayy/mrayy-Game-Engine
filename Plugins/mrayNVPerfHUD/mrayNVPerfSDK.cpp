
#include "stdafx.h"
#include "mrayNVPerfSDK.h"
#include "ILogManager.h"

#include <NVPerfSDK.h>


namespace mray
{
namespace video
{
	static char* g_NVPMCounterNames[]=
	{
		"gpu_idle",
		"pixel_shader_busy",
		"vertex_shader_busy",
		"gpu_busy"
	};

mrayNVPerfSDK::mrayNVPerfSDK()
{
	m_currentBufferEntry=0;
	m_isInited=false;

	for(int i=0;i<ECountersCount;++i)
	{
		m_counterIndex[i]=0;
		for(int j=0;j<m_maxBufferEntryCount;++j)
		{
			m_counterValues[i][j]=0.0f;
		}
	}
}
mrayNVPerfSDK::~mrayNVPerfSDK()
{
	Shutdown();
}

bool mrayNVPerfSDK::Init()
{
	NVPMRESULT stat;
	if(m_isInited)
		return true;

	stat=NVPMInit();
	m_isInited=true;
	if(stat!=NVPM_OK)
	{
		gLogManager.log(mT("mrayNVPerfSDK::Init() - Failed to init mrayNVPerfSDK"),ELL_WARNING);
		return false;
	}
	return true;
}
void mrayNVPerfSDK::Shutdown()
{
	if(!m_isInited)
		return;
	NVPMShutdown();

	m_isInited=false;
	for(int i=0;i<ECountersCount;++i)
		m_counterIndex[i]=0;
}

bool mrayNVPerfSDK::AddCounter(EPerfHUDCounterName name)
{
	if(!m_isInited)
		return false;
	UINT counterIndex;
	if(NVPMGetCounterIndex(g_NVPMCounterNames[name], &counterIndex) == NVPM_OK) 
	{
		if(NVPMAddCounter(counterIndex) == NVPM_OK)
		{
			m_counterIndex[name]=counterIndex;

			FIRE_LISTENR_METHOD(OnCounterAdd,(this,name));

			return true;
		}else
			return false;
	}else
		return false;
}
bool mrayNVPerfSDK::DeleteCounter(EPerfHUDCounterName name)
{
	if(!m_isInited)
		return false;

	UINT counterIndex;
	if(NVPMGetCounterIndex(g_NVPMCounterNames[name], &counterIndex) == NVPM_OK) 
	{
		if(NVPMRemoveCounter(counterIndex)== NVPM_OK) 
		{
			m_counterIndex[name]=0;
			FIRE_LISTENR_METHOD(OnCounterRemoved,(this,name));
			return true;
		}else
			return false;
	}else
		return false;
}
void mrayNVPerfSDK::ClearCounters()
{
	NVPMRemoveAllCounters();
	for(int i=0;i<ECountersCount;++i)
		m_counterIndex[i]=0;
	FIRE_LISTENR_METHOD(OnCounterClear,(this));
}
bool mrayNVPerfSDK::HasCounter(EPerfHUDCounterName name)
{
	return m_counterIndex[name]!=0;
}

void mrayNVPerfSDK::Sample()
{
	if(!m_isInited)
		return;

	UINT unused;
	UINT64 events, cycles;

	NVPMSample(0,&unused);	//sample gpu counters
	for(int i=0;i<ECountersCount;++i)
	{
		if(m_counterIndex[i]==0)
			continue;

		NVPMGetCounterValue(m_counterIndex[i],0,&events, &cycles);
		m_counterValues[i][m_currentBufferEntry]=cycles ? (100.0f*(float)events/(float)cycles):0.0f;
	}
	FIRE_LISTENR_METHOD(OnNewSample,(this));

	m_currentBufferEntry=(m_currentBufferEntry+1)%m_maxBufferEntryCount;

}

float mrayNVPerfSDK::GetCounterValue(EPerfHUDCounterName name)
{
	if(!m_isInited)
		return 0;

	if(m_counterIndex[name]==0)
		return 0.0f;

	float total=0;
	for(int j=0;j<m_maxBufferEntryCount;++j)
	{
		total+=m_counterValues[name][j]/(float)m_maxBufferEntryCount;
	}
	return total;
}

void mrayNVPerfSDK::WindowSwapBuffer(RenderWindow* wnd)
{
	Sample();
}


}
}


