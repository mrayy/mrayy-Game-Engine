

#include "stdafx.h"
#include "PortAudioCommon.h"
#include "ILogManager.h"
#include "StringConverter.h"

#include <portaudio/portaudio.h>


namespace mray
{
namespace sound
{

PortAudioCommon PortAudioCommon::Instance;

PortAudioCommon::PortAudioCommon()
{
	m_Inited=false;
	m_refCount=0;
}
PortAudioCommon::~PortAudioCommon()
{
	Shutdown();
}


void PortAudioCommon::Init()
{
	if(m_Inited)
		return;

	PaError err;
	err = Pa_Initialize();
	if( err != paNoError ){
		gLogManager.log(mT("PortAudio::Init() -  ")+core::StringConverter::toString(Pa_GetErrorText(err)),ELL_WARNING);
		return;
	}else{
		m_Inited=true;
	}
}
void PortAudioCommon::Shutdown()
{
	if(!m_Inited)
		return;
	Pa_Terminate();
}
bool PortAudioCommon::IsInited()
{
	return m_Inited;
}

void PortAudioCommon::AddRef()
{
	if(!m_refCount)
		Init();
	m_refCount++;
}
void PortAudioCommon::SubRef()
{
	m_refCount--;
	if(!m_refCount)
		Shutdown();
}


}
}
