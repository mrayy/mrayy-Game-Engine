

#include "stdafx.h"
#include "DirectSoundOutputStream.h"
#include "ILogManager.h"

#include <dsound.h>

namespace mray
{
namespace sound
{

struct DirectSoundOutDeviceInfo
{
	LPGUID lpGuid;
	core::string Desciption;
	core::string module;
};


class DirectSoundOutputStreamData
{
public:

	DirectSoundOutDeviceData* dsData;
	LPDIRECTSOUNDBUFFER	outputBuffer;
	int		lastWriteOffset;

	WAVEFORMATEX waveFormat;

	void* pHEvent[2];
	uchar* audioBuffer;

	MMRESULT timerID;

	uint circles[2];
};

struct DirectSoundOutDeviceData
{
	DirectSoundOutDeviceData()
	{
		device=0;
	}
	LPDIRECTSOUND device;
	std::vector<DirectSoundOutDeviceInfo> devices;
};


BOOL CALLBACK DirectSoundEnumCallback(LPGUID lpGuid,
	LPCSTR lpcstrDescription,
	LPCSTR lpcstrModule,
	LPVOID lpContext)
{
	std::vector<DirectSoundOutDeviceInfo> &lst=*(std::vector<DirectSoundOutDeviceInfo>*)lpContext;
	DirectSoundOutDeviceInfo ifo;
	ifo.lpGuid=lpGuid;
	ifo.Desciption= lpcstrDescription;
	ifo.module=lpcstrModule;
	lst.push_back(ifo);
	return true;
}


void CALLBACK TimerProcess(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	DirectSoundOutputStream *pDDS = (DirectSoundOutputStream *)dwUser;
	pDDS->_TimerCallback();	
}

DirectSoundOutputStream::DirectSoundOutputStream()
{
	m_data=new DirectSoundOutputStreamData;
	m_data->lastWriteOffset=0;
	m_data->outputBuffer=0;
	m_data->dsData=new DirectSoundOutDeviceData;
	m_listener=0;

	m_data->timerID=0;

	m_data->pHEvent[0]=CreateEvent(0,false,false,"DSB_Notify0");
	m_data->pHEvent[1]=CreateEvent(0,false,false,"DSB_Notify1");
	m_data->audioBuffer=0;
}
DirectSoundOutputStream::~DirectSoundOutputStream()
{
	StopOutputStream();
	CloseHandle(m_data->pHEvent[0]);
	CloseHandle(m_data->pHEvent[1]);

	if(m_data->audioBuffer)
	{
		delete [] m_data->audioBuffer;
		m_data->audioBuffer=0;
	}
	if(m_data->outputBuffer)
	{
		m_data->outputBuffer->Release();
		m_data->outputBuffer=0;
	}
	delete m_data;
}

void DirectSoundOutputStream::ListDevices(std::vector<OutputStreamDeviceInfo> &lst)
{
	m_data->dsData->devices.clear();
	DirectSoundEnumerate(DirectSoundEnumCallback,&m_data->dsData->devices);
	for(int i=0;i<m_data->dsData->devices.size();++i)
	{
		OutputStreamDeviceInfo ifo;
		ifo.id=i;
		ifo.name=m_data->dsData->devices[i].module;
		ifo.description=m_data->dsData->devices[i].Desciption;
		lst.push_back(ifo);
	}
}
bool DirectSoundOutputStream::CreateOutputStream(int device,int bps,uchar numChannels,uint samplesPerSec)
{

	m_data->dsData->devices.clear();
	DirectSoundEnumerate(DirectSoundEnumCallback,&m_data->dsData->devices);
	if(device>=m_data->dsData->devices.size())
		return false;
	if(DirectSoundCreate(m_data->dsData->devices[device].lpGuid,&m_data->dsData->device,0)<0)
	{
		gLogManager.log(mT("CreateOutputStream() - failed to create direct sound"),ELL_WARNING);
		return false;
	}
	HWND ownerWindow = GetForegroundWindow();
	if (ownerWindow == NULL) {
		ownerWindow = GetDesktopWindow();
	}
	m_data->dsData->device->SetCooperativeLevel(ownerWindow,DSSCL_PRIORITY);
	if(m_data->outputBuffer)
	{
		((LPDIRECTSOUNDCAPTUREBUFFER)m_data->outputBuffer)->Release();
		m_data->outputBuffer=0;
	}
	HRESULT hr;

	DSBUFFERDESC dscbd;
	memset(&dscbd,0,sizeof(DSBUFFERDESC));


	// wFormatTag, nChannels, nSamplesPerSec, mAvgBytesPerSec,
	// nBlockAlign, wBitsPerSample, cbSize
	m_data->waveFormat.cbSize=0;
	m_data->waveFormat.wFormatTag=WAVE_FORMAT_PCM;
	m_data->waveFormat.nSamplesPerSec=samplesPerSec;
	m_data->waveFormat.wBitsPerSample=bps;
	m_data->waveFormat.nChannels=numChannels;
	m_data->waveFormat.nBlockAlign=(bps>>3)*numChannels;
	m_data->waveFormat.nAvgBytesPerSec=samplesPerSec*m_data->waveFormat.nBlockAlign;

	//Create Primary Buffer 
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	LPDIRECTSOUNDBUFFER lpDSB = NULL;
	if ( FAILED(m_data->dsData->device->CreateSoundBuffer(&dsbd, &lpDSB, NULL)) ) {
		gLogManager.log(mT("CreateOutputStream() - failed to create primary sound buffer"),ELL_WARNING);
		return false;
	}

	//Set Primary Buffer Format
	if ( FAILED(lpDSB->SetFormat(&m_data->waveFormat)) ) {
		gLogManager.log(mT("CreateOutputStream() - failed to set primary format"),ELL_WARNING);
		return false;
	}

	//{ WAVE_FORMAT_PCM, numChannels, samplesPerSec, /*avgBytesPerSec*/ BlockSize*samplesPerSec, BlockSize, bitsPerSample, 0};

	dscbd.dwSize = sizeof(DSBUFFERDESC);
	dscbd.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS ;//| DSBCAPS_CTRLVOLUME DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
	//dscbd.dwBufferBytes = DirectSoundOutDeviceData::BufferBytes;
	dscbd.dwBufferBytes =2*m_data->waveFormat.nAvgBytesPerSec ;
	dscbd.dwReserved = 0;
	dscbd.lpwfxFormat = &m_data->waveFormat;

	if (FAILED(hr = m_data->dsData->device->CreateSoundBuffer(&dscbd, &m_data->outputBuffer, NULL)))
	{
		gLogManager.log("DirectSoundOutputStream::CreateOutputStream()- Failed to create DirectSound Buffer",ELL_WARNING);
		m_data->outputBuffer = NULL;
		return false;
	}

	LPDIRECTSOUNDNOTIFY lpDSNotify;
	if(FAILED(m_data->outputBuffer->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&lpDSNotify)))
	{
		gLogManager.log("DirectSoundOutputStream::CreateOutputStream()- Failed to query DirectSound Notify",ELL_WARNING);
		return false;
	}

