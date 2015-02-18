
#include "stdafx.h"
#include "AudioSoundInterface.h"
#include "OutputChannel.h"
#include "ILogManager.h"

namespace mray
{


	class AudioSoundInterfaceListener:public sound::IDirectSoundOutputListener
	{
		AudioSoundInterface* m_owner;
	public:
		AudioSoundInterfaceListener(AudioSoundInterface* o):m_owner(o)
		{
		}
		uint GetAudioSamples(sound::DirectSoundOutputStream* stream,uchar* buffer,uint samples)
		{
			OutputChannel* c1= m_owner->GetLeftChannel();
			OutputChannel* c2= m_owner->GetRightChannel();
			int b=m_owner->GetDesc().bps>>3;

			std::vector<short> frames[2];
			int buffLen=(samples);
			if(buffLen==0)
				return 0;
			frames[0].resize(buffLen,0);
			frames[1].resize(buffLen,0);
			short* p[2]={(short*)&frames[0][0],(short*)&frames[1][0]};
			int l1=c1->ReadFrames((short*)p[0],buffLen);
			int l2=c2->ReadFrames((short*)p[1],buffLen);

			int maxLen=math::Max(l1,l2);
			int channels=m_owner->GetDesc().stereo?2:1;
			int step=channels;
			short* ptr=(short*)buffer;

			int cnt[2]={l1,l2};
			int samples2=maxLen*2;
			for(int i=0;i<samples2;i+=step)
			{
				for(int j=0;j<channels;++j)
				{
					//for(int k=0;k<b;++k)
// 					{
// 						/*ptr[i+j*channels*b+k]*/
// 						ptr[k]=p[j][k];
// 						
// 					}
// 					ptr+=b;
// 					p[j]+=b;
					*ptr=*p[j];
					++ptr;
					++p[j];

				}

			}
			return samples2/2;
		}
		bool OnTimerTick(sound::DirectSoundOutputStream* stream)
		{
			OutputChannel* c1= m_owner->GetLeftChannel();
			OutputChannel* c2= m_owner->GetRightChannel();

			return c1->isDone()==false || c2->isDone()==false;
		}

	};

AudioSoundInterface::AudioSoundInterface()
{
	m_outChannels[0]=new OutputChannel(EChannel_Left,this);
	m_outChannels[1]=new OutputChannel(EChannel_Right,this);

	m_outputStream=0;
	m_dsListener=new AudioSoundInterfaceListener(this);
}

AudioSoundInterface::~AudioSoundInterface()
{
	if(m_outputStream)
	{
		m_outputStream->StopOutputStream();
		delete m_outputStream;
	}
	delete m_outChannels[0];
	delete m_outChannels[1];
	delete m_dsListener;
}
bool AudioSoundInterface::OpenStreamDevice(const sound::OutputStreamDeviceInfo& ifo,const AudioSoundInterfaceDesc& desc)
{
	m_desc=desc;
	m_streamInfo=ifo;
	m_outputStream=new sound::DirectSoundOutputStream();
	if(!m_outputStream->CreateOutputStream(ifo.id,desc.bps,desc.stereo?2:1,desc.samplesPerSec))
	{
		gLogManager.log("AudioSoundInterface::OpenStreamDevice - Failed to open stream "+core::string(ifo.description),ELL_WARNING);
		return false;
	}
	m_outputStream->SetListener(m_dsListener);
	m_outputStream->StartOutputStream();
	return true;
}
void AudioSoundInterface::Start()
{
	if(!m_outputStream)
		return;
	m_outputStream->StartOutputStream();
}

void AudioSoundInterface::Stop()
{
	if(!m_outputStream)
		return;
	m_outputStream->StopOutputStream();
}

void AudioSoundInterface::Update(float dt)
{
}


//////////////////////////////////////////////////////////////////////////

AudioInterfaceManager::AudioInterfaceManager()
{

}
AudioInterfaceManager::~AudioInterfaceManager()
{
	Clear();
}

void AudioInterfaceManager::Clear()
{
	for (int i=0;i<m_interfaces.size();++i)
	{
		delete m_interfaces[i];
	}
	m_interfaces.clear();
}
void AudioInterfaceManager::ReloadInterfaces(const AudioSoundInterfaceDesc &desc)
{
	Clear();
	std::vector<sound::OutputStreamDeviceInfo> sndLst;

	m_desc=desc;
	sound::DirectSoundOutputStream tmp;
	tmp.ListDevices(sndLst);
	for (int i=0;i<sndLst.size();++i)
	{
		AudioSoundInterface* audioSound=new AudioSoundInterface();

		if(!audioSound->OpenStreamDevice(sndLst[i],desc))
		{
			printf("Failed to open audio interface: %d-%s\n",i,sndLst[i].description.c_str());
		}else
		{
			printf("Adding audio interface: %d-%s\n",i,sndLst[i].description.c_str());
		}
		m_interfaces.push_back(audioSound);
	}
}

AudioSoundInterface* AudioInterfaceManager::GetInterface(int index)
{
	if(index>=m_interfaces.size())
		return 0;
	return m_interfaces[index];
}


}