	//Set Direct Sound buffer notify position
	DSBPOSITIONNOTIFY pPosNotify[2];
	pPosNotify[0].dwOffset=m_data->waveFormat.nAvgBytesPerSec/2 -1;
	pPosNotify[1].dwOffset=3*m_data->waveFormat.nAvgBytesPerSec/2 -1;
	pPosNotify[0].hEventNotify=m_data->pHEvent[0];
	pPosNotify[1].hEventNotify=m_data->pHEvent[1];
	if(FAILED(lpDSNotify->SetNotificationPositions(2,pPosNotify)))
	{
		gLogManager.log("DirectSoundOutputStream::CreateOutputStream()- Failed to set DirectSound Notification Positions",ELL_WARNING);
		return false;
	}
	if(m_data->audioBuffer)
	{
		delete m_data->audioBuffer;
		m_data->audioBuffer=0;
	}
	m_data->audioBuffer=new uchar[m_data->waveFormat.nAvgBytesPerSec];
	return true;
}
bool DirectSoundOutputStream::StartOutputStream()
{
	if(!m_data->outputBuffer)
		return false;
	DWORD st;
	m_data->outputBuffer->GetStatus(&st);
	if(st & DSBSTATUS_PLAYING)
		return true;

	if(m_listener)
	{
		if(!m_data->circles[0])
		{
			uint samples=m_listener->GetAudioSamples(this,m_data->audioBuffer,m_data->waveFormat.nSamplesPerSec);

			DWORD dwRetBytes = samples*m_data->waveFormat.nBlockAlign;

			//Write the audio data to DirectSoundBuffer
			LPVOID lpvAudio1 = NULL, lpvAudio2 = NULL;
			DWORD dwBytesAudio1 = 0, dwBytesAudio2 = 0;	

			//Lock DirectSoundBuffer
			HRESULT hr = m_data->outputBuffer->Lock(0, m_data->waveFormat.nAvgBytesPerSec, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0);
			if ( FAILED(hr) ) {
				return false;
			}

			//Init lpvAudio1
			if (NULL != lpvAudio1) {			
				memset(lpvAudio1, 0, dwBytesAudio1);			
			}

			//Init lpvAudio2
			if (NULL != lpvAudio2) {			
				memset(lpvAudio2, 0, dwBytesAudio2);			
			}

			//Copy Audio Buffer to DirectSoundBuffer
			if (NULL == lpvAudio2) {
				memcpy(lpvAudio1, m_data->audioBuffer, dwRetBytes);
			}
			else {
				memcpy(lpvAudio1, m_data->audioBuffer, dwBytesAudio1);
				memcpy(lpvAudio2, m_data->audioBuffer + dwBytesAudio1, dwBytesAudio2);
			}
			dwBytesAudio1=dwRetBytes;
			//Unlock DirectSoundBuffer
			m_data->outputBuffer->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);
		}

		m_data->outputBuffer->Play(0,0,DSBPLAY_LOOPING);

		m_data->timerID=timeSetEvent(300,100,TimerProcess,(DWORD)this,TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
	}

	return true;
}

void DirectSoundOutputStream::PlaySamples(const void* buffer,uint len,bool looping)
{
	if(!m_data->outputBuffer)
		return;
	void *buf1;
	void *buf2;
	DWORD count1;
	DWORD count2;

	HRESULT hr=m_data->outputBuffer->Lock(0, len*m_data->waveFormat.wBitsPerSample, &buf1, &count1, &buf2, &count2, 0);
	if(hr==DSERR_BUFFERLOST)
	{
		m_data->outputBuffer->Restore();
		hr=m_data->outputBuffer->Lock(0, len*m_data->waveFormat.wBitsPerSample, &buf1, &count1, &buf2, &count2, 0);
	}
	if(SUCCEEDED(hr))
	{
		memcpy(buf1,buffer,count1);
		if(buf2)
			memcpy(buf2,(uchar*)buffer+count1,count2);
		hr=m_data->outputBuffer->Unlock(buf1, count1, buf2, count2);
	}

// 	DWORD currentPos,writePos;
// 	m_data->outputBuffer->GetCurrentPosition(&currentPos,&writePos);

// 	DWORD st;
// 	hr=m_data->outputBuffer->GetStatus(&st);
// 	if((st & DSBSTATUS_PLAYING)  ==0)
	m_data->outputBuffer->Play(0,0,looping?DSBPLAY_LOOPING:0);
}

void DirectSoundOutputStream::PauseOutputStream()
{
	if(!m_data->outputBuffer)
		return;
	m_data->outputBuffer->Stop();

	timeKillEvent(m_data->timerID);
	m_data->timerID=0;
}
void DirectSoundOutputStream::StopOutputStream()
{

	if(!m_data->outputBuffer)
		return;

// 	DWORD st;
// 	HRESULT hr=m_data->outputBuffer->GetStatus(&st);
// 	if((st & DSBSTATUS_PLAYING))
	m_data->outputBuffer->Stop();

	timeKillEvent(m_data->timerID);

	LPVOID lpvAudio1 = NULL;
	DWORD dwBytesAudio1 = 0;
	HRESULT hr = m_data->outputBuffer->Lock(0, 0, &lpvAudio1, &dwBytesAudio1, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if ( FAILED(hr) ) {
		return;
	}

	memset(lpvAudio1,0,dwBytesAudio1);
	m_data->outputBuffer->Unlock(lpvAudio1,dwBytesAudio1,0,0);
	m_data->outputBuffer->SetCurrentPosition(0);
	ResetEvent(m_data->pHEvent[0]);
	ResetEvent(m_data->pHEvent[1]);

	m_data->circles[0]=m_data->circles[1]=0;

}

void DirectSoundOutputStream::SetFrequency(uint v)
{
	if(!m_data->outputBuffer)
		return;
	m_data->outputBuffer->SetFrequency(v);
}

uint DirectSoundOutputStream::GetFrequency()
{
	if(!m_data->outputBuffer)
		return 0;
	DWORD v;
	m_data->outputBuffer->GetFrequency(&v);
	return (uint)v;
}

void DirectSoundOutputStream::SetVolume(uint v)
{
	if(!m_data->outputBuffer)
		return;
	m_data->outputBuffer->SetVolume(v);
}

uint DirectSoundOutputStream::GetVolume()
{
	if(!m_data->outputBuffer)
		return 0;
	LONG v;
	m_data->outputBuffer->GetVolume(&v);
	return (uint)v;
}
void DirectSoundOutputStream::SetPan(uint v)
{
	if(!m_data->outputBuffer)
		return;
	m_data->outputBuffer->SetPan(v);
}

uint DirectSoundOutputStream::GetPan()
{
	if(!m_data->outputBuffer)
		return 0;
	LONG v;
	m_data->outputBuffer->GetPan(&v);
	return (uint)v;
}
uint DirectSoundOutputStream::GetCurrentPosition()
{

	if(!m_data->outputBuffer)
		return 0;
	DWORD cr,cw;
	m_data->outputBuffer->GetCurrentPosition(&cr,&cw);
	return (uint)cr;
}

bool DirectSoundOutputStream::IsPlaying()
{
	if(!m_data->outputBuffer)
		return false;
	DWORD st;
	m_data->outputBuffer->GetStatus(&st);
	return st&DSBSTATUS_PLAYING;
}
bool DirectSoundOutputStream::IsLooping()
{
	if(!m_data->outputBuffer)
		return false;
	DWORD st;
	m_data->outputBuffer->GetStatus(&st);
	return st&DSBSTATUS_LOOPING;
}

uint DirectSoundOutputStream::GetPlayedSamples()
{
	if(!m_data->outputBuffer)
		return 0;

	DWORD currPos=0,currPlayedSamples=0;
	m_data->outputBuffer->GetCurrentPosition(&currPos,0);
	currPlayedSamples=currPos/m_data->waveFormat.nBlockAlign;
	DWORD samplesPlayed=0;
	if(m_data->circles[0]<1)
		return currPlayedSamples;
	samplesPlayed=(m_data->circles[1]-1)*2*m_data->waveFormat.nSamplesPerSec+3*m_data->waveFormat.nSamplesPerSec/2;
	if(currPlayedSamples>3*m_data->waveFormat.nSamplesPerSec/2)
	{
		if(m_data->circles[1]<m_data->circles[0])
		{
			samplesPlayed=(m_data->circles[0]-1)*2*m_data->waveFormat.nSamplesPerSec+3*m_data->waveFormat.nSamplesPerSec/2;
		}
		samplesPlayed+=currPlayedSamples-3*m_data->waveFormat.nSamplesPerSec/2+1;
	}else
	{
		samplesPlayed+=currPlayedSamples+m_data->waveFormat.nSamplesPerSec/2;
	}
	return samplesPlayed;
}
uint DirectSoundOutputStream::GetBytesPerSecond()
{
	return m_data->waveFormat.nAvgBytesPerSec;
}
void DirectSoundOutputStream::_TimerCallback()
{

	LPVOID pAudio1=0,pAudio2=0;
	DWORD bytesAudio1=0,bytesAudio2=0;
	DWORD retSamples=0,retBytes=0;
	if(!m_listener->OnTimerTick(this))
	{
		StopOutputStream();
		return;
	}
	HRESULT hr=WaitForMultipleObjects(2,m_data->pHEvent,false,0);
	if(hr==WAIT_OBJECT_0)
	{
		++m_data->circles[0];
		HRESULT hr2=m_data->outputBuffer->Lock(m_data->waveFormat.nAvgBytesPerSec,m_data->waveFormat.nAvgBytesPerSec,&pAudio1,&bytesAudio1,
			&pAudio2,&bytesAudio2,0);
		if(FAILED(hr2))
		{
			return;
		}
	}else if(hr==WAIT_OBJECT_0+1)
	{
		++m_data->circles[1];
		HRESULT hr2=m_data->outputBuffer->Lock(0,m_data->waveFormat.nAvgBytesPerSec,&pAudio1,&bytesAudio1,&pAudio2,&bytesAudio2,0);
		if(FAILED(hr2))
		{
			return;
		}
	}else
		return;

	retSamples=m_listener->GetAudioSamples(this,m_data->audioBuffer,m_data->waveFormat.nSamplesPerSec);;
	retBytes=retSamples*m_data->waveFormat.nBlockAlign;
	//memset(m_data->audioBuffer,0,retBytes);
	if(retSamples<m_data->waveFormat.nSamplesPerSec)
	{
		memset(m_data->audioBuffer+retBytes,0,m_data->waveFormat.nAvgBytesPerSec-retBytes);
	}
	bytesAudio1=retBytes;
	if(!pAudio2)
	{
		memcpy(pAudio1,m_data->audioBuffer,bytesAudio1);
	}else
	{
		memcpy(pAudio1,m_data->audioBuffer,bytesAudio1);
		memcpy(pAudio2,m_data->audioBuffer+bytesAudio1,bytesAudio2);
	}
	m_data->outputBuffer->Unlock(pAudio1,bytesAudio1,pAudio2,bytesAudio2);
}

}
}


